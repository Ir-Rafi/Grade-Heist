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

using Vec2f = sf::Vector2f;

// Distance helper function
inline float dist(Vec2f a, Vec2f b) {
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}
