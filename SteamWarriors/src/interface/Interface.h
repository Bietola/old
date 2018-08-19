#ifndef INTERFACE_H_INCLUDED
#define INTERFACE_H_INCLUDED

#include <cassert>
#include <map>
#include <memory>
#include <iostream>
#include <boost/optional.hpp>

#include <SFML/Graphics.hpp>

#include "../utils.h" //!WIP: better management of utility headers
#include "TransBox.h"

namespace gr {

///Interface class
class Interface: public gr::TransBox, public sf::Drawable{
    public:
        ///aliases
        using Ptr = Interface*;
        ///constructor
        using TransBox::TransBox;
        ///copy constructor
        Interface(const Interface& other);
        ///destructor
        virtual ~Interface();
        ///interface manipulation
        //clone
        virtual Ptr clone() const {return new Interface(*this);}
        ///accessors functions
        //access child interface as a given derived type pointer (return false on conversion failure)
        template<typename T>
        T& get(const std::string& key) const;
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
        template<typename T> T* add(const std::string& key, T* inter);
        template<typename T, typename... Args> T* add(const std::string& key, Args&&... args);
        //interpret child's bounds as relative factors of the parent interface before adding the child
        template<typename T> T* addRelative(const std::string& key, T* inter);
        template<typename T, typename... Args> T* addRelative(const std::string& key, Args&&... args);
        //set if interface is active (true) or not (false)
        void setActive(bool active){mIsActive = active;}
        ///getters
        //get parent
        const Ptr getParent() const {return mParent;}
        ///setters
        //set margins
        void setMargins(const sf::FloatRect& margins); //!WIP check edge cases
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
T* Interface::add(const std::string& key, T* childInterface){
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
inline T* Interface::add(const std::string& key, Args&&... args){
    auto inter = new T(std::forward<Args>(args)...);
    return add(key, inter);
}
//add a child interface to the parent interface... with some modifications (see header)
template<typename T>
T* Interface::addRelative(const std::string& key, T* childInterface){
    //check validity
    if(childInterface == nullptr){
        std::cout << "ERROR: trying to add a null child interface (key = " << key << "." << std::endl;
        return nullptr;
    }
    //resize child (and all of its children) relatively to the parent
    auto resizeBounds = [parentBounds = getBounds()](Interface* childInterface) {
        auto childBounds = childInterface->getBounds();
        childInterface->setBounds(sf::FloatRect(parentBounds.left + childBounds.left * parentBounds.width,
                                                parentBounds.top + childBounds.top * parentBounds.height,
                                                childBounds.width * parentBounds.width,
                                                childBounds.height * parentBounds.height));
    };
    resizeBounds(childInterface);
    for(auto ele : childInterface->mChildInterfaces) {
        resizeBounds(ele.second);
    }
    //!DB: std::cout << object->getBounds().left << ", " << object->getBounds().top << ", " << object->getBounds().width << ", " << object->getBounds().height << std::endl;
    //add child to parent
    return add(key, childInterface);
}
//perfect forwarding version
template<typename T, typename... Args>
inline T* Interface::addRelative(const std::string& key, Args&&... args){
    auto inter = new T(std::forward<Args>(args)...);
    return addRelative(key, inter);
}
//get a child as its derived class (returns nullptr on failure)
template<typename T>
inline T& Interface::get(const std::string& key) const {
    //get child interface by key
    try{
        auto interPtr = mChildInterfaces.at(key);
        return dynamic_cast<T&>(*interPtr);
    }
    catch(std::out_of_range){
        std::cout << "FATAL ERROR: accessing nonexistent child interface (key = " << key << ")." << std::endl;
        assert(false);
    }
}

}

#endif // INTERFACE_H_INCLUDED
