#ifndef ENTITYMODELS_H_INCLUDED
#define ENTITYMODELS_H_INCLUDED

#include "MediaProvider.h"
#include "Entity.h"

///Entity Maker (Singleton)
class EntityMaker {
    public:
        //singleton
        EntityMaker() = delete;
        //create an entity from info
        static std::shared_ptr<Entity> create(const std::string& modelKey, bool isPlayer = false);
        //get models
        static const MediaProvider<Entity>& getModels() {return mModels;}
        //load models
        static bool loadModels();
    private:
        //models
        static MediaProvider<Entity> mModels;
};

#endif // ENTITY_MODELS_H
