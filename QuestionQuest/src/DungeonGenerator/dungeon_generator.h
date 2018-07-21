#ifndef DUNGEONGENERATOR_H_INCLUDED
#define DUNGEONGENERATOR_H_INCLUDED

#include <cstdlib>
#include <cassert>
#include <vector>
#include <iostream>
#include <algorithm>

namespace dgen{

using std::size_t;

///aliases
//percentage type
using perc_t = unsigned int;


///enumerations
//generalized tiles
enum class Tile {
    Wall,
    Floor
};

///classes and structures
//generation helper classes
struct Pos{
    int x, y;
    //operators
    bool operator==(const Pos &pos) const {
        return x == pos.x && y == pos.y;
    }
    bool operator!=(const Pos &pos) const {
        return !(*this == pos);
    }
    Pos operator+(const Pos &pos) const {
        return {x + pos.x, y + pos.y};
    }
    Pos operator-(const Pos &pos) const {
        return {x - pos.x, y - pos.y};
    }
};
struct Rect{
    int x, y, w, h;
    bool checkCollision(const Rect &rect){
        return (rect.x > x && rect.x < x + w) && (rect.y > y && rect.y < y + h);
    }
};
struct Room{
    Rect rect;
    unsigned int branchingCorridors;
};
struct GenInfo{
    int seed = 1;
    perc_t groundToCover = 50;
    perc_t roomChance = 5;
    unsigned int minRoomSize = 3;
    unsigned int maxRoomSize = 20;
    unsigned int roomPlacingAttempts = 100;
    unsigned int roomBranchingAttempts = 100;
    perc_t corridorTurnChance = 5;
    unsigned int minCorridorLength = 5;
    unsigned int maxCorridorLength = 30;
    unsigned int maxBranchingCorridors = 3;
};
//board class
class Board{
    public:

        Board(size_t width, size_t height);

        size_t getWidth() const {return width;}
        size_t getHeight() const {return height;}

        bool isEmpty() const {return tiles.empty();}

        bool placeTile(const Pos &pos, Tile tile);
        bool fillRect(const Rect &rect, Tile tile);

        bool posOutOfBounds(const Pos &pos) const;
        bool rectOutOfBounds(const Rect &pos) const;
        Pos getRandPerimeterPos(const Rect &rect) const;
        unsigned int countTilesInRect(const Rect &rect, Tile tile) const;
        Tile getTile(const Pos &pos) const;
        std::pair<int,int> countAdjacentTiles(const Pos &pos, Tile tile) const;

        perc_t generate(const GenInfo &ginf = GenInfo(), std::ostream &dbLog = std::cout);

        void print(std::ostream &stream = std::cout);
    private:
        std::vector<Tile> tiles;
        size_t width, height;
};

///global functions
//random functions
inline int rrange(int min,int max){
    return (rand()%(max-min))+min;
}
inline int rrange(int max){
    return rrange(0,max);
}
inline bool perc(int chance,int maxPerc){
    if(rand()%maxPerc <= chance-1) return true;
    return false;
}
inline bool perc(int chance){
    return perc(chance,100);
}

}

#endif // DUNGEONGENERATOR_H_INCLUDED
