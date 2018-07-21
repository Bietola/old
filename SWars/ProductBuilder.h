/*!NOT COMPLETED:
    To be completed only if needed
*/

#ifndef PRODUCTBUILDER_H
#define PRODUCTBUILDER_H

///product builder class
class ProductBuilder:public virtual Builder{
    public:
        //constructors
        ProductBuilder();
        ProductBuilder(int x,int y,Texture *t,COLLISION ct,COLLFACTION cf,int hp,int cdam,Thing *m,int cs);
        //virtual functions fulfillment
        void build();
    protected:
        //virtual inheritance constructors
        ProductBuilder(Thing *m,int cs);
};

#endif
