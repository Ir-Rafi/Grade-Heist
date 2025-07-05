#include "Collision.h"

bool checkCollision(const sf::FloatRect& b, const std::vector<sf::RectangleShape>& walls) {
    for (auto& w : walls)
        if (b.intersects(w.getGlobalBounds()))
            return true;
    return false;
}
