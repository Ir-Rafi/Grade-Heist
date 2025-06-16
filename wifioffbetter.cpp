#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

enum class GameState {
    INSTRUCTION,
    LOGIN,
    CONTROL_PANEL,
    RESULT
};

enum class TaskResult {
    NONE,
    PASSED,
    FAILED
};

class WiFiTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;
    TaskResult result;
    
    // Input handling
    std::string usernameInput;
    std::string passwordInput;
    bool usernameActive;
    bool passwordActive;
    
    // Instruction Screen Elements
    sf::Text hintText;
    sf::Text instructionTitle;
    sf::RectangleShape startButton;
    sf::Text startButtonText;
    
    // Login Screen Elements
    sf::Text loginTitle;
    sf::RectangleShape usernameBox;
    sf::RectangleShape passwordBox;
    sf::Text usernameLabel;
    sf::Text passwordLabel;
    sf::Text usernameDisplayText;
    sf::Text passwordDisplayText;
    sf::RectangleShape loginButton;
    sf::Text loginButtonText;
    sf::Text errorText;
    
    // Control Panel Elements
    sf::Text controlTitle;
    sf::RectangleShape onButton;
    sf::RectangleShape offButton;
    sf::Text onButtonText;
    sf::Text offButtonText;
    
    // Result Screen Elements
    sf::Text resultText;
    sf::Text resultMessage;
    sf::RectangleShape closeButton;
    sf::Text closeButtonText;
    
public:
    WiFiTask() : window(sf::VideoMode(800, 600), "WiFi Password Collection Task"), 
                 currentState(GameState::INSTRUCTION), result(TaskResult::NONE),
                 usernameActive(false), passwordActive(false) {
        
        if (!font.loadFromFile("arial.ttf")) {
            // Font loading warning removed - will use default font
        }
        
        setupInstructionScreen();
        setupLoginScreen();
        setupControlPanel();
        setupResultScreen();
    }
    
    void setupInstructionScreen() {
        instructionTitle.setFont(font);
        instructionTitle.setString("WiFi Access Task");
        instructionTitle.setCharacterSize(32);
        instructionTitle.setFillColor(sf::Color::Cyan);
        instructionTitle.setPosition(280, 100);
        
        hintText.setFont(font);
        hintText.setString("First collect the wifi password from dept office");
        hintText.setCharacterSize(20);
        hintText.setFillColor(sf::Color::Yellow);
        hintText.setPosition(200, 250);
        
        startButton.setSize(sf::Vector2f(120, 50));
        startButton.setPosition(340, 350);
        startButton.setFillColor(sf::Color(70, 130, 180));
        startButton.setOutlineThickness(2);
        startButton.setOutlineColor(sf::Color::White);
        
        startButtonText.setFont(font);
        startButtonText.setString("Start");
        startButtonText.setCharacterSize(20);
        startButtonText.setFillColor(sf::Color::White);
        startButtonText.setPosition(375, 370);
    }
    
    void setupLoginScreen() {
        loginTitle.setFont(font);
        loginTitle.setString("Enter WiFi Credentials");
        loginTitle.setCharacterSize(28);
        loginTitle.setFillColor(sf::Color::White);
        loginTitle.setPosition(260, 80);
        
        // Username elements
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(18);
        usernameLabel.setFillColor(sf::Color::White);
        usernameLabel.setPosition(150, 180);
        
        usernameBox.setSize(sf::Vector2f(300, 40));
        usernameBox.setPosition(250, 175);
        usernameBox.setFillColor(sf::Color(40, 40, 40));
        usernameBox.setOutlineThickness(2);
        usernameBox.setOutlineColor(sf::Color(128, 128, 128)); // Custom gray color
        
        usernameDisplayText.setFont(font);
        usernameDisplayText.setCharacterSize(16);
        usernameDisplayText.setFillColor(sf::Color::White);
        usernameDisplayText.setPosition(255, 185);
        
        // Password elements
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(18);
        passwordLabel.setFillColor(sf::Color::White);
        passwordLabel.setPosition(150, 250);
        
        passwordBox.setSize(sf::Vector2f(300, 40));
        passwordBox.setPosition(250, 245);
        passwordBox.setFillColor(sf::Color(40, 40, 40));
        passwordBox.setOutlineThickness(2);
        passwordBox.setOutlineColor(sf::Color(128, 128, 128)); // Custom gray color
        
        passwordDisplayText.setFont(font);
        passwordDisplayText.setCharacterSize(16);
        passwordDisplayText.setFillColor(sf::Color::White);
        passwordDisplayText.setPosition(255, 255);
        
        // Login button
        loginButton.setSize(sf::Vector2f(120, 45));
        loginButton.setPosition(340, 330);
        loginButton.setFillColor(sf::Color(50, 150, 50));
        loginButton.setOutlineThickness(2);
        loginButton.setOutlineColor(sf::Color::White);
        
        loginButtonText.setFont(font);
        loginButtonText.setString("Login");
        loginButtonText.setCharacterSize(18);
        loginButtonText.setFillColor(sf::Color::White);
        loginButtonText.setPosition(375, 347);
        
        // Error text
        errorText.setFont(font);
        errorText.setCharacterSize(16);
        errorText.setFillColor(sf::Color::Red);
        errorText.setPosition(250, 400);
    }
    
    void setupControlPanel() {
        controlTitle.setFont(font);
        controlTitle.setString("WiFi Control Panel");
        controlTitle.setCharacterSize(32);
        controlTitle.setFillColor(sf::Color::Green);
        controlTitle.setPosition(250, 150);
        
        // ON Button
        onButton.setSize(sf::Vector2f(150, 80));
        onButton.setPosition(200, 300);
        onButton.setFillColor(sf::Color(50, 150, 50));
        onButton.setOutlineThickness(3);
        onButton.setOutlineColor(sf::Color::White);
        
        onButtonText.setFont(font);
        onButtonText.setString("ON");
        onButtonText.setCharacterSize(28);
        onButtonText.setFillColor(sf::Color::White);
        onButtonText.setPosition(250, 330);
        
        // OFF Button
        offButton.setSize(sf::Vector2f(150, 80));
        offButton.setPosition(450, 300);
        offButton.setFillColor(sf::Color(150, 50, 50));
        offButton.setOutlineThickness(3);
        offButton.setOutlineColor(sf::Color::White);
        
        offButtonText.setFont(font);
        offButtonText.setString("OFF");
        offButtonText.setCharacterSize(28);
        offButtonText.setFillColor(sf::Color::White);
        offButtonText.setPosition(495, 330);
    }
    
    void setupResultScreen() {
        resultText.setFont(font);
        resultText.setCharacterSize(36);
        resultText.setPosition(300, 200);
        
        resultMessage.setFont(font);
        resultMessage.setCharacterSize(20);
        resultMessage.setFillColor(sf::Color::White);
        resultMessage.setPosition(250, 280);
        
        closeButton.setSize(sf::Vector2f(120, 50));
        closeButton.setPosition(340, 400);
        closeButton.setFillColor(sf::Color(80, 80, 100));
        closeButton.setOutlineThickness(2);
        closeButton.setOutlineColor(sf::Color::White);
        
        closeButtonText.setFont(font);
        closeButtonText.setString("Close");
        closeButtonText.setCharacterSize(18);
        closeButtonText.setFillColor(sf::Color::White);
        closeButtonText.setPosition(375, 420);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                handleMouseClick(mousePos);
            }
            
            if (event.type == sf::Event::TextEntered && currentState == GameState::LOGIN) {
                handleTextInput(event.text.unicode);
            }
            
            if (event.type == sf::Event::KeyPressed && currentState == GameState::LOGIN) {
                if (event.key.code == sf::Keyboard::Tab) {
                    // Switch between username and password fields
                    if (usernameActive) {
                        usernameActive = false;
                        passwordActive = true;
                    } else {
                        usernameActive = true;
                        passwordActive = false;
                    }
                    updateInputBoxColors();
                }
                
                if (event.key.code == sf::Keyboard::Enter) {
                    attemptLogin();
                }
            }
        }
    }
    
    void handleMouseClick(const sf::Vector2i& mousePos) {
        float x = static_cast<float>(mousePos.x);
        float y = static_cast<float>(mousePos.y);
        
        if (currentState == GameState::INSTRUCTION) {
            if (startButton.getGlobalBounds().contains(x, y)) {
                currentState = GameState::LOGIN;
                usernameActive = true;
                passwordActive = false;
                updateInputBoxColors();
            }
        }
        else if (currentState == GameState::LOGIN) {
            if (usernameBox.getGlobalBounds().contains(x, y)) {
                usernameActive = true;
                passwordActive = false;
                updateInputBoxColors();
            }
            else if (passwordBox.getGlobalBounds().contains(x, y)) {
                usernameActive = false;
                passwordActive = true;
                updateInputBoxColors();
            }
            else if (loginButton.getGlobalBounds().contains(x, y)) {
                attemptLogin();
            }
        }
        else if (currentState == GameState::CONTROL_PANEL) {
            if (onButton.getGlobalBounds().contains(x, y)) {
                // Clicking ON means user failed the task
                result = TaskResult::FAILED;
                currentState = GameState::RESULT;
                updateResultScreen();
            }
            else if (offButton.getGlobalBounds().contains(x, y)) {
                // Clicking OFF means user passed the task
                result = TaskResult::PASSED;
                currentState = GameState::RESULT;
                updateResultScreen();
            }
        }
        else if (currentState == GameState::RESULT) {
            if (closeButton.getGlobalBounds().contains(x, y)) {
                window.close();
            }
        }
    }
    
    void handleTextInput(sf::Uint32 unicode) {
        if (unicode == 8) { // Backspace
            if (usernameActive && !usernameInput.empty()) {
                usernameInput.pop_back();
            }
            else if (passwordActive && !passwordInput.empty()) {
                passwordInput.pop_back();
            }
        }
        else if (unicode >= 32 && unicode < 128) { // Printable characters
            if (usernameActive && usernameInput.length() < 20) {
                usernameInput += static_cast<char>(unicode);
            }
            else if (passwordActive && passwordInput.length() < 20) {
                passwordInput += static_cast<char>(unicode);
            }
        }
        
        updateInputDisplay();
    }
    
    void updateInputBoxColors() {
        if (usernameActive) {
            usernameBox.setOutlineColor(sf::Color::Cyan);
            passwordBox.setOutlineColor(sf::Color(128, 128, 128)); // Custom gray color
        } else if (passwordActive) {
            usernameBox.setOutlineColor(sf::Color(128, 128, 128)); // Custom gray color
            passwordBox.setOutlineColor(sf::Color::Cyan);
        } else {
            usernameBox.setOutlineColor(sf::Color(128, 128, 128)); // Custom gray color
            passwordBox.setOutlineColor(sf::Color(128, 128, 128)); // Custom gray color
        }
    }
    
    void updateInputDisplay() {
        usernameDisplayText.setString(usernameInput);
        
        // Display password as asterisks
        std::string maskedPassword(passwordInput.length(), '*');
        passwordDisplayText.setString(maskedPassword);
    }
    
    void attemptLogin() {
        if (usernameInput == "CSEDU_Faculty" && passwordInput == "CSEDU2024_Secure!") {
            // Correct credentials - proceed to control panel
            currentState = GameState::CONTROL_PANEL;
            errorText.setString("");
        } else {
            // Wrong credentials - show error and fail task
            errorText.setString("Wrong credentials! Task Failed!");
            result = TaskResult::FAILED;
            // Wait a moment then show result
            sf::sleep(sf::seconds(1.5f));
            currentState = GameState::RESULT;
            updateResultScreen();
        }
    }
    
    void updateResultScreen() {
        if (result == TaskResult::PASSED) {
            resultText.setString("TASK PASSED!");
            resultText.setFillColor(sf::Color::Green);
            resultMessage.setString("Congratulations! You completed the task successfully.");
            resultText.setPosition(270, 200);
        } else {
            resultText.setString("TASK FAILED!");
            resultText.setFillColor(sf::Color::Red);
            if (usernameInput != "CSEDU_Faculty" || passwordInput != "CSEDU2024_Secure!") {
                resultMessage.setString("Wrong credentials provided.");
            } else {
                resultMessage.setString("You should have turned OFF the WiFi.");
            }
            resultText.setPosition(280, 200);
        }
    }
    
    void render() {
        window.clear(sf::Color(20, 20, 30));
        
        switch (currentState) {
            case GameState::INSTRUCTION:
                renderInstructionScreen();
                break;
            case GameState::LOGIN:
                renderLoginScreen();
                break;
            case GameState::CONTROL_PANEL:
                renderControlPanel();
                break;
            case GameState::RESULT:
                renderResultScreen();
                break;
        }
        
        window.display();
    }
    
    void renderInstructionScreen() {
        window.draw(instructionTitle);
        window.draw(hintText);
        window.draw(startButton);
        window.draw(startButtonText);
    }
    
    void renderLoginScreen() {
        window.draw(loginTitle);
        window.draw(usernameLabel);
        window.draw(usernameBox);
        window.draw(usernameDisplayText);
        window.draw(passwordLabel);
        window.draw(passwordBox);
        window.draw(passwordDisplayText);
        window.draw(loginButton);
        window.draw(loginButtonText);
        window.draw(errorText);
    }
    
    void renderControlPanel() {
        window.draw(controlTitle);
        window.draw(onButton);
        window.draw(onButtonText);
        window.draw(offButton);
        window.draw(offButtonText);
    }
    
    void renderResultScreen() {
        window.draw(resultText);
        window.draw(resultMessage);
        window.draw(closeButton);
        window.draw(closeButtonText);
    }
    
    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }
};

int main() {
    WiFiTask task;
    task.run();
    return 0;
}