#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <cassert>
#include <deque>
#include <functional>
#include <memory>

#include <TGUI/TGUI.hpp>

#include "interface/TextBox.h"
#include "Animation.h"
#include "AnimationHandler.h"
#include "Status.h"

///class forwarding
class World;

///Entity class
class Entity{
    public:
        //types
        using action_t = std::function<void(Entity& entity)>;
        //static variables
        static World* world;
        static tgui::TextBox::Ptr messageBox;
        //constructor
        Entity(const sf::Vector2i& position,
               const AnimationInfo& idleAnim, const AnimationInfo& attackAnim,
               const Status& status);
        //destructor
        virtual ~Entity();
        //setters
        void setPosition(const sf::Vector2i& position);
        //getters
        const Status& getStatus() const {return mStatus;}
        sf::Vector2i getPosition() const {return mPosition;}
        const sf::IntRect& getTextureRect() const {return mAnimationHandler.getCurrentAnimation().getCurrentFrame();}
        //checkers
        bool isDone() const {return mIsDone;}
        //input handling
        virtual void handleEvent(const sf::Event& event){};
        //logic handling functions
        void handleLogic(const sf::Time& time);
        virtual void customLogic() = 0;
        //action handling
        void addAction(const action_t& action){mActions.push_back(action);}
        bool executeNextAction();
        bool noActions(){return mActions.empty();}
    private:
        //position
        sf::Vector2i mPosition;
        //status
        Status mStatus;
        //actions
        std::deque<action_t> mActions;
        //graphics
        const AnimationInfo* mAttackAnimation;
        const AnimationInfo* mIdleAnimation;
        AnimationHandler mAnimationHandler;
        //true if entity is done processing logic
        bool mIsDone = true;
};

#endif // ENTITY_H_INCLUDED
