// Enemy.h
#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"        // Vec2f, ENEMY_SPEED, WAYPOINT_REACH_THRESHOLD :contentReference[oaicite:0]{index=0}
#include "Collision.h"        // checkCollision(...) :contentReference[oaicite:1]{index=1}
#include "Pathfinding.h"      // findPathBFS(...) (unchanged chase code)

class Enemy {
public:
    // Pass in your walls & grid once; the same grid is used for chase only
    Enemy(const std::vector<sf::RectangleShape>& walls,
          const std::vector<bool>& grid,
          const std::vector<Vec2f>& circuit);

    // switch between patrol and chase
    void setChasing(bool chase);
    void update(const Vec2f& playerPos, float dt);

    // for drawing
    sf::RectangleShape& shape();

private:
    // chase data
    const std::vector<sf::RectangleShape>& walls;
    const std::vector<bool>&           grid;
    std::vector<Vec2f>                 path;
    size_t                             pi = 0;
    bool                               chasing = false;
    float                              lostT   = 0.f;

    // new patrol data
    std::vector<Vec2f> patrolPoints;
    size_t             currentPatrolIndex = 0;

    // visual
    sf::RectangleShape spr;

    // helpers
    float            dist(const Vec2f& a, const Vec2f& b) const;
    void             moveToward(const Vec2f& tgt, float dt);
    bool             tryMove(const sf::Vector2f& mv);
    float            length(const sf::Vector2f& v) const;
    sf::Vector2f     normalize(const sf::Vector2f& v) const;
};
