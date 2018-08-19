#ifndef BUTTON_H_INCLUDED
#define BUTTON_H_INCLUDED

#include <memory>
#include <functional>

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

#include "Interface.h"

namespace gr {

///Button class
class Button: public Interface {
    public:
        ///enumerations
        //click status
        enum CLICKSTATUS{
            CLICKST_OUTSIDE,    //mouse outside bounds
            CLICKST_HOVERING,   //mouse inside bounds
            CLICKST_CLICKING,   //mouse inside bounds and clicking
            CLICKST_MAX
        };
        ///type aliases
        //type of function called every time the button is updated
        using click_function_t =  std::function<void(Button &button)>;
        ///constructors
        Button();
        Button(const sf::FloatRect &clickRect);
        Button(const Button& button);
        ///destructor (WIP)
        virtual ~Button(){}
        ///clone
        Button* clone() const override {return new Button(*this);}
        ///accessors
        //button was just clicked
        bool wasJustClicked() const {return mWasJustClicked;}
        //returns number of times the button was clicked
        int getClicks() const {return mClicks;}
        //get the click status
        CLICKSTATUS getClickStatus() const {return mClickStatus;}
        ///setters
        //set number of times the button was clicked
        void setClicks(int clicks){mClicks = clicks;}
        //add a click function
        void addClickFunction(const click_function_t &clickFunction){mClickFunctions.push_back(clickFunction);}

    protected:
        ///override-able virtual functions
        //do something after parent button update
        virtual void parentAfterUpdate(const sf::RenderWindow& window) {}

    private:
        ///variables
        //incremented every time the button is clicked
        int mClicks = 0;
        //true if the button was just clicked
        bool mWasJustClicked = false;
        //click status
        CLICKSTATUS mClickStatus = CLICKST_OUTSIDE;
        //functions called every time the button is updated
        std::vector<click_function_t> mClickFunctions;
        ///interface functions
        //update
        void parentUpdate(const sf::RenderWindow &window) override;
};

}

#endif // BUTTON_H_INCLUDED
