#include "dungeon_generator.h"

///constructor
dgen::Board::Board(size_t width, size_t height):
    width(width),
    height(height)
{
    tiles.resize(width * height);
}

///board modification functions
//check if specified position is out of range (true) or not (false)
bool dgen::Board::posOutOfBounds(const dgen::Pos &pos) const {
    //check if empty (always out of range)
    if(isEmpty()) return true;
    //out of range
    if(pos.x < 0 || pos.x >= width || pos.y < 0 || pos.y >= height) return true;
    //not out of range
    return false;
}
//check if given rectangle is out of bounds
bool dgen::Board::rectOutOfBounds(const dgen::Rect &r) const {
    //check if board is valid
    if(isEmpty()) return true;
    //rect out of range
    if(r.x < 0 || r.y < 0 || r.w < 0 || r.h < 0 || r.x >= width || r.y >= height || r.w >= width || r.h >= height) return true;
    //not out of bounds
    return false;
}
//substitute tile at (x;y) with the specified tile - return false if out of range
bool dgen::Board::placeTile(const Pos &pos, dgen::Tile tile){
    //check if board is empty
    if(isEmpty()) return false;
    //check if out of range
    if(posOutOfBounds(pos)) return false;
    //replace tile
    tiles[pos.y * width + pos.x] = tile;
    //success
    return true;
}

//fill a given rectangle on a given z level with the given tile - return false on failure
bool dgen::Board::fillRect(const Rect &rect, Tile tile){
    //check if rectangle is valid
    if(rect.x < 0 || rect.y < 0 || rect.w >= width || rect.h >= height) return false;
    //place tile model all over rect, if one canàt be placed, return false
    bool success = true;
    for(int x = rect.x; x < rect.x+rect.w; x++){
        for(int y = rect.y; y < rect.y+rect.h; y++){
            if(!placeTile({x, y}, tile)) success = false;
        }
    }
    return success;
}

///board query function
//get the tile at given position
dgen::Tile dgen::Board::getTile(const Pos &pos) const {
    assert(!posOutOfBounds(pos));
    return tiles[pos.y * width + pos.x];
}
//pick the position of a random tile that is adjacent to the perimeter of a given rectangle - return {-1,-1} on failure
dgen::Pos dgen::Board::getRandPerimeterPos(const dgen::Rect &rect) const {
    Pos retPos = {-1,-1};
    //pick a random perimeter tile of the rectangle
    do{
        retPos.x = rrange(rect.x,rect.x + rect.w - 1);
        retPos.y = rrange(rect.y,rect.y + rect.h - 1);
    }while(retPos.x != rect.x && retPos.y != rect.y && retPos.x != rect.x + rect.w - 1 && retPos.y != rect.y + rect.h - 1);
    return retPos;

}
//returns the number of cubes (containing the specified tile model) that are present in the given chunk - returns -1 on failure
unsigned int dgen::Board::countTilesInRect(const dgen::Rect &rect, Tile tile) const {
    //check if chunk is valid
    if(rectOutOfBounds(rect)) return -1;
    //get on with it!
    int count = 0;
    for(int x = rect.x; x <= rect.w; x++){
        for(int y = rect.y; y <= rect.h; y++){
            if(getTile({x, y}) == tile) count++;
        }
    }
    return count;
}
//counts the number of tiles (corresponding to the given tile) adjacent to the tile in the given position - returns -1 on failure
std::pair<int,int> dgen::Board::countAdjacentTiles(const dgen::Pos &tilePos, Tile tile) const {
    //check if position is valid
    if(posOutOfBounds(tilePos)) return {-1,-1};
    //create chunk representing adjacent tiles and check if it's valid
    Pos stPos = tilePos;
    stPos.x--;
    stPos.y--;
    Pos endPos = tilePos;
    endPos.x++;
    endPos.y++;
    if(stPos.x < 0) stPos.x = 0;
    if(stPos.y < 0) stPos.y = 0;
    if(endPos.x >= width) endPos.x = width - 1;
    if(endPos.y >= height) endPos.y = height - 1;
    //get on with it!
    std::pair<int,int> count = {0,0};
    for(int x = stPos.x; x <= endPos.x; x++){
        for(int y = stPos.y; y <= endPos.y; y++){
            Pos pos = {x,y};
            if(tilePos != pos && getTile({x, y}) == tile){
                //cross
                if(x == tilePos.x || y == tilePos.y) count.first++;
                //diagonal
                else count.second++;
            }
        }
    }
    return count;
}

///board generation functions
//generate 2D dungeon
dgen::perc_t dgen::Board::generate(const dgen::GenInfo &ginf, std::ostream &dbLog){
    ///*generate an interconnected level with randomly sized rectangular rooms
    ///set seed
    srand(ginf.seed);
    ///start by filling all the space with rocks
    for(size_t j = 0; j < width * height; j++){
        tiles[j] == Tile::Wall;
    }
    dbLog << "board filled" << std::endl;
    ///generate dungeon rooms and corridors
    //rooms
    std::vector<Room> rooms;
    //ground to cover and ground covered so far
    int groundToCover = (double(width*height) / 100.0) * double(ginf.groundToCover);
    int groundCovered = 0;
    //dig first room in random position
    rooms.push_back({{rrange(1,width-ginf.maxRoomSize),rrange(1,height-ginf.maxRoomSize),rrange(ginf.minRoomSize,ginf.maxRoomSize),rrange(ginf.minRoomSize,ginf.maxRoomSize)},1});
    fillRect(rooms[0].rect, Tile::Floor);
    groundCovered += rooms[0].rect.w * rooms[0].rect.h;
    dbLog << "first room created ( " << rooms[0].rect.x << "; " << rooms[0].rect.y << "; " << rooms[0].rect.w << "; " << rooms[0].rect.h << ")." << std::endl;
    //select random wall of first room and start generating a corridor
    Pos corridorPos = getRandPerimeterPos(rooms[0].rect);
    assert(corridorPos.x != -1);
    int corridorLength = 0;
    int corridorPreferredDirection = rrange(3);
    int sourceRoomIndex = 0;
    bool corridorIsDeadEnd = false;
    std::vector<Pos> corridorPastPositions;
    std::vector<Tile> corridorPastCubes;
    dbLog << "first corridor initiated (" << corridorPos.x << "; " << corridorPos.y << ")." << std::endl;
    //extend corridor and gradually add new features randomly
    while(true){
        bool findNewBranchingPosition = false;
        ///ROOM GENERATOR
        if(corridorLength >= ginf.minCorridorLength){
            if(corridorLength >= ginf.maxCorridorLength || perc(ginf.roomChance,1000)){
                dbLog << "creating room... (corridor length = " << corridorLength << ")." << std::endl;
                //try placing a room multiple times
                Rect newRoom = {-1,-1,-1,-1};
                int roomX, roomY;
                int roomW, roomH;
                for(int j=0; j < ginf.roomPlacingAttempts; j++){
                    //decide dimensions of room randomly
                    roomW = rrange(ginf.minRoomSize,ginf.maxRoomSize);
                    roomH = rrange(ginf.minRoomSize,ginf.maxRoomSize);
                    switch(corridorPreferredDirection){
                        //north
                        case 0:
                            roomX = std::max(1 , corridorPos.x - rrange(roomW-1));
                            roomY = corridorPos.y - roomH;
                            break;
                        //east
                        case 1:
                            roomX = corridorPos.x + 1;
                            roomY = std::max(1 , corridorPos.y - rrange(roomH-1));
                            break;
                        //south
                        case 2:
                            roomX = std::max(1 , corridorPos.x - rrange(roomW-1));
                            roomY = corridorPos.y + 1;
                            break;
                        //west
                        case 3:
                            roomX = corridorPos.x - roomW;
                            roomY = std::max(1 , corridorPos.y - rrange(roomH-1));
                            break;
                        default:
                            assert(false);
                    }
                    //check if room is out of bounds
                    if(roomX < 1 || roomY < 1 || roomX+roomW >= width || roomY+roomH >= height) continue;
                    //check if room intersects empty spaces
                    if(countTilesInRect({roomX-1, roomY-1, roomX+roomW, roomY+roomH}, Tile::Floor) != 1) continue;
                    //if room fits create room
                    newRoom = {roomX,roomY,roomW,roomH};
                    fillRect(newRoom, Tile::Floor);
                    rooms.push_back({newRoom,1});
                    groundCovered += newRoom.w * newRoom.h;
                    break;
                }
                //set parameters to start new corridor, if the room could not be created, delete dead ended corridor
                findNewBranchingPosition = true;
                if(newRoom.x != -1){
                    dbLog << "room created successfully! (" << newRoom.x << "; " << newRoom.y << "; " << newRoom.w << "; " << newRoom.h << ")." << std::endl;
                }
                else{
                    dbLog << "room could not be created... ";
                    corridorIsDeadEnd = true;
                }
            }
        }
        ///CORRIDOR EXTENDER
        else{
            //choose direction in which to expand corridor
            int expectedDirection = perc(ginf.corridorTurnChance,1000) ? rrange(3) : corridorPreferredDirection;
            //check if direction is OK, otherwise, pick another one rotating clockwise
            int atmps;
            for(atmps = 0; atmps < 4; atmps++){
                Pos newCorridorPos = corridorPos;
                switch(expectedDirection){
                    //north
                    case 0:
                        newCorridorPos.y--;
                        break;
                    //east
                    case 1:
                        newCorridorPos.x++;
                        break;
                    //south
                    case 2:
                        newCorridorPos.y++;
                        break;
                    //west
                    case 3:
                        newCorridorPos.x--;
                        break;
                }
                //check if out of bounds
                if(getTile(newCorridorPos) != Tile::Floor && newCorridorPos.x > 0 && newCorridorPos.y > 0 && newCorridorPos.x < width-1 && newCorridorPos.y < height-1){
                    //check if perforating another corridor or room
                    std::pair<int,int> adjacentEmptySpace = countAdjacentTiles(newCorridorPos, Tile::Floor);
                    //not cross perforating anything - check for diagonal perforation
                    if(adjacentEmptySpace.first == 1){
                        //check diagonal perforations
                        enum {FALSE = 0,TRUE = 1,UNDEFINED = 2} diagonalsOK = UNDEFINED;
                        //if corridor has just started generation, then bypass diagonal perforation checks
                        if(corridorPastPositions.size() == 0) diagonalsOK = TRUE;
                        //no diagonal perforations (OK)
                        else if(adjacentEmptySpace.second == 0) diagonalsOK = TRUE;
                        //may be diagonally perforating a room or another corridor
                        else if(adjacentEmptySpace.second == 1){
                            //check if diagonally perforated corridor corresponds to the current one by checking if it has just curved
                            int lastIndx = corridorPastPositions.size() - 1;
                            Pos newDPos = corridorPos - corridorPastPositions[lastIndx];
                            Pos oldDPos = newCorridorPos - corridorPos;
                            //check if the corridor has just curved (in this case, the extension pos is valid)
                            if((abs(newDPos.x) != abs(oldDPos.x)) || (abs(newDPos.y) != abs(oldDPos.y))){
                                //curve - diagonals ok
                                diagonalsOK = TRUE;
                            }
                            else{
                                //there is not curve... invalid diagonal perforation detected
                                diagonalsOK = FALSE;
                            }
                        }
                        //more that one diagonal perforation always means bad stuff...
                        else{
                            diagonalsOK = FALSE;
                        }
                        //make sure that diagonalsOK has been defined
                        assert(diagonalsOK != UNDEFINED);
                        //if diagonals are ok, accept position
                        if(diagonalsOK == TRUE){
                            corridorPos = newCorridorPos;
                            break;
                        }
                    }
                    //may be cross perforating room or a corridor
                    else if(adjacentEmptySpace.first == 2){
                        bool roomFound = false;
                        //iterate though all rooms
                        for(int j=0;j<rooms.size();j++){
                            Rect corrRect;
                            corrRect.x = std::max(1 , newCorridorPos.x-1);
                            corrRect.y = std::max(1 , newCorridorPos.y-1);
                            int crwx = std::min(width-1 , (size_t)newCorridorPos.x+1);
                            int crhy = std::min(height-1 , (size_t)newCorridorPos.y+1);
                            corrRect.w = crwx - corrRect.x + 1;
                            corrRect.h = crhy - corrRect.y + 1;
                            //check if corridor is perforating a room
                            if(corrRect.checkCollision(rooms[j].rect) && rooms[j].branchingCorridors < ginf.maxBranchingCorridors && j != sourceRoomIndex){
                                //cross perforating a room - new position valid! (but new branching position required by branching position finder algorithm)
                                //perforate room
                                corridorPos = newCorridorPos;
                                //prevent corridor from being treated like a dead ended corridor by the branching position finding algorithm
                                corridorIsDeadEnd = false;
                                //find new branching position for new corridor
                                findNewBranchingPosition = true;
                                roomFound = true;
                                rooms[j].branchingCorridors++;
                                break;
                            }
                        }
                        //if room has been perforated, exit loop with valid new position, else continue searching
                        if(roomFound) break;
                    }
                }
                //iterate through all possible corridor extension directions
                expectedDirection = (expectedDirection + 1) % 4;
            }
            //if all directions fail, make corridor a dead end (to be handled by branching position finder)
            if(atmps == 4){
                dbLog << "failed to extend corridor... ";
                corridorIsDeadEnd = true;
                findNewBranchingPosition = true;
            }
            //valid position found - extend corridor
            else{
                //increment corridor length
                corridorLength++;
                //adjust decided direction
                corridorPreferredDirection = expectedDirection;
                //add position and deleted cube to memory vector
                corridorPastPositions.push_back(corridorPos);
                corridorPastCubes.push_back(getTile(corridorPos));
                //carve corridor
                placeTile(corridorPos, Tile::Floor);
                //augment ground covered
                groundCovered++;
            }
        }
        ///NEW BRANCHING POSITION FINDER, DEAD END DELETER and CORRIDOR REFINER
        if(findNewBranchingPosition){
            //refine walls or clear dead ends
            if(corridorIsDeadEnd) dbLog << "clearing dead ended corridor" << std::endl;
            else dbLog << "refining corridor walls' link configuration" << std::endl;
            //decrease source room branching positions
            rooms[sourceRoomIndex].branchingCorridors--;
            assert(rooms[sourceRoomIndex].branchingCorridors >= 0);
            //clear dead ended corridor
            //!SDL_Point memDPos = {corridorPastPositions[1].x - corridorPastPositions[0].x,corridorPastPositions[1].y - corridorPastPositions[0].y};
            if(corridorIsDeadEnd){
                for(int j = 0; j < corridorPastPositions.size(); j++){
                    placeTile(corridorPastPositions[j], Tile::Wall);
                }
            }
            //exit if ground covered is enough
            if(groundCovered >= groundToCover) break;
            //find the starting position to make a new one
            dbLog << "searching for another branching position for new corridor..." << std::endl;
            //reset corridor
            corridorLength = 0;
            corridorPreferredDirection = rrange(3);
            corridorPastPositions.clear();
            corridorPastCubes.clear();
            corridorIsDeadEnd = false;
            //try branching from all other rooms in a random order
            std::vector<int> randRoomIndexes;
            for(int j = 0; j < rooms.size(); j++){
                randRoomIndexes.push_back(j);
            }
            std::random_shuffle(randRoomIndexes.begin(),randRoomIndexes.end());
            bool branchingDone = false;
            int atmps;
            for(atmps = 0; atmps < rooms.size(); atmps++){
                for(int j = 0; j < ginf.roomBranchingAttempts; j++){
                    auto &room = rooms[randRoomIndexes[atmps]];
                    corridorPos = getRandPerimeterPos(room.rect);
                    sourceRoomIndex = randRoomIndexes[atmps];
                    if(corridorPos.x > 0 && corridorPos.y > 0 && corridorPos.x < width-1 && corridorPos.y < height-1 && countAdjacentTiles(corridorPos, Tile::Floor).first == 3 && room.branchingCorridors < ginf.maxBranchingCorridors){
                        branchingDone = true;
                        room.branchingCorridors++;
                        break;
                    }
                }
                if(branchingDone) break;
            }
            //if even this fails, stop generation prematurely
            if(atmps == rooms.size()){
                dbLog << "failed to find new branching position, stopping generation prematurely." << std::endl;
                break;
            }
            else dbLog << "new branching corridor position found (" << corridorPos.x << "; " << corridorPos.y << ")." << std::endl;
        }
    }
    //return ground covered
    dbLog << "xy generation done - ground covered: " << groundCovered << "/" << groundToCover << "." << std::endl;
    //success
    return groundCovered;
}

//print it
void dgen::Board::print(std::ostream &stream) {
    size_t j = 1;
    for(const auto &ele : tiles){
        if(ele == dgen::Tile::Floor) stream << ' ';
        else stream << '#';
        if(!(j % width)) stream << '\n';
        ++j;
    }
}
