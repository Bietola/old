#include "Interface.h"

#include "Interface.h"

///Interface functions
//destructor
Interface::~Interface(){
    for(auto& inter : mChildInterfaces){
        delete inter.second;
    }
}
//get child interface by key
Interface::Ptr Interface::getChild(const std::string& key) const {
    try{
        auto interPtr = mChildInterfaces.at(key);
        return interPtr;
    }
    catch(std::out_of_range){
        std::cout << "FATAL ERROR: accessing nonexistent child interface (key = " << key << ")." << std::endl;
        assert(false);
    }
}
//handle event
void Interface::handleEvent(const sf::Event& e){
    if(isActive()){
        //handle parent
        parentHandleEvent(e);
        //make all child inter.s handle events
        for(auto& inter : mChildInterfaces){
            inter.second->handleEvent(e);
        }
    }
}
//handle logic
void Interface::update(const sf::RenderWindow& window){
    if(isActive()){
        //handle parent
        parentUpdate(window);
        //make all child inter.s handle logic
        for(auto& inter : mChildInterfaces){
            inter.second->update(window);
        }
    }
}
//draw interface and all of its child interfaces
void Interface::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    if(isActive()){
        //draw parent and modify states for child nodes
        parentDraw(target, states);
        //apply parent transformation to render states for child nodes
        states.transform *= getTransform();
        //draw child inter.s
        for(const auto& inter : mChildInterfaces){
            inter.second->draw(target, states);
        }
    }
}
//set margins
void Interface::setMargins(const sf::FloatRect& margins){
    //set margins
    mMargins = margins;
    //transform margins according to parent
    const auto& parentBounds = getParent()->getBounds();
    mMargins.left = parentBounds.left + mMargins.left * parentBounds.width;
    mMargins.top = parentBounds.top + mMargins.top * parentBounds.height;
    mMargins.width *= parentBounds.width;
    mMargins.height *= parentBounds.height;
    //make up a rectangle with the modifications applied to the margins
    auto marginMods = sf::FloatRect(mMargins.left, mMargins.top,
                                    -mMargins.width - mMargins.left,
                                    -mMargins.height - mMargins.top);
    //update margins for parent
    mBounds = addSfRects(mBounds, marginMods);
    //update margins for all child nodes
    for(auto& inter : mChildInterfaces){
        inter.second->setBounds(addSfRects(inter.second->getBounds(), marginMods));
    }
}
