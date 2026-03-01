#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

// Check if the game should end (enemy caught the player)
bool isend(const std::vector<sf::RectangleShape>& walls,
           const sf::Vector2f& pos_player,
           const sf::Vector2f& pos_enemy);
