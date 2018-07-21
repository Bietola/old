#include "Food.h"

///food functions
//constructor
Food::Food():Thing(gTextures[GTXTR_FOOD],rand()%BOARD_W,rand()%BOARD_H,CT_FOOD){
    start(rand()%100);
}
//initialization function
void Food::start(int amnt){
    amount=amnt;
}
//act
void Food::act(){
    ;
}
