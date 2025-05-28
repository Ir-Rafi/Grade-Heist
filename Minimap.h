#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Minimap {
    const std::vector<sf::RectangleShape>& walls;
    const sf::Sprite& playerSprite;
    const sf::RectangleShape& enemyShape;

    sf::View view;
    sf::RectangleShape background;

    float scale;
    sf::Vector2f size;
    sf::Vector2f position;

    sf::CircleShape playerDot;
    sf::CircleShape enemyDot;

public:
    Minimap(const std::vector<sf::RectangleShape>& walls,
            const sf::Sprite& playerSprite,
            const sf::RectangleShape& enemyShape,
            float scale = 0.15f,
            sf::Vector2f position = {10.f, 10.f});

    void draw(sf::RenderWindow& window);
};
