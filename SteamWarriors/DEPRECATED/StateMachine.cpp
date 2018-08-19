#include "StateMachine.h"

///StateMachine static variables
//next state to be set by change function
StateMachine* StateMachine::mNext = nullptr;

///StateMachine functions
//change state (static)
void StateMachine::change(StateMachine*& state)
{
    assert(state);
    if(mNext != nullptr)
    {
        delete state;
        state = mNext;
        mNext = nullptr;
    }
}
//set next state - return false on failure
bool StateMachine::setNext(StateMachine* state)
{
    if(mNext != nullptr || state == nullptr) return false;
    mNext = state;
    return true;
}
