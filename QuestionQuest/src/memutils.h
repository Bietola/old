///INCLUDE GUARD
#ifndef MEMUTILS_H
#define MEMUTILS_H

///INCLUDE
#include<stdlib.h>
#include<string.h>

///CONSTANTS
#define BUF_LEN  500

#ifdef __cplusplus
namespace{
#endif

///FUNCTION DECLARATIONS
void cut(char*);
void* memexpand(void*,int,int,int);

///STRING UTILS FUNCTIONS
//reallocates strings in the most efficient way possible (until the terminator)
void cut(char* str){
	unsigned int j;
	for(j=0;j<strlen(str);j++){
		if(str[j]=='\0'){
			str=(char*)realloc(str,sizeof(char)*j);
			break;
		}
	}
}
//copies a string inside a new one, and allocates the needed memory
void mstrcpy(char* paste,char* copy){
    paste=(char*)malloc(sizeof(char)*strlen(copy));
    strcpy(paste,copy);
}

///DYNAMIC ALLOCATION UTILS FUNCTIONS
//expand a dynamic allocated pointer (made to be used every time an element is added to the array)
void* memexpand(void* arr,int maxEle,int eleSize,int memExp){
    if(maxEle==0) return malloc(eleSize*memExp);
    else if(maxEle==memExp*(maxEle/(memExp+1)+1)) return realloc(arr,eleSize*(maxEle+memExp+1));
    else return arr;
}

#ifdef __cplusplus
}
#endif

#endif
