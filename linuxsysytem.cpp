#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class LinuxWiFiSystem {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // States
    bool loginSuccess;
    bool showWiFiInfo;
    
    // Login Screen Elements
    sf::Text welcomeHeader;
    sf::RectangleShape loginBox;
    sf::Text usernameLabel;
    sf::Text passwordLabel;
    sf::RectangleShape usernameField;
    sf::RectangleShape passwordField;
    sf::Text usernameText;
    sf::Text passwordText;
    sf::RectangleShape loginButton;
    sf::Text loginButtonText;
    sf::Text errorMessage;
    
    // WiFi Info Screen Elements
    sf::Text wifiHeader;
    sf::RectangleShape wifiInfoBox;
    sf::Text wifiUsernameLabel;
    sf::Text wifiPasswordLabel;
    sf::RectangleShape wifiUsernameField;
    sf::RectangleShape wifiPasswordField;
    sf::Text wifiUsernameValue;
    sf::Text wifiPasswordValue;
    sf::Text completionMessage;
    sf::Text exitMessage;
    
    // Input handling
    std::string username;
    std::string password;
    bool usernameActive;
    bool passwordActive;
    bool showError;

public:
    LinuxWiFiSystem() : window(sf::VideoMode(900, 700), "Linux WiFi System"), 
                        loginSuccess(false), showWiFiInfo(false),
                        usernameActive(false), passwordActive(false), showError(false) {
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load arial.ttf, using default font\n";
        }
        
        setupUI();
    }
    
    void setupUI() {
        // Welcome header
        welcomeHeader.setFont(font);
        welcomeHeader.setString("Welcome to Linux System");
        welcomeHeader.setCharacterSize(32);
        welcomeHeader.setFillColor(sf::Color::White);
        welcomeHeader.setPosition(280, 100);
        
        // Login box
        loginBox.setSize(sf::Vector2f(500, 350));
        loginBox.setPosition(200, 180);
        loginBox.setFillColor(sf::Color(50, 50, 50));
        loginBox.setOutlineThickness(3);
        loginBox.setOutlineColor(sf::Color::Green);
        
        // Username label
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(20);
        usernameLabel.setFillColor(sf::Color::White);
        usernameLabel.setPosition(230, 230);
        
        // Username field
        usernameField.setSize(sf::Vector2f(300, 40));
        usernameField.setPosition(230, 260);
        usernameField.setFillColor(sf::Color::White);
        usernameField.setOutlineThickness(2);
        usernameField.setOutlineColor(sf::Color::Black);
        
        // Username text
        usernameText.setFont(font);
        usernameText.setCharacterSize(18);
        usernameText.setFillColor(sf::Color::Black);
        usernameText.setPosition(235, 270);
        
        // Password label
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(20);
        passwordLabel.setFillColor(sf::Color::White);
        passwordLabel.setPosition(230, 320);
        
        // Password field
        passwordField.setSize(sf::Vector2f(300, 40));
        passwordField.setPosition(230, 350);
        passwordField.setFillColor(sf::Color::White);
        passwordField.setOutlineThickness(2);
        passwordField.setOutlineColor(sf::Color::Black);
        
        // Password text
        passwordText.setFont(font);
        passwordText.setCharacterSize(18);
        passwordText.setFillColor(sf::Color::Black);
        passwordText.setPosition(235, 360);
        
        // Login button
        loginButton.setSize(sf::Vector2f(120, 45));
        loginButton.setPosition(320, 420);
        loginButton.setFillColor(sf::Color::Blue);
        
        // Login button text
        loginButtonText.setFont(font);
        loginButtonText.setString("Login");
        loginButtonText.setCharacterSize(20);
        loginButtonText.setFillColor(sf::Color::White);
        loginButtonText.setPosition(365, 432);
        
        // Error message
        errorMessage.setFont(font);
        errorMessage.setString("Invalid credentials! Try again.");
        errorMessage.setCharacterSize(16);
        errorMessage.setFillColor(sf::Color::Red);
        errorMessage.setPosition(320, 480);
        
        // WiFi Info Screen Setup
        // WiFi header
        wifiHeader.setFont(font);
        wifiHeader.setString("wifi-server's Info");
        wifiHeader.setCharacterSize(32);
        wifiHeader.setFillColor(sf::Color::White);
        wifiHeader.setPosition(320, 100);
        
        // WiFi info box
        wifiInfoBox.setSize(sf::Vector2f(600, 400));
        wifiInfoBox.setPosition(150, 160);
        wifiInfoBox.setFillColor(sf::Color(40, 40, 40));
        wifiInfoBox.setOutlineThickness(3);
        wifiInfoBox.setOutlineColor(sf::Color::Cyan);
        
        // WiFi username label
        wifiUsernameLabel.setFont(font);
        wifiUsernameLabel.setString("WiFi Username:");
        wifiUsernameLabel.setCharacterSize(22);
        wifiUsernameLabel.setFillColor(sf::Color::White);
        wifiUsernameLabel.setPosition(180, 200);
        
        // WiFi username field
        wifiUsernameField.setSize(sf::Vector2f(400, 45));
        wifiUsernameField.setPosition(180, 235);
        wifiUsernameField.setFillColor(sf::Color::White);
        wifiUsernameField.setOutlineThickness(2);
        wifiUsernameField.setOutlineColor(sf::Color::Black);
        
        // WiFi username value
        wifiUsernameValue.setFont(font);
        wifiUsernameValue.setString("CSEDU_Faculty");
        wifiUsernameValue.setCharacterSize(20);
        wifiUsernameValue.setFillColor(sf::Color::Black);
        wifiUsernameValue.setPosition(190, 245);
        
        // WiFi password label
        wifiPasswordLabel.setFont(font);
        wifiPasswordLabel.setString("WiFi Password:");
        wifiPasswordLabel.setCharacterSize(22);
        wifiPasswordLabel.setFillColor(sf::Color::White);
        wifiPasswordLabel.setPosition(180, 300);
        
        // WiFi password field
        wifiPasswordField.setSize(sf::Vector2f(400, 45));
        wifiPasswordField.setPosition(180, 335);
        wifiPasswordField.setFillColor(sf::Color::White);
        wifiPasswordField.setOutlineThickness(2);
        wifiPasswordField.setOutlineColor(sf::Color::Black);
        
        // WiFi password value
        wifiPasswordValue.setFont(font);
        wifiPasswordValue.setString("CSEDU2024_Secure!");
        wifiPasswordValue.setCharacterSize(20);
        wifiPasswordValue.setFillColor(sf::Color::Black);
        wifiPasswordValue.setPosition(190, 345);
        
        // Completion message
        completionMessage.setFont(font);
        completionMessage.setString("Task completed!!!!");
        completionMessage.setCharacterSize(24);
        completionMessage.setFillColor(sf::Color::Green);
        completionMessage.setPosition(350, 420);
        
        // Exit message
        exitMessage.setFont(font);
        exitMessage.setString("Press the enter button to exit the task");
        exitMessage.setCharacterSize(20);
        exitMessage.setFillColor(sf::Color::Yellow);
        exitMessage.setPosition(280, 460);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            // Handle keyboard input
            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Enter) {
                    if (showWiFiInfo) {
                        // Exit the task when Enter is pressed on WiFi info screen
                        window.close();
                    }
                    else if (!loginSuccess) {
                        // Try to login when Enter is pressed on login screen
                        attemptLogin();
                    }
                }
            }
            
            if (event.type == sf::Event::MouseButtonPressed && !loginSuccess) {
                sf::Vector2f mousePos = sf::Vector2f(event.mouseButton.x, event.mouseButton.y);
                
                // Username field click
                if (usernameField.getGlobalBounds().contains(mousePos)) {
                    usernameActive = true;
                    passwordActive = false;
                    usernameField.setOutlineColor(sf::Color::Blue);
                    passwordField.setOutlineColor(sf::Color::Black);
                }
                // Password field click
                else if (passwordField.getGlobalBounds().contains(mousePos)) {
                    passwordActive = true;
                    usernameActive = false;
                    passwordField.setOutlineColor(sf::Color::Blue);
                    usernameField.setOutlineColor(sf::Color::Black);
                }
                // Login button click
                else if (loginButton.getGlobalBounds().contains(mousePos)) {
                    attemptLogin();
                }
                else {
                    usernameActive = false;
                    passwordActive = false;
                    usernameField.setOutlineColor(sf::Color::Black);
                    passwordField.setOutlineColor(sf::Color::Black);
                }
            }
            
            // Handle text input
            if (event.type == sf::Event::TextEntered && !loginSuccess) {
                if (usernameActive) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!username.empty()) {
                            username.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
                        username += static_cast<char>(event.text.unicode);
                    }
                    usernameText.setString(username);
                }
                else if (passwordActive) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!password.empty()) {
                            password.pop_back();
                        }
                    }
                    else if (event.text.unicode < 128 && event.text.unicode != 8 && event.text.unicode != 13) {
                        password += static_cast<char>(event.text.unicode);
                    }
                    // Display asterisks for password
                    std::string displayPassword(password.length(), '*');
                    passwordText.setString(displayPassword);
                }
            }
        }
    }
    
    void attemptLogin() {
        if (username == "student" && password == "student") {
            loginSuccess = true;
            showWiFiInfo = true;
            showError = false;
        } else {
            showError = true;
        }
    }
    
    void render() {
        window.clear(sf::Color::Black);
        
        if (!loginSuccess) {
            // Show login screen
            window.draw(welcomeHeader);
            window.draw(loginBox);
            window.draw(usernameLabel);
            window.draw(usernameField);
            window.draw(usernameText);
            window.draw(passwordLabel);
            window.draw(passwordField);
            window.draw(passwordText);
            window.draw(loginButton);
            window.draw(loginButtonText);
            
            if (showError) {
                window.draw(errorMessage);
            }
        }
        else if (showWiFiInfo) {
            // Show WiFi info screen
            window.draw(wifiHeader);
            window.draw(wifiInfoBox);
            window.draw(wifiUsernameLabel);
            window.draw(wifiUsernameField);
            window.draw(wifiUsernameValue);
            window.draw(wifiPasswordLabel);
            window.draw(wifiPasswordField);
            window.draw(wifiPasswordValue);
            window.draw(completionMessage);
            window.draw(exitMessage);
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
    LinuxWiFiSystem app;
    app.run();
    return 0;
}
