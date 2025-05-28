#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "Constants.h"
#include "Collision.h"
#include "Enemy.h"
#include "Player.h"

int main() {
    sf::RenderWindow window({WORLD_W/2, WORLD_H/2}, "Patrol + Chase Floorplan Modular");
    window.setFramerateLimit(60);

    // Build walls
    std::vector<sf::RectangleShape> walls;
    const float T = 20.f; // wall thickness

    // Outer walls
    walls.emplace_back(sf::Vector2f(WORLD_W, T)); walls.back().setPosition(0, 0);
    walls.emplace_back(sf::Vector2f(WORLD_W, T)); walls.back().setPosition(0, WORLD_H - T);
    walls.emplace_back(sf::Vector2f(T, WORLD_H)); walls.back().setPosition(0, 0);
    walls.emplace_back(sf::Vector2f(T, WORLD_H)); walls.back().setPosition(WORLD_W - T, 0);

    // Internal walls with door gaps (same floorplan as before)
    float yDiv = TILE_SIZE * 12;
    float leftW = TILE_SIZE * 18;
    float corridorW = TILE_SIZE * 4;
    float rightW = WORLD_W - leftW - corridorW;
    float doorW = TILE_SIZE * 2;

    std::vector<float> dx = {
        leftW/2.f,
        leftW + corridorW/2.f,
        leftW + corridorW + rightW/2.f
    };

    std::vector<float> hcuts = {0};
    for(auto c: dx) hcuts.push_back(c - doorW/2);
    for(auto c: dx) hcuts.push_back(c + doorW/2);
    hcuts.push_back(WORLD_W);
    std::sort(hcuts.begin(), hcuts.end());

    for(int i=0; i+1<int(hcuts.size()); i+=2){
        float x0 = hcuts[i], x1 = hcuts[i+1];
        sf::RectangleShape r({x1 - x0, T});
        r.setPosition(x0, yDiv);
        walls.push_back(r);
    }

    {
        float xDiv = leftW;
        float y1 = yDiv/2.f;
        float y2 = yDiv + (WORLD_H - yDiv)/2;
        float doorH = TILE_SIZE * 2;
        std::vector<float> cuts = {0,
            y1 - doorH/2, y1 + doorH/2,
            y2 - doorH/2, y2 + doorH/2,
            float(WORLD_H)
        };
        for(int i=0; i+1<int(cuts.size()); i+=2){
            float y0 = cuts[i], y1 = cuts[i+1];
            sf::RectangleShape r({T, y1 - y0});
            r.setPosition(xDiv, y0);
            walls.push_back(r);
        }
    }
    {
        float xDiv = leftW + corridorW;
        float y0 = yDiv + (WORLD_H - yDiv)/2 - TILE_SIZE;
        float y1 = y0 + TILE_SIZE * 2;
        std::vector<std::pair<float,float>> segs = {
            {0, yDiv}, {yDiv, y0}, {y1, float(WORLD_H)}
        };
        for(auto [a,b]: segs){
            sf::RectangleShape r({T, b - a});
            r.setPosition(xDiv, a);
            walls.push_back(r);
        }
    }

    // Build collision grid
    int cols = WORLD_W / TILE_SIZE, rows = WORLD_H / TILE_SIZE;
    std::vector<bool> grid(cols * rows, false);
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            sf::FloatRect cell(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            for(auto& w: walls){
                if(cell.intersects(w.getGlobalBounds())){
                    grid[y * cols + x] = true;
                    break;
                }
            }
        }
    }

    // Patrol waypoints
    std::vector<Vec2f> circuit = {
        {950, 350}, {900, 160}, {1150, 160}, {1300, 160}, {1450, 160}, {1450, 400},
        {1450, 600}, {1350, 700}, {1350, 900}, {1200, 1100}, {1000, 1100},
        {680, 1100}, {680, 900}, {680, 800}, {350, 800}, {200, 800},
        {150, 700}, {150, 600}, {200, 600}, {200, 430}, {300, 400}, {450, 430},
        {400, 300}, {350, 250}, {300, 200}, {250, 150}, {300, 100}, {400, 100},
        {500, 150}, {600, 200}, {680, 300}, {680, 430},
        {680, 300}, {680, 160}, {900, 300}
    };

    Enemy enemy(walls, grid, circuit);

    sf::Texture playerTexture;
    if(!playerTexture.loadFromFile("sprite.png")){
        // handle error
    }
    Player player(playerTexture, walls);

    sf::RectangleShape zone({200, 150});
    zone.setPosition(WORLD_W - 200, WORLD_H - 150);
    zone.setFillColor({0, 255, 0, 100});

    sf::Font font;
    if(!font.loadFromFile("arial.ttf")){
        // handle error
    }
    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);

    sf::View view({0, 0, WORLD_W/2.f, WORLD_H/2.f});
    int score = 0;
    sf::Clock clock;

    while(window.isOpen()){
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();
        }

        float dt = clock.restart().asSeconds();

        player.handleInput(dt);
        player.update();

        enemy.setChasing(zone.getGlobalBounds().intersects(player.getGlobalBounds()));
        enemy.update(player.getPosition(), dt);

        if(player.getGlobalBounds().intersects(enemy.shape().getGlobalBounds())){
            score++;
            scoreText.setString("Score: " + std::to_string(score));
            enemy.setChasing(false);
        }

        sf::FloatRect pb = player.getGlobalBounds();
        view.setCenter(player.getPosition() + Vec2f(pb.width, pb.height) / 2.f);
        window.setView(view);

        window.clear();
        for(auto& w : walls) window.draw(w);
        window.draw(zone);
        window.draw(enemy.shape());
        player.draw(window);

        sf::Vector2f tl = view.getCenter() - view.getSize() / 2.f;
        scoreText.setPosition(tl + Vec2f(10, 10));
        window.draw(scoreText);

        window.display();
    }

    return 0;
}
