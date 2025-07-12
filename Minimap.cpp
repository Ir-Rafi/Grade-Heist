
// Minimap.cpp
#include "Minimap.h"

Minimap::Minimap(float w, float h, sf::Font& font)
: worldW(w), worldH(h), fontPtr(&font)
{
    // Set up view for full-world draw in top-right corner
    miniView = sf::View({0.f, 0.f, worldW, worldH});
    miniView.setViewport({0.75f, 0.f, 0.25f, 0.25f});
}

void Minimap::handleEvent(const sf::Event& event, sf::RenderWindow& window)
{
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mp(float(event.mouseButton.x), float(event.mouseButton.y));
        auto sz = window.getSize();
        sf::FloatRect btnRect(
            sz.x - buttonWidth - buttonPadding,
            buttonPadding,
            buttonWidth,
            buttonHeight
        );
        if (btnRect.contains(mp)) open = !open;
    }
}

void Minimap::draw(sf::RenderWindow& window,
                   const MapLoader& map,
                   const std::vector<sf::RectangleShape>& walls,
                   Player& player,
                   Enemy& enemy)
{
    // 1) Draw the Map button
    auto sz = window.getSize();
    sf::RectangleShape btnBg({buttonWidth, buttonHeight});
    btnBg.setPosition(sz.x - buttonWidth - buttonPadding, buttonPadding);
    btnBg.setFillColor({0,0,0,180});
    btnBg.setOutlineColor(sf::Color::White);
    btnBg.setOutlineThickness(1.f);
    window.draw(btnBg);

    sf::Text btnText("Map", *fontPtr);
    btnText.setCharacterSize(14);
    btnText.setFillColor(sf::Color::White);
    auto tb = btnText.getLocalBounds();
    btnText.setPosition(
        btnBg.getPosition().x + (buttonWidth - tb.width) / 2.f - tb.left,
        btnBg.getPosition().y + (buttonHeight - tb.height) / 2.f - tb.top
    );
    window.draw(btnText);

    if (!open) return;

    // 2) Draw textured map + optional walls under miniView
    window.setView(miniView);
    window.draw(map);
    for (const auto& w : walls) window.draw(w);
    window.draw(enemy.shape());
    player.draw(window);

    // 3) Overlay fixed-size markers in pixel coords
    window.setView(window.getDefaultView());
    auto vp = miniView.getViewport();
    float left   = vp.left   * sz.x;
    float top    = vp.top    * sz.y;
    float width  = vp.width  * sz.x;
    float height = vp.height * sz.y;

    auto toPixel = [&](const sf::Vector2f& p){
        return sf::Vector2f(
            left + (p.x / worldW) * width,
            top  + (p.y / worldH) * height
        );
    };

    sf::CircleShape dot(markerRadius);
    dot.setOrigin(markerRadius, markerRadius);
    dot.setFillColor(sf::Color::Green);
    dot.setPosition(toPixel(player.getPosition()));
    window.draw(dot);

    dot.setFillColor(sf::Color::Red);
    dot.setPosition(toPixel(enemy.shape().getPosition()));
    window.draw(dot);

    // 4) Frame the minimap
    sf::RectangleShape frame({width, height});
    frame.setPosition(left, top);
    frame.setFillColor(sf::Color::Transparent);
    frame.setOutlineColor(sf::Color::White);
    frame.setOutlineThickness(2.f);
    window.draw(frame);
}
