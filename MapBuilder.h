#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"

std::vector<sf::RectangleShape> buildWalls();
std::vector<bool> buildCollisionGrid(const std::vector<sf::RectangleShape>& walls);
std::vector<Vec2f> getPatrolCircuit(const std::vector<sf::RectangleShape>& walls);