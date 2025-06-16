#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <cctype>

class TypingChallenge {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text stringText;
    sf::Text inputText;
    sf::Text timerText;
    sf::Text resultText;
    sf::Text instructionText;
    sf::RectangleShape inputBox;
    
    std::string targetString;
    std::string userInput;
    std::string correctAnswer;
    
    sf::Clock gameClock;
    float timeLimit;
    bool gameActive;
    bool gameCompleted;
    bool taskPassed;
    
    std::mt19937 rng;
    
public:
    TypingChallenge() : window(sf::VideoMode(800, 600), "Letter to Number Challenge"),
                       timeLimit(25.0f), gameActive(false), gameCompleted(false), 
                       taskPassed(false), rng(std::random_device{}()) {
        
        // Load font (you might need to adjust the path)
        if (!font.loadFromFile("arial.ttf")) {
            // If arial.ttf is not found, try to use default system font
            // In a real application, you should handle this more gracefully
            std::cout << "Warning: Could not load arial.ttf. Using default font." << std::endl;
        }
        
        setupUI();
        generateRandomString();
        startGame();
    }
    
    void setupUI() {
        // Title
        titleText.setFont(font);
        titleText.setString("Letter to Number Challenge");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(200, 50);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Convert each letter to its position number (A=1, B=2, ... Z=26)\nType the numbers separated by spaces");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::Yellow);
        instructionText.setPosition(50, 100);
        
        // Target string
        stringText.setFont(font);
        stringText.setCharacterSize(28);
        stringText.setFillColor(sf::Color::Cyan);
        stringText.setPosition(50, 180);
        
        // Input box
        inputBox.setSize(sf::Vector2f(700, 40));
        inputBox.setPosition(50, 250);
        inputBox.setFillColor(sf::Color(50, 50, 50));
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(sf::Color::White);
        
        // User input text
        inputText.setFont(font);
        inputText.setCharacterSize(20);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(55, 255);
        
        // Timer
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::Red);
        timerText.setPosition(50, 320);
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(32);
        resultText.setPosition(200, 400);
    }
    
    void generateRandomString() {
        const int stringLength = 5 + (rng() % 6); // 5-10 characters
        targetString = "";
        correctAnswer = "";
        
        for (int i = 0; i < stringLength; i++) {
            char letter = 'A' + (rng() % 26);
            // Randomly make some letters lowercase
            if (rng() % 2 == 0) {
                letter = std::tolower(letter);
            }
            targetString += letter;
            
            // Calculate the correct number (A=1, B=2, etc.)
            int letterValue = std::toupper(letter) - 'A' + 1;
            if (i > 0) correctAnswer += " ";
            correctAnswer += std::to_string(letterValue);
        }
        
        stringText.setString("String: " + targetString);
    }
    
    void startGame() {
        gameClock.restart();
        gameActive = true;
        gameCompleted = false;
        userInput = "";
        inputText.setString("");
        resultText.setString("");
        resultText.setFillColor(sf::Color::White);
    }
    
    void handleInput(sf::Event& event) {
        if (!gameActive || gameCompleted) return;
        
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (!userInput.empty()) {
                    userInput.pop_back();
                    inputText.setString(userInput);
                }
            } else if (event.text.unicode == 13) { // Enter
                checkAnswer();
            } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                userInput += static_cast<char>(event.text.unicode);
                inputText.setString(userInput);
            }
        }
    }
    
    void checkAnswer() {
        if (!gameActive) return;
        
        gameActive = false;
        gameCompleted = true;
        
        // Trim whitespace from user input
        std::string trimmedInput = userInput;
        trimmedInput.erase(0, trimmedInput.find_first_not_of(" \t"));
        trimmedInput.erase(trimmedInput.find_last_not_of(" \t") + 1);
        
        // Check if the answer is correct
        if (trimmedInput == correctAnswer) {
            taskPassed = true;
            resultText.setString("TASK PASSED!\nRoom Number: 405(Go and turn off this room's Ac)");
            resultText.setFillColor(sf::Color::Green);
        } else {
            taskPassed = false;
            resultText.setString("TASK FAILED!\nCorrect answer: " + correctAnswer);
            resultText.setFillColor(sf::Color::Red);
        }
    }
    
    void update() {
        if (!gameActive || gameCompleted) return;
        
        float elapsedTime = gameClock.getElapsedTime().asSeconds();
        float remainingTime = timeLimit - elapsedTime;
        
        if (remainingTime <= 0) {
            // Time's up!
            gameActive = false;
            gameCompleted = true;
            taskPassed = false;
            resultText.setString("TASK FAILED!\nTime's up!\nCorrect answer: " + correctAnswer);
            resultText.setFillColor(sf::Color::Red);
            remainingTime = 0;
        }
        
        timerText.setString("Time remaining: " + std::to_string(static_cast<int>(remainingTime)) + "s");
        
        // Change timer color as time runs out
        if (remainingTime < 10) {
            timerText.setFillColor(sf::Color::Red);
        } else if (remainingTime < 15) {
            timerText.setFillColor(sf::Color::Yellow);
        } else {
            timerText.setFillColor(sf::Color::Green);
        }
    }
    
    void render() {
        window.clear(sf::Color::Black);
        
        window.draw(titleText);
        window.draw(instructionText);
        window.draw(stringText);
        window.draw(inputBox);
        window.draw(inputText);
        window.draw(timerText);
        
        if (gameCompleted) {
            window.draw(resultText);
        }
        
        window.display();
    }
    
    void run() {
        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space && gameCompleted) {
                        // Restart the game
                        generateRandomString();
                        startGame();
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
                
                handleInput(event);
            }
            
            update();
            render();
        }
    }
};

int main() {
    try {
        TypingChallenge game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}