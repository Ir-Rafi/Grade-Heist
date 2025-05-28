// Minimap.h
#ifndef MINIMAP_H
#define MINIMAP_H

#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"

class Minimap {
public:
    Minimap(const std::vector<sf::RectangleShape>& walls,
            const sf::Sprite& playerSprite,
            const sf::RectangleShape& enemyShape,
            float scale,
            sf::Vector2f position);

    // Draw the minimap overlay to the provided window
    void draw(sf::RenderWindow& window);

    // Retrieve the minimap’s size (in pixels) for layout purposes
    sf::Vector2f getSize() const;

private:
    std::vector<sf::RectangleShape> walls;
    sf::Sprite                    playerSprite;
    sf::RectangleShape            enemyShape;
    float                         scale;
    sf::Vector2f                  position;
    sf::Vector2f                  size;

    sf::View          view;
    sf::RectangleShape background;
    sf::CircleShape   playerDot;
    sf::CircleShape   enemyDot;
};

#endif // MINIMAP_H
