#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <cctype>
#include <sstream>

class GradeChangerSystem {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // Login phase elements
    sf::Text loginTitleText, usernameLabel, passwordLabel;
    sf::RectangleShape usernameBox, passwordBox, loginButton;
    sf::Text usernameText, passwordText, loginButtonText, loginResultText;
    std::string loginUsername, loginPassword;
    bool usernameActive, passwordActive;
    bool loginSuccess;
    
    // Grade change phase elements
    sf::Text gradeTitleText, nameLabel, gradeLabel;
    sf::RectangleShape nameBox, gradeBox, submitButton;
    sf::Text nameText, gradeText, submitButtonText, submitResultText;
    std::string studentName, studentGrade;
    bool nameActive, gradeActive;
    bool gradeChanged;
    
    // Auto-close functionality
    sf::Clock closeClock;
    bool shouldClose;
    float closeDelay;
    
    enum Phase { LOGIN_PHASE, GRADE_PHASE };
    Phase currentPhase;
    
public:
    GradeChangerSystem() : window(sf::VideoMode(600, 500), "Grade Changer System"),
                          usernameActive(false), passwordActive(false), loginSuccess(false),
                          nameActive(false), gradeActive(false), gradeChanged(false),
                          shouldClose(false), closeDelay(2.0f),
                          currentPhase(LOGIN_PHASE) {
        
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load font file. Using default font." << std::endl;
        }
        
        setupLoginUI();
        setupGradeUI();
    }
    
    void setupLoginUI() {
        // Login title
        loginTitleText.setFont(font);
        loginTitleText.setString("Admin Login");
        loginTitleText.setCharacterSize(28);
        loginTitleText.setFillColor(sf::Color::White);
        loginTitleText.setPosition(200, 30);
        
        // Username label and box
        usernameLabel.setFont(font);
        usernameLabel.setString("Username:");
        usernameLabel.setCharacterSize(18);
        usernameLabel.setFillColor(sf::Color::Yellow);
        usernameLabel.setPosition(50, 100);
        
        usernameBox.setSize(sf::Vector2f(300, 40));
        usernameBox.setFillColor(sf::Color::White);
        usernameBox.setOutlineThickness(2);
        usernameBox.setOutlineColor(sf::Color::Blue);
        usernameBox.setPosition(50, 130);
        
        usernameText.setFont(font);
        usernameText.setCharacterSize(16);
        usernameText.setFillColor(sf::Color::Black);
        usernameText.setPosition(55, 140);
        
        // Password label and box
        passwordLabel.setFont(font);
        passwordLabel.setString("Password:");
        passwordLabel.setCharacterSize(18);
        passwordLabel.setFillColor(sf::Color::Yellow);
        passwordLabel.setPosition(50, 190);
        
        passwordBox.setSize(sf::Vector2f(300, 40));
        passwordBox.setFillColor(sf::Color::White);
        passwordBox.setOutlineThickness(2);
        passwordBox.setOutlineColor(sf::Color::Blue);
        passwordBox.setPosition(50, 220);
        
        passwordText.setFont(font);
        passwordText.setCharacterSize(16);
        passwordText.setFillColor(sf::Color::Black);
        passwordText.setPosition(55, 230);
        
        // Login button
        loginButton.setSize(sf::Vector2f(100, 40));
        loginButton.setFillColor(sf::Color::Green);
        loginButton.setOutlineThickness(2);
        loginButton.setOutlineColor(sf::Color::White);
        loginButton.setPosition(50, 280);
        
        loginButtonText.setFont(font);
        loginButtonText.setString("Login");
        loginButtonText.setCharacterSize(16);
        loginButtonText.setFillColor(sf::Color::White);
        loginButtonText.setPosition(75, 295);
        
        // Login result text
        loginResultText.setFont(font);
        loginResultText.setCharacterSize(18);
        loginResultText.setFillColor(sf::Color::Red);
        loginResultText.setPosition(50, 340);
    }
    
    void setupGradeUI() {
        // Grade change title
        gradeTitleText.setFont(font);
        gradeTitleText.setString("Grade Changer");
        gradeTitleText.setCharacterSize(28);
        gradeTitleText.setFillColor(sf::Color::White);
        gradeTitleText.setPosition(180, 30);
        
        // Name label and box
        nameLabel.setFont(font);
        nameLabel.setString("Student Name:");
        nameLabel.setCharacterSize(18);
        nameLabel.setFillColor(sf::Color::Yellow);
        nameLabel.setPosition(50, 100);
        
        nameBox.setSize(sf::Vector2f(300, 40));
        nameBox.setFillColor(sf::Color::White);
        nameBox.setOutlineThickness(2);
        nameBox.setOutlineColor(sf::Color::Blue);
        nameBox.setPosition(50, 130);
        
        nameText.setFont(font);
        nameText.setCharacterSize(16);
        nameText.setFillColor(sf::Color::Black);
        nameText.setPosition(55, 140);
        
        // Grade label and box
        gradeLabel.setFont(font);
        gradeLabel.setString("Grade:");
        gradeLabel.setCharacterSize(18);
        gradeLabel.setFillColor(sf::Color::Yellow);
        gradeLabel.setPosition(50, 190);
        
        gradeBox.setSize(sf::Vector2f(300, 40));
        gradeBox.setFillColor(sf::Color::White);
        gradeBox.setOutlineThickness(2);
        gradeBox.setOutlineColor(sf::Color::Blue);
        gradeBox.setPosition(50, 220);
        
        gradeText.setFont(font);
        gradeText.setCharacterSize(16);
        gradeText.setFillColor(sf::Color::Black);
        gradeText.setPosition(55, 230);
        
        // Submit button
        submitButton.setSize(sf::Vector2f(100, 40));
        submitButton.setFillColor(sf::Color::Green);
        submitButton.setOutlineThickness(2);
        submitButton.setOutlineColor(sf::Color::White);
        submitButton.setPosition(50, 280);
        
        submitButtonText.setFont(font);
        submitButtonText.setString("Submit");
        submitButtonText.setCharacterSize(16);
        submitButtonText.setFillColor(sf::Color::White);
        submitButtonText.setPosition(70, 295);
        
        // Submit result text
        submitResultText.setFont(font);
        submitResultText.setCharacterSize(20);
        submitResultText.setFillColor(sf::Color::Green);
        submitResultText.setPosition(50, 340);
    }
    
    void handleLoginTextInput(char c) {
        if (c == '\b') { // Backspace
            if (usernameActive && !loginUsername.empty()) {
                loginUsername.pop_back();
            } else if (passwordActive && !loginPassword.empty()) {
                loginPassword.pop_back();
            }
        } else if (c == '\t') { // Tab to switch fields
            usernameActive = !usernameActive;
            passwordActive = !passwordActive;
        } else if (std::isprint(c)) {
            if (usernameActive && loginUsername.length() < 20) {
                loginUsername += c;
            } else if (passwordActive && loginPassword.length() < 20) {
                loginPassword += c;
            }
        }
        
        usernameText.setString(loginUsername);
        passwordText.setString(std::string(loginPassword.length(), '*')); // Show asterisks for password
        
        // Update box colors based on active state
        usernameBox.setOutlineColor(usernameActive ? sf::Color::Green : sf::Color::Blue);
        passwordBox.setOutlineColor(passwordActive ? sf::Color::Green : sf::Color::Blue);
    }
    
    void handleGradeTextInput(char c) {
        if (c == '\b') { // Backspace
            if (nameActive && !studentName.empty()) {
                studentName.pop_back();
            } else if (gradeActive && !studentGrade.empty()) {
                studentGrade.pop_back();
            }
        } else if (c == '\t') { // Tab to switch fields
            nameActive = !nameActive;
            gradeActive = !gradeActive;
        } else if (std::isprint(c)) {
            if (nameActive && studentName.length() < 30) {
                studentName += c;
            } else if (gradeActive && studentGrade.length() < 10) {
                // Allow digits, decimal point, and letters for grades like "A+", "95.5", etc.
                if (std::isdigit(c) || c == '.' || std::isalpha(c) || c == '+' || c == '-') {
                    studentGrade += c;
                }
            }
        }
        
        nameText.setString(studentName);
        gradeText.setString(studentGrade);
        
        // Update box colors based on active state
        nameBox.setOutlineColor(nameActive ? sf::Color::Green : sf::Color::Blue);
        gradeBox.setOutlineColor(gradeActive ? sf::Color::Green : sf::Color::Blue);
    }
    
    void handleLoginMouseClick(sf::Vector2i mousePos) {
        // Check username box click
        if (usernameBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            usernameActive = true;
            passwordActive = false;
        }
        // Check password box click
        else if (passwordBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            usernameActive = false;
            passwordActive = true;
        }
        // Check login button click
        else if (loginButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            checkLogin();
        }
    }
    
    void handleGradeMouseClick(sf::Vector2i mousePos) {
        // Check name box click
        if (nameBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            nameActive = true;
            gradeActive = false;
        }
        // Check grade box click
        else if (gradeBox.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            nameActive = false;
            gradeActive = true;
        }
        // Check submit button click
        else if (submitButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
            submitGradeChange();
        }
    }
    
    void checkLogin() {
        if (loginUsername == "Admin" && loginPassword == "924689093745") {
            loginSuccess = true;
            currentPhase = GRADE_PHASE;
            loginResultText.setString("Login Successful!");
            loginResultText.setFillColor(sf::Color::Green);
        } else {
            loginResultText.setString("Invalid credentials!");
            loginResultText.setFillColor(sf::Color::Red);
        }
    }
    
    void submitGradeChange() {
        if (!studentName.empty() && !studentGrade.empty()) {
            gradeChanged = true;
            submitResultText.setString("Grade Changed - Closing in 2 seconds...");
            submitResultText.setFillColor(sf::Color::Green);
            shouldClose = true;
            closeClock.restart();
        } else {
            submitResultText.setString("Please fill both fields!");
            submitResultText.setFillColor(sf::Color::Red);
        }
    }
    
    void run() {
        while (window.isOpen()) {
            // Check if we should close the window after displaying grade changed message
            if (shouldClose && closeClock.getElapsedTime().asSeconds() >= closeDelay) {
                window.close();
                break;
            }
            
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                }
                
                if (event.type == sf::Event::TextEntered) {
                    if (currentPhase == LOGIN_PHASE) {
                        handleLoginTextInput(static_cast<char>(event.text.unicode));
                    } else if (currentPhase == GRADE_PHASE && !shouldClose) {
                        handleGradeTextInput(static_cast<char>(event.text.unicode));
                    }
                }
                
                if (event.type == sf::Event::MouseButtonPressed) {
                    if (event.mouseButton.button == sf::Mouse::Left) {
                        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                        if (currentPhase == LOGIN_PHASE) {
                            handleLoginMouseClick(mousePos);
                        } else if (currentPhase == GRADE_PHASE && !shouldClose) {
                            handleGradeMouseClick(mousePos);
                        }
                    }
                }
                
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Enter) {
                        if (currentPhase == LOGIN_PHASE) {
                            checkLogin();
                        } else if (currentPhase == GRADE_PHASE && !shouldClose) {
                            submitGradeChange();
                        }
                    }
                }
            }
            
            window.clear(sf::Color::Black);
            
            if (currentPhase == LOGIN_PHASE) {
                // Draw login UI
                window.draw(loginTitleText);
                window.draw(usernameLabel);
                window.draw(usernameBox);
                window.draw(usernameText);
                window.draw(passwordLabel);
                window.draw(passwordBox);
                window.draw(passwordText);
                window.draw(loginButton);
                window.draw(loginButtonText);
                window.draw(loginResultText);
            } else if (currentPhase == GRADE_PHASE) {
                // Draw grade change UI
                window.draw(gradeTitleText);
                window.draw(nameLabel);
                window.draw(nameBox);
                window.draw(nameText);
                window.draw(gradeLabel);
                window.draw(gradeBox);
                window.draw(gradeText);
                window.draw(submitButton);
                window.draw(submitButtonText);
                window.draw(submitResultText);
            }
            
            window.display();
        }
    }
};

int main() {
    GradeChangerSystem system;
    system.run();
    return 0;
}
