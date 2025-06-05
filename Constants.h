#pragma once
#include <SFML/Graphics.hpp>
#include <cmath>

constexpr int WORLD_W = 1600;
constexpr int WORLD_H = 1200;
constexpr int TILE_SIZE = 40;
constexpr float ENEMY_SPEED = 120.f;
constexpr float PLAYER_SPEED = 200.f;
constexpr float WAYPOINT_REACH_THRESHOLD = 8.f;
constexpr float LOST_PLAYER_TIMEOUT = 3.0f;

// ------------------- NEW LIGHT EFFECT CONSTANTS -------------------
static constexpr float LIGHT_RADIUS    = 200.f; 
// (adjust this radius as desired)

static const sf::Color LIGHT_COLOR = sf::Color(255, 255, 200,  100); 
// (a very translucent yellowish‐white; alpha=100 on a 0–255 scale)


using Vec2f = sf::Vector2f;

// Distance helper function
inline float dist(Vec2f a, Vec2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}
