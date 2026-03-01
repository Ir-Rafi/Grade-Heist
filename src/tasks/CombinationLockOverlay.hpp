#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>

class CombinationLockOverlay {
public:
    enum class State { Inactive, HintDisplay, Playing, Completed };

    // w×h = the size you pass from main (e.g. 800×600)
    CombinationLockOverlay(unsigned w, unsigned h);

    // Called when the player steps on the “locker” tile
    void start();

    // Forward SFML events from your main loop
    void handleEvent(const sf::Event& e);

    // Per-frame update (handles auto-close, animations)
    void update(float dt);

    // Draw into your existing RenderWindow
    void draw(sf::RenderWindow& window);

    // Query from main:
    bool isActive()   const;  // true while HintDisplay or Playing
    bool isFinished() const;  // true once Completed
    bool passed()     const;  // true if the answer was correct

private:
    // Helpers to build and manage UI
    void setupUI();
    void setupBackground();
    void setupVisualElements();
    void setupTexts();
    void updateBackground();
    void updateCodeDisplay();
    void handleMouseClick(int x, int y);
    void handleKeyPress(sf::Keyboard::Key k);
    void checkCombination();
    bool checkDigitsMatch(const std::string& entered,
                          const std::string& correct);

    // Off-screen render target + sprite
    sf::RenderTexture      m_rt;
    sf::Sprite             m_overlay;

    // Font & texts
    sf::Font               font;
    sf::Text               titleText;
    sf::Text               hintDisplayText;
    sf::Text               codeDisplayText;
    sf::Text               instructionText;
    sf::Text               resultText;
    sf::Text               attemptText;
    sf::Text               exitText;

    // Visual elements
    sf::RectangleShape     lockFrame;
    sf::CircleShape        lockDial;
    sf::RectangleShape     codeDisplay;
    std::vector<sf::RectangleShape> digitBoxes;
    std::vector<sf::Text>  digitTexts;
    std::vector<sf::RectangleShape> numberButtons;
    std::vector<sf::Text>  buttonTexts;
    sf::RectangleShape     enterButton;
    sf::RectangleShape     clearButton;
    sf::Text               enterButtonText;
    sf::Text               clearButtonText;

    // Background particles
    std::vector<sf::CircleShape> backgroundParticles;

    // Game data
    std::string            correctCode;
    std::string            enteredCode;
    std::string            hintText;
    int                    maxDigits;
    int                    maxAttempts;
    int                    attemptsRemaining;

    // State
    State                  currentState;
    bool                   taskPassed;
    bool                   autoClose;

    // Timing
    sf::Clock              particleClock;
    sf::Clock              gameClock;
    sf::Clock              finishClock;

    // Colors
    sf::Color              backgroundColor;
    sf::Color              successColor;
    sf::Color              failColor;

    // RNG
    std::mt19937           rng;
};
