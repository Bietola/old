#ifndef BUTTON_H
#define BUTTON_H

#include <vector>

#include "Texture.h"

///class forwarding
class Button;

///button group class
class ButtonGroup{
    public:
        //friend classes
        friend class Button;
        //group behaviors
        enum BEHAVIOR{
            ONLY_ONE_CLICKED
        };
        //constructors
        ButtonGroup();
        //functions
        void applyBehaviors(Button *last);
        void addButton(Button *b);
        void removeButton(Button *b);
        void addBehavior(BEHAVIOR b);
        void removeBehavior(BEHAVIOR b);
    private:
        //buttons
        std::vector<Button*> buttons;
        //behaviors
        int behaviors;
};

///button class
class Button{
    public:
        //friend classes
        friend class ButtonGroup;
        //button states
        enum BUTTON_MOUSE_POSITION{
            BMP_OUTSIDE,
            BMP_OVER,
            BMP_CLICKED,
            BMP_UNCLICKED,
            BMP_MAX
        };
        //constructors
        Button(SDL_Rect clickBox,Texture *texture);
        //pseudo-getters
        bool isClicked(){if(buttonMousePosition==BMP_CLICKED) return true;}
        //pseudo-setters
        void invert();
        //functions
        void handleEvent(SDL_Event *e);
        //virtual functions
        virtual void show();
    protected:
        //position, width and height of button
        SDL_Rect clickBox;
        //texture to show
        Texture *texture;
        //mouse position relative to button
        BUTTON_MOUSE_POSITION buttonMousePosition;
        //optional button group
        ButtonGroup *buttonGroup;
        //virtual functions
        void updateButton();
        virtual void cUpdateButton()=0;
        //initialization functions
        void start(SDL_Rect clickBox,Texture *t);
};

///clip button class
//!WARING: do not use an animation as a ClipButton texture.
class ClipButton: public Button{
    public:
        //constructor
        ClipButton(SDL_Rect clickBox,Texture *texture);
        //virtual functions fulfillment
        void show();
    private:
        //clip size
        SDL_Rect clipBox;
        //virtual functions fulfillment
        void cUpdateButton();
        //initialization function
        void start(SDL_Rect clipBox);
};

///texture button
//!WARNING: animations may be used as textures here.
class TextureButton: public Button{
    public:
        //constructor
        TextureButton(SDL_Rect clickBox,Texture *textures[BMP_MAX]);
    private:
        //textures used
        Texture *textures[BMP_MAX];
        //virtual functions fulfillment
        void cUpdateButton();
        //initialization functions
        void start(Texture *txtrs[BMP_MAX]);
};

///alpha button
class ColorButton: public Button{
    public:
        //constructor
        ColorButton(SDL_Rect clickBox,Texture *texture,SDL_Color colors[BMP_MAX]);
    private:
        //colors used
        SDL_Color colors[BMP_MAX];
        //virtual functions fulfillment
        void cUpdateButton();
        //initialization functions
        void start(SDL_Color clrs[BMP_MAX]);
};

#endif // BUTTON_H
