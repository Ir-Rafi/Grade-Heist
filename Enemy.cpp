// Enemy.cpp
#include "Enemy.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::RectangleShape>& W,
             const std::vector<bool>&           G,
             const std::vector<Vec2f>&          C)
    : walls(W)
    , grid(G)
    , patrolPoints(C)      // treat incoming “circuit” as direct waypoints
{
    spr.setSize({36.f, 36.f});
    spr.setOrigin(18.f, 18.f);
    spr.setFillColor(sf::Color::Red);

    if (!patrolPoints.empty())
        spr.setPosition(patrolPoints[0]);

    currentPatrolIndex = 0;
}

sf::RectangleShape& Enemy::shape() {
    return spr;
}

void Enemy::setChasing(bool c) {
    if (chasing != c) {
        chasing = c;
        lostT   = 0.f;
        path.clear();
        pi = 0;
    }
}

float Enemy::dist(const Vec2f& a, const Vec2f& b) const {
    float dx = a.x - b.x, dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}

void Enemy::update(const Vec2f& playerPos, float dt) {
    if (chasing) {
        // ——————— unchanged BFS chase ———————
        if (path.empty() || pi >= path.size()) {
            path = findPathBFS(spr.getPosition(), playerPos, grid);
            pi = 0;
        }
        if (pi < path.size()) {
            moveToward(path[pi], dt);
            if (dist(spr.getPosition(), path[pi]) < WAYPOINT_REACH_THRESHOLD)
                pi++;
        }
        if (dist(spr.getPosition(), playerPos) > 300.f) {
            lostT += dt;
            if (lostT > LOST_PLAYER_TIMEOUT) setChasing(false);
        } else {
            lostT = 0.f;
        }
    } else {
        // ——————— pure waypoint patrol ———————
        if (patrolPoints.empty()) return;

        Vec2f target = patrolPoints[currentPatrolIndex];
        float d = dist(spr.getPosition(), target);

        if (d < WAYPOINT_REACH_THRESHOLD) {
            // snap exactly to avoid drift, then advance
            spr.setPosition(target);
            currentPatrolIndex = (currentPatrolIndex + 1) % patrolPoints.size();
        } else {
            moveToward(target, dt);
        }
    }
}

void Enemy::moveToward(const Vec2f& tgt, float dt) {
    sf::Vector2f pos = spr.getPosition();
    sf::Vector2f delta = tgt - pos;
    float len = length(delta);
    if (len < 1e-3f) return;

    sf::Vector2f dir = normalize(delta);
    sf::Vector2f mv  = dir * ENEMY_SPEED * dt;

    // Try full move, then axis‐aligned fallbacks
    if      (tryMove(mv))           return;
    else if (tryMove({mv.x, 0.f}))  return;
    else if (tryMove({0.f, mv.y}))  return;
}

bool Enemy::tryMove(const sf::Vector2f& mv) {
    sf::FloatRect nb = spr.getGlobalBounds();
    nb.left += mv.x; nb.top += mv.y;
    if (!checkCollision(nb, walls)) {
        spr.move(mv);
        return true;
    }
    return false;
}

float Enemy::length(const sf::Vector2f& v) const {
    return std::sqrt(v.x*v.x + v.y*v.y);
}

sf::Vector2f Enemy::normalize(const sf::Vector2f& v) const {
    float len = length(v);
    return (len > 0.f) ? sf::Vector2f(v.x/len, v.y/len)
                       : sf::Vector2f(0.f, 0.f);
}
