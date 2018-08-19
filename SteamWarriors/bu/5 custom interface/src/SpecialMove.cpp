#include "SpecialMove.h"

#include "Entity.h"

//static variables
MediaProvider<SpecialMove> SpecialMove::models;

//constructor
SpecialMove::SpecialMove(const Json::Value& root) {
    assert(loadFromJsonObject(root));
}

//copy constructor
SpecialMove::SpecialMove(const SpecialMove& other):
    Describable(other.mName, other.mDescription),
    mAction(other.mAction->clone()),
    mAnimationType(other.mAnimationType)
{}

//static functions
SpecialMove::ANIMATION_TYPE SpecialMove::stringToAnimationType(const std::string& str) {
    if(str == "idle") {
        return ANIM_IDLE;
    }
    else if(str == "melee") {
        return ANIM_MELEE;
    }
    assert(false);
}

//load all models
bool SpecialMove::loadModels() {
    return models.loadMediaFromDirectory("data/moves/");
}

//load from a json file - returns false on failure
bool SpecialMove::loadFromFile(const std::string& path){
    //open json file
    std::ifstream file(path.c_str(), std::ifstream::binary);
    Json::Value root;
    file >> root;
    //load animation data
    return loadFromJsonObject(root);
}

//create from json data
bool SpecialMove::loadFromJsonObject(const Json::Value& root) {
    //get text info
    mName = root.get("name", "NONAME").asString();
    mDescription = root.get("description", "NODESCRIPTION").asString();
    //get animation type
    auto animationType = root.get("animation", "melee").asString();
    mAnimationType = stringToAnimationType(animationType);
    //get move action
    auto moveActionName = root.get("action", "_ERROR").asString();
    if(moveActionName == "PhysicalAttack")
        mAction = new smov::PhysicalAttack();
    else if(moveActionName == "PoisonousAttack")
        mAction = new smov::PoisonousAttack();
    if(moveActionName == "_ERROR") {
        std::cout << "ERRROR: could not load attack action of name " << moveActionName << " of move " << mName << std::endl;
        return false;
    }
    //return success
    return true;
}

//execute
void SpecialMove::execute(Entity& attacker, boost::optional<Entity&> defender) const {
    //assert action existence
    assert(mAction);
    //graphics
    attacker.addAnimation(getAnimationType());
    //execute action
    mAction->execute(attacker, defender);
}
