#pragma once
#include <vector>
#include "Vec2f.h" // or use sf::Vector2f if that's your type
#include <SFML/Graphics.hpp>

// Filter patrol points that do not collide with walls
std::vector<Vec2f> filterPatrolCircuit(const std::vector<Vec2f>& rawCircuit, float enemySize, const std::vector<sf::RectangleShape>& walls, bool (*checkCollision)(const sf::FloatRect&, const std::vector<sf::RectangleShape>&));
