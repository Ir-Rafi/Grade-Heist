// ------------------------- main.cpp -------------------------
#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "Constants.h"
#include "Collision.h"
#include "Enemy.h"
#include "Player.h"
#include "MapBuilder.h"
#include "GameLogic.h"
#include "UI.h"

void runGame() {
    sf::RenderWindow window({WORLD_W/2, WORLD_H/2}, "Patrol + Chase Floorplan Modular");
    window.setFramerateLimit(60);

    auto walls = buildWalls();
    auto grid  = buildCollisionGrid(walls);

    std::vector<Vec2f> circuit = getPatrolCircuit(walls);
    Enemy enemy(walls, grid, circuit);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("sprite.png");
    Player player(playerTexture, walls);

    sf::RectangleShape zone({200, 150});
    zone.setPosition(WORLD_W - 200, WORLD_H - 150);
    zone.setFillColor({0, 255, 0, 100});

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);

    sf::View view({0, 0, WORLD_W/2.f, WORLD_H/2.f});
    int score = 0;
    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // ── Here: check zone overlap and tell enemy to chase if needed ──
        if (player.getGlobalBounds().intersects(zone.getGlobalBounds())) {
            enemy.setChasing(true);
        }

        updateGameLogic(player, enemy, walls, dt, score, scoreText);
        updateView(view, player);

        window.setView(view);
        window.clear(sf::Color(20, 20, 20));  // dark background

        drawScene(window, walls, zone, enemy, player, scoreText);
        window.display();
    }
}

int main() {
    runGame();
    return 0;
}
