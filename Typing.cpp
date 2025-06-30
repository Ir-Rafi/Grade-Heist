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
    sf::Text resultText;
    sf::Text promptText;
    sf::Text exitText;
    
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
    bool waitingForExit;

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
        waitingForExit = false;
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
        taskText.setPosition(100, 150);
        
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
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(24);
        resultText.setPosition(150, 480);
        
        // Exit instruction text
        exitText.setFont(font);
        exitText.setString("Task Passed!!!! Press the enter button to exit the task");
        exitText.setCharacterSize(20);
        exitText.setFillColor(sf::Color::Green);
        exitText.setPosition(120, 520);
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
        waitingForExit = false;
        
        // Update display
        taskText.setString("REQUIRED: " + currentTask);
        inputText.setString("");
    }
    
    void handleInput(const sf::Event& event) {
        if (event.type == sf::Event::TextEntered) {
            if (waitingForExit) {
                // If waiting for exit and Enter is pressed, close the window
                if (event.text.unicode == 13) { // Enter
                    windowShouldClose = true;
                }
            } else if (currentState == PLAYING) {
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
    }
    
    void checkTaskCompletion() {
        std::string upperInput = userInput;
        std::transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);
        
        if (upperInput == currentTask) {
            taskCompleted = true;
            currentState = SUCCESS;
            waitingForExit = true;
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
        // No timer updates needed anymore
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
            
            // Draw blinking cursor
            sf::RectangleShape cursor(sf::Vector2f(3, 22));
            cursor.setFillColor(sf::Color::White);
            cursor.setPosition(110 + inputText.getLocalBounds().width, 415);
            
            // Simple blinking effect using a counter
            static int blinkCounter = 0;
            blinkCounter++;
            if ((blinkCounter / 30) % 2 == 0) { // Blink every 30 frames
                taskWindow.draw(cursor);
            }
        } else if (currentState == SUCCESS) {
            taskWindow.draw(taskText);
            taskWindow.draw(inputBox);
            taskWindow.draw(inputText);
            taskWindow.draw(resultText);
            taskWindow.draw(exitText);
        } else if (currentState == FAILED) {
            taskWindow.draw(taskText);
            taskWindow.draw(inputBox);
            taskWindow.draw(inputText);
            taskWindow.draw(resultText);
            
            sf::Text retryText;
            retryText.setFont(font);
            retryText.setString("Try again...");
            retryText.setCharacterSize(18);
            retryText.setFillColor(sf::Color::Yellow);
            retryText.setPosition(350, 520);
            taskWindow.draw(retryText);
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



int main() {
    ComputerTaskWindow taskWindow;
    taskWindow.run();
    return 0;
}
