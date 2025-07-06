// ACServerOverlay.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <string>
#include <vector>
#include <algorithm>
#include <random>

class ACServerOverlay {
public:
    enum class State { Inactive, Welcome, RoomSelection, ACControl, Result };

    ACServerOverlay(unsigned width, unsigned height);

    // Start the task overlay
    void start();

    // Handle single SFML event
    void handleEvent(const sf::Event& event);

    // Update per-frame (if needed)
    void update(float dt);

    // Draw overlay atop main window
    void draw(sf::RenderWindow& window);

    bool isActive()    const;
    bool isFinished()  const;
    bool passed()      const;

private:
    void setupWelcomeScreen();
    void setupRoomSelection();
    void setupACControl();
    void setupResultScreen();
    void handleMouseClick(const sf::Vector2f& pos);
    void showResult();

    // Off-screen render
    sf::RenderTexture    m_rt;
    sf::Sprite           m_overlay;

    // Shared resources
    sf::Font             font;
    sf::Color            backgroundColor;
    sf::Color            primaryColor;
    sf::Color            buttonColor;
    sf::Color            successColor;
    sf::Color            failColor;

    // State
    State                currentState;
    int                  selectedRoom;
    bool                 taskPassed;

    // UI elements
    sf::Text             titleText;
    sf::Text             welcomeText;
    sf::Text             instructionText;
    sf::Text             resultText;
    sf::RectangleShape   startButton;
    sf::Text             startButtonText;

    std::vector<sf::RectangleShape> roomButtons;
    std::vector<sf::Text>           roomTexts;

    sf::RectangleShape   onButton;
    sf::RectangleShape   offButton;
    sf::Text             onButtonText;
    sf::Text             offButtonText;
    sf::Text             selectedRoomText;

    sf::RectangleShape   closeButton;
    sf::Text             closeButtonText;
};

