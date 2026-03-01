#pragma once
#include <SFML/Graphics.hpp>
#include "entities/Player.h"
#include "entities/Enemy.h"

void updateView(sf::View& view, const Player& player);
void drawScene(sf::RenderWindow& window,
               const std::vector<sf::RectangleShape>& walls,
               Enemy& enemy,
               Player& player,
               const sf::Text& scoreText);
