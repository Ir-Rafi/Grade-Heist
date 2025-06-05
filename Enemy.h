#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "Constants.h"

class Enemy {
    sf::RectangleShape spr;
    const std::vector<sf::RectangleShape>& walls;
    const std::vector<bool>& grid;
    std::vector<Vec2f> circuit;
    size_t wpIdx = 0;
    std::vector<Vec2f> path;
    size_t pi = 0;
    bool chasing = false;
    float lostT = 0.f;

  public:
    Enemy(const std::vector<sf::RectangleShape>& walls,
          const std::vector<bool>& grid,
          const std::vector<Vec2f>& circuit);

    void update(Vec2f playerPos, float dt);
    void setChasing(bool c);
    sf::RectangleShape& shape();

  private:
    static float dist(Vec2f a, Vec2f b);
    void moveToward(Vec2f tgt, float dt);
    bool tryMove(sf::Vector2f mv);
};
