#pragma once

#include <SFML/Graphics.hpp>

class PCSimulatorOverlay {
public:
    enum State { Inactive, WaitingFlip, Info };

    PCSimulatorOverlay(unsigned width, unsigned height);

    // Activate/reset overlay
    void start();

    // Feed every SFML event here
    void handleEvent(const sf::Event& e);

    // Call once per frame (no-op)
    void update(float dt);

    // Draw on top of your main window
    void draw(sf::RenderWindow& window);

    // Helpers for main loop
    bool isActive()   const;
    bool isFinished() const;

private:
    void setupUI();

    sf::RenderTexture    m_rt;
    sf::Sprite           m_overlay;
    sf::Font             font;

    State                currentState;
    bool                 switchOn;
    bool                 showInfoWindow;

    // Switch UI
    sf::Text             switchMessage;
    sf::RectangleShape   switchBackground;
    sf::RectangleShape   switchToggle;
    sf::Text             onText;
    sf::Text             offText;

    // Info‐screen UI
    sf::RectangleShape   infoWindow;
    sf::RectangleShape   infoBox;
    sf::Text             infoTitle;
    sf::Text             usernameLabel;
    sf::RectangleShape   usernameField;
    sf::Text             usernameValue;
    sf::Text             passwordLabel;
    sf::RectangleShape   passwordField;
    sf::Text             passwordValue;
    sf::Text             exitMessage;
};
