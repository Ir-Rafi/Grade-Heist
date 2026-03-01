#include "ui/isend.h"
#include "core/Constants.h"
#include <cmath>

static sf::FloatRect createLineAABB(const sf::Vector2f& p1,
                                    const sf::Vector2f& p2,
                                    float thickness = 2.f)
{
    float left   = std::min(p1.x, p2.x);
    float top    = std::min(p1.y, p2.y) - thickness/2.f;
    float width  = std::abs(p2.x - p1.x);
    float height = std::abs(p2.y - p1.y) + thickness;

    return { left, top, width, height };
}

bool isend(const std::vector<sf::RectangleShape>& walls,
           const sf::Vector2f& pos_player,
           const sf::Vector2f& pos_enemy)
{
    // Range check: if enemy >= 100 units away, never end the game
    if (dist(pos_enemy, pos_player) >= 100.f)
        return false;

    bool wallBlocks = false;

    // Obstacle check: build a thin AABB "line" between enemy and player
    sf::FloatRect line = createLineAABB(pos_enemy, pos_player, 2.f);
    for (auto& w : walls) {
        if (line.intersects(w.getGlobalBounds()))
            wallBlocks = true;
    }

    // If no wall intersects the line, enemy has clear sight -> game over
    return (!wallBlocks);
}
