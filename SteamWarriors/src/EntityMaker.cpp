#include "EntityMaker.h"

///Static variables
MediaProvider<Entity> EntityMaker::mModels;

///Entity models functions
//create an entity from info
std::shared_ptr<Entity> EntityMaker::create(const std::string& modelKey, bool isPlayer) {
    //clone model
    auto clone = mModels.getMedia(modelKey).clone();
    //modify it
    clone->setAsPlayer(isPlayer);
    //return it
    return clone;
}
//load models
bool EntityMaker::loadModels() {
    mModels.loadMediaFromDirectory("./data/entities");
}
