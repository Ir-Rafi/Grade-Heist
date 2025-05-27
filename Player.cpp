#include "Player.h"
#include "Collision.h"
#include "Constants.h"
#include <cmath>

Player::Player(const sf::Texture& texture, const std::vector<sf::RectangleShape>& W)
    : walls(W)
{
    sprite.setTexture(texture);
    sprite.setScale(0.1f, 0.1f);
    sprite.setPosition(150.f, 150.f);
}

void Player::handleInput(float dt) {
    sf::Vector2f movement(0.f, 0.f);

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))  movement.x -= 1.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) movement.x += 1.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))    movement.y -= 1.f;
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))  movement.y += 1.f;

    if(movement.x != 0 || movement.y != 0) {
        float length = std::sqrt(movement.x*movement.x + movement.y*movement.y);
        movement = (movement / length) * PLAYER_SPEED * dt;

        sf::FloatRect pb = sprite.getGlobalBounds();

        sf::FloatRect ph = pb; ph.left += movement.x;
        if(!checkCollision(ph, walls)) sprite.move(movement.x, 0);

        sf::FloatRect pv = pb; pv.top += movement.y;
        if(!checkCollision(pv, walls)) sprite.move(0, movement.y);
    }
}

void Player::update() {
    auto pos = sprite.getPosition();
    auto bounds = sprite.getGlobalBounds();

    pos.x = std::clamp(pos.x, 0.f, float(WORLD_W) - bounds.width);
    pos.y = std::clamp(pos.y, 0.f, float(WORLD_H) - bounds.height);

    sprite.setPosition(pos);
}

sf::Vector2f Player::getPosition() const {
    return sprite.getPosition();
}

const sf::FloatRect Player::getGlobalBounds() const {
    return sprite.getGlobalBounds();
}

void Player::draw(sf::RenderWindow& window) {
    window.draw(sprite);
}
