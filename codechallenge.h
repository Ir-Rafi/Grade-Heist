/* codechallenge.h */
#pragma once
#include <SFML/Graphics.hpp>
#include <string>

// Challenge states
enum GameState {
    CHALLENGE_INACTIVE,
    CHALLENGE_ACTIVE,
    CHALLENGE_FINISHED
};

class CodeChallenge {
public:
    // w,h: size of the overlay render texture
    CodeChallenge(unsigned w, unsigned h);

    // Start or restart the challenge
    void start();

    // Handle SFML events when the challenge is active
    void handleEvent(const sf::Event& evt);

    // Advance logic by dt seconds
    void update(float dt);

    // Draw the overlay onto the given window (centered)
    void draw(sf::RenderWindow& window);

    // Query whether the challenge just finished
    bool isFinished() const;
    bool isActive()   const;

private:
    // Off-screen buffer and its sprite
    sf::RenderTexture m_rt;
    sf::Sprite        m_overlay;

    // UI resources
    sf::Font          font;
    sf::Text          titleText;
    sf::Text          instructionText;
    sf::Text          codeText;
    sf::Text          inputPromptText;
    sf::Text          userInputText;
    sf::Text          attemptsText;
    sf::Text          resultText;
    sf::Text          correctAnswerText;

    // Background shapes
    sf::RectangleShape codeBackground;
    sf::RectangleShape inputBackground;

    // Timing & state
    GameState         currentState;
    sf::Clock         autoCloseTimer;
    bool              shouldAutoClose;

    // Challenge logic
    int               attemptsLeft;
    std::string       userInput;
    bool              gameWon;
    std::string       correctAnswer;
    std::string       challengeCode;

    // Colors
    sf::Color         backgroundColor;
    sf::Color         primaryColor;
    sf::Color         successColor;
    sf::Color         errorColor;
    sf::Color         codeBackgroundColor;

    // Helpers
    void initializeUI();
    void updateAttemptsText();
    void processTextInput(sf::Uint32 unicode);
    void handleKeyPress(sf::Keyboard::Key key);
};
