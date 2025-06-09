#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <memory>

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
    
    // Timer
    sf::Clock gameClock;
    sf::Clock blinkClock;
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

public:
    CombinationLockWindow() : lockWindow(sf::VideoMode(900, 700), "Security Lock - Access Panel", sf::Style::Close) {
        initialize();
    }
    
    void initialize() {
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        // Setup lock data
        correctCode = "703";
        hintText = "The room where u got to see the whole csedu-30 for the very first time";
        maxDigits = 3;
        
        // Setup colors
        backgroundColor = sf::Color(20, 20, 30);
        lockColor = sf::Color(80, 80, 100);
        successColor = sf::Color(0, 200, 0);
        failColor = sf::Color(200, 0, 0);
        
        setupVisualElements();
        setupTexts();
        
        // Initialize game state
        currentState = HINT_DISPLAY;
        timeLimit = 30.0f; // 30 seconds to solve
        taskCompleted = false;
        taskFailed = false;
        windowShouldClose = false;
        showingHint = true;
        enteredCode = "";
        
        gameClock.restart();
    }
    
    void setupVisualElements() {
        // Main lock frame
        lockFrame.setSize(sf::Vector2f(400, 500));
        lockFrame.setPosition(250, 100);
        lockFrame.setFillColor(lockColor);
        lockFrame.setOutlineThickness(5);
        lockFrame.setOutlineColor(sf::Color(150, 150, 150));
        
        // Lock dial (decorative)
        lockDial.setRadius(80);
        lockDial.setPosition(360, 180);
        lockDial.setFillColor(sf::Color(60, 60, 80));
        lockDial.setOutlineThickness(3);
        lockDial.setOutlineColor(sf::Color::White);
        
        // Code display
        codeDisplay.setSize(sf::Vector2f(200, 50));
        codeDisplay.setPosition(350, 320);
        codeDisplay.setFillColor(sf::Color::Black);
        codeDisplay.setOutlineThickness(2);
        codeDisplay.setOutlineColor(sf::Color::Green);
        
        // Digit boxes
        digitBoxes.clear();
        digitTexts.clear();
        for (int i = 0; i < maxDigits; i++) {
            sf::RectangleShape box(sf::Vector2f(50, 40));
            box.setPosition(360 + i * 60, 330);
            box.setFillColor(sf::Color(40, 40, 40));
            box.setOutlineThickness(1);
            box.setOutlineColor(sf::Color::White);
            digitBoxes.push_back(box);
            
            sf::Text digit;
            digit.setFont(font);
            digit.setCharacterSize(24);
            digit.setFillColor(sf::Color::Green);
            digit.setPosition(375 + i * 60, 335);
            digitTexts.push_back(digit);
        }
        
        // Number buttons (1-9, 0) arranged like phone keypad
        numberButtons.clear();
        buttonTexts.clear();
        
        // Buttons 1-9 in 3x3 grid
        for (int i = 1; i <= 9; i++) {
            sf::RectangleShape button(sf::Vector2f(60, 50));
            int row = (i - 1) / 3;
            int col = (i - 1) % 3;
            button.setPosition(100 + col * 70, 400 + row * 60);
            button.setFillColor(sf::Color(60, 60, 80));
            button.setOutlineThickness(2);
            button.setOutlineColor(sf::Color::White);
            numberButtons.push_back(button);
            
            sf::Text buttonText;
            buttonText.setFont(font);
            buttonText.setCharacterSize(20);
            buttonText.setFillColor(sf::Color::White);
            buttonText.setString(std::to_string(i));
            
            sf::FloatRect textBounds = buttonText.getLocalBounds();
            buttonText.setPosition(100 + col * 70 + 30 - textBounds.width/2, 400 + row * 60 + 25 - textBounds.height/2);
            buttonTexts.push_back(buttonText);
        }
        
        // Button 0 at bottom center
        sf::RectangleShape zeroButton(sf::Vector2f(60, 50));
        zeroButton.setPosition(100 + 70, 580); // Center position under 8
        zeroButton.setFillColor(sf::Color(60, 60, 80));
        zeroButton.setOutlineThickness(2);
        zeroButton.setOutlineColor(sf::Color::White);
        numberButtons.push_back(zeroButton);
        
        sf::Text zeroText;
        zeroText.setFont(font);
        zeroText.setCharacterSize(20);
        zeroText.setFillColor(sf::Color::White);
        zeroText.setString("0");
        
        sf::FloatRect zeroBounds = zeroText.getLocalBounds();
        zeroText.setPosition(100 + 70 + 30 - zeroBounds.width/2, 580 + 25 - zeroBounds.height/2);
        buttonTexts.push_back(zeroText);
        
        // Enter button
        enterButton.setSize(sf::Vector2f(80, 50));
        enterButton.setPosition(650, 450);
        enterButton.setFillColor(sf::Color(0, 120, 0));
        enterButton.setOutlineThickness(2);
        enterButton.setOutlineColor(sf::Color::White);
        
        // Clear button
        clearButton.setSize(sf::Vector2f(80, 50));
        clearButton.setPosition(650, 520);
        clearButton.setFillColor(sf::Color(120, 0, 0));
        clearButton.setOutlineThickness(2);
        clearButton.setOutlineColor(sf::Color::White);
    }
    
    void setupTexts() {
        // Title
        titleText.setFont(font);
        titleText.setString("SECURITY ACCESS PANEL");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::Red);
        titleText.setPosition(280, 30);
        
        // Hint display
        hintDisplayText.setFont(font);
        hintDisplayText.setString("HINT: " + hintText);
        hintDisplayText.setCharacterSize(16);
        hintDisplayText.setFillColor(sf::Color::Yellow);
        hintDisplayText.setPosition(50, 150);
        
        // Code display
        codeDisplayText.setFont(font);
        codeDisplayText.setString("Enter 3-digit code:");
        codeDisplayText.setCharacterSize(18);
        codeDisplayText.setFillColor(sf::Color::White);
        codeDisplayText.setPosition(350, 295);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Click numbers to enter code");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::Cyan);
        instructionText.setPosition(50, 380);
        
        // Button texts
        enterButtonText.setFont(font);
        enterButtonText.setString("ENTER");
        enterButtonText.setCharacterSize(16);
        enterButtonText.setFillColor(sf::Color::White);
        enterButtonText.setPosition(665, 465);
        
        clearButtonText.setFont(font);
        clearButtonText.setString("CLEAR");
        clearButtonText.setCharacterSize(16);
        clearButtonText.setFillColor(sf::Color::White);
        clearButtonText.setPosition(665, 535);
        
        // Status text
        statusText.setFont(font);
        statusText.setCharacterSize(18);
        statusText.setFillColor(sf::Color::Green);
        statusText.setPosition(50, 100);
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(24);
        resultText.setPosition(300, 580);
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
    
    void checkCombination() {
        if (enteredCode == correctCode) {
            taskCompleted = true;
            currentState = SUCCESS;
            resultText.setString("ACCESS GRANTED!");
            resultText.setFillColor(successColor);
            resultText.setPosition(320, 580);
            
            // Change lock color to green
            lockFrame.setOutlineColor(successColor);
            lockDial.setOutlineColor(successColor);
        } else {
            currentState = FAILED;
            resultText.setString("ACCESS DENIED - WRONG CODE");
            resultText.setFillColor(failColor);
            resultText.setPosition(280, 580);
            
            // Change lock color to red
            lockFrame.setOutlineColor(failColor);
            lockDial.setOutlineColor(failColor);
            
            // Clear entered code after wrong attempt
            enteredCode.clear();
            updateCodeDisplay();
        }
    }
    
    void update() {
        if (currentState == PLAYING) {
            timeRemaining = timeLimit - gameClock.getElapsedTime().asSeconds();
            
            if (timeRemaining <= 0 && !taskCompleted) {
                taskFailed = true;
                currentState = FAILED;
                resultText.setString("TIMEOUT - SECURITY LOCKED");
                resultText.setFillColor(failColor);
                resultText.setPosition(290, 580);
            }
            
            // Update status
            statusText.setString("Time remaining: " + std::to_string((int)timeRemaining) + "s");
            
            // Change status color based on time
            if (timeRemaining < 10) {
                statusText.setFillColor(failColor);
            } else if (timeRemaining < 20) {
                statusText.setFillColor(sf::Color::Yellow);
            } else {
                statusText.setFillColor(sf::Color::Green);
            }
        }
        
        // Auto-close window after completion
        if ((currentState == SUCCESS || (currentState == FAILED && taskFailed)) && 
            gameClock.getElapsedTime().asSeconds() > timeLimit + 3.0f) {
            windowShouldClose = true;
        }
        
        // For failed attempts, allow retry after 2 seconds
        if (currentState == FAILED && !taskFailed && 
            gameClock.getElapsedTime().asSeconds() > 2.0f) {
            currentState = PLAYING;
            gameClock.restart();
        }
    }
    
    void render() {
        lockWindow.clear(backgroundColor);
        
        lockWindow.draw(titleText);
        
        if (currentState == HINT_DISPLAY) {
            // Show hint screen
            sf::Text hintTitle;
            hintTitle.setFont(font);
            hintTitle.setString("SECURITY CHALLENGE");
            hintTitle.setCharacterSize(32);
            hintTitle.setFillColor(sf::Color::Yellow);
            hintTitle.setPosition(280, 200);
            lockWindow.draw(hintTitle);
            
            // Wrap hint text
            sf::Text wrappedHint;
            wrappedHint.setFont(font);
            wrappedHint.setString("HINT:\n" + hintText);
            wrappedHint.setCharacterSize(18);
            wrappedHint.setFillColor(sf::Color::Cyan);
            wrappedHint.setPosition(150, 300);
            lockWindow.draw(wrappedHint);
            
            sf::Text clickText;
            clickText.setFont(font);
            clickText.setString("Click anywhere to start the challenge");
            clickText.setCharacterSize(20);
            clickText.setFillColor(sf::Color::White);
            clickText.setPosition(280, 450);
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
            lockWindow.draw(resultText);
            
            if (currentState == SUCCESS || (currentState == FAILED && taskFailed)) {
                sf::Text closeText;
                closeText.setFont(font);
                closeText.setString("Window will close automatically...");
                closeText.setCharacterSize(16);
                closeText.setFillColor(sf::Color::White);
                closeText.setPosition(320, 650);
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

class MainGame {
private:
    sf::RenderWindow mainWindow;
    sf::Texture lockerTexture;
    sf::Sprite lockerSprite;
    sf::Font font;
    
    // UI elements
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text statusText;
    
    sf::Color backgroundColor;
    bool taskInProgress;
    int completedTasks;

public:
    MainGame() : mainWindow(sf::VideoMode(1024, 768), "Among Us - Security Room", sf::Style::Close) {
        initialize();
    }
    
    void initialize() {
        // Load resources
        if (!lockerTexture.loadFromFile("lock.png")) {
            std::cout << "Error loading lock.png" << std::endl;
        }
        
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        // Setup locker sprite
        lockerSprite.setTexture(lockerTexture);
        float scaleX = 300.0f / lockerTexture.getSize().x;
        float scaleY = 400.0f / lockerTexture.getSize().y;
        lockerSprite.setScale(scaleX, scaleY);
        lockerSprite.setPosition(350, 180);
        
        backgroundColor = sf::Color(25, 25, 35);
        taskInProgress = false;
        completedTasks = 0;
        
        setupTexts();
    }
    
    void setupTexts() {
        // Title
        titleText.setFont(font);
        titleText.setString("SECURITY LOCKER ROOM");
        titleText.setCharacterSize(36);
        titleText.setFillColor(sf::Color::Red);
        titleText.setPosition(280, 80);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Click on the security locker to access");
        instructionText.setCharacterSize(22);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(280, 150);
        
        // Status
        statusText.setFont(font);
        statusText.setCharacterSize(18);
        statusText.setFillColor(sf::Color::Green);
        statusText.setPosition(280, 600);
        updateStatus();
    }
    
    void updateStatus() {
        statusText.setString("Lockers unlocked: " + std::to_string(completedTasks));
    }
    
    void handleMouseClick(int x, int y) {
        if (!taskInProgress) {
            sf::FloatRect lockerBounds = lockerSprite.getGlobalBounds();
            if (lockerBounds.contains(x, y)) {
                taskInProgress = true;
                
                // Create and run combination lock window
                CombinationLockWindow lockWindow;
                bool taskSuccess = lockWindow.run();
                
                if (taskSuccess) {
                    completedTasks++;
                    updateStatus();
                }
                
                taskInProgress = false;
            }
        }
    }
    
    void render() {
        mainWindow.clear(backgroundColor);
        
        // Draw locker with hover effect
        sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);
        if (lockerSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && !taskInProgress) {
            // Add glow effect when hovering
            sf::RectangleShape glow(sf::Vector2f(220, 320));
            glow.setPosition(390, 190);
            glow.setFillColor(sf::Color(200, 0, 0, 50));
            mainWindow.draw(glow);
        }
        
        mainWindow.draw(lockerSprite);
        
        mainWindow.draw(titleText);
        mainWindow.draw(instructionText);
        mainWindow.draw(statusText);
        
        if (taskInProgress) {
            sf::Text processingText;
            processingText.setFont(font);
            processingText.setString("Accessing security panel...");
            processingText.setCharacterSize(20);
            processingText.setFillColor(sf::Color::Yellow);
            processingText.setPosition(320, 550);
            mainWindow.draw(processingText);
        }
        
        mainWindow.display();
    }
    
    void run() {
        while (mainWindow.isOpen()) {
            sf::Event event;
            while (mainWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    mainWindow.close();
                }
                
                if (event.type == sf::Event::MouseButtonPressed && !taskInProgress) {
                    handleMouseClick(event.mouseButton.x, event.mouseButton.y);
                }
            }
            
            render();
        }
    }
};

int main() {
    MainGame game;
    game.run();
    return 0;
}