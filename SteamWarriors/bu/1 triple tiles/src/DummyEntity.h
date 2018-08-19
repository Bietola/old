#ifndef DUMMYENTITY_H_INCLUDED
#define DUMMYENTITY_H_INCLUDED

#include "Entity.h"

///Dummy entity class
class DummyEntity: public Entity{
    public:
        using Entity::Entity;
        void customLogic() override;
};


#endif // DUMMYENTITY_H_INCLUDED
