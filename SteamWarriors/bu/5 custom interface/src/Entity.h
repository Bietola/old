#ifndef ENTITY_H_INCLUDED
#define ENTITY_H_INCLUDED

#include <cassert>
#include <deque>
#include <functional>
#include <memory>

#include <json/json.h>

#include "Interface/Bar.h"
#include "Interface/TextBox.h"

#include "MediaProvider.h"
#include "Describable.h"
#include "Animation.h"
#include "AnimationHandler.h"
#include "SpecialMove.h"
#include "DialogTree.h"
#include "AI.h"
#include "actions.h"
#include "media.h"

///class forwarding
class World;

///Entity class
class Entity : public Describable, public sf::Drawable {
    public:
        //friends
        friend World;
        //aliases
        using AnimationArray = std::array<const AnimationInfo*, SpecialMove::ANIM_MAX>;
        //static variables
        static World* world;
        //constructor
        Entity();
        Entity(const std::string& name, const std::string& description, const std::string& faction,
               const sf::Vector2i& position,
               const eai::EntityAI* brain,
               const AnimationArray& animations,
               const DialogTree* dialog,
               const SpecialMove* defaultMeleeMove,
               const Status& status);
        //copy constructor
        Entity(const Entity& entity);
        //destructor
        virtual ~Entity();
        //clone entity
        std::shared_ptr<Entity> clone() const {return std::make_shared<Entity>(*this);}
        //loaders
        bool loadFromFile(const std::string& filePath);
        //setters
        void setAsPlayer(bool isPlayer) {mIsPlayer = isPlayer;}
        void setPosition(const sf::Vector2i& position);
        void applyDamage(double damage) {mStatus["life"] -= damage;}
        //getters
        const std::string& getFaction() const {return mFaction;}
        const Status& getStatus() const {return mStatus;}
        double getStat(const std::string& statName) const {return mStatus.mStats.at(statName);}
        const sf::Vector2i& getPosition() const {return mPosition;}
        const sf::Texture& getTexture() const {return mAnimationHandler.getCurrentAnimation().getTexture();}
        const sf::IntRect& getTextureRect() const {return mAnimationHandler.getCurrentAnimation().getCurrentFrame();}
        const SpecialMove* getDefaultMeleeMove() const {return mDefaultMeleeMove;}
        const DialogTree* getDialogTree() const {return mDialog;}
        //checkers
        bool isPlayer() const {return mIsPlayer;}
        bool isReadyForInput() const {return mAnimationHandler.isDone();}
        bool canTalk() const {return mDialog;}
        //input handling (for player only) - returns if input has been successfully received
        bool handleEvent(const sf::Event& event);
        //perform one turn (if enough energy)
        void performTurn();
        //action handling
        void addAction(eact::Action* action) {mActions.push_back(action);}
        bool executeNextAction();
        bool noActions() {return mActions.empty();}
        //gameplay
        void think() {if(mBrain) mBrain->think(*this);}
        void attack(Entity& defender);
        void talk(gr::TextBox& textBox);
        //graphics
        void updateGraphics(const sf::Time& time);
        void addAnimation(SpecialMove::ANIMATION_TYPE animation);
        void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
        sf::View calcView() const;
        //interface
        void updateInterface();
    private:
        ///general info
        //true if entity is player
        bool mIsPlayer = false;
        //main status
        Status mStatus;
        //stuff spent to perform a turn
        double mEnergy = 0;
        //faction
        std::string mFaction;
        ///dialog system
        const DialogTree* mDialog = nullptr;
        ///AI
        //entity's brain
        const eai::EntityAI* mBrain;
        ///world info
        //position
        sf::Vector2i mPosition;
        ///combat
        //default melee move
        const SpecialMove* mDefaultMeleeMove = nullptr;
        //special attack moves
        std::vector<const SpecialMove*> mSpecialMoves;
        ///technical info
        //actions
        std::deque<eact::Action*> mActions;
        ///graphics
        //combat animations
        AnimationArray mAnimations;
        AnimationHandler mAnimationHandler;
        ///interface
        gr::Interface::Ptr mTileInfo = nullptr;
};

#endif // ENTITY_H_INCLUDED
