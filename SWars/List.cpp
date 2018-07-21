#include "List.h"

///functions
///constructors
void List::List(){
    head=NULL;
    restart();
}
void List::List(Thing *data){
    initInsert(data);
    restart();
}
///functions
//initialize list
void List::initInsert(Thing *data){
    head=new Node;
    head->data=data;
    head->next=NULL;
    restart();
}
//insert before head with initialization check
void List::initHeadInsert(Thing *data){
    if(head==NULL) initInsert(data);
    else headInsert(data);
}
//insert before head without initialization check
void List::headInsert(Thing *data){
    Node *tmp=head;
    head=new Node;
    head->data=data;
    head->next=tmp;
}
//insert after tail with initialization check
void List::initTailInsert(Thing *data){
    if(head==NULL) initInsert(data);
    else headInsert(data);
}
//insert after tail without initialization check
void List::sTailInsert(Thing *data){
    while(i->next!=NULL){
        increment();
    }
    i->next=new Node;
    i->next->data=data;
    i->next->next=NULL;
}
//insert in any place but before the head and after the tail
void List::midInsert(Thing *data){
    Node *tmp=i->next;
    i->next=new Node();
    i->next->data=data;
    i->next->next=tmp;
}
//insert in any place
void List::insert(Thing *data){
    if(i==NULL){
        initInsert(data);
    }
    else if(i==head){
        headInsert(data);
    }
    else if(i->next==NULL){
        tailInsert(data);
    }
    else{
        midInsert(data);
    }
}
//remove from head
void List::headRemove(){
    Node *tmp=head->next;
    delete head;
    head=tmp;
}
//remove from tail
void List::tailRemove(){
    if(head->next==NULL){
        delete i;
        head=NULL;
        i=head;
    }
}
//remove from middle (not head or tail)
void List::midRemove(){
    Node *tmp=i->next;
    delete i;
    b->next=tmp;
    i=tmp;
}
//manually increment global iterator
void List::increment(){
    if(i->next->next!=NULL){
        i=i->next;
    }
}
//manually reset global iterator
void List::restart(){
    i=head;
}
