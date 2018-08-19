#include "AnimationHandler.h"

///ANIMATION HANDLER functions
//constructor
AnimationHandler::AnimationHandler():
    mAnimations(),
    mLoopingAnimation(boost::optional<Animation>())
{
    ;
}
//getters
const Animation& AnimationHandler::getCurrentAnimation() const {
    if(mAnimations.empty()){
        if(!mLoopingAnimation)
            throw std::runtime_error("calling current animation on empty handler");
        else
            return mLoopingAnimation.get();
    }
    else{
        return mAnimations.top();
    }
}
//animation handling functions
void AnimationHandler::addAnimation(const AnimationInfo& animationInfo){
    //only one looping animation is allowed
    if(animationInfo.isLooping){
        mLoopingAnimation = Animation(animationInfo);
    }
    else{
        mAnimations.push(Animation(animationInfo));
    }
}
void AnimationHandler::update(const sf::Time& time){
    if(time != sf::Time::Zero){
        if(mAnimations.empty()){
            if(mLoopingAnimation) mLoopingAnimation.get().update(time);
        }
        else{
            auto remainingTime = mAnimations.top().update(time);
            if(mAnimations.top().isDone())
                mAnimations.pop();
            //update(remainingTime);
        }
    }
}
