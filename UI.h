#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"

void updateView(sf::View& view, const Player& player);
void drawScene(sf::RenderWindow& window,
               const std::vector<sf::RectangleShape>& walls,
               const sf::RectangleShape& zone,
               Enemy& enemy,
               Player& player,
               const sf::Text& scoreText);
