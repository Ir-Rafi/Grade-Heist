#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>
#include <algorithm>
#include<cmath>

class CombinationLockWindow {
private:
    sf::RenderWindow lockWindow;
    sf::Font font;
    
    // Lock data
    std::string correctCode;
    std::string enteredCode;
    std::string hintText;
    
    // UI elements
    sf::Text titleText;
    sf::Text hintDisplayText;
    sf::Text codeDisplayText;
    sf::Text instructionText;
    sf::Text resultText;
    sf::Text statusText;
    sf::Text attemptText;
    
    // Visual elements
    sf::RectangleShape lockFrame;
    sf::CircleShape lockDial;
    sf::RectangleShape codeDisplay;
    std::vector<sf::RectangleShape> digitBoxes;
    std::vector<sf::Text> digitTexts;
    std::vector<sf::RectangleShape> numberButtons;
    std::vector<sf::Text> buttonTexts;
    sf::RectangleShape enterButton;
    sf::RectangleShape clearButton;
    sf::Text enterButtonText;
    sf::Text clearButtonText;
    
    // Background elements
    sf::RectangleShape backgroundGradient;
    std::vector<sf::CircleShape> backgroundParticles;
    
    // Timer
    sf::Clock gameClock;
    sf::Clock blinkClock;
    sf::Clock particleClock;
    float timeLimit;
    float timeRemaining;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color lockColor;
    sf::Color successColor;
    sf::Color failColor;
    
    // Game state
    enum LockState { PLAYING, SUCCESS, FAILED, HINT_DISPLAY };
    LockState currentState;
    
    bool taskCompleted;
    bool taskFailed;
    bool windowShouldClose;
    bool showingHint;
    int maxDigits;
    int attemptsRemaining;
    int maxAttempts;

public:
    CombinationLockWindow() : lockWindow(sf::VideoMode(1000, 800), "Welcome to the Last Stage", sf::Style::Close) {
        initialize();
    }
    
    void initialize() {
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        // Setup lock data
        correctCode = "74526";
        hintText = "Put all the numbers u got throughout the whole journey";
        maxDigits = 5;
        maxAttempts = 2;
        attemptsRemaining = maxAttempts;
        
        // Setup colors
        backgroundColor = sf::Color(15, 15, 25);
        lockColor = sf::Color(70, 70, 90);
        successColor = sf::Color(0, 255, 100);
        failColor = sf::Color(255, 50, 50);
        
        setupBackground();
        setupVisualElements();
        setupTexts();
        
        // Initialize game state
        currentState = HINT_DISPLAY;
        timeLimit = 60.0f; // 60 seconds to solve
        taskCompleted = false;
        taskFailed = false;
        windowShouldClose = false;
        showingHint = true;
        enteredCode = "";
        
        gameClock.restart();
        particleClock.restart();
    }
    
    void setupBackground() {
        // Create animated background particles
        backgroundParticles.clear();
        for (int i = 0; i < 20; i++) {
            sf::CircleShape particle(2 + rand() % 4);
            particle.setPosition(rand() % 1000, rand() % 800);
            particle.setFillColor(sf::Color(100 + rand() % 100, 100 + rand() % 100, 150 + rand() % 100, 100 + rand() % 100));
            backgroundParticles.push_back(particle);
        }
    }
    
    void updateBackground() {
        // Animate background particles
        float elapsed = particleClock.restart().asSeconds();
        for (auto& particle : backgroundParticles) {
            sf::Vector2f pos = particle.getPosition();
            pos.x += (30 + rand() % 20) * elapsed;
            pos.y += sin(pos.x * 0.01f) * 20 * elapsed;
            
            if (pos.x > 1020) {
                pos.x = -20;
                pos.y = rand() % 800;
            }
            particle.setPosition(pos);
        }
    }
    
    void setupVisualElements() {
        // Main lock frame with enhanced design
        lockFrame.setSize(sf::Vector2f(450, 550));
        lockFrame.setPosition(275, 120);
        lockFrame.setFillColor(sf::Color(40, 40, 60, 200));
        lockFrame.setOutlineThickness(6);
        lockFrame.setOutlineColor(sf::Color(120, 120, 140));
        
        // Lock dial (decorative) - larger and more prominent
        lockDial.setRadius(90);
        lockDial.setPosition(405, 180);
        lockDial.setFillColor(sf::Color(50, 50, 70));
        lockDial.setOutlineThickness(4);
        lockDial.setOutlineColor(sf::Color::White);
        
        // Code display area
        codeDisplay.setSize(sf::Vector2f(350, 60));
        codeDisplay.setPosition(325, 340);
        codeDisplay.setFillColor(sf::Color(20, 20, 20));
        codeDisplay.setOutlineThickness(3);
        codeDisplay.setOutlineColor(sf::Color(0, 255, 0));
        
        // Digit boxes - 5 boxes for 5 digits
        digitBoxes.clear();
        digitTexts.clear();
        for (int i = 0; i < maxDigits; i++) {
            sf::RectangleShape box(sf::Vector2f(55, 45));
            box.setPosition(345 + i * 65, 350);
            box.setFillColor(sf::Color(30, 30, 30));
            box.setOutlineThickness(2);
            box.setOutlineColor(sf::Color(0, 200, 0));
            digitBoxes.push_back(box);
            
            sf::Text digit;
            digit.setFont(font);
            digit.setCharacterSize(28);
            digit.setFillColor(sf::Color(0, 255, 0));
            digit.setPosition(360 + i * 65, 355);
            digitTexts.push_back(digit);
        }
        
        // Number buttons (1-9, 0) arranged like phone keypad - enhanced design
        numberButtons.clear();
        buttonTexts.clear();
        
        // Buttons 1-9 in 3x3 grid
        for (int i = 1; i <= 9; i++) {
            sf::RectangleShape button(sf::Vector2f(70, 55));
            int row = (i - 1) / 3;
            int col = (i - 1) % 3;
            button.setPosition(80 + col * 80, 450 + row * 65);
            button.setFillColor(sf::Color(60, 80, 100));
            button.setOutlineThickness(3);
            button.setOutlineColor(sf::Color(150, 150, 150));
            numberButtons.push_back(button);
            
            sf::Text buttonText;
            buttonText.setFont(font);
            buttonText.setCharacterSize(24);
            buttonText.setFillColor(sf::Color::White);
            buttonText.setString(std::to_string(i));
            
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            buttonText.setPosition(80 + col * 80 + 35 - textBounds.width/2, 450 + row * 65 + 27 - textBounds.height/2);
            buttonTexts.push_back(buttonText);
        }
        
        // Button 0 at bottom center
        sf::RectangleShape zeroButton(sf::Vector2f(70, 55));
        zeroButton.setPosition(80 + 80, 645); // Center position under 8
        zeroButton.setFillColor(sf::Color(60, 80, 100));
        zeroButton.setOutlineThickness(3);
        zeroButton.setOutlineColor(sf::Color(150, 150, 150));
        numberButtons.push_back(zeroButton);
        
        sf::Text zeroText;
        zeroText.setFont(font);
        zeroText.setCharacterSize(24);
        zeroText.setFillColor(sf::Color::White);
        zeroText.setString("0");
        
        sf::FloatRect zeroBounds = zeroText.getLocalBounds();
        zeroText.setPosition(80 + 80 + 35 - zeroBounds.width/2, 645 + 27 - zeroBounds.height/2);
        buttonTexts.push_back(zeroText);
        
        // Enter button - enhanced design
        enterButton.setSize(sf::Vector2f(100, 60));
        enterButton.setPosition(750, 480);
        enterButton.setFillColor(sf::Color(0, 150, 0));
        enterButton.setOutlineThickness(3);
        enterButton.setOutlineColor(sf::Color::White);
        
        // Clear button - enhanced design
        clearButton.setSize(sf::Vector2f(100, 60));
        clearButton.setPosition(750, 560);
        clearButton.setFillColor(sf::Color(150, 0, 0));
        clearButton.setOutlineThickness(3);
        clearButton.setOutlineColor(sf::Color::White);
    }
    
    void setupTexts() {
        // Title - enhanced
        titleText.setFont(font);
        titleText.setString("Welcome to the Last Stage");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color(255, 100, 100));
        titleText.setPosition(280, 30);
        
        // Hint display - enhanced
        hintDisplayText.setFont(font);
        hintDisplayText.setString("HINT: " + hintText);
        hintDisplayText.setCharacterSize(18);
        hintDisplayText.setFillColor(sf::Color(255, 255, 100));
        hintDisplayText.setPosition(50, 150);
        
        // Code display
        codeDisplayText.setFont(font);
        codeDisplayText.setString("Enter 5-digit code:");
        codeDisplayText.setCharacterSize(20);
        codeDisplayText.setFillColor(sf::Color::White);
        codeDisplayText.setPosition(380, 315);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Click numbers to enter code (order doesn't matter)");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color(100, 255, 255));
        instructionText.setPosition(50, 420);
        
        // Button texts
        enterButtonText.setFont(font);
        enterButtonText.setString("ENTER");
        enterButtonText.setCharacterSize(18);
        enterButtonText.setFillColor(sf::Color::White);
        enterButtonText.setPosition(775, 500);
        
        clearButtonText.setFont(font);
        clearButtonText.setString("CLEAR");
        clearButtonText.setCharacterSize(18);
        clearButtonText.setFillColor(sf::Color::White);
        clearButtonText.setPosition(775, 580);
        
        // Status text
        statusText.setFont(font);
        statusText.setCharacterSize(18);
        statusText.setFillColor(sf::Color::Green);
        statusText.setPosition(50, 100);
        
        // Attempt text
        attemptText.setFont(font);
        attemptText.setCharacterSize(20);
        attemptText.setFillColor(sf::Color::Yellow);
        attemptText.setPosition(600, 420);
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(24);
        resultText.setPosition(200, 720);
    }
    
    void handleMouseClick(int x, int y) {
        if (currentState == HINT_DISPLAY) {
            // Click anywhere to start the actual lock challenge
            currentState = PLAYING;
            gameClock.restart();
            return;
        }
        
        if (currentState != PLAYING) return;
        
        // Check number buttons
        for (int i = 0; i < 10; i++) {
            if (numberButtons[i].getGlobalBounds().contains(x, y)) {
                if (enteredCode.length() < maxDigits) {
                    std::string digit;
                    if (i < 9) {
                        digit = std::to_string(i + 1); // Buttons 1-9
                    } else {
                        digit = "0"; // Button 0 is at index 9
                    }
                    enteredCode += digit;
                    updateCodeDisplay();
                }
                return;
            }
        }
        
        // Check enter button
        if (enterButton.getGlobalBounds().contains(x, y)) {
            if (enteredCode.length() == maxDigits) {
                checkCombination();
            }
            return;
        }
        
        // Check clear button
        if (clearButton.getGlobalBounds().contains(x, y)) {
            enteredCode.clear();
            updateCodeDisplay();
            return;
        }
    }
    
    void updateCodeDisplay() {
        for (int i = 0; i < maxDigits; i++) {
            if (i < enteredCode.length()) {
                digitTexts[i].setString(std::string(1, enteredCode[i]));
            } else {
                digitTexts[i].setString("_");
            }
        }
    }
    
    bool checkDigitsMatch(const std::string& entered, const std::string& correct) {
        // Check if entered code contains all digits from correct code (order doesn't matter)
        std::string enteredSorted = entered;
        std::string correctSorted = correct;
        std::sort(enteredSorted.begin(), enteredSorted.end());
        std::sort(correctSorted.begin(), correctSorted.end());
        return enteredSorted == correctSorted;
    }
    
    void checkCombination() {
        if (checkDigitsMatch(enteredCode, correctCode)) {
            taskCompleted = true;
            currentState = SUCCESS;
            resultText.setString("Task passed, Hope next time u will study hard to get a good grade");
            resultText.setFillColor(successColor);
            resultText.setPosition(150, 720);
            
            // Change lock color to green
            lockFrame.setOutlineColor(successColor);
            lockDial.setOutlineColor(successColor);
            codeDisplay.setOutlineColor(successColor);
            for (auto& box : digitBoxes) {
                box.setOutlineColor(successColor);
            }
        } else {
            attemptsRemaining--;
            if (attemptsRemaining <= 0) {
                taskFailed = true;
                currentState = FAILED;
                resultText.setString("You are failed! It is not the correct time for u to escape");
                resultText.setFillColor(failColor);
                resultText.setPosition(150, 720);
                
                // Change lock color to red
                lockFrame.setOutlineColor(failColor);
                lockDial.setOutlineColor(failColor);
                codeDisplay.setOutlineColor(failColor);
                for (auto& box : digitBoxes) {
                    box.setOutlineColor(failColor);
                }
            } else {
                // Wrong attempt but still have attempts left
                resultText.setString("Wrong code! " + std::to_string(attemptsRemaining) + " attempt(s) remaining");
                resultText.setFillColor(sf::Color::Yellow);
                resultText.setPosition(300, 720);
                
                // Flash red briefly
                lockFrame.setOutlineColor(failColor);
                lockDial.setOutlineColor(failColor);
                codeDisplay.setOutlineColor(failColor);
                for (auto& box : digitBoxes) {
                    box.setOutlineColor(failColor);
                }
                
                // Clear entered code after wrong attempt
                enteredCode.clear();
                updateCodeDisplay();
                
                // Reset to normal colors after 1 second
                gameClock.restart();
            }
        }
    }
    
    void update() {
        updateBackground();
        
        if (currentState == PLAYING) {
            timeRemaining = timeLimit - gameClock.getElapsedTime().asSeconds();
            
            if (timeRemaining <= 0 && !taskCompleted) {
                taskFailed = true;
                currentState = FAILED;
                resultText.setString("You are failed! It is not the correct time for u to escape");
                resultText.setFillColor(failColor);
                resultText.setPosition(150, 720);
            }
            
            // Update status
            statusText.setString("Time remaining: " + std::to_string((int)timeRemaining) + "s");
            attemptText.setString("Attempts: " + std::to_string(attemptsRemaining) + "/" + std::to_string(maxAttempts));
            
            // Change status color based on time
            if (timeRemaining < 15) {
                statusText.setFillColor(failColor);
            } else if (timeRemaining < 30) {
                statusText.setFillColor(sf::Color::Yellow);
            } else {
                statusText.setFillColor(sf::Color::Green);
            }
            
            // Reset colors after wrong attempt flash
            if (gameClock.getElapsedTime().asSeconds() > 1.0f && attemptsRemaining > 0 && !taskCompleted) {
                lockFrame.setOutlineColor(sf::Color(120, 120, 140));
                lockDial.setOutlineColor(sf::Color::White);
                codeDisplay.setOutlineColor(sf::Color(0, 255, 0));
                for (auto& box : digitBoxes) {
                    box.setOutlineColor(sf::Color(0, 200, 0));
                }
                resultText.setString("");
            }
        }
        
        // Auto-close window after completion
        if ((currentState == SUCCESS || (currentState == FAILED && taskFailed)) && 
            gameClock.getElapsedTime().asSeconds() > 5.0f) {
            windowShouldClose = true;
        }
    }
    
    void render() {
        lockWindow.clear(backgroundColor);
        
        // Draw animated background
        for (const auto& particle : backgroundParticles) {
            lockWindow.draw(particle);
        }
        
        lockWindow.draw(titleText);
        
        if (currentState == HINT_DISPLAY) {
            // Show hint screen with enhanced design
            sf::Text hintTitle;
            hintTitle.setFont(font);
            hintTitle.setString("FINAL CHALLENGE - SECURITY LOCK");
            hintTitle.setCharacterSize(36);
            hintTitle.setFillColor(sf::Color(255, 200, 100));
            hintTitle.setPosition(200, 250);
            lockWindow.draw(hintTitle);
            
            // Enhanced hint display
            sf::RectangleShape hintBox(sf::Vector2f(700, 150));
            hintBox.setPosition(150, 320);
            hintBox.setFillColor(sf::Color(30, 30, 50, 180));
            hintBox.setOutlineThickness(3);
            hintBox.setOutlineColor(sf::Color(100, 200, 255));
            lockWindow.draw(hintBox);
            
            sf::Text wrappedHint;
            wrappedHint.setFont(font);
            wrappedHint.setString("HINT:\n" + hintText + "\n\nDigits: 7, 4, 5, 2, 6 (any order)");
            wrappedHint.setCharacterSize(20);
            wrappedHint.setFillColor(sf::Color(150, 255, 150));
            wrappedHint.setPosition(180, 350);
            lockWindow.draw(wrappedHint);
            
            sf::Text clickText;
            clickText.setFont(font);
            clickText.setString("Click anywhere to start the challenge");
            clickText.setCharacterSize(22);
            clickText.setFillColor(sf::Color::White);
            clickText.setPosition(300, 550);
            lockWindow.draw(clickText);
            
        } else {
            // Draw lock interface
            lockWindow.draw(lockFrame);
            lockWindow.draw(lockDial);
            lockWindow.draw(codeDisplay);
            lockWindow.draw(codeDisplayText);
            
            // Draw digit boxes and digits
            for (int i = 0; i < maxDigits; i++) {
                lockWindow.draw(digitBoxes[i]);
                lockWindow.draw(digitTexts[i]);
            }
            
            // Draw number buttons
            for (int i = 0; i < 10; i++) {
                lockWindow.draw(numberButtons[i]);
                lockWindow.draw(buttonTexts[i]);
            }
            
            // Draw control buttons
            lockWindow.draw(enterButton);
            lockWindow.draw(clearButton);
            lockWindow.draw(enterButtonText);
            lockWindow.draw(clearButtonText);
            
            lockWindow.draw(instructionText);
            lockWindow.draw(statusText);
            lockWindow.draw(attemptText);
            lockWindow.draw(resultText);
            
            if (currentState == SUCCESS || (currentState == FAILED && taskFailed)) {
                sf::Text closeText;
                closeText.setFont(font);
                closeText.setString("Window will close automatically...");
                closeText.setCharacterSize(18);
                closeText.setFillColor(sf::Color::White);
                closeText.setPosition(350, 750);
                lockWindow.draw(closeText);
            }
        }
        
        lockWindow.display();
    }
    
    bool run() {
        updateCodeDisplay();
        
        while (lockWindow.isOpen() && !windowShouldClose) {
            sf::Event event;
            while (lockWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    lockWindow.close();
                    return false;
                }
                
                if (event.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
            }
            
            update();
            render();
        }
        
        lockWindow.close();
        return taskCompleted;
    }
};

int main() {
    CombinationLockWindow game;
    game.run();
    return 0;
}