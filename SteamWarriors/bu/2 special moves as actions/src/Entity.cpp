#include "Entity.h"

///Entity static variables
World* Entity::world = nullptr;
tgui::TextBox::Ptr Entity::messageBox = std::make_shared<tgui::TextBox>();

///Entity functions
//constructor
Entity::Entity(const std::string& name, const std::string& description,
               const sf::Vector2i& position,
               const AnimationInfo& idleAnim, const AnimationInfo& attackAnim,
               const Status& status):
    Describable(name, description),
    mPosition(position),
    mAnimations{&idleAnim, &attackAnim},
    mStatus(status),
    mAnimationHandler()
{
    addAnimation(ANIM_IDLE);
    //mAnimationHandler.addAnimation(*mAttackAnimation);
}

//destructor
Entity::~Entity(){
    ;
}

//setters
void Entity::setPosition(const sf::Vector2i& position){
    mPosition = position;
}

//action handling functions
bool Entity::executeNextAction(){
    if(mActions.empty()) return false;
    mActions.front()->execute(*this);
    mActions.pop_front();
    return true;
}

//handle logic
void Entity::handleLogic(const sf::Time& time){
    bool isDone = true;
    //assert world existence in itself...
    assert(world);
    //proceed with custom logic
    customLogic();
    //update animations
    mAnimationHandler.update(time);
    isDone &= mAnimationHandler.isDone();
    //execute actions
    if(!noActions()) executeNextAction();
    //report if entity is done processing logic
    mIsDone = isDone;
}

//gameplay
void Entity::attack(Entity& defender){
    mDefaultMeleeMove->setAttacker(this);
    defender.addAction(mDefaultMeleeMove->clone());
}

//graphics
void Entity::addAnimation(Entity::ANIMATION animation){
    mAnimationHandler.addAnimation(*mAnimations[int(animation)]);
}
