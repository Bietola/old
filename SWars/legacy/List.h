#ifndef LIST_H
#define LIST_H

#include "Thing.h"

///LIST class
class List{
    public:
        //constructors
        List();
        List(Thing *data);
        //insert functions
        void initInsert(Thing *data);
        void initHeadInsert(Thing *data);
        void headInsert(Thing *data);
        void initTailInsert(Thing *data);
        void tailInsert(Thing *data);
        void midInsert(Thing *data);
        void insert(Thing *data);
        //remove functions
        void headRemove();
        void tailRemove();
        void remove(int pos);
        //iterator functions
        void increment();
        void restart();
    private:
        //structures
        struct Node{
            Thing *data;
            Node *next;
        };
        //variables
        Node *head;
        Node *i;
};

#endif
