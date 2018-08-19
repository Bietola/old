#include <SFML/Graphics.hpp>

///sfml vector operations
template<typename T>
inline sf::Vector2<T> operator+(const sf::Vector2<T>& lhs, const sf::Vector2<T>& rhs) {
    return sf::Vector2<T>(lhs.x + rhs.x, lhs.y + rhs.y);
}
