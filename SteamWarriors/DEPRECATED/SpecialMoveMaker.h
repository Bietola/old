#ifndef SPECIALMOVEMAKER_H_INCLUDED
#define SPECIALMOVEMAKER_H_INCLUDED

#include "MediaProvider.h"
#include "SpecialMove.h"

///Special Move Maker (Singleton)
class SpecialMoveMaker {
    public:
        //singleton
        SpecialMoveMaker() = delete;
        //create a special move from info
        static SpecialMove* create(const std::string& modelKey);
        //load models
        static bool loadModels();
    private:
        //models
        static MediaProvider<SpecialMove> mModels;
};

#endif // SPECIALMOVEMAKER_H_INCLUDED
