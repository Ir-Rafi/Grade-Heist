#include "CollisionWalls.h"

std::vector<sf::RectangleShape> extractCollisionWalls(const MapLoader& map) {
    const auto& collisionRects = map.getCollisionRects();
    std::vector<sf::RectangleShape> walls;
    walls.reserve(collisionRects.size());
    for (auto& r : collisionRects) {
        sf::RectangleShape w;
        w.setPosition(r.left, r.top);
        w.setSize({r.width, r.height});
        walls.push_back(w);
    }
    return walls;
}
