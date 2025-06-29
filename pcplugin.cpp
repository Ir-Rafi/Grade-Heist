#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class PCSimulator {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // States
    bool switchOn;
    bool showInfoWindow;
    
    // Switch UI Elements
    sf::Text switchMessage;
    sf::RectangleShape switchBackground;
    sf::RectangleShape switchToggle;
    sf::Text onText;
    sf::Text offText;
    
    // Info Window Elements
    sf::RectangleShape infoWindow;
    sf::RectangleShape infoBox;
    sf::Text infoTitle;
    sf::Text usernameLabel;
    sf::Text passwordLabel;
    sf::RectangleShape usernameField;
    sf::RectangleShape passwordField;
    sf::Text usernameValue;
    sf::Text passwordValue;
    sf::Text exitMessage;

public:
    PCSimulator() : window(sf::VideoMode(800, 600), "PC Simulator"), 
                    switchOn(false), showInfoWindow(false) {
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load arial.ttf, using default font\n";
        }
        
        setupUI();
    }
    
    void setupUI() {
        // Switch message
        switchMessage.setFont(font);
        switchMessage.setString("Turn on the switch");
        switchMessage.setCharacterSize(24);
        switchMessage.setFillColor(sf::Color::White);
        switchMessage.setPosition(300, 200);
        
        // Switch background (the switch container)
        switchBackground.setSize(sf::Vector2f(120, 60));
        switchBackground.setPosition(340, 250);
        switchBackground.setFillColor(sf::Color(100, 100, 100));
        switchBackground.setOutlineThickness(2);
        switchBackground.setOutlineColor(sf::Color::White);
        
        // Switch toggle (the movable part)
        switchToggle.setSize(sf::Vector2f(56, 56));
        switchToggle.setPosition(342, 252); // OFF position
        switchToggle.setFillColor(sf::Color::Red);
        
        // ON text
        onText.setFont(font);
        onText.setString("ON");
        onText.setCharacterSize(14);
        onText.setFillColor(sf::Color::White);
        onText.setPosition(420, 270);
        
        // OFF text
        offText.setFont(font);
        offText.setString("OFF");
        offText.setCharacterSize(14);
        offText.setFillColor(sf::Color::White);
        offText.setPosition(350, 270);
        
        // Info window background
        infoWindow.setSize(sf::Vector2f(600, 400));
        infoWindow.setPosition(100, 100);
        infoWindow.setFillColor(sf::Color(40, 40, 40));
        infoWindow.setOutlineThickness(3);
        infoWindow.setOutlineColor(sf::Color::White);
        
        // Info box (inner container)
        infoBox.setSize(sf::Vector2f(500, 300));
        infoBox.setPosition(150, 150);
        infoBox.setFillColor(sf::Color(60, 60, 60));
        infoBox.setOutlineThickness(2);
        infoBox.setOutlineColor(sf::Color::Cyan);
        
        // Info title
        infoTitle.setFont(font);
        infoTitle.setString("Here is the username and password of PC");
        infoTitle.setCharacterSize(22);
        infoTitle.setFillColor(sf::Color::White);
        infoTitle.setPosition(180, 170);
        
        // Username label
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(20);
        usernameLabel.setFillColor(sf::Color::White);
        usernameLabel.setPosition(200, 230);
        
        // Username field
        usernameField.setSize(sf::Vector2f(250, 40));
        usernameField.setPosition(200, 260);
        usernameField.setFillColor(sf::Color::White);
        usernameField.setOutlineThickness(2);
        usernameField.setOutlineColor(sf::Color::Black);
        
        // Username value
        usernameValue.setFont(font);
        usernameValue.setString("student");
        usernameValue.setCharacterSize(18);
        usernameValue.setFillColor(sf::Color::Black);
        usernameValue.setPosition(210, 270);
        
        // Password label
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(20);
        passwordLabel.setFillColor(sf::Color::White);
        passwordLabel.setPosition(200, 320);
        
        // Password field
        passwordField.setSize(sf::Vector2f(250, 40));
        passwordField.setPosition(200, 350);
        passwordField.setFillColor(sf::Color::White);
        passwordField.setOutlineThickness(2);
        passwordField.setOutlineColor(sf::Color::Black);
        
        // Password value
        passwordValue.setFont(font);
        passwordValue.setString("student");
        passwordValue.setCharacterSize(18);
        passwordValue.setFillColor(sf::Color::Black);
        passwordValue.setPosition(210, 360);
        
        // Exit message
        exitMessage.setFont(font);
        exitMessage.setString("Press the Enter Button to exit the task");
        exitMessage.setCharacterSize(18);
        exitMessage.setFillColor(sf::Color::Yellow);
        exitMessage.setPosition(230, 410);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Handle keyboard input
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter && showInfoWindow) {
                    window.close(); // Exit the task when Enter is pressed
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                
                // Check switch click
                if (!switchOn && switchBackground.getGlobalBounds().contains(mousePos)) {
                    switchOn = true;
                    showInfoWindow = true;
                    
                    // Move toggle to ON position and change color
                    switchToggle.setPosition(402, 252); // ON position (right side)
                    switchToggle.setFillColor(sf::Color::Green);
                }
            }
        }
    }
    
    void render() {
        window.clear(sf::Color::Black);
        
        if (!switchOn) {
            // Show switch interface
            window.draw(switchMessage);
            window.draw(switchBackground);
            window.draw(switchToggle);
            window.draw(onText);
            window.draw(offText);
        }
        else if (showInfoWindow) {
            // Show info window
            window.draw(infoWindow);
            window.draw(infoBox);
            window.draw(exitMessage);
            window.draw(infoTitle);
            window.draw(usernameLabel);
            window.draw(usernameField);
            window.draw(usernameValue);
            window.draw(passwordLabel);
            window.draw(passwordField);
            window.draw(passwordValue);
        }
        
        window.display();
    }
    
    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }
};

int main() {
    PCSimulator app;
    app.run();
    return 0;
}
