// Minimap.cpp
#include "Minimap.h"

Minimap::Minimap(const std::vector<sf::RectangleShape>& walls,
                 const sf::Sprite&                       playerSprite,
                 const sf::RectangleShape&               enemyShape,
                 float                                    scale,
                 sf::Vector2f                             position)
    : walls(walls)
    , playerSprite(playerSprite)
    , enemyShape(enemyShape)
    , scale(scale)
    , position(position)
{
    // Compute the on‐screen size of the minimap
    size = sf::Vector2f(WORLD_W * scale, WORLD_H * scale);

    // Set up a view that covers the entire game world
    view.setSize(WORLD_W, WORLD_H);
    view.setCenter(WORLD_W / 2.f, WORLD_H / 2.f);

    // Semi‐transparent background
    background.setSize(size);
    background.setFillColor(sf::Color(50, 50, 50, 200));
    background.setPosition(0, 0);

    // Blue dot for player
    playerDot.setRadius(5.f);
    playerDot.setOrigin(5.f, 5.f);
    playerDot.setFillColor(sf::Color::Blue);

    // Red dot for enemy
    enemyDot.setRadius(5.f);
    enemyDot.setOrigin(5.f, 5.f);
    enemyDot.setFillColor(sf::Color::Red);
}

void Minimap::draw(sf::RenderWindow& window) {
    // 1) compute viewport in [0,1] normalized coords
    auto winSize = window.getSize();
    float vx = position.x / float(winSize.x);
    float vy = position.y / float(winSize.y);
    float vw = size.x     / float(winSize.x);
    float vh = size.y     / float(winSize.y);
    view.setViewport({vx, vy, vw, vh});

    // 2) switch views
    sf::View old = window.getView();
    window.setView(view);

    // 3) draw background & walls
    window.draw(background);
    for (auto& w : walls) {
        sf::RectangleShape mini(w);
        mini.setFillColor(sf::Color::White);
        window.draw(mini);
    }

    // 4) draw player dot at sprite center
    sf::Vector2f p = playerSprite.getPosition();
    p += { playerSprite.getGlobalBounds().width/2.f,
           playerSprite.getGlobalBounds().height/2.f };
    playerDot.setPosition(p);
    window.draw(playerDot);

    // 5) draw enemy dot
    enemyDot.setPosition(enemyShape.getPosition());
    window.draw(enemyDot);

    // 6) restore view
    window.setView(old);
}

sf::Vector2f Minimap::getSize() const {
    return size;
}
