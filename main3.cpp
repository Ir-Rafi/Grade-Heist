#include <SFML/Graphics.hpp>
#include <vector>
#include <algorithm>
#include "Constants.h"
#include "Collision.h"
#include "Enemy.h"
#include "Player.h"
#include "Minimap.h"    // ← our new minimap

int main() {
    sf::RenderWindow window({WORLD_W/2, WORLD_H/2}, "Patrol + Chase Floorplan Modular");
    window.setFramerateLimit(60);

    // --- build walls exactly as before ---
    std::vector<sf::RectangleShape> walls;
    const float T = 20.f;
    walls.emplace_back(sf::Vector2f(WORLD_W, T)); walls.back().setPosition(0, 0);
    walls.emplace_back(sf::Vector2f(WORLD_W, T)); walls.back().setPosition(0, WORLD_H - T);
    walls.emplace_back(sf::Vector2f(T, WORLD_H)); walls.back().setPosition(0, 0);
    walls.emplace_back(sf::Vector2f(T, WORLD_H)); walls.back().setPosition(WORLD_W - T, 0);
    // … your internal‐wall loops …

    // --- collision grid as before ---
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

    // --- patrol circuit & entities ---
    std::vector<Vec2f> circuit = { /*… your points …*/ };
    Enemy      enemy(walls, grid, circuit);

    sf::Texture playerTexture;
    if(!playerTexture.loadFromFile("sprite.png")){/*…*/}
    Player     player(playerTexture, walls);

    // green “zone”
    sf::RectangleShape zone({200,150});
    zone.setPosition(WORLD_W - 200, WORLD_H - 150);
    zone.setFillColor({0,255,0,100});

    // score text
    sf::Font font;
    font.loadFromFile("arial.ttf");  // assuming this always succeeds
    sf::Text scoreText("Score: 0", font, 24);
    scoreText.setFillColor(sf::Color::White);

    // main camera view
    sf::View view({0,0, WORLD_W/2.f, WORLD_H/2.f});
    int    score = 0;
    sf::Clock clock;

    // --- minimap + UI buttons setup ---
    bool showMinimap = false;

    // Reuse the same font for buttons
    const float MINIMAP_SCALE = 0.15f;
    const sf::Vector2f MINIMAP_POS{10.f,10.f};
    Minimap minimap(walls,
                    player.getSprite(),      // ← assumes you expose getSprite()
                    enemy.shape(),
                    MINIMAP_SCALE,
                    MINIMAP_POS);

    // Floating “M” button
    sf::RectangleShape mapButton({40,40});
    mapButton.setFillColor({100,200,100,200});
    mapButton.setOrigin(20,20);
    sf::Text mapLabel("M", font, 24);
    mapLabel.setFillColor(sf::Color::White);
    mapLabel.setOrigin(
      mapLabel.getLocalBounds().width/2,
      mapLabel.getLocalBounds().height/2
    );

    // Close “X” button
    sf::RectangleShape closeButton({30,30});
    closeButton.setFillColor({200,100,100,200});
    sf::Text closeLabel("X", font, 20);
    closeLabel.setFillColor(sf::Color::White);
    closeLabel.setOrigin(
      closeLabel.getLocalBounds().width/2,
      closeLabel.getLocalBounds().height/2
    );

    // --- game loop ---
    while(window.isOpen()){
        // — handle events —
        sf::Event event;
        while(window.pollEvent(event)){
            if(event.type == sf::Event::Closed)
                window.close();

            if(event.type == sf::Event::MouseButtonPressed
            && event.mouseButton.button == sf::Mouse::Left)
            {
                // map pixel → world coords (in the active view)
                sf::Vector2f click = window.mapPixelToCoords(
                    {event.mouseButton.x, event.mouseButton.y});

                if(!showMinimap) {
                    if(mapButton.getGlobalBounds().contains(click))
                        showMinimap = true;
                } else {
                    if(closeButton.getGlobalBounds().contains(click))
                        showMinimap = false;
                }
            }
        }

        // — update logic —
        float dt = clock.restart().asSeconds();
        player.handleInput(dt);
        player.update();

        bool caught = zone.getGlobalBounds()
                        .intersects(player.getGlobalBounds());
        enemy.setChasing(caught);
        enemy.update(player.getPosition(), dt);

        if(player.getGlobalBounds().intersects(enemy.shape().getGlobalBounds())){
            score++;
            scoreText.setString("Score: " + std::to_string(score));
            enemy.setChasing(false);
        }

        // center camera on player
        sf::FloatRect pb = player.getGlobalBounds();
        view.setCenter(
          player.getPosition() + Vec2f(pb.width, pb.height)/2.f
        );
        window.setView(view);

        // — draw world —
        window.clear();
        for(auto& w: walls)    window.draw(w);
        window.draw(zone);
        window.draw(enemy.shape());
        player.draw(window);

        // draw score
        sf::Vector2f topLeft = view.getCenter() - view.getSize()/2.f;
        scoreText.setPosition(topLeft + Vec2f(10,10));
        window.draw(scoreText);

        // — draw UI / minimap —
        if(!showMinimap) {
            // position floating M above the player
            sf::Vector2f pPos = player.getPosition();
            mapButton.setPosition(pPos + Vec2f(0,-50));
            mapLabel.setPosition(mapButton.getPosition());

            window.draw(mapButton);
            window.draw(mapLabel);

        } else {
            // draw minimap overlay
            minimap.draw(window);

            // restore default view for the close button
            window.setView(window.getDefaultView());

            // position close button in minimap corner
            float pad = 8.f;
            auto msize = minimap.getSize();
            closeButton.setPosition(
              MINIMAP_POS.x + msize.x - closeButton.getSize().x - pad,
              MINIMAP_POS.y + pad
            );
            closeLabel.setPosition(
              closeButton.getPosition() + closeButton.getSize()/2.f
            );

            window.draw(closeButton);
            window.draw(closeLabel);
        }

        window.display();
    }

    return 0;
}
