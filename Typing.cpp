#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <memory>

class ComputerTaskWindow {
private:
    sf::RenderWindow taskWindow;
    sf::Font font;
    
    // Task data
    std::vector<std::string> taskLines;
    std::string currentTask;
    std::string userInput;
    
    // UI elements
    sf::Text titleText;
    sf::Text taskText;
    sf::Text inputText;
    sf::Text timerText;
    sf::Text resultText;
    sf::Text promptText;
    
    // Timer
    sf::Clock gameClock;
    float timeLimit;
    float timeRemaining;
    
    // Colors and shapes
    sf::Color backgroundColor;
    sf::RectangleShape inputBox;
    sf::RectangleShape screenBorder;
    
    // Game state
    enum TaskState { PLAYING, SUCCESS, FAILED };
    TaskState currentState;
    
    bool taskCompleted;
    bool taskFailed;
    bool windowShouldClose;

public:
    ComputerTaskWindow() : taskWindow(sf::VideoMode(800, 600), "Computer Terminal - Task Interface", sf::Style::Close) {
        initialize();
    }
    
    void initialize() {
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        // Setup colors
        backgroundColor = sf::Color(15, 15, 25);
        
        // Setup screen border
        screenBorder.setSize(sf::Vector2f(780, 580));
        screenBorder.setPosition(10, 10);
        screenBorder.setFillColor(sf::Color(0, 40, 0));
        screenBorder.setOutlineThickness(3);
        screenBorder.setOutlineColor(sf::Color::Green);
        
        // Setup input box
        inputBox.setSize(sf::Vector2f(600, 45));
        inputBox.setPosition(100, 400);
        inputBox.setFillColor(sf::Color(30, 30, 30));
        inputBox.setOutlineThickness(2);
        inputBox.setOutlineColor(sf::Color::Cyan);
        
        // Initialize task lines
        taskLines = {
            "INITIALIZE_REACTOR_CORE",
            "CALIBRATE_NAVIGATION_SYSTEM", 
            "DOWNLOAD_ENGINE_DIAGNOSTICS",
            "SYNCHRONIZE_COMMUNICATIONS",
            "STABILIZE_OXYGEN_LEVELS",
            "UPLOAD_SECURITY_PROTOCOLS",
            "RESTART_WIFI_CONNECTION",
            "EMPTY_GARBAGE_CHUTE",
            "DIVERT_POWER_TO_SHIELDS",
            "ALIGN_ENGINE_OUTPUT",
            "PRIME_SHIELDS_SYSTEM",
            "SCAN_BOARDING_PASS"
        };
        
        setupTexts();
        startNewTask();
        windowShouldClose = false;
    }
    
    void setupTexts() {
        // Title
        titleText.setFont(font);
        titleText.setString("=== COMPUTER TERMINAL ACCESS ===");
        titleText.setCharacterSize(28);
        titleText.setFillColor(sf::Color::Green);
        titleText.setPosition(180, 50);
        
        // Task text
        taskText.setFont(font);
        taskText.setCharacterSize(24);
        taskText.setFillColor(sf::Color::Cyan);
        taskText.setPosition(100, 200);
        
        // Prompt text
        promptText.setFont(font);
        promptText.setString("ENTER COMMAND:");
        promptText.setCharacterSize(20);
        promptText.setFillColor(sf::Color::Yellow);
        promptText.setPosition(100, 350);
        
        // Input text
        inputText.setFont(font);
        inputText.setCharacterSize(22);
        inputText.setFillColor(sf::Color::White);
        inputText.setPosition(110, 410);
        
        // Timer
        timerText.setFont(font);
        timerText.setCharacterSize(20);
        timerText.setFillColor(sf::Color::Red);
        timerText.setPosition(100, 120);
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(28);
        resultText.setPosition(200, 480);
    }
    
    void startNewTask() {
        // Select random task
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(0, taskLines.size() - 1);
        currentTask = taskLines[dis(gen)];
        
        // Reset game state
        userInput.clear();
        taskCompleted = false;
        taskFailed = false;
        currentState = PLAYING;
        gameClock.restart();
        timeLimit = 15.0f;
        timeRemaining = timeLimit;
        
        // Update display
        taskText.setString("REQUIRED: " + currentTask);
        inputText.setString("");
    }
    
    void handleInput(const sf::Event& event) {
        if (currentState == PLAYING && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (!userInput.empty()) {
                    userInput.pop_back();
                }
            } else if (event.text.unicode == 13) { // Enter
                checkTaskCompletion();
            } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                userInput += static_cast<char>(event.text.unicode);
            }
            
            // Convert to uppercase for display
            std::string displayInput = userInput;
            std::transform(displayInput.begin(), displayInput.end(), displayInput.begin(), ::toupper);
            inputText.setString(displayInput);
        }
    }
    
    void checkTaskCompletion() {
        std::string upperInput = userInput;
        std::transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);
        
        if (upperInput == currentTask) {
            taskCompleted = true;
            currentState = SUCCESS;
            resultText.setString("ACCESS GRANTED - TASK COMPLETE");
            resultText.setFillColor(sf::Color::Green);
            resultText.setPosition(150, 480);
        } else {
            taskFailed = true;
            currentState = FAILED;
            resultText.setString("ACCESS DENIED - INVALID COMMAND");
            resultText.setFillColor(sf::Color::Red);
            resultText.setPosition(140, 480);
        }
    }
    
    void update() {
        if (currentState == PLAYING) {
            timeRemaining = timeLimit - gameClock.getElapsedTime().asSeconds();
            
            if (timeRemaining <= 0 && !taskCompleted) {
                taskFailed = true;
                currentState = FAILED;
                resultText.setString("TIMEOUT - SYSTEM LOCKED");
                resultText.setFillColor(sf::Color::Red);
                resultText.setPosition(200, 480);
            }
            
            // Update timer display
            timerText.setString("TIME REMAINING: " + std::to_string((int)timeRemaining) + " SECONDS");
            
            // Change timer color based on remaining time
            if (timeRemaining < 5) {
                timerText.setFillColor(sf::Color::Red);
            } else if (timeRemaining < 10) {
                timerText.setFillColor(sf::Color::Yellow);
            } else {
                timerText.setFillColor(sf::Color::Green);
            }
        }
        
        // Auto-close window after task completion
        if ((currentState == SUCCESS || currentState == FAILED) && 
            gameClock.getElapsedTime().asSeconds() > timeLimit + 3.0f) {
            windowShouldClose = true;
        }
    }
    
    void render() {
        taskWindow.clear(backgroundColor);
        
        // Draw terminal screen
        taskWindow.draw(screenBorder);
        taskWindow.draw(titleText);
        
        if (currentState == PLAYING) {
            taskWindow.draw(taskText);
            taskWindow.draw(promptText);
            taskWindow.draw(inputBox);
            taskWindow.draw(inputText);
            taskWindow.draw(timerText);
            
            // Draw blinking cursor
            sf::RectangleShape cursor(sf::Vector2f(3, 22));
            cursor.setFillColor(sf::Color::White);
            cursor.setPosition(110 + inputText.getLocalBounds().width, 415);
            
            if ((int)(gameClock.getElapsedTime().asSeconds() * 2) % 2 == 0) {
                taskWindow.draw(cursor);
            }
        } else {
            taskWindow.draw(taskText);
            taskWindow.draw(inputBox);
            taskWindow.draw(inputText);
            taskWindow.draw(resultText);
            
            sf::Text closeText;
            closeText.setFont(font);
            closeText.setString("Window will close automatically...");
            closeText.setCharacterSize(16);
            closeText.setFillColor(sf::Color::White);
            closeText.setPosition(250, 550);
            taskWindow.draw(closeText);
        }
        
        taskWindow.display();
    }
    
    bool run() {
        while (taskWindow.isOpen() && !windowShouldClose) {
            sf::Event event;
            while (taskWindow.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    taskWindow.close();
                    return false;
                }
                
                handleInput(event);
            }
            
            update();
            render();
        }
        
        taskWindow.close();
        return taskCompleted;
    }
};

class MainGame {
private:
    sf::RenderWindow mainWindow;
    sf::Texture computerTexture;
    sf::Sprite computerSprite;
    sf::Font font;
    
    // UI elements
    sf::Text titleText;
    sf::Text instructionText;
    sf::Text statusText;
    
    sf::Color backgroundColor;
    bool taskInProgress;
    int completedTasks;

public:
    MainGame() : mainWindow(sf::VideoMode(1024, 768), "Among Us - Computer Room", sf::Style::Close) {
        initialize();
    }
    
    void initialize() {
        // Load resources
        if (!computerTexture.loadFromFile("computer.png")) {
            std::cout << "Error loading computer.png" << std::endl;
        }
        
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        // Setup computer sprite
        computerSprite.setTexture(computerTexture);
        float scaleX = 400.0f / computerTexture.getSize().x;
        float scaleY = 300.0f / computerTexture.getSize().y;
        computerSprite.setScale(scaleX, scaleY);
        computerSprite.setPosition(300, 250);
        
        backgroundColor = sf::Color(25, 25, 35);
        taskInProgress = false;
        completedTasks = 0;
        
        setupTexts();
    }
    
    void setupTexts() {
        // Title
        titleText.setFont(font);
        titleText.setString("COMPUTER TERMINAL ROOM");
        titleText.setCharacterSize(36);
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setPosition(280, 80);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Click on the computer to access terminal");
        instructionText.setCharacterSize(22);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(280, 180);
        
        // Status
        statusText.setFont(font);
        statusText.setCharacterSize(18);
        statusText.setFillColor(sf::Color::Green);
        statusText.setPosition(280, 600);
        updateStatus();
    }
    
    void updateStatus() {
        statusText.setString("Tasks completed: " + std::to_string(completedTasks));
    }
    
    void handleMouseClick(int x, int y) {
        if (!taskInProgress) {
            sf::FloatRect computerBounds = computerSprite.getGlobalBounds();
            if (computerBounds.contains(x, y)) {
                taskInProgress = true;
                
                // Create and run computer task window
                ComputerTaskWindow taskWindow;
                bool taskSuccess = taskWindow.run();
                
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
        
        // Draw computer with hover effect
        sf::Vector2i mousePos = sf::Mouse::getPosition(mainWindow);
        if (computerSprite.getGlobalBounds().contains(mousePos.x, mousePos.y) && !taskInProgress) {
            // Add glow effect when hovering
            sf::RectangleShape glow(sf::Vector2f(420, 320));
            glow.setPosition(290, 240);
            glow.setFillColor(sf::Color(0, 100, 200, 50));
            mainWindow.draw(glow);
        }
        
        mainWindow.draw(computerSprite);
        mainWindow.draw(titleText);
        mainWindow.draw(instructionText);
        mainWindow.draw(statusText);
        
        if (taskInProgress) {
            sf::Text processingText;
            processingText.setFont(font);
            processingText.setString("Accessing terminal...");
            processingText.setCharacterSize(20);
            processingText.setFillColor(sf::Color::Yellow);
            processingText.setPosition(350, 450);
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