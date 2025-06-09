#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class WaterSupplyTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // Game state
    bool waterSupplyOn;
    bool taskCompleted;
    float completionTime;
    
    // UI elements
    sf::Text taskTitle;
    sf::Text instructionText;
    sf::Text statusText;
    sf::Text completionText;
    
    // Water supply panel
    sf::RectangleShape controlPanel;
    sf::RectangleShape switchButton;
    sf::Text switchLabel;
    sf::Text switchStatusText;
    
    // Visual indicators
    sf::CircleShape statusIndicator;
    sf::RectangleShape waterPipeVisual;
    sf::Text waterFlowText;
    
    // Animation for water flow
    float animationTimer;
    
public:
    WaterSupplyTask() : window(sf::VideoMode(800, 600), "Water Supply Control"),
                        waterSupplyOn(true), taskCompleted(false), completionTime(0.0f),
                        animationTimer(0.0f) {
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        setupUI();
    }
    
    void setupUI() {
        // Task title
        taskTitle.setFont(font);
        taskTitle.setCharacterSize(28);
        taskTitle.setFillColor(sf::Color::White);
        taskTitle.setPosition(250, 50);
        taskTitle.setString("Water Supply Control");
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setCharacterSize(18);
        instructionText.setFillColor(sf::Color::Cyan);
        instructionText.setPosition(200, 100);
        instructionText.setString("Turn off the water supply switch");
        
        // Status text
        statusText.setFont(font);
        statusText.setCharacterSize(20);
        statusText.setFillColor(sf::Color::Yellow);
        statusText.setPosition(50, 150);
        statusText.setString("Current Status: Water Supply ON");
        
        // Control panel
        controlPanel.setSize(sf::Vector2f(400, 250));
        controlPanel.setPosition(200, 200);
        controlPanel.setFillColor(sf::Color(60, 60, 80));
        controlPanel.setOutlineThickness(3);
        controlPanel.setOutlineColor(sf::Color::White);
        
        // Water supply switch
        switchButton.setSize(sf::Vector2f(120, 60));
        switchButton.setPosition(340, 300);
        switchButton.setFillColor(sf::Color::Green);
        switchButton.setOutlineThickness(2);
        switchButton.setOutlineColor(sf::Color::White);
        
        // Switch label
        switchLabel.setFont(font);
        switchLabel.setCharacterSize(16);
        switchLabel.setFillColor(sf::Color::White);
        switchLabel.setPosition(320, 250);
        switchLabel.setString("Water Supply Switch");
        
        // Switch status text
        switchStatusText.setFont(font);
        switchStatusText.setCharacterSize(14);
        switchStatusText.setFillColor(sf::Color::White);
        switchStatusText.setPosition(375, 320);
        switchStatusText.setString("ON");
        
        // Status indicator light
        statusIndicator.setRadius(20);
        statusIndicator.setPosition(250, 310);
        statusIndicator.setFillColor(sf::Color::Green);
        statusIndicator.setOutlineThickness(2);
        statusIndicator.setOutlineColor(sf::Color::White);
        
        // Water pipe visual representation
        waterPipeVisual.setSize(sf::Vector2f(300, 30));
        waterPipeVisual.setPosition(250, 380);
        waterPipeVisual.setFillColor(sf::Color::Blue);
        waterPipeVisual.setOutlineThickness(2);
        waterPipeVisual.setOutlineColor(sf::Color::White);
        
        // Water flow text
        waterFlowText.setFont(font);
        waterFlowText.setCharacterSize(14);
        waterFlowText.setFillColor(sf::Color::Blue);
        waterFlowText.setPosition(350, 420);
        waterFlowText.setString("Water Flowing ≈≈≈≈≈≈");
        
        // Completion text
        completionText.setFont(font);
        completionText.setCharacterSize(20);
        completionText.setFillColor(sf::Color::Green);
        completionText.setPosition(250, 500);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed && !taskCompleted) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Check if switch was clicked
                if (switchButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    toggleWaterSupply();
                }
            }
            
            // Restart with R key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                resetTask();
            }
        }
    }
    
    void toggleWaterSupply() {
        if (waterSupplyOn) {
            // Turn off water supply
            waterSupplyOn = false;
            taskCompleted = true;
            
            // Update UI elements
            switchButton.setFillColor(sf::Color::Red);
            switchStatusText.setString("OFF");
            switchStatusText.setFillColor(sf::Color::Red);
            
            statusIndicator.setFillColor(sf::Color::Red);
            statusText.setString("Current Status: Water Supply OFF");
            statusText.setFillColor(sf::Color::Red);
            
            waterPipeVisual.setFillColor(sf::Color(100, 100, 100)); // Gray for no water
            waterFlowText.setString("No Water Flow - - - - - -");
            waterFlowText.setFillColor(sf::Color::Gray);
            
            instructionText.setString("Water supply has been turned off");
            instructionText.setFillColor(sf::Color::Green);
            
            completionText.setString("✓ Task Completed Successfully!");
        }
    }
    
    void update(float deltaTime) {
        if (!taskCompleted) {
            // Animate water flow text
            animationTimer += deltaTime;
            if (animationTimer >= 0.5f) {
                animationTimer = 0.0f;
                static int animFrame = 0;
                animFrame = (animFrame + 1) % 3;
                
                if (animFrame == 0) {
                    waterFlowText.setString("Water Flowing ≈≈≈≈≈≈");
                } else if (animFrame == 1) {
                    waterFlowText.setString("Water Flowing ~~~≈≈≈");
                } else {
                    waterFlowText.setString("Water Flowing ≈~~~≈≈");
                }
            }
        }
        
        if (taskCompleted) {
            completionTime += deltaTime;
        }
    }
    
    void resetTask() {
        waterSupplyOn = true;
        taskCompleted = false;
        completionTime = 0.0f;
        animationTimer = 0.0f;
        
        // Reset UI
        switchButton.setFillColor(sf::Color::Green);
        switchStatusText.setString("ON");
        switchStatusText.setFillColor(sf::Color::White);
        
        statusIndicator.setFillColor(sf::Color::Green);
        statusText.setString("Current Status: Water Supply ON");
        statusText.setFillColor(sf::Color::Yellow);
        
        waterPipeVisual.setFillColor(sf::Color::Blue);
        waterFlowText.setString("Water Flowing ≈≈≈≈≈≈");
        waterFlowText.setFillColor(sf::Color::Blue);
        
        instructionText.setString("Turn off the water supply switch");
        instructionText.setFillColor(sf::Color::Cyan);
        
        completionText.setString("");
    }
    
    void render() {
        window.clear(sf::Color(20, 20, 30));
        
        // Draw main UI
        window.draw(taskTitle);
        window.draw(instructionText);
        window.draw(statusText);
        
        // Draw control panel
        window.draw(controlPanel);
        window.draw(switchButton);
        window.draw(switchLabel);
        window.draw(switchStatusText);
        
        // Draw indicators
        window.draw(statusIndicator);
        window.draw(waterPipeVisual);
        window.draw(waterFlowText);
        
        // Draw completion message
        if (taskCompleted) {
            window.draw(completionText);
            
            // Show restart option
            sf::Text restartText;
            restartText.setFont(font);
            restartText.setCharacterSize(16);
            restartText.setFillColor(sf::Color::White);
            restartText.setPosition(320, 550);
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
};

int main() {
    WaterSupplyTask game;
    game.run();
    
    return 0;
}