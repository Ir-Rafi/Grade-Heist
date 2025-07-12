#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

// Build walkable grid from walls
std::vector<bool> buildWalkableGrid(const std::vector<sf::RectangleShape>& walls, int worldW, int worldH, int tileSize);
