#include "Bar.h"

//constructor
gr::Bar::Bar(const sf::FloatRect& bounds, const std::function<double()>& valueGetter,
             double maxValue, double startingValue,
             const sf::Color& color):
    Interface(bounds),
    mMaxValue(maxValue),
    mValueGetter(valueGetter)
{
    mRectangle.setFillColor(color);
    mRectangle.setSize(sf::Vector2f(0, bounds.height));
}
gr::Bar::Bar(const Bar& other):
    Interface(other),
    mRectangle(other.mRectangle),
    mValueGetter(mValueGetter),
    mMaxValue(mMaxValue)
{
    ;
}


//get bounds
void gr::Bar::setBounds(const sf::FloatRect& bounds) {
    if(bounds != mBounds) {
        mBounds = bounds;
        mRectangle.setPosition(sf::Vector2f(bounds.left, bounds.top));
        mRectangle.setSize(sf::Vector2f(0, bounds.height));
    }
}

//update
void gr::Bar::parentUpdate(const sf::RenderWindow& window) {
    mRectangle.setSize(sf::Vector2f(mValueGetter() * getBounds().width / mMaxValue, getBounds().height));
}

//draw
void gr::Bar::parentDraw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(mRectangle, states);
}
