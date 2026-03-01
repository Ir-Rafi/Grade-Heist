#pragma once
#include <vector>
#include <SFML/Graphics.hpp>
#include "MapLoader.hpp"

// Extract collision walls from map
std::vector<sf::RectangleShape> extractCollisionWalls(const MapLoader& map);
