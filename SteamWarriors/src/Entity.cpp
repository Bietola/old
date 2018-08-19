#include "Entity.h"

#include "World.h"

///Entity static variables
World* Entity::world = nullptr;

///Entity functions
//constructors
Entity::Entity():
    Entity("NONAME", "NODESCRIPTION", "NOFACTION",
           {0, 0},
           nullptr,
           [](){AnimationArray a; a.fill(nullptr); return a;}(), //!I didn't even think this would work but wow...
           nullptr,
           nullptr,
           Status())
{
    ;
}
Entity::Entity(const std::string& name, const std::string& description, const std::string& faction,
               const sf::Vector2i& position,
               const eai::EntityAI* brain,
               const AnimationArray& animations,
               const DialogTree* dialog,
               const SpecialMove* defaultMeleeMove,
               const Status& status):
    Describable(name, description),
    mFaction(faction),
    mPosition(position),
    mAnimations(animations),
    mDialog(dialog),
    mDefaultMeleeMove(defaultMeleeMove),
    mStatus(status),
    mBrain(brain),
    mAnimationHandler(),
    mTileInfo(nullptr)
{
    //set up graphics
    if(mAnimations[SpecialMove::ANIM_IDLE]) addAnimation(SpecialMove::ANIM_IDLE);
    ///set up interface
    //tile info interface
    mTileInfo = std::make_shared<tgui::Panel>();
    mTileInfo->setPosition(world->tileToWindowPosition(mPosition));
    mTileInfo->setSize(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT));
    mTileInfo->setBackgroundColor(sf::Color::Transparent);
    auto lifeBar = std::make_shared<tgui::ProgressBar>();
    lifeBar->setPosition(0, TILE_HEIGHT - TILE_HEIGHT / 32.f);
    lifeBar->setSize(TILE_WIDTH, TILE_HEIGHT / 32.f);
    mTileInfo->add(lifeBar, "life_bar");
    updateInterface();
}

//copy constructor
Entity::Entity(const Entity& e):
    Entity(e.mName, e.mDescription, e.mFaction,
           e.mPosition,
           e.mBrain,
           e.mAnimations,
           e.mDialog,
           e.mDefaultMeleeMove,
           e.mStatus)
{}

//destructor
Entity::~Entity(){
    //kill remaining actions
    for(auto& ele : mActions) {
        delete ele;
    }
}

//load from a json file
bool Entity::loadFromFile(const std::string& filePath) {
    //assert(filePath.substr('.') == "json");
    //open json file
    std::ifstream file(filePath.c_str(), std::ifstream::binary);
    Json::Value root;
    file >> root;
    //load text (Describable) data
    mName = root.get("name", "NONAME").asString();
    mDescription = root.get("description", "NODESCRIPTION").asString();
    //load status
    mStatus = Status();
    auto jStatus = root["status"];
    if(!jStatus.isNull()) {
        for(Json::ValueIterator itr = jStatus.begin(); itr != jStatus.end(); ++itr) {
            if(!itr->isConvertibleTo(Json::ValueType::realValue)) {
                std::cout << "FATERR: status stat is not convertible to double\n";
                std::cout << "\tfile: " << filePath << "\n\tvar name: " << itr.name() << "\n";
                assert(false);
            }
            mStatus[itr.name()] = itr->asDouble();
        }
    }
    //load faction
    mFaction = root.get("faction", "neutral").asString();
    //load brain (ai)
    auto jAi = root.get("ai", "");
    if(jAi == "Dummy") {
        mBrain = new eai::Dummy();
    }
    //load dialog
    auto jDialog = root.get("dialog", "NODIALOG");
    if(jDialog.isString() && jDialog.asString() != "NODIALOG")
        mDialog = DialogTree::getModels().getMediaPtr(jDialog.asString());
    //load animations
    auto jAnims = root["animation"];
    if(!jAnims.isNull()) {
        for(auto itr = jAnims.begin(); itr != jAnims.end(); ++itr) {
            if(!itr->isString() && !itr->isObject()) {
                std::cout << "FATERR: animation loading path is not string nor object\n";
                std::cout << "\tfile: " << filePath << "\n\tvar name: " << itr.name() << "\n";
                assert(false);
            }
            else {
                auto animType = SpecialMove::stringToAnimationType(itr.name());
                //load animation using id
                if(itr->isString()) {
                    mAnimations[animType] = gAnimationProvider.getMediaPtr(itr->asString());
                }
                //load animation from json data
                else if(itr->isObject()) {
                    auto tmpKey = gAnimationProvider.addTempMedia(AnimationInfo(*itr));
                    mAnimations[animType] = gAnimationProvider.getMediaPtr(tmpKey);
                }
            }
        }
    }
    //load moves
    auto jMoves = root["moves"];
    //load default move
    auto jDefaultMove = jMoves["default"];
    if(!jDefaultMove.isNull()) {
        //load move from move models
        if(jDefaultMove.isString()) {
            mDefaultMeleeMove = SpecialMove::models.getMediaPtr(jDefaultMove.asString());
        }
        //load move directly from entity json data
        else if(jDefaultMove.isObject()) {
            auto tmpKey = SpecialMove::models.addTempMedia(SpecialMove(jDefaultMove));
            mDefaultMeleeMove = SpecialMove::models.getMediaPtr(tmpKey);
        }
        else assert(false);
    }
    //load other moves
    //!WIP
    //always return success (asserts are handling errors...)
    return true;
}

//setters
void Entity::setPosition(const sf::Vector2i& position) {
    //move entity per se
    mPosition = position;
    //move tile info
    mTileInfo->setPosition(world->tileToWindowPosition(position));
}

//action handling functions
bool Entity::executeNextAction(){
    if(mActions.empty()) return false;
    mActions.front()->execute(*this);
    mActions.pop_front();
    return true;
}

//input handling (for player only) - returns if input was received successfully
bool Entity::handleEvent(const sf::Event& event) {
    //get input for turn decisions
    if(mIsPlayer) {
        //wait for animations and other future stuff...
        if(isReadyForInput()){
            //pick a direction and move
            if(event.type == sf::Event::KeyPressed){
                auto direction = sf::Vector2i(0, 0);
                if(event.key.code == sf::Keyboard::W){
                    direction.y = -1;
                }
                else if(event.key.code == sf::Keyboard::S){
                    direction.y = 1;
                }
                else if(event.key.code == sf::Keyboard::A){
                    direction.x = -1;
                }
                else if(event.key.code == sf::Keyboard::D){
                    direction.x = 1;
                }
                else {
                    return false;
                }
                if(direction.x != 0 || direction.y != 0) addAction(new eact::Walk(direction));
                return true;
            }
            else return false;
        }
        else return false;
    }
}

//perform one turn
void Entity::performTurn(){
    //assert world existence in itself...
    assert(world);
    //replenish energy
    if(!isPlayer()) mEnergy += mStatus.get("recovery");
    //if enough energy, make a move
    if(mEnergy >= 1.0 || isPlayer()) {
        //spend energy
        mEnergy -= 1.0;
        //activate passive special moves
        for(auto& move : mSpecialMoves) {
            if(move->getActivationType() == SpecialMove::ACTIV_PASSIVE)
                move->execute(*this);
        }
        //execute action
        if(!noActions()) executeNextAction();
        //update interface
        updateInterface();
    }
}

//update interface
void Entity::updateInterface() {
    //update tile info
    //update life
    mTileInfo->get<tgui::ProgressBar>("life_bar")->setMaximum(getStat("max-life"));
    mTileInfo->get<tgui::ProgressBar>("life_bar")->setValue(getStat("life"));
}

//update graphics
void Entity::updateGraphics(const sf::Time& time) {
    //update animations
    mAnimationHandler.update(time);
}

//gameplay
void Entity::attack(Entity& defender){
    if(mDefaultMeleeMove)
        mDefaultMeleeMove->execute(*this, defender);
    else {
        //!WIP
        std::cout << getName() << " awkwardly bumps into " << defender.getName() << "...\n";
    }
}

//graphics
void Entity::addAnimation(SpecialMove::ANIMATION_TYPE animation){
    mAnimationHandler.addAnimation(*mAnimations[int(animation)]);
}
void Entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    //draw entity sprite
    const auto& entPos = getPosition();
    sf::RectangleShape entSprite(sf::Vector2f(TILE_WIDTH, TILE_HEIGHT));
    entSprite.setPosition(sf::Vector2f(entPos.x * TILE_WIDTH, entPos.y * TILE_HEIGHT));
    entSprite.setTexture(&getTexture());
    entSprite.setTextureRect(getTextureRect());
    target.draw(entSprite, states);
    //draw interface
    target.draw(*mTileInfo, states);
}
sf::View Entity::calcView() const {
    const auto& pos = getPosition();
    sf::View view(sf::Vector2f(pos.x * TILE_WIDTH + TILE_WIDTH/2,
                               pos.y * TILE_HEIGHT + TILE_HEIGHT/2),
                  sf::Vector2f(10 * TILE_WIDTH, 10 * TILE_HEIGHT));
    return view;
}
