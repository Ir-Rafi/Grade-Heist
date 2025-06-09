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
    
    // Game state
    GameState currentState;
    sf::Clock autoCloseTimer;
    bool shouldAutoClose;
    
    // Text elements
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text codeText;
    sf::Text inputPromptText;
    sf::Text userInputText;
    sf::Text attemptsText;
    sf::Text resultText;
    sf::Text correctAnswerText;
    sf::Text imageWindowText;
    
    // Input handling
    std::string userInput;
    int attemptsLeft;
    bool gameOver;
    bool gameWon;
    std::string correctAnswer;
    std::string challengeCode;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color primaryColor;
    sf::Color successColor;
    sf::Color errorColor;
    sf::Color codeBackgroundColor;
    
public:
    CodeChallenge() : imageWindow(sf::VideoMode(600, 400), "Click to Start Challenge"),
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
        
        // Set up the challenge code
        challengeCode = "#include <iostream>\nusing namespace std;\n#define ll long long\n\nint main() {\n    float i = 2.8000006;\n    i = i++ + ++i;\n    cout << i << endl;\n    return 0;\n}";
        
        // Initially hide the challenge window
        challengeWindow.setVisible(false);
        
        initializeUI();
        loadAssets();
    }
    
    void initializeUI() {
        // Load font (you'll need to provide a font file)
        if (!font.loadFromFile("arial.ttf")) {
            // Fallback to default font if arial.ttf not found
            std::cout << "Warning: Could not load arial.ttf font file\n";
        }
        
        // Image window text
        imageWindowText.setFont(font);
        imageWindowText.setString("Click on the code image to start the challenge!");
        imageWindowText.setCharacterSize(20);
        imageWindowText.setFillColor(sf::Color::White);
        imageWindowText.setPosition(50, 30);
        
        // Title
        titleText.setFont(font);
        titleText.setString("C++ Code Output Challenge");
        titleText.setCharacterSize(36);
        titleText.setFillColor(primaryColor);
        titleText.setPosition(50, 30);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Guess the output of the C++ code below. You have 2 attempts!");
        instructionText.setCharacterSize(18);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(50, 80);
        
        // Code display
        codeText.setFont(font);
        codeText.setString(challengeCode);
        codeText.setCharacterSize(14);
        codeText.setFillColor(sf::Color(171, 178, 191));
        codeText.setPosition(50, 200);
        
        // Input prompt
        inputPromptText.setFont(font);
        inputPromptText.setString("Enter your answer:");
        inputPromptText.setCharacterSize(20);
        inputPromptText.setFillColor(sf::Color::White);
        inputPromptText.setPosition(50, 450);
        
        // User input display
        userInputText.setFont(font);
        userInputText.setString("");
        userInputText.setCharacterSize(20);
        userInputText.setFillColor(primaryColor);
        userInputText.setPosition(250, 450);
        
        // Attempts left
        attemptsText.setFont(font);
        updateAttemptsText();
        attemptsText.setCharacterSize(16);
        attemptsText.setFillColor(sf::Color::Yellow);
        attemptsText.setPosition(50, 500);
        
        // Result text
        resultText.setFont(font);
        resultText.setString("");
        resultText.setCharacterSize(24);
        resultText.setPosition(50, 550);
        
        // Correct answer text
        correctAnswerText.setFont(font);
        correctAnswerText.setString("");
        correctAnswerText.setCharacterSize(20);
        correctAnswerText.setFillColor(primaryColor);
        correctAnswerText.setPosition(50, 600);
    }
    
    void loadAssets() {
        // Load code image
        if (!codeImageTexture.loadFromFile("code.png")) {
            std::cout << "Warning: Could not load code.png image file\n";
        } else {
            codeImageSprite.setTexture(codeImageTexture);
            
            // Scale image to fit in the image window
            sf::Vector2u imageSize = codeImageTexture.getSize();
            sf::Vector2u windowSize = imageWindow.getSize();
            
            float scaleX = (float)(windowSize.x - 100) / imageSize.x;
            float scaleY = (float)(windowSize.y - 150) / imageSize.y;
            float scale = std::min(scaleX, scaleY);
            
            codeImageSprite.setScale(scale, scale);
            codeImageSprite.setPosition(50, 80);
        }
    }
    
    void updateAttemptsText() {
        attemptsText.setString("Attempts left: " + std::to_string(attemptsLeft));
    }
    
    void handleInput(const std::string& input) {
        if (gameOver) return;
        
        if (input == correctAnswer) {
            gameWon = true;
            gameOver = true;
            resultText.setString("🎉 Correct! Well done!");
            resultText.setFillColor(successColor);
            shouldAutoClose = true;
            autoCloseTimer.restart();
        } else {
            attemptsLeft--;
            updateAttemptsText();
            
            if (attemptsLeft <= 0) {
                gameOver = true;
                resultText.setString("❌ Game Over! Better luck next time!");
                resultText.setFillColor(errorColor);
                correctAnswerText.setString("The correct answer was: " + correctAnswer);
                shouldAutoClose = true;
                autoCloseTimer.restart();
            } else {
                resultText.setString("❌ Wrong answer! Try again.");
                resultText.setFillColor(errorColor);
            }
        }
    }
    
    void processTextInput(sf::Uint32 unicode) {
        if (gameOver || currentState != CHALLENGE_ACTIVE) return;
        
        if (unicode == 8) { // Backspace
            if (!userInput.empty()) {
                userInput.pop_back();
            }
        } else if (unicode == 13) { // Enter
            if (!userInput.empty()) {
                handleInput(userInput);
                userInput.clear();
            }
        } else if (unicode >= 32 && unicode < 127) { // Printable characters
            userInput += static_cast<char>(unicode);
        }
        
        userInputText.setString(userInput + "_"); // Show cursor
    }
    
    void handleImageWindowClick(int x, int y) {
        if (currentState == WAITING_FOR_START) {
            // Check if click is on the image
            sf::FloatRect imageBounds = codeImageSprite.getGlobalBounds();
            if (imageBounds.contains(x, y)) {
                startChallenge();
            }
        }
    }
    
    void startChallenge() {
        currentState = CHALLENGE_ACTIVE;
        imageWindow.setVisible(false);
        challengeWindow.setVisible(true);
        
        // Reset game state
        attemptsLeft = 2;
        gameOver = false;
        gameWon = false;
        userInput.clear();
        resultText.setString("");
        correctAnswerText.setString("");
        userInputText.setString("");
        updateAttemptsText();
    }
    
    void handleKeyPress(sf::Keyboard::Key key) {
        if (key == sf::Keyboard::Escape) {
            imageWindow.close();
            challengeWindow.close();
        }
    }
    
    void drawImageWindow() {
        imageWindow.clear(backgroundColor);
        
        // Draw instruction text
        imageWindow.draw(imageWindowText);
        
        // Draw code image if loaded
        if (codeImageTexture.getSize().x > 0) {
            imageWindow.draw(codeImageSprite);
        }
        
        // Draw click instruction
        sf::Text clickText;
        clickText.setFont(font);
        clickText.setString("Click on the image above to begin!");
        clickText.setCharacterSize(16);
        clickText.setFillColor(sf::Color::Yellow);
        clickText.setPosition(50, imageWindow.getSize().y - 50);
        imageWindow.draw(clickText);
        
        imageWindow.display();
    }
    
    void drawChallengeWindow() {
        challengeWindow.clear(backgroundColor);
        
        // Draw code background rectangle
        sf::RectangleShape codeBackground;
        codeBackground.setSize(sf::Vector2f(1100, 200));
        codeBackground.setPosition(40, 190);
        codeBackground.setFillColor(codeBackgroundColor);
        codeBackground.setOutlineThickness(2);
        codeBackground.setOutlineColor(primaryColor);
        challengeWindow.draw(codeBackground);
        
        // Draw input field background
        sf::RectangleShape inputBackground;
        inputBackground.setSize(sf::Vector2f(300, 35));
        inputBackground.setPosition(240, 445);
        inputBackground.setFillColor(sf::Color(60, 64, 72));
        inputBackground.setOutlineThickness(2);
        inputBackground.setOutlineColor(primaryColor);
        challengeWindow.draw(inputBackground);
        
        // Draw all text elements
        challengeWindow.draw(titleText);
        challengeWindow.draw(instructionText);
        challengeWindow.draw(codeText);
        challengeWindow.draw(inputPromptText);
        challengeWindow.draw(userInputText);
        challengeWindow.draw(attemptsText);
        challengeWindow.draw(resultText);
        challengeWindow.draw(correctAnswerText);
        
        // Show auto-close message if game is over
        if (shouldAutoClose) {
            sf::Text autoCloseText;
            autoCloseText.setFont(font);
            autoCloseText.setString("Window will close automatically in 3 seconds...");
            autoCloseText.setCharacterSize(16);
            autoCloseText.setFillColor(sf::Color::White);
            autoCloseText.setPosition(50, 650);
            challengeWindow.draw(autoCloseText);
        }
        
        challengeWindow.display();
    }
    
    void run() {
        while (imageWindow.isOpen() || challengeWindow.isOpen()) {
            // Handle image window events
            sf::Event imageEvent;
            while (imageWindow.pollEvent(imageEvent)) {
                switch (imageEvent.type) {
                    case sf::Event::Closed:
                        imageWindow.close();
                        challengeWindow.close();
                        break;
                        
                    case sf::Event::MouseButtonPressed:
                        if (imageEvent.mouseButton.button == sf::Mouse::Left) {
                            handleImageWindowClick(imageEvent.mouseButton.x, imageEvent.mouseButton.y);
                        }
                        break;
                        
                    case sf::Event::KeyPressed:
                        handleKeyPress(imageEvent.key.code);
                        break;
                        
                    default:
                        break;
                }
            }
            
            // Handle challenge window events
            sf::Event challengeEvent;
            while (challengeWindow.pollEvent(challengeEvent)) {
                switch (challengeEvent.type) {
                    case sf::Event::Closed:
                        challengeWindow.close();
                        imageWindow.close();
                        break;
                        
                    case sf::Event::TextEntered:
                        processTextInput(challengeEvent.text.unicode);
                        break;
                        
                    case sf::Event::KeyPressed:
                        handleKeyPress(challengeEvent.key.code);
                        break;
                        
                    default:
                        break;
                }
            }
            
            // Auto-close logic
            if (shouldAutoClose && autoCloseTimer.getElapsedTime().asSeconds() >= 3.0f) {
                challengeWindow.close();
                imageWindow.close();
                break;
            }
            
            // Draw windows
            if (imageWindow.isOpen() && currentState == WAITING_FOR_START) {
                drawImageWindow();
            }
            
            if (challengeWindow.isOpen() && currentState == CHALLENGE_ACTIVE) {
                drawChallengeWindow();
            }
        }
    }
};

int main() {
    CodeChallenge challenge;
    challenge.run();
    return 0;
}

// Compilation instructions:
// g++ -o code_challenge main.cpp -lsfml-graphics -lsfml-window -lsfml-system
// 
// Required files:
// - code.png (your code image)
// - arial.ttf (or any other TTF font file)
//
// Features:
// - Separate image window that must be clicked to start
// - Challenge window appears after clicking the image
// - Automatically closes after 3 seconds when game ends (win or lose)
// - 2 attempts to guess the output
// - Visual feedback for correct/wrong answers
// - Clean, modern UI with syntax highlighting colors