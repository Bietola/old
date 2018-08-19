#include "Animation.h"

#include "media.h"

///ANIMATION INFO functions
//constructors
AnimationInfo::AnimationInfo(const Json::Value& root) {
    assert(loadFromJsonObject(root));
}
//load from a json object - return false on failure
bool AnimationInfo::loadFromJsonObject(const Json::Value& root) {
    texture = gTextureProvider.getMediaPtr(root.get("texture", "NOGRA").asString());
    isLooping = root.get("isLooping", false).asBool();
    speed = root.get("speed", 1).asUInt();
    Json::Value jFrameRects;
    assert(jFrameRects = root["frameRects"]);
    for(std::size_t j = 0; j < jFrameRects.size(); ++j){
        Json::Value jFrameRect;
        assert(jFrameRect = jFrameRects[j]);
        assert(jFrameRect.size() == 4);
        frameRects.push_back({jFrameRect[0].asInt(),
                              jFrameRect[1].asInt(),
                              jFrameRect[2].asInt(),
                              jFrameRect[3].asInt()});
    }
}
//load from a json file - returns false on failure
bool AnimationInfo::loadFromFile(const std::string& path){
    //open json file
    std::ifstream file(path.c_str(), std::ifstream::binary);
    Json::Value root;
    file >> root;
    //get specific animation
    /*!DEPRECATED
    auto name = path.substr(path.find(":") + 1);
    std::cout << name << std::endl;
    if(name == "") jAnim = root;
    else assert(jAnim = root[name]);
    */
    //load animation data
    return loadFromJsonObject(root);
}

///ANIMATION functions
//constructor
Animation::Animation(const AnimationInfo& info):
    mInfo(&info)
{}

//graphics methods
sf::Time Animation::update(const sf::Time& time) {
    sf::Time remainingTime = sf::Time::Zero;
    auto initialFrameIndex = mCurrentFrameIndex;
    auto frameIncrement = mInfo->speed * time.asSeconds() + mFrameDebt;
    mCurrentFrameIndex += int(frameIncrement);
    mFrameDebt = frameIncrement - int(frameIncrement);
    if(mCurrentFrameIndex >= getFramesNum()){
        if(mInfo->isLooping){
            mCurrentFrameIndex %= getFramesNum();
            remainingTime = sf::Time::Zero;
        }
        else{
            mIsDone = true;
            remainingTime = sf::seconds(time.asSeconds() - float(mCurrentFrameIndex - initialFrameIndex) / mInfo->speed);
        }
    }
    return remainingTime;
}
