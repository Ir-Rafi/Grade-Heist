#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <iomanip>

class PasswordGame {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText, instructionText, cipherText, inputText, resultText, attemptsText, timerText;
    sf::RectangleShape inputBox;
    
    std::string originalMessage;
    std::string encryptedMessage;
    std::string userInput;
    int caesarShift;
    int attempts;
    bool gameWon;
    bool gameLost;
    bool showCredentials;
    bool inputActive;
    
    sf::RectangleShape credentialBox;
    sf::Text credentialText;
    
    sf::Clock gameTimer;
    sf::Clock closeTimer;
    float timeLimit; // 1 minute 50 seconds = 110 seconds
    bool startCloseTimer;
    
public:
    PasswordGame() : window(sf::VideoMode(1200, 800), "Capture The Ultimate Password"),
                     attempts(3), gameWon(false), gameLost(false), showCredentials(false),
                     inputActive(true), timeLimit(110.0f), startCloseTimer(false) {
        
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load font file. Using default font." << std::endl;
        }
        
        generateCipherChallenge();
        setupUI();
        gameTimer.restart();
    }
    
    void generateCipherChallenge() {
        std::random_device rd;
        std::mt19937 gen(rd());
        
        // Random messages to encrypt
        std::vector<std::string> messages = {
            "CONGRATULATIONS YOU SOLVED THE CIPHER",
            "THE ULTIMATE PASSWORD IS YOURS NOW",
            "EXCELLENT WORK ON BREAKING THE CODE",
            "YOU HAVE MASTERED THE CAESAR CIPHER",
            "VICTORY IS YOURS WELL DONE AGENT",
            "SECRET MESSAGE DECODED SUCCESSFULLY",
            "MISSION ACCOMPLISHED PASSWORD UNLOCKED"
        };
        
        std::uniform_int_distribution<> msgDis(0, messages.size() - 1);
        std::uniform_int_distribution<> shiftDis(1, 25);
        
        originalMessage = messages[msgDis(gen)];
        caesarShift = shiftDis(gen);
        
        // Encrypt the message using Caesar cipher
        encryptedMessage = caesarEncrypt(originalMessage, caesarShift);
    }
    
    std::string caesarEncrypt(const std::string& text, int shift) {
        std::string result = "";
        for (char c : text) {
            if (std::isalpha(c)) {
                char base = std::isupper(c) ? 'A' : 'a';
                result += char((c - base + shift) % 26 + base);
            } else {
                result += c;
            }
        }
        return result;
    }
    
    std::string caesarDecrypt(const std::string& text, int shift) {
        return caesarEncrypt(text, 26 - shift);
    }
    
    void setupUI() {
        // Title
        titleText.setFont(font);
        titleText.setString("Capture The Ultimate Password");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(300, 20);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("To get the ultimate sir's laptop password, you need to decode this Caesar cipher.\nUse Caesar Shifting to decrypt the message below.\nThe shift value is between 1-25. You have 1 minute 50 seconds and 3 attempts.\nType your decrypted message in the box below:");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::Yellow);
        instructionText.setPosition(50, 70);
        
        // Encrypted message display
        cipherText.setFont(font);
        cipherText.setString("Encrypted Message: " + encryptedMessage);
        cipherText.setCharacterSize(20);
        cipherText.setFillColor(sf::Color::Cyan);
        cipherText.setPosition(50, 180);
        
        // Input box
        inputBox.setSize(sf::Vector2f(800, 50));
        inputBox.setFillColor(sf::Color::White);
        inputBox.setOutlineThickness(3);
        inputBox.setOutlineColor(sf::Color::Blue);
        inputBox.setPosition(50, 250);
        
        // Input text
        inputText.setFont(font);
        inputText.setCharacterSize(18);
        inputText.setFillColor(sf::Color::Black);
        inputText.setPosition(60, 265);
        
        // Timer display
        timerText.setFont(font);
        timerText.setCharacterSize(20);
        timerText.setFillColor(sf::Color::Red);
        timerText.setPosition(50, 320);
        
        // Attempts display
        attemptsText.setFont(font);
        attemptsText.setCharacterSize(20);
        attemptsText.setFillColor(sf::Color::Red);
        attemptsText.setPosition(50, 350);
        updateAttemptsText();
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(24);
        resultText.setFillColor(sf::Color::Green);
        resultText.setPosition(50, 400);
        
        // Credential box
        credentialBox.setSize(sf::Vector2f(500, 150));
        credentialBox.setFillColor(sf::Color::Black);
        credentialBox.setOutlineThickness(3);
        credentialBox.setOutlineColor(sf::Color::Green);
        credentialBox.setPosition(350, 450);
        
        credentialText.setFont(font);
        credentialText.setCharacterSize(20);
        credentialText.setFillColor(sf::Color::Green);
        credentialText.setPosition(370, 480);
    }
    
    void updateAttemptsText() {
        attemptsText.setString("Attempts remaining: " + std::to_string(attempts));
    }
    
    void updateTimer() {
        float elapsed = gameTimer.getElapsedTime().asSeconds();
        float remaining = timeLimit - elapsed;
        
        if (remaining <= 0 && !gameWon) {
            gameLost = true;
            resultText.setString("Time's up! Task Failed! Window will close in 3 seconds...");
            resultText.setFillColor(sf::Color::Red);
            inputActive = false;
            startCloseTimer = true;
            closeTimer.restart();
            return;
        }
        
        int minutes = (int)remaining / 60;
        int seconds = (int)remaining % 60;
        timerText.setString("Time remaining: " + std::to_string(minutes) + ":" + 
                           (seconds < 10 ? "0" : "") + std::to_string(seconds));
    }
    
    void handleTextInput(char c) {
        if (!inputActive || gameWon || gameLost) return;
        
        if (c == '\b') { // Backspace
            if (!userInput.empty()) {
                userInput.pop_back();
            }
        } else if (c == '\r' || c == '\n') { // Enter key
            checkAnswer();
        } else if (std::isprint(c) && userInput.length() < 60) {
            userInput += std::toupper(c);
        }
        
        inputText.setString(userInput);
    }
    
    void checkAnswer() {
        if (userInput.empty()) return;
        
        // Check if the decrypted message matches the original
        if (userInput == originalMessage) {
            gameWon = true;
            resultText.setString("Task Passed! Correct decryption!");
            resultText.setFillColor(sf::Color::Green);
            showCredentials = true;
            credentialText.setString("CREDENTIALS UNLOCKED:\n\nUsername: Admin\nPassword: 924689093745\n\nWindow will close in 10 seconds...");
            inputActive = false;
            startCloseTimer = true;
            closeTimer.restart();
        } else {
            attempts--;
            updateAttemptsText();
            
            if (attempts <= 0) {
                gameLost = true;
                resultText.setString("Task Failed! Correct message was: " + originalMessage + "\nWindow will close in 3 seconds...");
                resultText.setFillColor(sf::Color::Red);
                inputActive = false;
                startCloseTimer = true;
                closeTimer.restart();
            } else {
                resultText.setString("Wrong! Try again. Hint: Caesar shift = " + std::to_string(caesarShift));
                resultText.setFillColor(sf::Color::Red);
                userInput.clear();
                inputText.setString("");
            }
        }
    }
    
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                
                if (event.type == sf::Event::TextEntered) {
                    handleTextInput(static_cast<char>(event.text.unicode));
                }
                
                if (event.type == sf::Event::KeyPressed) {
                    if ((gameLost || gameWon) && event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
            }
            
            // Update timer
            if (!gameWon && !gameLost) {
                updateTimer();
            }
            
            // Auto-close after game ends
            float closeTime = gameWon ? 10.0f : 3.0f; // 10 seconds for win, 3 for loss
            if (startCloseTimer && closeTimer.getElapsedTime().asSeconds() > closeTime) {
                window.close();
            }
            
            window.clear(sf::Color::Black);
            
            // Draw UI elements
            window.draw(titleText);
            window.draw(instructionText);
            window.draw(cipherText);
            window.draw(inputBox);
            window.draw(inputText);
            window.draw(timerText);
            window.draw(attemptsText);
            window.draw(resultText);
            
            // Draw credentials if won
            if (showCredentials) {
                window.draw(credentialBox);
                window.draw(credentialText);
            }
            
            window.display();
        }
    }
};

int main() {
    PasswordGame game;
    game.run();
    return 0;
}
