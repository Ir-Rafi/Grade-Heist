#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include<cmath>

class WiFiServerTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // Game states
    enum GameState {
        LOGIN_SCREEN,
        OPTION_SCREEN,
        RESULT_SCREEN
    };
    GameState currentState;
    
    // UI elements
    sf::Text titleText;
    sf::Text usernameLabel;
    sf::Text passwordLabel;
    sf::Text usernameInput;
    sf::Text passwordInput;
    sf::Text loginButton;
    sf::Text onButton;
    sf::Text offButton;
    sf::Text resultText;
    sf::Text exitText;
    
    // Input boxes
    sf::RectangleShape usernameBox;
    sf::RectangleShape passwordBox;
    sf::RectangleShape loginBtnBox;
    sf::RectangleShape onBtnBox;
    sf::RectangleShape offBtnBox;
    
    // Input states
    bool usernameActive;
    bool passwordActive;
    std::string usernameStr;
    std::string passwordStr;
    std::string passwordDisplay; // For showing asterisks
    
    // Valid credentials
    std::vector<std::string> validUsernames;
    std::vector<std::string> validPasswords;
    
    bool credentialsValid;
    bool taskPassed;
    
public:
    WiFiServerTask() : window(sf::VideoMode(800, 600), "WiFi Server Task") {
        // Initialize game state
        currentState = LOGIN_SCREEN;
        usernameActive = false;
        passwordActive = false;
        credentialsValid = false;
        taskPassed = false;
        
        // Load font (you'll need to provide a font file)
        if (!font.loadFromFile("arial.ttf")) {
            // If arial.ttf is not found, try to use system default
            // You may need to adjust the font path based on your system
            std::cout << "Warning: Could not load font. Using default font.\n";
        }
        
        initializeCredentials();
        setupUI();
    }
    
    void initializeCredentials() {
        // Base usernames
        validUsernames = {
            "CSEDU_Faculty", "CSEDU_Student", "CSEDU_Staff", "CSEDU_Admin", "CSEDU_Guest",
            "WiFi_Faculty", "WiFi_Student", "WiFi_Staff", "WiFi_Admin", "WiFi_Guest",
            "Network_User", "Campus_User", "Secure_User", "Access_User", "Connect_User"
        };
        
        // Generate valid passwords
        std::vector<std::string> passwordPrefixes = {
            "CSEDU2024_", "Secure_", "Access_", "Network_", "Campus_", "WiFi_", "Connect_",
            "Login_", "Pass_", "Key_", "Auth_", "Safe_", "Guard_", "Shield_", "Lock_"
        };
        
        std::vector<std::string> passwordSuffixes = {
            "Secure!", "Safe#", "Key@", "Pass$", "Auth%", "Net&", "Lock*", "Code+",
            "Hash=", "Crypt!", "Guard#", "Shield@", "Wall$", "Gate%", "Door&"
        };
        
        // Generate combinations
        for (const auto& prefix : passwordPrefixes) {
            for (const auto& suffix : passwordSuffixes) {
                validPasswords.push_back(prefix + suffix);
            }
        }
    }
    
    void setupUI() {
        // Title
        titleText.setFont(font);
        titleText.setString("Welcome to WIFI Server");
        titleText.setCharacterSize(30);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(250, 50);
        
        // Username label and input
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(20);
        usernameLabel.setFillColor(sf::Color::White);
        usernameLabel.setPosition(200, 150);
        
        usernameInput.setFont(font);
        usernameInput.setCharacterSize(18);
        usernameInput.setFillColor(sf::Color::Black);
        usernameInput.setPosition(210, 185);
        
        usernameBox.setSize(sf::Vector2f(300, 35));
        usernameBox.setPosition(200, 180);
        usernameBox.setFillColor(sf::Color(255, 255, 255, 230));
        usernameBox.setOutlineColor(sf::Color(100, 149, 237));
        usernameBox.setOutlineThickness(2);
        
        // Password label and input
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(20);
        passwordLabel.setFillColor(sf::Color::White);
        passwordLabel.setPosition(200, 250);
        
        passwordInput.setFont(font);
        passwordInput.setCharacterSize(18);
        passwordInput.setFillColor(sf::Color::Black);
        passwordInput.setPosition(210, 285);
        
        passwordBox.setSize(sf::Vector2f(300, 35));
        passwordBox.setPosition(200, 280);
        passwordBox.setFillColor(sf::Color(255, 255, 255, 230));
        passwordBox.setOutlineColor(sf::Color(100, 149, 237));
        passwordBox.setOutlineThickness(2);
        
        // Login button
        loginButton.setFont(font);
        loginButton.setString("Login");
        loginButton.setCharacterSize(20);
        loginButton.setFillColor(sf::Color::White);
        loginButton.setPosition(370, 355);
        
        loginBtnBox.setSize(sf::Vector2f(80, 40));
        loginBtnBox.setPosition(350, 350);
        loginBtnBox.setFillColor(sf::Color(72, 61, 139));
        loginBtnBox.setOutlineColor(sf::Color(255, 255, 255, 100));
        loginBtnBox.setOutlineThickness(2);
        
        // ON/OFF buttons
        onButton.setFont(font);
        onButton.setString("ON");
        onButton.setCharacterSize(20);
        onButton.setFillColor(sf::Color::White);
        onButton.setPosition(280, 255);
        
        onBtnBox.setSize(sf::Vector2f(80, 40));
        onBtnBox.setPosition(260, 250);
        onBtnBox.setFillColor(sf::Color(34, 139, 34));
        onBtnBox.setOutlineColor(sf::Color(255, 255, 255, 150));
        onBtnBox.setOutlineThickness(2);
        
        offButton.setFont(font);
        offButton.setString("OFF");
        offButton.setCharacterSize(20);
        offButton.setFillColor(sf::Color::White);
        offButton.setPosition(460, 255);
        
        offBtnBox.setSize(sf::Vector2f(80, 40));
        offBtnBox.setPosition(440, 250);
        offBtnBox.setFillColor(sf::Color(220, 20, 60));
        offBtnBox.setOutlineColor(sf::Color(255, 255, 255, 150));
        offBtnBox.setOutlineThickness(2);
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(30);
        resultText.setPosition(300, 200);
        
        // Exit text
        exitText.setFont(font);
        exitText.setString("Press Enter button to exit the task");
        exitText.setCharacterSize(18);
        exitText.setFillColor(sf::Color::White);
        exitText.setPosition(250, 400);
    }
    
    bool isValidCredentials(const std::string& username, const std::string& password) {
        bool usernameValid = std::find(validUsernames.begin(), validUsernames.end(), username) != validUsernames.end();
        bool passwordValid = std::find(validPasswords.begin(), validPasswords.end(), password) != validPasswords.end();
        return usernameValid && passwordValid;
    }
    
    void handleInput(sf::Event& event) {
        if (currentState == LOGIN_SCREEN) {
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Check username box click
                if (usernameBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    usernameActive = true;
                    passwordActive = false;
                    usernameBox.setOutlineColor(sf::Color(0, 255, 255));
                    passwordBox.setOutlineColor(sf::Color(100, 149, 237));
                }
                // Check password box click
                else if (passwordBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    passwordActive = true;
                    usernameActive = false;
                    passwordBox.setOutlineColor(sf::Color(0, 255, 255));
                    usernameBox.setOutlineColor(sf::Color(100, 149, 237));
                }
                // Check login button click
                else if (loginBtnBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    if (isValidCredentials(usernameStr, passwordStr)) {
                        credentialsValid = true;
                        currentState = OPTION_SCREEN;
                    } else {
                        credentialsValid = false;
                        taskPassed = false;
                        currentState = RESULT_SCREEN;
                        resultText.setString("Task Failed!");
                        resultText.setFillColor(sf::Color::Red);
                    }
                }
                else {
                    usernameActive = false;
                    passwordActive = false;
                    usernameBox.setOutlineColor(sf::Color(100, 149, 237));
                    passwordBox.setOutlineColor(sf::Color(100, 149, 237));
                }
            }
            
            if (event.type == sf::Event::TextEntered) {
                if (usernameActive) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!usernameStr.empty()) {
                            usernameStr.pop_back();
                        }
                    } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                        usernameStr += static_cast<char>(event.text.unicode);
                    }
                    usernameInput.setString(usernameStr);
                }
                
                if (passwordActive) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!passwordStr.empty()) {
                            passwordStr.pop_back();
                            passwordDisplay.pop_back();
                        }
                    } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                        passwordStr += static_cast<char>(event.text.unicode);
                        passwordDisplay += '*';
                    }
                    passwordInput.setString(passwordDisplay);
                }
            }
        }
        
        if (currentState == OPTION_SCREEN) {
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                // Check ON button click
                if (onBtnBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    taskPassed = false;
                    currentState = RESULT_SCREEN;
                    resultText.setString("Task Failed!");
                    resultText.setFillColor(sf::Color::Red);
                }
                // Check OFF button click
                else if (offBtnBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    taskPassed = true;
                    currentState = RESULT_SCREEN;
                    resultText.setString("Task Passed!!(4)");
                    resultText.setFillColor(sf::Color::Green);
                }
            }
        }
        
        if (currentState == RESULT_SCREEN) {
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    window.close();
                }
            }
        }
    }
    
    void drawGradientBackground() {
        // Create a gradient background using vertex array
        sf::VertexArray gradient(sf::Quads, 4);
        
        // Define gradient colors
        sf::Color topLeft(30, 60, 114);     // Dark blue
        sf::Color topRight(42, 82, 152);    // Medium blue
        sf::Color bottomLeft(138, 43, 226); // Purple
        sf::Color bottomRight(255, 20, 147); // Pink
        
        // Set vertex positions and colors
        gradient[0].position = sf::Vector2f(0, 0);
        gradient[0].color = topLeft;
        
        gradient[1].position = sf::Vector2f(800, 0);
        gradient[1].color = topRight;
        
        gradient[2].position = sf::Vector2f(800, 600);
        gradient[2].color = bottomRight;
        
        gradient[3].position = sf::Vector2f(0, 600);
        gradient[3].color = bottomLeft;
        
        window.draw(gradient);
        
        // Add some animated circles for extra coolness
        static float time = 0;
        time += 0.02f;
        
        // Create floating circles
        for (int i = 0; i < 8; i++) {
            sf::CircleShape circle(30 + sin(time + i) * 10);
            circle.setFillColor(sf::Color(255, 255, 255, 20 + sin(time + i * 0.5) * 15));
            
            float x = 100 + i * 100 + cos(time + i * 0.3) * 50;
            float y = 100 + sin(time + i * 0.7) * 200;
            
            circle.setPosition(x, y);
            window.draw(circle);
        }
    }
    
    void render() {
        drawGradientBackground();
        
        if (currentState == LOGIN_SCREEN) {
            window.draw(titleText);
            window.draw(usernameBox);
            window.draw(usernameLabel);
            window.draw(usernameInput);
            window.draw(passwordBox);
            window.draw(passwordLabel);
            window.draw(passwordInput);
            window.draw(loginBtnBox);
            window.draw(loginButton);
        }
        else if (currentState == OPTION_SCREEN) {
            window.draw(titleText);
            window.draw(onBtnBox);
            window.draw(onButton);
            window.draw(offBtnBox);
            window.draw(offButton);
        }
        else if (currentState == RESULT_SCREEN) {
            window.draw(resultText);
            window.draw(exitText);
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
    WiFiServerTask game;
    game.run();
    
    return 0;
}

// Compilation instructions:
// Make sure you have SFML installed and compile with:
// g++ -o wifi_task wifi_task.cpp -lsfml-graphics -lsfml-window -lsfml-system
// 
// You'll also need a font file (arial.ttf) in the same directory as the executable.
// You can download a free font like DejaVu Sans or modify the code to use a different font path.
