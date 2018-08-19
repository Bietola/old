#include "Animation.h"

#include "media.h"

///ANIMATION INFO functions
//load from a json file - returns false on failure
bool AnimationInfo::loadFromFile(const std::string& file){
    //!WIP
    texture = gTextureProvider.getMediaPtr("characters");
    frameRects = {{0, 0, 64, 64}, {64, 0, 64, 64}};
    isLooping = true;
    speed = 5;
    return true;
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
