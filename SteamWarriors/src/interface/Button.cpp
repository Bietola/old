#include "Button.h"

gr::Button::Button(const sf::FloatRect &clickBox):
    Interface(clickBox)
{}
gr::Button::Button():
    Button({0, 0, 0, 0})
{}
gr::Button::Button(const Button& other):
    Interface(other),
    mClicks(other.mClicks),
    mWasJustClicked(other.mWasJustClicked),
    mClickStatus(other.mClickStatus),
    mClickFunctions(other.mClickFunctions)
{}


void gr::Button::parentUpdate(const sf::RenderWindow &window){
    //don't do anything it button isn't active
    if(!mIsActive) return;
    //get mouse position
    sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
    //cursor inside button
    if(getBounds().contains(mousePos)){
        //cursor clicking inside button
        if(sf::Mouse::isButtonPressed(sf::Mouse::Left)){
            if(mClickStatus != CLICKST_CLICKING){
                mClicks++;
                mWasJustClicked = true;
            }
            else mWasJustClicked = false;
            mClickStatus = CLICKST_CLICKING;
        }
        //cursor hovering over button without clicking
        else mClickStatus = CLICKST_HOVERING;
    }
    //cursor outside button
    else mClickStatus = CLICKST_OUTSIDE;
    //call click functions
    for(auto& func : mClickFunctions)
        if(func) func(*this);
}
