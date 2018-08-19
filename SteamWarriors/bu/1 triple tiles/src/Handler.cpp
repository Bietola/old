#include "Handler.h"

///Handler static variables
//next handler to be set by change function
Handler* Handler::mNext = nullptr;

///Handler functions
//change handler (static)
void Handler::change(Handler*& handler)
{
    assert(handler);
    if(mNext != nullptr)
    {
        delete handler;
        handler = mNext;
        mNext = nullptr;
    }
}
//set next handler - return false on failure
bool Handler::setNext(Handler* handler)
{
    if(mNext != nullptr || handler == nullptr) return false;
    mNext = handler;
    return true;
}
