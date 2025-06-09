#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class PrinterSabotage {
private:
    sf::RenderWindow window;
    sf::Texture printerTexture;
    sf::Sprite printerSprite;
    sf::Font font;
    
    // Game state
    bool switchPressed;
    bool taskCompleted;
    float inkProgress;
    float inkSpeed;
    
    // UI elements
    sf::RectangleShape switchButton;
    sf::RectangleShape progressBarBg;
    sf::RectangleShape progressBarFill;
    sf::Text statusText;
    sf::Text instructionText;
    sf::Text switchText;
    
    // Screen overlay for printer display
    sf::RectangleShape printerScreen;
    sf::Text printerDisplayText;
    
public:
    PrinterSabotage() : window(sf::VideoMode(800, 600), "Printer Sabotage Task"),
                        switchPressed(false), taskCompleted(false), 
                        inkProgress(0.0f), inkSpeed(50.0f) {
        
        // Load printer texture
        if (!printerTexture.loadFromFile("printer.png")) {
            std::cout << "Error loading printer.png" << std::endl;
        }
        printerSprite.setTexture(printerTexture);
        
        // Scale and position printer
        float scaleX = 400.0f / printerTexture.getSize().x;
        float scaleY = 300.0f / printerTexture.getSize().y;
        printerSprite.setScale(scaleX, scaleY);
        printerSprite.setPosition(200, 50);
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        setupUI();
    }
    
    void setupUI() {
        // Switch button
        switchButton.setSize(sf::Vector2f(80, 40));
        switchButton.setPosition(100, 400);
        switchButton.setFillColor(sf::Color::Red);
        
        // Progress bar background
        progressBarBg.setSize(sf::Vector2f(300, 20));
        progressBarBg.setPosition(250, 450);
        progressBarBg.setFillColor(sf::Color(50, 50, 50));
        progressBarBg.setOutlineThickness(2);
        progressBarBg.setOutlineColor(sf::Color::White);
        
        // Progress bar fill
        progressBarFill.setSize(sf::Vector2f(0, 20));
        progressBarFill.setPosition(250, 450);
        progressBarFill.setFillColor(sf::Color::Blue);
        
        // Printer screen overlay (where error message appears)
        printerScreen.setSize(sf::Vector2f(200, 80));
        printerScreen.setPosition(300, 120);
        printerScreen.setFillColor(sf::Color::Black);
        printerScreen.setOutlineThickness(2);
        printerScreen.setOutlineColor(sf::Color(100, 100, 100));
        
        // Text elements
        statusText.setFont(font);
        statusText.setCharacterSize(24);
        statusText.setFillColor(sf::Color::White);
        statusText.setPosition(50, 50);
        statusText.setString("Sabotage the Printer");
        
        instructionText.setFont(font);
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::Yellow);
        instructionText.setPosition(50, 500);
        instructionText.setString("Click the switch to void the ink tube");
        
        switchText.setFont(font);
        switchText.setCharacterSize(14);
        switchText.setFillColor(sf::Color::White);
        switchText.setPosition(105, 410);
        switchText.setString("SWITCH");
        
        printerDisplayText.setFont(font);
        printerDisplayText.setCharacterSize(16);
        printerDisplayText.setFillColor(sf::Color::Red);
        printerDisplayText.setPosition(310, 140);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Check if switch was clicked
                if (switchButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && !taskCompleted) {
                    switchPressed = true;
                    switchButton.setFillColor(sf::Color::Green);
                    instructionText.setString("Ink is draining...");
                }
            }
        }
    }
    
    void update(float deltaTime) {
        if (switchPressed && !taskCompleted) {
            // Increase ink drainage progress
            inkProgress += inkSpeed * deltaTime;
            
            // Update progress bar
            float progressWidth = (inkProgress / 100.0f) * 300.0f;
            progressBarFill.setSize(sf::Vector2f(progressWidth, 20));
            
            // Change progress bar color as it fills
            if (inkProgress < 30) {
                progressBarFill.setFillColor(sf::Color::Blue);
            } else if (inkProgress < 70) {
                progressBarFill.setFillColor(sf::Color::Yellow);
            } else {
                progressBarFill.setFillColor(sf::Color::Red);
            }
            
            // Check if task is completed
            if (inkProgress >= 100.0f) {
                taskCompleted = true;
                instructionText.setString("Printer sabotaged successfully!");
                instructionText.setFillColor(sf::Color::Green);
                printerDisplayText.setString("Printer Is\nNot Working");
                
                // Make printer screen flash red to indicate error
                printerScreen.setFillColor(sf::Color(80, 0, 0));
            }
        }
    }
    
    void render() {
        window.clear(sf::Color(20, 20, 30));
        
        // Draw printer
        window.draw(printerSprite);
        
        // Draw printer screen
        window.draw(printerScreen);
        if (taskCompleted) {
            window.draw(printerDisplayText);
        }
        
        // Draw UI elements
        window.draw(switchButton);
        window.draw(progressBarBg);
        window.draw(progressBarFill);
        
        // Draw text
        window.draw(statusText);
        window.draw(instructionText);
        window.draw(switchText);
        
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
    PrinterSabotage game;
    game.run();
    
    return 0;
}