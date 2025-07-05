// Minimap.h
#pragma once
#include <SFML/Graphics.hpp>
#include "MapLoader.hpp"
#include "Player.h"
#include "Enemy.h"
#include <vector>

class Minimap {
public:
    // worldW/worldH: dimensions of full map
    // font: to render the Map toggle button
    Minimap(float worldW, float worldH, sf::Font& font);

    // Process click events for the Map button
    void handleEvent(const sf::Event& event, sf::RenderWindow& window);

    // Draws the button, and if open, the minimap
    void draw(sf::RenderWindow& window,
              const MapLoader& map,
              const std::vector<sf::RectangleShape>& walls,
              Player& player,
              Enemy& enemy);

private:
    bool open = false;
    float worldW;
    float worldH;
    sf::View miniView;
    sf::Font* fontPtr;
    float markerRadius = 8.f;
    float buttonWidth = 60.f;
    float buttonHeight = 24.f;
    float buttonPadding = 8.f;
};