///INCLUDE GUARD
#ifndef COMMENTS_H
#define COMMENTS_H

///INCLUDES
#include<string.h>
#include<curses.h>

#include "memutils.h"
#include "palette.h"

///COMM STRUCTURE
typedef struct{
    int curLines;	//current lines of comments
    int maxLines;	//max lines of comments
    int lineLen;    //the length of a comment line (character-wise)
    int printPos;   //the last position analized by the function "comm_print" in the comments file
    int xPos;		//x position
    int yPos;		//y position
    char* path;     //comments file path
}Comm;

///GLOBAL FUNCTIONS
//start comms
Comm* start_comm(Comm* comm,char* path,int x,int y,int lines,int lenght);
//add a comment to the comms
void add_comm(Comm* comm,char* str,...);
//print comments
void print_comm(Comm comm);
//print comments backwards
void invprint_comm(Comm comm);

#endif
