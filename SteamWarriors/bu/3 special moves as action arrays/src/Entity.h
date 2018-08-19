#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <cassert>
#include <deque>
#include <functional>
#include <memory>

#include <TGUI/TGUI.hpp>

#include "interface/TextBox.h"
#include "Describable.h"
#include "Animation.h"
#include "AnimationHandler.h"
#include "SpecialMove.h"

///class forwarding
class World;

///Entity class
class Entity : public Describable {
    public:
        //types
        enum ANIMATION {
            ANIM_IDLE,
            ANIM_ATTACK,
            ANIM_MAX
        };
        //static variables
        static World* world;
        static tgui::TextBox::Ptr messageBox;
        //constructor
        Entity(const std::string& name, const std::string& description,
               const sf::Vector2i& position,
               const AnimationInfo& idleAnim, const AnimationInfo& attackAnim,
               const Status& status);
        //destructor
        virtual ~Entity();
        //setters
        void setPosition(const sf::Vector2i& position);
        void applyDamage(double damage) {mStatus["life"] -= damage;}
        //getters
        const Status& getStatus() const {return mStatus;}
        const sf::Vector2i& getPosition() const {return mPosition;}
        const sf::Texture& getTexture() const {return mAnimationHandler.getCurrentAnimation().getTexture();}
        const sf::IntRect& getTextureRect() const {return mAnimationHandler.getCurrentAnimation().getCurrentFrame();}
        //checkers
        bool isDone() const {return mIsDone;}
        //input handling
        virtual void handleEvent(const sf::Event& event){};
        //logic handling functions
        void handleLogic(const sf::Time& time);
        virtual void customLogic() = 0;
        //action handling
        void addAction(eact::Action* action){mActions.push_back(action);}
        bool executeNextAction();
        bool noActions(){return mActions.empty();}
        //gameplay
        void attack(Entity& defender);
        //graphics
        void addAnimation(ANIMATION animation);
    private:
        //position
        sf::Vector2i mPosition;
        //main status
        Status mStatus;
        //default melee move
        SpecialMove* mDefaultMeleeMove = nullptr;
        //special attack moves
        std::vector<SpecialMove*> mSpecialMoves;
        //actions
        std::deque<eact::Action*> mActions;
        //graphics
        std::array<const AnimationInfo*, ANIM_MAX> mAnimations;
        AnimationHandler mAnimationHandler;
        //true if entity is done processing logic
        bool mIsDone = true;
};

#endif // ENTITY_H_INCLUDED
