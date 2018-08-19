#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <cassert>
#include <map>
#include <memory>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "../utils.h" //!WIP: better management of utility headers
#include "TransBox.h"

///Interface class
class Interface: public TransBox, public sf::Drawable{
    public:
        ///aliases
        using Ptr = Interface*;
        ///constructor
        using TransBox::TransBox;
        ///destructor
        virtual ~Interface();
        ///accessors functions
        //access child interface as a given derived type pointer (return false on conversion failure)
        template<typename T>
        T* getChildAsDerived(const std::string& key) const;
        //access child interface
        Ptr getChild(const std::string& key) const;
        //access child map
        const std::map<std::string, Ptr>& getChildMap() const {return mChildInterfaces;}
        ///state checker functions
        //true if interface is empty
        bool empty() const {return mChildInterfaces.empty();}
        //clears and deletes child interfaces
        void clear() {mChildInterfaces.clear();}
        //true if interface is activated
        bool isActive() const {return mIsActive;}
        ///interface builder functions
        //add child interface to interface
        template<typename T> T* addChild(const std::string& key, T* inter);
        template<typename T, typename... Args> T* addChild(const std::string& key, Args&&... args);
        //interpret child's bounds as relative factors of the parent interface before adding the child
        template<typename T> T* addChildRelative(const std::string& key, T* inter);
        template<typename T, typename... Args> T* addChildRelative(const std::string& key, Args&&... args);
        //set if interface is active (true) or not (false)
        void setActive(bool active){mIsActive = active;}
        ///getters
        //get parent
        const Ptr getParent() const {return mParent;}
        ///setters
        //set margins
        //!WIP check edge cases
        void setMargins(const sf::FloatRect& margins);
        ///execution functions
        //handle event
        void handleEvent(const sf::Event& event);
        //update logic
        void update(const sf::RenderWindow& window);
        //draw
        void draw(sf::RenderTarget& target, sf::RenderStates states) const;
    protected:
        ///parent execution functions (to be overloaded by derived interfaces)
        //handle event
        virtual void parentHandleEvent(const sf::Event& event){};
        //handle logic
        virtual void parentUpdate(const sf::RenderWindow& window){};
        //draw
        virtual void parentDraw(sf::RenderTarget& target, sf::RenderStates states) const {};
        ///variables
        //children interfaces
        std::map<std::string, Ptr> mChildInterfaces;
        //parent interface
        Ptr mParent = nullptr;
        //true if interface is activated (an inactive interface doesn't execute any of its execution functions)
        bool mIsActive = true;
        //margins: a margin is the minimum amount of space between the child's bounds and the actual child
        sf::FloatRect mMargins = sf::FloatRect(0, 0, 0, 0);
};

///Interface template functions
//add a child interface to the parent interface
template<typename T>
T* Interface::addChild(const std::string& key, T* childInterface){
    //check if key was already used
    for(const auto& inter : mChildInterfaces){
        if(inter.first == key) {
            std::cout << "ERROR: trying to add a child interface with an already existing key (key = " << key << ")." << std::endl;
            return nullptr;
        }
    }
    //!DB: std::cout << childInterface->getBounds().left << ", " << childInterface->getBounds().top << ", " << childInterface->getBounds().width << ", " << childInterface->getBounds().height << std::endl;
    //add child to parent
    mChildInterfaces[key] = childInterface;
    //link child to parent
    childInterface->mParent = this;
    //return child interface
    return childInterface;
}
//perfect forwarding version
template<typename T, typename... Args>
inline T* Interface::addChild(const std::string& key, Args&&... args){
    auto inter = new T(std::forward<Args>(args)...);
    return addChild(key, inter);
}
//add a child interface to the parent interface... with some modifications (see header)
template<typename T>
T* Interface::addChildRelative(const std::string& key, T* childInterface){
    //check validity
    if(childInterface == nullptr){
        std::cout << "ERROR: trying to add a null child interface (key = " << key << "." << std::endl;
        return nullptr;
    }
    //resize child interface relatively to interface bounds
    auto childBounds = childInterface->getBounds();
    auto parentBounds = getBounds();
    childInterface->setBounds(sf::FloatRect(parentBounds.left + childBounds.left * parentBounds.width,
                                            parentBounds.top + childBounds.top * parentBounds.height,
                                            childBounds.width * parentBounds.width,
                                            childBounds.height * parentBounds.height));
    //!DB: std::cout << object->getBounds().left << ", " << object->getBounds().top << ", " << object->getBounds().width << ", " << object->getBounds().height << std::endl;
    //add child to parent
    return addChild(key, childInterface);
}
//perfect forwarding version
template<typename T, typename... Args>
inline T* Interface::addChildRelative(const std::string& key, Args&&... args){
    auto inter = new T(std::forward<Args>(args)...);
    return addChildRelative(key, inter);
}
//get a child as its derived class (returns nullptr on failure)
template<typename T>
inline T* Interface::getChildAsDerived(const std::string& key) const {
    return dynamic_cast<T*>(getChild(key));
}

#endif // INTERFACE_H_INCLUDED
