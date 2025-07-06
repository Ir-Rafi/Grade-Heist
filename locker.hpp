// CombinationLock.hpp
#pragma once
#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>

class CombinationLock {
public:
    // w,h = overlay size
    CombinationLock(unsigned w, unsigned h);
    
    // Call when the player clicks the button on a combination lock tile
    void start();
    
    // Forward SFML events from your main loop
    void handleEvent(const sf::Event& e);
    
    // Update per-frame (auto-close after completion)
    void update(float dt);
    
    // Draw into your existing RenderWindow
    void draw(sf::RenderWindow& window);
    
    // Overlay state queries
    bool isActive() const;    // true while playing
    bool isFinished() const;  // true after completed, before close
    bool passed() const;      // did the player succeed?

private:
    enum class State { Inactive, HintDisplay, Playing, Completed };
    State currentState;
    
    // Off-screen buffer + sprite
    sf::RenderTexture m_rt;
    sf::Sprite m_overlay;
    
    // Shared font + UI elements
    sf::Font font;
    sf::Text titleText;
    sf::Text hintDisplayText;
    sf::Text codeDisplayText;
    sf::Text instructionText;
    sf::Text resultText;
    sf::Text attemptText;
    sf::Text keyboardInstructions;
    sf::Text exitText;
    
    // Visual elements
    sf::RectangleShape lockFrame;
    sf::CircleShape lockDial;
    sf::RectangleShape codeDisplay;
    std::vector<sf::RectangleShape> digitBoxes;
    std::vector<sf::Text> digitTexts;
    sf::RectangleShape enterButton;
    sf::RectangleShape clearButton;
    sf::Text enterButtonText;
    sf::Text clearButtonText;
    
    // Background elements
    std::vector<sf::CircleShape> backgroundParticles;
    
    // Game data
    std::string correctCode;
    std::string enteredCode;
    std::string hintText;
    
    // Game state
    bool taskPassed;
    bool autoClose;
    int maxDigits;
    int attemptsRemaining;
    int maxAttempts;
    
    // Auto-close after finish
    sf::Clock finishClock;
    sf::Clock particleClock;
    sf::Clock gameClock;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color successColor;
    sf::Color failColor;
    
    // RNG for particles
    std::mt19937 rng;
    
    // Helpers
    void setupUI();
    void setupBackground();
    void setupVisualElements();
    void setupTexts();
    void updateBackground();
    void updateCodeDisplay();
    void handleMouseClick(float x, float y);
    void handleKeyPress(sf::Keyboard::Key k);
    void checkCombination();
    bool checkDigitsMatch(const std::string& entered, const std::string& correct);
    void renderHintScreen();
    void renderGameScreen();
};