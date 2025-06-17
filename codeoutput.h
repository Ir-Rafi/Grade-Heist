#include "codeoutput.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>

enum GameState {
    WAITING_FOR_START,
    CHALLENGE_ACTIVE,
    GAME_FINISHED
};

class CodeChallenge {
private:
    sf::RenderWindow imageWindow;
    sf::RenderWindow challengeWindow;
    sf::Font font;
    sf::Texture codeImageTexture;
    sf::Sprite codeImageSprite;
    
    GameState currentState;
    sf::Clock autoCloseTimer;
    bool shouldAutoClose;
    
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text codeText;
    sf::Text inputPromptText;
    sf::Text userInputText;
    sf::Text attemptsText;
    sf::Text resultText;
    sf::Text correctAnswerText;
    
    std::string userInput;
    int attemptsLeft;
    bool gameOver;
    bool gameWon;
    std::string correctAnswer;
    std::string challengeCode;
    
    sf::Color backgroundColor;
    sf::Color primaryColor;
    sf::Color successColor;
    sf::Color errorColor;
    sf::Color codeBackgroundColor;

public:
    CodeChallenge()
    : imageWindow(sf::VideoMode(600, 400), "Click to Start Challenge"),
      challengeWindow(sf::VideoMode(1200, 800), "C++ Code Output Challenge"),
      currentState(WAITING_FOR_START),
      attemptsLeft(2), gameOver(false), gameWon(false),
      shouldAutoClose(false),
      correctAnswer("7.6"),
      backgroundColor(sf::Color(40, 44, 52)),
      primaryColor(sf::Color(97, 218, 251)),
      successColor(sf::Color(152, 195, 121)),
      errorColor(sf::Color(224, 108, 117)),
      codeBackgroundColor(sf::Color(30, 34, 40)) {
        challengeCode =
            "#include <iostream>\nusing namespace std;\n#define ll long long\n\n"
            "int main() {\n    float i = 2.8000006;\n    i = i++ + ++i;\n    cout << i << endl;\n    return 0;\n}";
        challengeWindow.setVisible(false);
        initializeUI();
        loadAssets();
    }
    
    void initializeUI() {
        // Initialize SFML Text objects with font, positions, etc.
    }
    void loadAssets() {
        // Load images, font, textures, etc.
    }
    void updateAttemptsText() {
        attemptsText.setString("Attempts left: " + std::to_string(attemptsLeft));
    }
    void handleInput(const std::string& input) {
        if (input == correctAnswer) {
            gameWon = true;
            resultText.setString("Correct! Well done.");
            resultText.setFillColor(successColor);
            shouldAutoClose = true;
            autoCloseTimer.restart();
        } else {
            attemptsLeft--;
            updateAttemptsText();
            if (attemptsLeft <= 0) {
                gameOver = true;
                resultText.setString("Game Over. Correct was: " + correctAnswer);
                resultText.setFillColor(errorColor);
                shouldAutoClose = true;
                autoCloseTimer.restart();
            } else {
                resultText.setString("Wrong, try again.");
                resultText.setFillColor(errorColor);
            }
        }
    }
    void processTextInput(sf::Uint32 unicode) {
        // Handle backspace, enter, and character input
    }
    void handleImageWindowClick(int x, int y) {
        if (currentState == WAITING_FOR_START) startChallenge();
    }
    void startChallenge() {
        imageWindow.setVisible(false);
        challengeWindow.setVisible(true);
        currentState = CHALLENGE_ACTIVE;
    }
    void handleKeyPress(sf::Keyboard::Key key) {
        // On Enter, submit userInput
    }
    void drawImageWindow() {
        imageWindow.clear(backgroundColor);
        // Draw start prompt
        imageWindow.display();
    }
    void drawChallengeWindow() {
        challengeWindow.clear(backgroundColor);
        // Draw code listing, input prompt, attempt count, result
        challengeWindow.display();
    }

    void run() {
        while (imageWindow.isOpen() || challengeWindow.isOpen()) {
            sf::Event evt;
            while (imageWindow.pollEvent(evt)) {
                if (evt.type == sf::Event::Closed)
                    imageWindow.close();
                if (evt.type == sf::Event::MouseButtonPressed)
                    handleImageWindowClick(evt.mouseButton.x, evt.mouseButton.y);
            }
            while (challengeWindow.pollEvent(evt)) {
                if (evt.type == sf::Event::Closed)
                    challengeWindow.close();
                if (evt.type == sf::Event::TextEntered)
                    processTextInput(evt.text.unicode);
                if (evt.type == sf::Event::KeyPressed)
                    handleKeyPress(evt.key.code);
            }
            if (shouldAutoClose && autoCloseTimer.getElapsedTime().asSeconds() >= 3.0f) {
                challengeWindow.close(); break;
            }
            if (imageWindow.isOpen() && currentState == WAITING_FOR_START) drawImageWindow();
            if (challengeWindow.isOpen() && currentState == CHALLENGE_ACTIVE) drawChallengeWindow();
        }
    }
};

void runCodeChallenge() {
    CodeChallenge challenge;
    challenge.run();
}