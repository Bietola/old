#ifndef ANIMATIONHANDLER_H_INCLUDED
#define ANIMATIONHANDLER_H_INCLUDED

#include <stack>
#include <boost/optional.hpp>

#include "Animation.h"

///ANIMATION HANDLER class
class AnimationHandler {
    public:
        ///constructor
        AnimationHandler();
        ///checkers
        bool isDone() const {return mAnimations.empty();}
        ///animation handling functions
        void addAnimation(const AnimationInfo& animationInfo);
        void update(const sf::Time& time);
        const Animation& getCurrentAnimation() const;
    //private:
        ///variables
        //animations to be handled
        std::stack<Animation> mAnimations;
        boost::optional<Animation> mLoopingAnimation;
};

#endif // ANIMATIONHANDLER_H_INCLUDED
