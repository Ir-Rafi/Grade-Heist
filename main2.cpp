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
        {100,100},{300,100},{650,100},{680,160},{680,300},{680,430},{450,430},
        {200,430},{200,600},{200,800},{350,800},{680,800},{680,1100},{1000,1100},
        {1200,1100},{1350,900},{1350,700},{1350,600},{1350,430},{1100,430},{900,430},
        {900,300},{900,160},{1150,160},{1300,160},{1450,160},{1450,400},{1450,600},
        {1350,900},{1000,900},{680,900},{680,800}
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

    // Minimap parameters
    const float MINIMAP_SCALE = 0.15f;
    const sf::Vector2f MINIMAP_SIZE(WORLD_W * MINIMAP_SCALE, WORLD_H * MINIMAP_SCALE);
    const sf::Vector2f MINIMAP_POS(10.f, 10.f);
    sf::View minimapView;
    minimapView.setSize(WORLD_W, WORLD_H);
    minimapView.setCenter(WORLD_W / 2.f, WORLD_H / 2.f);

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

        // Draw minimap
        sf::Vector2u winSize = window.getSize();
        float vx = MINIMAP_POS.x / (float)winSize.x;
        float vy = MINIMAP_POS.y / (float)winSize.y;
        float vw = MINIMAP_SIZE.x / (float)winSize.x;
        float vh = MINIMAP_SIZE.y / (float)winSize.y;
        minimapView.setViewport(sf::FloatRect(vx, vy, vw, vh));
        window.setView(minimapView);

        // Minimap background
        sf::RectangleShape minimapBg(MINIMAP_SIZE);
        minimapBg.setFillColor(sf::Color(50, 50, 50, 200));
        minimapBg.setPosition(0, 0);
        window.draw(minimapBg);

        // Draw walls on minimap
        for(auto& w : walls){
            sf::RectangleShape miniWall;
            miniWall.setSize(w.getSize());
            miniWall.setPosition(w.getPosition());
            miniWall.setFillColor(sf::Color::White);
            window.draw(miniWall);
        }

        // Draw player on minimap
        sf::CircleShape playerDot(5.f);
        playerDot.setOrigin(5.f, 5.f);
        playerDot.setFillColor(sf::Color::Blue);
        auto playerCenter = player.getPosition() + sf::Vector2f(player.getGlobalBounds().width / 2.f, player.getGlobalBounds().height / 2.f);
        playerDot.setPosition(playerCenter);
        window.draw(playerDot);

        // Draw enemy on minimap
        sf::CircleShape enemyDot(5.f);
        enemyDot.setOrigin(5.f, 5.f);
        enemyDot.setFillColor(sf::Color::Red);
        enemyDot.setPosition(enemy.shape().getPosition());
        window.draw(enemyDot);

        // Restore main view
        window.setView(view);

        window.display();
    }

    return 0;
}
