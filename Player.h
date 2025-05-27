#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

class Player {
    sf::Sprite sprite;
    const std::vector<sf::RectangleShape>& walls;

public:
    Player(const sf::Texture& texture, const std::vector<sf::RectangleShape>& walls);

    void handleInput(float dt);
    void update();
    sf::Vector2f getPosition() const;
    const sf::FloatRect getGlobalBounds() const;
    void draw(sf::RenderWindow& window);
};
