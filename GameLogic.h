#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"

// Now includes walls so we can check occlusion
void updateGameLogic(Player& player,
                     Enemy& enemy,
                     const std::vector<sf::RectangleShape>& walls,
                     float dt,
                     int& score,
                     sf::Text& scoreText);