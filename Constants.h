#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

constexpr int WORLD_W = 2700;
constexpr int WORLD_H = 2580;
constexpr int TILE_SIZE = 12;
constexpr float ENEMY_SPEED = 120.f;
constexpr float PLAYER_SPEED = 200.f;
constexpr float WAYPOINT_REACH_THRESHOLD = 8.f;
constexpr float LOST_PLAYER_TIMEOUT = 3.0f;

// Define the light color (e.g., a soft yellow light)
const sf::Color LIGHT_COLOR(255, 255, 0, 128); // RGBA format (Yellow with some transparency)

// Define the radius of the light (e.g., 100 units)
const float LIGHT_RADIUS = 100.f;
const bool gameon = true;

using Vec2f = sf::Vector2f;

// Distance helper function
inline float dist(Vec2f a, Vec2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}
