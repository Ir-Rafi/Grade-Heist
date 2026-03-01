#include "entities/Enemy.h"
#include "world/Pathfinding.h"
#include <cmath>
#include <algorithm>

Enemy::Enemy(const std::vector<sf::RectangleShape>& W,
             const std::vector<bool>& G,
             int colsC, int rowsC,
             const std::vector<Tile>& patrolT)
    : walls(W), coarseGrid(G), colsCoarse(colsC), rowsCoarse(rowsC), patrolTiles(patrolT)
{
    spr.setSize({ENEMY_SIZE, ENEMY_SIZE});
    spr.setOrigin(ENEMY_SIZE/2.f, ENEMY_SIZE/2.f);
    spr.setFillColor(sf::Color::Red);
    currentPatrolIndex = 0;
    moving = false;
    moveElapsed = 0.f;
    tileMoveTime = 0.3f;

    if (!patrolTiles.empty())
        spr.setPosition(tileCenter(patrolTiles[0]));
}

sf::RectangleShape& Enemy::shape() { return spr; }

void Enemy::setChasing(bool c) {
    if (chasing != c) {
        chasing = c;
        path.clear();
        pi = 0;
        moving = false;
    }
}

// Call with playerTile = (cx,cy) of player in coarse grid
void Enemy::update(Tile playerTile, float dt) {
    int k = ENEMY_SIZE / TILE_SIZE;
    Tile currentTile = Tile(
        int((spr.getPosition().x / TILE_SIZE) - (k/2)),
        int((spr.getPosition().y / TILE_SIZE) - (k/2))
    );

    if (chasing) {
        if (path.empty() || pi >= path.size()) {
            path = findPathBFS(currentTile, playerTile, coarseGrid, colsCoarse, rowsCoarse);
            pi = 0;
            moving = false;
        }
    } else {
        if (path.empty() || pi >= path.size()) {
            Tile patrolTarget = patrolTiles[(currentPatrolIndex + 1) % patrolTiles.size()];
            path = findPathBFS(currentTile, patrolTarget, coarseGrid, colsCoarse, rowsCoarse);
            pi = 0;
            moving = false;
        }
    }

    if (!path.empty() && pi+1 < path.size()) {
        if (!moving) {
            tileFrom = path[pi];
            tileTo   = path[pi+1];
            moveElapsed = 0.f;
            moving = true;
        }
        moveElapsed += dt;
        float t = std::min(moveElapsed / tileMoveTime, 1.f);

        sf::Vector2f pos = tileCenter(tileFrom) + (tileCenter(tileTo) - tileCenter(tileFrom)) * t;
        spr.setPosition(pos);

        if (t >= 1.f) {
            ++pi;
            moving = false;
            if (!chasing && pi+1 >= path.size()) {
                // Reached patrol target
                currentPatrolIndex = (currentPatrolIndex + 1) % patrolTiles.size();
                path.clear();
                pi = 0;
            }
        }
    }
}

sf::Vector2f Enemy::tileCenter(Tile tile) const {
    int k = ENEMY_SIZE / TILE_SIZE;
    return sf::Vector2f(
        (tile.first + k / 2) * TILE_SIZE + TILE_SIZE / 2.f,
        (tile.second + k / 2) * TILE_SIZE + TILE_SIZE / 2.f
    );
}