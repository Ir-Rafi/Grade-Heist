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
#include "codeoutput.h"  // header for the challenge

void runGame() {
    sf::RenderWindow window(sf::VideoMode(WORLD_W/2, WORLD_H/2), "Patrol + Chase Floorplan Modular");
    window.setFramerateLimit(60);

    auto walls = buildWalls();
    auto grid  = buildCollisionGrid(walls);

    std::vector<Vec2f> circuit = getPatrolCircuit(walls);
    Enemy enemy(walls, grid, circuit);

    sf::Texture playerTexture;
    playerTexture.loadFromFile("sprite.png");
    Player player(playerTexture, walls);

    sf::RectangleShape zone(sf::Vector2f(200, 150));
    zone.setPosition(WORLD_W - 200.f, WORLD_H - 150.f);
    zone.setFillColor(sf::Color(0, 255, 0, 100));

    sf::Font font;
    font.loadFromFile("arial.ttf");
    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);

    sf::View view(sf::FloatRect(0.f, 0.f, WORLD_W/2.f, WORLD_H/2.f));
    int score = 0;
    sf::Clock clock;

    bool challengeStarted = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        // If player enters green zone, start chase and the challenge
        if (player.getGlobalBounds().intersects(zone.getGlobalBounds())) {
            enemy.setChasing(true);
            if (!challengeStarted) {
                challengeStarted = true;
                runCodeChallenge();
            }
        }

        updateGameLogic(player, enemy, walls, dt, score, scoreText);
        updateView(view, player);

        window.setView(view);
        window.clear(sf::Color(20, 20, 20));

        drawScene(window, walls, zone, enemy, player, scoreText);
        window.display();
    }
}

int main() {
    runGame();
    return 0;
}