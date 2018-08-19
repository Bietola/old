#ifndef ANIMATION_H_INCLUDED
#define ANIMATION_H_INCLUDED

#include <cassert>
#include <vector>

#include <json/json.h>
#include <SFML/Graphics.hpp>

///ANIMATION INFO class
class AnimationInfo {
    public:
        //friends
        friend class Animation;
        friend class AnimationHandler;
        //texture
        const sf::Texture* texture;
        //true if animation is looping
        bool isLooping;
        //animation speed in frames per seconds
        unsigned int speed;
        //frame rectangles
        std::vector<sf::IntRect> frameRects;
        //load from a json file
        bool loadFromFile(const std::string& file);
};

///ANIMATION class
class Animation {
    public:
        ///constructors
        explicit Animation(const AnimationInfo& animationInfo);
        ///getters
        const sf::Texture& getTexture() const {return *(mInfo->texture);}
        const sf::IntRect& getCurrentFrame() const {return mInfo->frameRects[mCurrentFrameIndex];}
        bool isDone() const {return mIsDone;}
        bool isLooping() const {return mInfo->isLooping;}
        std::size_t getFramesNum() const {return mInfo->frameRects.size();}
        ///graphic functions
        //returns the time not used up by the animation if the animation is done
        sf::Time update(const sf::Time& time);
    private:
        ///variables
        //animation info
        const AnimationInfo* mInfo;
        //index indicating first animation frame
        std::size_t mCurrentFrameIndex = 0;
        //frame debt
        float mFrameDebt = 0;
        //true when animation is done (never true if loop mode is enabled)
        bool mIsDone = false;
};

#endif // ANIMATION_H_INCLUDED
