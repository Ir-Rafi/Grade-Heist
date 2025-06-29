#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <climits>

class IntegerOverflowTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // States
    enum State { INPUT_PHASE, SUCCESS_PHASE };
    State currentState;
    
    // Input handling
    std::string inputText;
    bool inputActive;
    
    // UI elements
    sf::RectangleShape inputBox;
    sf::RectangleShape usernameBox;
    sf::RectangleShape passwordBox;
    sf::Text messageText;
    sf::Text inputDisplayText;
    sf::Text usernameLabel;
    sf::Text usernameText;
    sf::Text passwordLabel;
    sf::Text passwordText;
    sf::Text successMessage;
    
    // Background elements
    sf::RectangleShape backgroundGradient;
    std::vector<sf::CircleShape> backgroundCircles;
    sf::RectangleShape titleBar;
    
    const long long INT32_MAX_VALUE = 2147483647; // 2^31 - 1
    
public:
    IntegerOverflowTask() : window(sf::VideoMode(800, 600), "Integer Overflow Task"), 
                           currentState(INPUT_PHASE), inputActive(true) {
        
        // Load font (using default system font)
        if (!font.loadFromFile("arial.ttf")) {
            // If arial.ttf is not found, try to use a system font
            // You might need to provide a path to a font file on your system
            std::cout << "Warning: Could not load font. Using default font." << std::endl;
        }
        
        setupUI();
    }
    
    void setupUI() {
        // Create attractive background elements
        setupBackground();
        
        // Title bar
        titleBar.setSize(sf::Vector2f(800, 60));
        titleBar.setPosition(0, 0);
        titleBar.setFillColor(sf::Color(45, 52, 64, 200)); // Dark blue-gray with transparency
        
        // Input box setup with modern styling
        inputBox.setSize(sf::Vector2f(400, 50));
        inputBox.setPosition(200, 280);
        inputBox.setFillColor(sf::Color(255, 255, 255, 240));
        inputBox.setOutlineColor(sf::Color(76, 175, 80)); // Green outline
        inputBox.setOutlineThickness(3);
        
        // Message text with better styling
        messageText.setFont(font);
        messageText.setString("Make integer overflow");
        messageText.setCharacterSize(28);
        messageText.setFillColor(sf::Color::White);
        messageText.setStyle(sf::Text::Bold);
        messageText.setPosition(250, 15);
        
        // Input display text
        inputDisplayText.setFont(font);
        inputDisplayText.setCharacterSize(22);
        inputDisplayText.setFillColor(sf::Color(33, 37, 41));
        inputDisplayText.setPosition(210, 293);
        
        // Success phase UI elements with modern styling
        usernameBox.setSize(sf::Vector2f(320, 45));
        usernameBox.setPosition(240, 200);
        usernameBox.setFillColor(sf::Color(255, 255, 255, 240));
        usernameBox.setOutlineColor(sf::Color(33, 150, 243)); // Blue outline
        usernameBox.setOutlineThickness(3);
        
        passwordBox.setSize(sf::Vector2f(320, 45));
        passwordBox.setPosition(240, 280);
        passwordBox.setFillColor(sf::Color(255, 255, 255, 240));
        passwordBox.setOutlineColor(sf::Color(33, 150, 243)); // Blue outline
        passwordBox.setOutlineThickness(3);
        
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(20);
        usernameLabel.setFillColor(sf::Color::White);
        usernameLabel.setStyle(sf::Text::Bold);
        usernameLabel.setPosition(130, 210);
        
        usernameText.setFont(font);
        usernameText.setString("Admin");
        usernameText.setCharacterSize(20);
        usernameText.setFillColor(sf::Color(33, 37, 41));
        usernameText.setPosition(250, 210);
        
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(20);
        passwordLabel.setFillColor(sf::Color::White);
        passwordLabel.setStyle(sf::Text::Bold);
        passwordLabel.setPosition(130, 290);
        
        passwordText.setFont(font);
        passwordText.setString("924689093745");
        passwordText.setCharacterSize(20);
        passwordText.setFillColor(sf::Color(33, 37, 41));
        passwordText.setPosition(250, 290);
        
        successMessage.setFont(font);
        successMessage.setString("Task passed!!! Press the Enter button to exit the task");
        successMessage.setCharacterSize(22);
        successMessage.setFillColor(sf::Color(76, 175, 80)); // Green color
        successMessage.setStyle(sf::Text::Bold);
        successMessage.setPosition(120, 370);
    }
    
    void setupBackground() {
        // Create gradient-like background with multiple rectangles
        backgroundGradient.setSize(sf::Vector2f(800, 600));
        backgroundGradient.setPosition(0, 0);
        
        // Create decorative circles for background
        backgroundCircles.clear();
        
        // Large decorative circles
        sf::CircleShape circle1(80);
        circle1.setPosition(-40, -40);
        circle1.setFillColor(sf::Color(103, 58, 183, 30)); // Purple with low opacity
        backgroundCircles.push_back(circle1);
        
        sf::CircleShape circle2(60);
        circle2.setPosition(700, 50);
        circle2.setFillColor(sf::Color(233, 30, 99, 25)); // Pink with low opacity
        backgroundCircles.push_back(circle2);
        
        sf::CircleShape circle3(100);
        circle3.setPosition(650, 450);
        circle3.setFillColor(sf::Color(33, 150, 243, 20)); // Blue with low opacity
        backgroundCircles.push_back(circle3);
        
        sf::CircleShape circle4(45);
        circle4.setPosition(50, 450);
        circle4.setFillColor(sf::Color(76, 175, 80, 35)); // Green with low opacity
        backgroundCircles.push_back(circle4);
        
        sf::CircleShape circle5(70);
        circle5.setPosition(100, 100);
        circle5.setFillColor(sf::Color(255, 193, 7, 25)); // Amber with low opacity
        backgroundCircles.push_back(circle5);
    }
    
    bool isIntegerOverflow(const std::string& input) {
        if (input.empty()) return false;
        
        try {
            // Convert to long long to check if it exceeds int32 max
            long long value = std::stoll(input);
            return value > INT32_MAX_VALUE;
        } catch (const std::exception&) {
            return false;
        }
    }
    
    void handleInput(sf::Event& event) {
        if (currentState == INPUT_PHASE) {
            if (event.type == sf::Event::TextEntered) {
                if (event.text.unicode == 8) { // Backspace
                    if (!inputText.empty()) {
                        inputText.pop_back();
                    }
                } else if (event.text.unicode >= 48 && event.text.unicode <= 57) { // Numbers 0-9
                    inputText += static_cast<char>(event.text.unicode);
                } else if (event.text.unicode == 13) { // Enter key
                    if (isIntegerOverflow(inputText)) {
                        currentState = SUCCESS_PHASE;
                    }
                }
                inputDisplayText.setString(inputText);
            }
        } else if (currentState == SUCCESS_PHASE) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    window.close(); // Exit the task
                }
            }
        }
    }
    
    void render() {
        // Create gradient background effect
        window.clear(sf::Color(67, 56, 202)); // Deep purple base
        
        // Draw gradient overlay
        sf::Vertex gradient[] = {
            sf::Vertex(sf::Vector2f(0, 0), sf::Color(67, 56, 202)),     // Top-left: Deep purple
            sf::Vertex(sf::Vector2f(800, 0), sf::Color(79, 70, 229)),   // Top-right: Lighter purple
            sf::Vertex(sf::Vector2f(800, 600), sf::Color(99, 102, 241)), // Bottom-right: Even lighter
            sf::Vertex(sf::Vector2f(0, 600), sf::Color(67, 56, 202))    // Bottom-left: Deep purple
        };
        window.draw(gradient, 4, sf::Quads);
        
        // Draw decorative background circles
        for (const auto& circle : backgroundCircles) {
            window.draw(circle);
        }
        
        if (currentState == INPUT_PHASE) {
            // Draw input phase
            window.draw(titleBar);
            window.draw(messageText);
            window.draw(inputBox);
            window.draw(inputDisplayText);
        } else if (currentState == SUCCESS_PHASE) {
            // Draw success phase with semi-transparent overlay
            sf::RectangleShape overlay;
            overlay.setSize(sf::Vector2f(800, 600));
            overlay.setFillColor(sf::Color(0, 0, 0, 50)); // Dark overlay
            window.draw(overlay);
            
            // Draw decorative background circles again for success phase
            for (const auto& circle : backgroundCircles) {
                window.draw(circle);
            }
            
            window.draw(usernameLabel);
            window.draw(usernameBox);
            window.draw(usernameText);
            
            window.draw(passwordLabel);
            window.draw(passwordBox);
            window.draw(passwordText);
            
            window.draw(successMessage);
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
                
                handleInput(event);
            }
            
            render();
        }
    }
};

int main() {
    IntegerOverflowTask task;
    task.run();
    return 0;
}

// Compilation command:
// g++ -o integer_overflow_task main.cpp -lsfml-graphics -lsfml-window -lsfml-system
