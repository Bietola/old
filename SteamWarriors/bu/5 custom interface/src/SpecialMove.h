#ifndef SPECIALMOVE_H_INCLUDED
#define SPECIALMOVE_H_INCLUDED

#include <vector>
#include <exception>
#include <fstream>

#include <boost/optional.hpp>
#include <json/json.h>

#include "Describable.h"
#include "Status.h"
#include "Effect.h"
#include "MediaProvider.h"
#include "actions.h"
#include "moves.h"

class Entity;

///SpecialMove class
class SpecialMove: public Describable {
    public:
        //friends
        friend Entity;
        //enumerations
        enum ANIMATION_TYPE {
            ANIM_IDLE,
            ANIM_MELEE,
            ANIM_MAX
        };
        enum ACTIVATION_TYPE {
            ACTIV_PASSIVE,
            ACTIV_ACTIVE
        };
        //constructors
        SpecialMove():
            SpecialMove("NONAME", "NODESCRIPTION",
                        ANIM_MELEE, ACTIV_ACTIVE,
                        nullptr /*!REPLACE WITH NullSpecialAction*/){}
        SpecialMove(const std::string& name, const std::string& description,
                    ANIMATION_TYPE animationType, ACTIVATION_TYPE activationType,
                    smov::SpecialAction* action):
            Describable(name, description),
            mAnimationType(animationType),
            mActivationType(activationType),
            mAction(action) {}
        SpecialMove(const Json::Value& root);
        //copy constructor
        SpecialMove(const SpecialMove& other);
        //clone
        SpecialMove* clone() const {return new SpecialMove(*this);}
        //getters
        static const MediaProvider<SpecialMove>& getModels() {return models;}
        ANIMATION_TYPE getAnimationType() const {return mAnimationType;}
        ACTIVATION_TYPE getActivationType() const {return mActivationType;}
        //execute action by said attacker on said defender
        void execute(Entity& attacker, boost::optional<Entity&> defender = boost::optional<Entity&>()) const;
        //media functions
        static bool loadModels();
        bool loadFromFile(const std::string& path);
        bool loadFromJsonObject(const Json::Value& root);
        //graphics functions
        static ANIMATION_TYPE stringToAnimationType(const std::string& str);
    private:
        //static variables
        static MediaProvider<SpecialMove> models;
        //variables
        smov::SpecialAction* mAction;
        ANIMATION_TYPE mAnimationType;
        ACTIVATION_TYPE mActivationType;
};

#endif // SPECIALMOVE_H_INCLUDED
