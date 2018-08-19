#ifndef EFFECT_H_INCLUDED
#define EFFECT_H_INCLUDED

#include "Describable.h"

class Effect : public Describable {
    public:
        enum class Application {
            Boost,
            Drain
        };
        enum class Duration {
            Time,
            Sickness
        };
    private:
        Status mStatus;
        Duration mDurationType;
        unsigned int mDurationTime = 0;
};

#endif // EFFECT_H_INCLUDED
