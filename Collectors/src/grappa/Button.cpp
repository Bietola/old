#include "Button.h"

///Button group functions
//constructor
ButtonGroup::ButtonGroup(){
    behaviors=0;
}
//apply behaviors buttons
void ButtonGroup::applyBehaviors(Button *last){
    if((behaviors & ONLY_ONE_CLICKED) && last->isClicked()){
        for(int j=0;j<buttons.size();j++){
            if(buttons[j]!=last && buttons[j]->isClicked()) buttons[j]->invert();
        }
    }
}
//add button to buttons
void ButtonGroup::addButton(Button *b){
    assert(b!=NULL);
    b->buttonGroup=this;
    buttons.push_back(b);
}
//remove button from buttons
void ButtonGroup::removeButton(Button *b){
    assert(b!=NULL);
    b->buttonGroup=NULL;
    for(int j=0;j<buttons.size();j++){
        if(buttons[j]==b){
            buttons.erase(buttons.begin()+j);
            break;
        }
    }
}
//add behavior flag to behaviors
void ButtonGroup::addBehavior(BEHAVIOR b){
    behaviors |= b;
}
//remove behavior flag from behaviors
void ButtonGroup::removeBehavior(BEHAVIOR b){
    behaviors ^= b;
}

///Button functions
//constructor
Button::Button(SDL_Rect clickBox,Texture *texture){
    start(clickBox,texture);
}
//initialization function
void Button::start(SDL_Rect cbox,Texture *t){
    clickBox=cbox;
    texture=t;
}
//invert button click state
void Button::invert(){
    if(buttonMousePosition==BMP_CLICKED) buttonMousePosition=BMP_UNCLICKED;
    else if(buttonMousePosition==BMP_UNCLICKED) buttonMousePosition=BMP_CLICKED;
}
//show the button on screen
void Button::show(){
    texture->show(clickBox.x,clickBox.y);
}
//handle events, make out where the mouse is in relation to the button
void Button::handleEvent(SDL_Event *e){
    //check if event is a mouse event
    if(e->type==SDL_MOUSEMOTION || e->type==SDL_MOUSEBUTTONDOWN || e->type==SDL_MOUSEBUTTONUP){
        //determine mouse position
        int x,y;
        SDL_GetMouseState(&x,&y);
        //check if mouse is inside or outside of button
        int wxPos=clickBox.x+clickBox.w;
        int hyPos=clickBox.y+clickBox.h;
        BUTTON_MOUSE_POSITION mBmp=buttonMousePosition;
        if((x>clickBox.x && x<wxPos) && (y>clickBox.y && y<hyPos)){
            //check if mouse button has been pressed, release, or not pressed
            if(e->type==SDL_MOUSEMOTION) buttonMousePosition=BMP_OVER;
            else if(e->type==SDL_MOUSEBUTTONDOWN) buttonMousePosition=BMP_CLICKED;
            else if(e->type==SDL_MOUSEBUTTONUP) buttonMousePosition=BMP_UNCLICKED;
        }
        else buttonMousePosition=BMP_OUTSIDE;
        //update button
        if(mBmp!=buttonMousePosition) updateButton();
    }
}
//update the button
void Button::updateButton(){
    if(buttonGroup!=NULL) buttonGroup->applyBehaviors(this);
    cUpdateButton();
}

///Clip Button functions
//constructors
ClipButton::ClipButton(SDL_Rect clickBox,Texture *texture):Button(clickBox,texture){
    start(clickBox);
}
//initialization functions
void ClipButton::start(SDL_Rect clipbox){
    clipBox=clipbox;
}
//show the button on screen
void ClipButton::show(){
    texture->show(&clipBox,new SDL_Rect{clickBox.x,clickBox.y,clickBox.w,clickBox.h});
}
//update the button texture every time a mouse event occurs
void ClipButton::cUpdateButton(){
    clipBox={clipBox.w*(int)buttonMousePosition,0,clipBox.w,clipBox.h};
}

///Texture Button functions
//constructors
TextureButton::TextureButton(SDL_Rect clickBox,Texture *textures[Button::BMP_MAX]):Button(clickBox,textures[0]){
    start(textures);
}
//initialization functions
void TextureButton::start(Texture *txtrs[Button::BMP_MAX]){
    for(int j=0;j<BMP_MAX;j++) textures[j]=txtrs[j];
}
//update the button texture every time a mouse event occurs
void TextureButton::cUpdateButton(){
    texture=textures[(int)buttonMousePosition];
}

///Color Button Functions
//constructors
ColorButton::ColorButton(SDL_Rect clickBox,Texture *texture,SDL_Color colors[Button::BMP_MAX]):Button(clickBox,texture){
    start(colors);
}
//initialization function
void ColorButton::start(SDL_Color clrs[Button::BMP_MAX]){
    for(int j=0;j<BMP_MAX;j++) colors[j]=clrs[j];
}
//update the button texture every time a mouse event occurs
void ColorButton::cUpdateButton(){
    SDL_Color c=colors[(int)buttonMousePosition];
    texture->setColor(c.r,c.g,c.b);
}
