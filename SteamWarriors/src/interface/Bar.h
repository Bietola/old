#ifndef BAR_H_INCLUDED
#define BAR_H_INCLUDED

#include <functional>

#include "Interface.h"

namespace gr {

///Bar class
class Bar: public Interface {
    public:
        ///constructors
        Bar(const sf::FloatRect& bounds, const std::function<double()>& valueGetter,
            double maxValue, double startingValue = 0,
            const sf::Color& color = sf::Color::White);
        Bar(const Bar& other);
        ///destructor
        ~Bar() {}
        ///clone
        Bar* clone() {return new Bar(*this);}
        ///functions
        //set bounds
        void setBounds(const sf::FloatRect& bounds) override;
    private:
        ///variables
        sf::RectangleShape mRectangle;
        std::function<double()> mValueGetter;
        double mMaxValue;
        ///functions
        //update
        void parentUpdate(const sf::RenderWindow& window) override;
        //draw
        void parentDraw(sf::RenderTarget& target, sf::RenderStates states) const override;
};

}

#endif
