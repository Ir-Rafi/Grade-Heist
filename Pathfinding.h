#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

using Vec2f = sf::Vector2f;

std::vector<Vec2f> findPathBFS(Vec2f start, Vec2f goal, const std::vector<bool>& grid);
