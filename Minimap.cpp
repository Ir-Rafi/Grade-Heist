#include "Minimap.h"
#include "Constants.h"

Minimap::Minimap(const std::vector<sf::RectangleShape>& W,
                 const sf::Sprite& P,
                 const sf::RectangleShape& E,
                 float scl,
                 sf::Vector2f pos)
    : walls(W), playerSprite(P), enemyShape(E), scale(scl), position(pos)
{
    size = sf::Vector2f(WORLD_W * scale, WORLD_H * scale);

    // Setup the minimap view to cover full world but scaled down
    view.setSize(WORLD_W, WORLD_H);
    // Viewport normalized relative to window size (assumes window size = WORLD_W/2, WORLD_H/2)
    // We calculate normalized coords below dynamically in draw() because window size can change.
    view.setCenter(WORLD_W / 2.f, WORLD_H / 2.f);

    // Background rectangle (dark semi-transparent)
    background.setSize(size);
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setPosition(0, 0);

    // Player and enemy dots (small circles)
    playerDot.setRadius(5.f);
    playerDot.setOrigin(5.f, 5.f);
    playerDot.setFillColor(sf::Color::Blue);

    enemyDot.setRadius(5.f);
    enemyDot.setOrigin(5.f, 5.f);
    enemyDot.setFillColor(sf::Color::Red);
}

void Minimap::draw(sf::RenderWindow& window) {
    // Calculate viewport normalized coords based on window size and desired minimap position & size
    sf::Vector2u winSize = window.getSize();

    float vx = position.x / winSize.x;
    float vy = position.y / winSize.y;
    float vw = size.x / winSize.x;
    float vh = size.y / winSize.y;

    view.setViewport(sf::FloatRect(vx, vy, vw, vh));

    // Save current view
    sf::View oldView = window.getView();

    // Set minimap view
    window.setView(view);

    // Draw minimap background at (0,0) in minimap coords
    window.draw(background);

    // Draw walls on minimap
    for (auto& w : walls) {
        sf::RectangleShape miniWall;
        miniWall.setSize(w.getSize());
        miniWall.setPosition(w.getPosition());
        miniWall.setFillColor(sf::Color::White);
        window.draw(miniWall);
    }

    // Draw player dot (centered on player sprite)
    sf::Vector2f playerPos = playerSprite.getPosition() + sf::Vector2f(playerSprite.getGlobalBounds().width / 2.f, playerSprite.getGlobalBounds().height / 2.f);
    playerDot.setPosition(playerPos);
    window.draw(playerDot);

    // Draw enemy dot
    enemyDot.setPosition(enemyShape.getPosition());
    window.draw(enemyDot);

    // Restore previous view
    window.setView(oldView);
}
