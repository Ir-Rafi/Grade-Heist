#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <utility>
#include "Constants.h"

using Tile = std::pair<int, int>;

class Enemy {
public:
    Enemy(const std::vector<sf::RectangleShape>& walls,
          const std::vector<bool>& coarseGrid,
          int colsCoarse, int rowsCoarse,
          const std::vector<Tile>& patrolTiles);

    void setChasing(bool chase);
    void update(Tile playerTile, float dt);

    sf::RectangleShape& shape();

private:
    const std::vector<sf::RectangleShape>& walls;
    const std::vector<bool>& coarseGrid;
    int colsCoarse, rowsCoarse;

    std::vector<Tile> patrolTiles;
    size_t currentPatrolIndex = 0;

    std::vector<Tile> path;
    size_t pi = 0;
    bool chasing = false;
    bool moving = false;
    float moveElapsed = 0.f;
    float tileMoveTime = 0.3f;

    sf::RectangleShape spr;

    Tile tileFrom, tileTo;

    // Helper
    sf::Vector2f tileCenter(Tile tile) const;
};