#include "comments.h"

//start comms
Comm* start_comm(Comm* comm,char* path,int x,int y,int lines,int lenght){
	//set save path
	comm=(Comm*)malloc(sizeof(Comm));
	comm->path=(char*)malloc(sizeof(path));
	strcpy(comm->path,path);
	FILE* f;
	f=fopen(comm->path,"w");
	fclose(f);
	comm->curLines=0;
	comm->maxLines=lines;
	comm->lineLen=lenght;
	comm->xPos=x;
	comm->yPos=y;
	comm->printPos=0;
	return comm;
}
//add a comment to the comms
void add_comm(Comm* comm,char* str,...){
	FILE *f;
	int j;
	va_list lst;
	va_start(lst,str);
	//comm string interpretation (printed in the comments file)
	f=fopen(comm->path,"a");
	j=0;
	while(str[j]!='\0'){
        //select color (WIP)
        //% - print arguments
        if(str[j]=='%'){
            j++;
            //d - integers
            if(str[j]=='d'){
                fprintf(f,"%d",va_arg(lst,int));
            }
            //f - floats
            else if(str[j]=='f'){
                fprintf(f,"%f",va_arg(lst,double));
            }
            //c - character
            else if(str[j]=='c'){
                fprintf(f,"%c",va_arg(lst,int));
            }
            //s - strings
            else if(str[j]=='s'){
                fprintf(f,"%s",va_arg(lst,char*));
            }
            //nothing
            else{
                j--;
                fprintf(f,"%c",str[j]);
            }
        }
        //~ - easter egg
        else if(str[j]=='~'){
            j++;
            //g - gigio
            if(str[j]=='g'){
                fprintf(f,"gigio");
            }
            //nothing
            else{
                j--;
                fprintf(f,"%c",str[j]);
            }
        }
        //nothing
        else fprintf(f,"%c",str[j]);
        j++;
	}
	fprintf(f,"\n");
	fclose(f);
	va_end(lst);
	//
	if(comm->curLines<comm->maxLines) comm->curLines++;
	//recalculate commPrintPos
	else{
        char c;
        f=fopen(comm->path,"r");
        fseek(f,comm->printPos,SEEK_SET);
        do{
            fscanf(f,"%c",&c);
        }while(c!='\n');
        comm->printPos=ftell(f);
        fclose(f);
	}
}
//print comments
void print_comm(Comm comm){
	FILE *f;
	char c;
	int j=0,x=comm.xPos,y=comm.yPos;
	short mfclr=curFclr,mbclr=curBclr;
	set_pair(COLOR_WHITE,COLOR_BLACK);
	f=fopen(comm.path,"r");
	fseek(f,comm.printPos,SEEK_SET);
	clear_line(comm.xPos,y,comm.lineLen);
	while(j<comm.curLines){
        fscanf(f,"%c",&c);
		if(c!='\n'){
            mvprintw(y,x,"%c",c);
            x++;
		}
		else{
            x=comm.xPos;
            y++;
            j++;
            clear_line(comm.xPos,y,comm.lineLen);
		}
	}
	fclose(f);
	set_pair(mfclr,mbclr);
}
//print comments backwards
void invprint_comm(Comm comm){
    ;
}
