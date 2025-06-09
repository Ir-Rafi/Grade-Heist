#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>

class WifiOffTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // Game state
    bool wifiOn;
    bool taskCompleted;
    bool taskFailed;
    float timeRemaining;
    const float TASK_TIME_LIMIT = 30.0f;
    
    // UI elements
    sf::RectangleShape wifiScreen;
    sf::Text wifiStatusText;
    sf::RectangleShape switchButton;
    sf::Text switchText;
    sf::Text timerText;
    sf::Text instructionText;
    sf::Text taskTitleText;
    sf::Text resultText;
    
    // Visual effects
    sf::CircleShape wifiIndicator;
    sf::RectangleShape urgencyOverlay;
    float blinkTimer;
    
public:
    WifiOffTask() : window(sf::VideoMode(800, 600), "Wifi-Off Sabotage Task"),
                    wifiOn(true), taskCompleted(false), taskFailed(false),
                    timeRemaining(TASK_TIME_LIMIT), blinkTimer(0.0f) {
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        setupUI();
    }
    
    void setupUI() {
        // Wifi screen (monitor display)
        wifiScreen.setSize(sf::Vector2f(400, 150));
        wifiScreen.setPosition(200, 150);
        wifiScreen.setFillColor(sf::Color::Black);
        wifiScreen.setOutlineThickness(5);
        wifiScreen.setOutlineColor(sf::Color(100, 100, 100));
        
        // Wifi status text on screen
        wifiStatusText.setFont(font);
        wifiStatusText.setCharacterSize(32);
        wifiStatusText.setFillColor(sf::Color::Green);
        wifiStatusText.setPosition(280, 200);
        wifiStatusText.setString("Wifi Is On");
        
        // Switch button
        switchButton.setSize(sf::Vector2f(120, 60));
        switchButton.setPosition(340, 350);
        switchButton.setFillColor(sf::Color::Red);
        switchButton.setOutlineThickness(3);
        switchButton.setOutlineColor(sf::Color::White);
        
        // Switch label
        switchText.setFont(font);
        switchText.setCharacterSize(16);
        switchText.setFillColor(sf::Color::White);
        switchText.setPosition(365, 375);
        switchText.setString("SWITCH");
        
        // Timer display
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::Yellow);
        timerText.setPosition(50, 50);
        
        // Task title
        taskTitleText.setFont(font);
        taskTitleText.setCharacterSize(28);
        taskTitleText.setFillColor(sf::Color::White);
        taskTitleText.setPosition(250, 50);
        taskTitleText.setString("SABOTAGE: Turn Off Wifi");
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setCharacterSize(18);
        instructionText.setFillColor(sf::Color::Cyan);
        instructionText.setPosition(200, 450);
        instructionText.setString("Click the switch to turn off the wifi!");
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(24);
        resultText.setFillColor(sf::Color::Green);
        resultText.setPosition(250, 500);
        
        // Wifi indicator light
        wifiIndicator.setRadius(15);
        wifiIndicator.setPosition(620, 200);
        wifiIndicator.setFillColor(sf::Color::Green);
        
        // Urgency overlay for time pressure
        urgencyOverlay.setSize(sf::Vector2f(800, 600));
        urgencyOverlay.setPosition(0, 0);
        urgencyOverlay.setFillColor(sf::Color(255, 0, 0, 0)); // Transparent red
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed && !taskCompleted && !taskFailed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Check if switch was clicked
                if (switchButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    toggleWifi();
                }
            }
            
            // Restart task with R key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                resetTask();
            }
        }
    }
    
    void toggleWifi() {
        if (wifiOn) {
            // Turn off wifi - complete the task
            wifiOn = false;
            taskCompleted = true;
            
            // Update UI
            wifiStatusText.setString("Wifi Is Off");
            wifiStatusText.setFillColor(sf::Color::Red);
            switchButton.setFillColor(sf::Color::Green);
            wifiIndicator.setFillColor(sf::Color::Red);
            instructionText.setString("Mission accomplished! Grades blocked!");
            instructionText.setFillColor(sf::Color::Green);
            resultText.setString("SUCCESS: Wifi turned off!");
            resultText.setFillColor(sf::Color::Green);
        }
    }
    
    void update(float deltaTime) {
        if (!taskCompleted && !taskFailed) {
            // Update timer
            timeRemaining -= deltaTime;
            
            // Update timer display
            std::stringstream ss;
            ss << "Time: " << std::fixed << std::setprecision(1) << timeRemaining << "s";
            timerText.setString(ss.str());
            
            // Change timer color based on remaining time
            if (timeRemaining <= 10.0f) {
                timerText.setFillColor(sf::Color::Red);
                
                // Add urgency effect
                blinkTimer += deltaTime;
                if (blinkTimer >= 0.5f) {
                    blinkTimer = 0.0f;
                }
                
                // Red overlay that blinks when time is running out
                int alpha = static_cast<int>(30 + 25 * sin(blinkTimer * 12.0f));
                urgencyOverlay.setFillColor(sf::Color(255, 0, 0, alpha));
            } else if (timeRemaining <= 20.0f) {
                timerText.setFillColor(sf::Color::Yellow);
            }
            
            // Check if time ran out
            if (timeRemaining <= 0.0f) {
                taskFailed = true;
                instructionText.setString("Time's up! Sir submitted the grades!");
                instructionText.setFillColor(sf::Color::Red);
                resultText.setString("FAILED: Too slow!");
                resultText.setFillColor(sf::Color::Red);
                timerText.setString("Time: 0.0s");
            }
        }
    }
    
    void resetTask() {
        wifiOn = true;
        taskCompleted = false;
        taskFailed = false;
        timeRemaining = TASK_TIME_LIMIT;
        blinkTimer = 0.0f;
        
        // Reset UI
        wifiStatusText.setString("Wifi Is On");
        wifiStatusText.setFillColor(sf::Color::Green);
        switchButton.setFillColor(sf::Color::Red);
        wifiIndicator.setFillColor(sf::Color::Green);
        instructionText.setString("Click the switch to turn off the wifi!");
        instructionText.setFillColor(sf::Color::Cyan);
        resultText.setString("");
        timerText.setFillColor(sf::Color::Yellow);
        urgencyOverlay.setFillColor(sf::Color(255, 0, 0, 0));
    }
    
    void render() {
        window.clear(sf::Color(20, 20, 30));
        
        // Draw wifi screen
        window.draw(wifiScreen);
        window.draw(wifiStatusText);
        
        // Draw switch
        window.draw(switchButton);
        window.draw(switchText);
        
        // Draw wifi indicator
        window.draw(wifiIndicator);
        
        // Draw UI text
        window.draw(taskTitleText);
        window.draw(timerText);
        window.draw(instructionText);
        window.draw(resultText);
        
        // Draw urgency overlay (when time is running out)
        if (timeRemaining <= 10.0f && !taskCompleted && !taskFailed) {
            window.draw(urgencyOverlay);
        }
        
        // Draw restart instruction
        if (taskCompleted || taskFailed) {
            sf::Text restartText;
            restartText.setFont(font);
            restartText.setCharacterSize(16);
            restartText.setFillColor(sf::Color::White);
            restartText.setPosition(300, 550);
            restartText.setString("Press R to restart");
            window.draw(restartText);
        }
        
        window.display();
    }
    
    void run() {
        sf::Clock clock;
        
        while (window.isOpen()) {
            float deltaTime = clock.restart().asSeconds();
            
            handleEvents();
            update(deltaTime);
            render();
        }
    }
    
    bool isTaskCompleted() const {
        return taskCompleted;
    }
    
    bool isTaskFailed() const {
        return taskFailed;
    }
};

int main() {
    WifiOffTask game;
    game.run();
    
    return 0;
}