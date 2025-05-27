#include "Enemy.h"
#include "Collision.h"
#include "Pathfinding.h"
#include <cmath>

Enemy::Enemy(const std::vector<sf::RectangleShape>& W,
             const std::vector<bool>& G,
             const std::vector<Vec2f>& C)
    : walls(W), grid(G), circuit(C)
{
    spr.setSize({36, 36});
    spr.setOrigin(18, 18);
    spr.setFillColor(sf::Color::Red);
    if(!circuit.empty())
        spr.setPosition(circuit[0]);
}

sf::RectangleShape& Enemy::shape() { return spr; }

void Enemy::setChasing(bool c){
    if(chasing != c){
        chasing = c; lostT = 0; path.clear(); pi = 0;
    }
}

float Enemy::dist(Vec2f a, Vec2f b){
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    return std::sqrt(dx*dx + dy*dy);
}

void Enemy::moveToward(Vec2f tgt, float dt){
    Vec2f pos = spr.getPosition();
    Vec2f d = tgt - pos;
    float len = std::sqrt(d.x*d.x + d.y*d.y);
    if(len < 1e-3f) return;
    d /= len;

    sf::Vector2f mv = d * ENEMY_SPEED * dt;
    if(tryMove(mv)) return;
    if(tryMove({mv.x, 0})) return;
    if(tryMove({0, mv.y})) return;

    path.clear();
    pi = 0;
}

bool Enemy::tryMove(sf::Vector2f mv){
    sf::FloatRect nb = spr.getGlobalBounds();
    nb.left += mv.x; nb.top += mv.y;
    if(!checkCollision(nb, walls)){
        spr.move(mv);
        return true;
    }
    return false;
}

void Enemy::update(Vec2f playerPos, float dt){
    if(chasing){
        if(path.empty() || pi >= path.size()){
            path = findPathBFS(spr.getPosition(), playerPos, grid);
            pi = 0;
        }
        if(pi < path.size()){
            moveToward(path[pi], dt);
            if(dist(spr.getPosition(), path[pi]) < WAYPOINT_REACH_THRESHOLD)
                pi++;
        }
        if(dist(spr.getPosition(), playerPos) > 300){
            lostT += dt;
            if(lostT > LOST_PLAYER_TIMEOUT) setChasing(false);
        } else {
            lostT = 0;
        }
    } else {
        if(circuit.empty()) return;
        Vec2f tgt = circuit[wpIdx];
        if(path.empty() || pi >= path.size()){
            path = findPathBFS(spr.getPosition(), tgt, grid);
            pi = 0;
            if(path.empty()){
                wpIdx = (wpIdx + 1) % circuit.size();
                return;
            }
        }
        if(pi < path.size()){
            moveToward(path[pi], dt);
            if(dist(spr.getPosition(), path[pi]) < WAYPOINT_REACH_THRESHOLD){
                pi++;
                if(pi >= path.size()){
                    wpIdx = (wpIdx + 1) % circuit.size();
                    path.clear();
                    pi = 0;
                }
            }
        }
    }
}
