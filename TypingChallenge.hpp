// TypingChallenge.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <random>

class TypingChallenge {
public:
    // w,h = overlay size
    TypingChallenge(unsigned w, unsigned h);

    // Call when the player clicks the button on a "tyac" tile
    void start();

    // Forward SFML events from your main loop
    void handleEvent(const sf::Event& e);

    // Update per-frame (auto-close after completion)
    void update(float dt);

    // Draw into your existing RenderWindow
    void draw(sf::RenderWindow& window);

    // Overlay state queries
    bool isActive()   const;  // true while playing
    bool isFinished() const;  // true after completed, before close
    bool passed()     const;  // did the player succeed?

private:
    enum class State { Inactive, Playing, Completed };
    State          currentState;

    // Off-screen buffer + sprite
    sf::RenderTexture m_rt;
    sf::Sprite        m_overlay;

    // Shared font + UI elements
    sf::Font          font;
    sf::Text          titleText;
    sf::Text          instructionText;
    sf::Text          stringText;
    sf::Text          inputText;
    sf::Text          resultText;
    sf::Text          exitText;
    sf::RectangleShape inputBox;

    // Game data
    std::string       targetString;
    std::string       userInput;
    std::string       correctAnswer;

    // Auto-close after finish
    sf::Clock         finishClock;
    bool              autoClose;

    bool              taskPassed;

    // RNG for string generation
    std::mt19937      rng;

    // Helpers
    void setupUI();
    void generateRandomString();
    void checkAnswer();
    static std::string trim(const std::string& s);
};
