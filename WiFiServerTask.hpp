// WiFiServerTask.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class WiFiServerTask {
public:
    // w,h = overlay size
    WiFiServerTask(unsigned w, unsigned h);

    // Kick off the overlay (called from your main task-dispatch)
    void start();

    // Forward SFML events from your main loop
    void handleEvent(const sf::Event& e);

    // Update per-frame logic (animations, timers)
    void update(float dt);

    // Draw into your existing RenderWindow
    void draw(sf::RenderWindow& window);

    // Is the overlay currently shown?
    bool isActive()   const;
    // Has the overlay finished (and dismissed itself)?
    bool isFinished() const;

private:
    enum class State { Inactive, Login, Option, Result };
    State currentState;

    // Off-screen buffer & sprite for centering
    sf::RenderTexture m_rt;
    sf::Sprite        m_overlay;

    // Shared UI resources
    sf::Font font;

    // Text elements
    sf::Text titleText;
    sf::Text usernameLabel, passwordLabel;
    sf::Text usernameInputText, passwordInputText;
    sf::Text loginButtonText;
    sf::Text onButtonText, offButtonText;
    sf::Text resultText, exitText;

    // Clickable boxes
    sf::RectangleShape usernameBox, passwordBox;
    sf::RectangleShape loginBtnBox, onBtnBox, offBtnBox;

    // Input state
    bool usernameActive = false;
    bool passwordActive = false;
    std::string usernameStr;
    std::string passwordStr;
    std::string passwordDisplay;

    // Credential store
    std::vector<std::string> validUsernames;
    std::vector<std::string> validPasswords;
    bool taskPassed = false;

    // Helpers
    void initializeCredentials();
    void initializeUI();
    bool isValidCredentials() const;
};
