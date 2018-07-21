///INCLUDE GUARD
#ifndef MEMUTILS_H
#define MEMUTILS_H

///INCLUDE
#include<stdlib.h>
#include<string.h>

///CONSTANTS
#define BUF_LEN  500

///STRING UTILS FUNCTIONS
//reallocates strings in the most efficient way possible (until the terminator)
void cut(char* str);
//copies a string inside a new one, and allocates the needed memory
void mstrcpy(char* paste,char* copy);

///DYNAMIC ALLOCATION UTILS FUNCTIONS
//expand a dynamic allocated pointer (made to be used every time an element is added to the array)
void* memexpand(void* arr,int maxEle,int eleSize,int memExp);

#endif
