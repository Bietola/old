#ifndef TRANSBOX_H_INCLUDED
#define TRANSBOX_H_INCLUDED

#include <SFML/Graphics.hpp>

namespace gr {

///TransBox class
class TransBox: public sf::Transformable {
    public:
        //constructor
        TransBox(){}
        TransBox(const sf::FloatRect& bounds):
            mBounds(bounds){}
        TransBox(const TransBox& other):
            mBounds(other.mBounds){}
        //get transformed bounds
        sf::FloatRect getBounds() const {return getTransform().transformRect(mBounds);}
        //set bounds
        virtual void setBounds(const sf::FloatRect& bounds){mBounds = bounds;}
        //deleted functions
        void setRotation(float) = delete;
        void rotate(float) = delete;
    protected:
        //bounds
        sf::FloatRect mBounds;
};

}


#endif // TRANSBOX_H_INCLUDED
