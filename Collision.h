#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

bool checkCollision(const sf::FloatRect& b, const std::vector<sf::RectangleShape>& walls);
