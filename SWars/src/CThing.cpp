#include "CThing.h"

///controllable thing functions
//(VIRTUAL) show specified thing in relative to the controllable thing
void CThing::showRelativeToThis(Thing *thing){
    thing->show();
}
