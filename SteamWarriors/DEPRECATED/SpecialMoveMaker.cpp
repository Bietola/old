#include "SpecialMoveMaker.h"

///Static variables
MediaProvider<SpecialMove> SpecialMoveMaker::mModels;

///Entity models functions
//create a special move from info
SpecialMove* SpecialMoveMaker::create(const std::string& modelKey) {
    //clone model
    auto clone = mModels.getMedia(modelKey).clone();
    //return it
    return clone;
}
//load models
bool SpecialMoveMaker::loadModels() {
    mModels.loadMediaFromDirectory("./data/moves");
}
