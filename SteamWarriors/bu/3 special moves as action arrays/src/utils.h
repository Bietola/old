#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <cstdlib>
#include <cassert>

#include <SFML/Graphics.hpp>

///---FUNCTION DECLARATIONS---
///functions to get random numbers
//return a random number within a given range
float rrange(float min, float max);
float rrange(float max);

///general arithmetic functions
//function that runs a callable object through a variadic list of any given type
template<typename Func, typename T>
T operate(Func opr, T op1);
template<typename Func, typename T, typename... Args>
T operate(Func opr, T op1, Args... args);

///geometry helper functions
//--arithmetical operations among sf::Rectangles--
//multiply 2 sf::Rect(s) of any type
template<typename T>
sf::Rect<T> multiplySfRects(sf::Rect<T> op1, sf::Rect<T> op2);
//divide 2 sf::Rect(s) of any type
template<typename T>
sf::Rect<T> divideSfRects(sf::Rect<T> op1, sf::Rect<T> op2);
//add 2 sf::Rect(s) of any type
template<typename T>
sf::Rect<T> addSfRects(sf::Rect<T> op1, sf::Rect<T> op2);
//subtract 2 sf::Rect(s) of any type
template<typename T>
sf::Rect<T> subtractSfRects(sf::Rect<T> op1, sf::Rect<T> op2);


///---FUNCTION TEMPLATE DEFINITIONS---
///general arithmetic functions
//function that runs a callable object through a variadic list of any given type
template<typename Func, typename T>
inline T operate(Func opr, T op1)
{
    return op1;
}
template<typename Func, typename T, typename... Args>
inline T operate(Func opr, T op1, Args... args)
{
    return opr(operate(opr, std::forward<Args>(args)...), op1);
}

///geometry helper functions
//multiply 2 sf::Rect(s) of any type
template<typename T>
inline sf::Rect<T> multiplySfRects(sf::Rect<T> op1, sf::Rect<T> op2)
{
    sf::Rect<T> result;
    result.left = op1.left * op2.left;
    result.top = op1.top * op2.top;
    result.width = op1.width * op2.width;
    result.height = op1.height * op2.height;
    return result;
}
//divide 2 sf::Rect(s) of any type
template<typename T>
inline sf::Rect<T> divideSfRects(sf::Rect<T> op1, sf::Rect<T> op2)
{
    sf::Rect<T> result;
    result.left = op1.left / op2.left;
    result.top = op1.top / op2.top;
    result.width = op1.width / op2.width;
    result.height = op1.height / op2.height;
    return result;
}
//add 2 sf::Rect(s) of any type
template<typename T>
inline sf::Rect<T> addSfRects(sf::Rect<T> op1, sf::Rect<T> op2)
{
    sf::Rect<T> result;
    result.left = op1.left + op2.left;
    result.top = op1.top + op2.top;
    result.width = op1.width + op2.width;
    result.height = op1.height + op2.height;
    return result;
}
//subtract 2 sf::Rect(s) of any type
template<typename T>
inline sf::Rect<T> subtractSfRects(sf::Rect<T> op1, sf::Rect<T> op2)
{
    sf::Rect<T> result;
    result.left = op1.left - op2.left;
    result.top = op1.top - op2.top;
    result.width = op1.width - op2.width;
    result.height = op1.height - op2.height;
    return result;
}

#endif // UTILS_H_INCLUDED
