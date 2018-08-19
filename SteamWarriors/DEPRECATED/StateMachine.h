#ifndef STATEMACHINE_H_INCLUDED
#define STATEMACHINE_H_INCLUDED

#include <cassert>

///State machine base class
class StateMachine {
    public:
        ///static functions
        //deallocate state and change it to point to the next one
        static void change(StateMachine*& state);
        ///setters
        //sets next state machine only if current next state machine is not null.
        //returns success.
        bool setNext(StateMachine* state);
    private:
        ///static variables
        //next state to be used
        static StateMachine* mNext;
};

#endif // STATEMACHINE_H_INCLUDED
