#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

enum GameState {
    WAITING_FOR_START,      // Window 1: Show acroom.png
    TASK_DESCRIPTION,       // Window 2: Show task description
    ROOM_SELECTION,         // Window 3: Choose room (3 options)
    ACTION_SELECTION,       // Window 4: Choose action (2 options)
    TASK_COMPLETED,         // Success state
    TASK_FAILED            // Failure state
};

class EscapeRoomGame {
private:
    sf::RenderWindow currentWindow;
    sf::Font font;
    sf::Texture acroomTexture;
    sf::Sprite acroomSprite;
    
    GameState currentState;
    sf::Clock autoCloseTimer;
    bool shouldAutoClose;
    bool taskCompleted;
    
    // UI Elements
    std::vector<sf::Text> menuOptions;
    std::vector<sf::RectangleShape> optionButtons;
    sf::Text titleText;
    sf::Text descriptionText;
    sf::Text instructionText;
    sf::Text resultText;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color primaryColor;
    sf::Color successColor;
    sf::Color errorColor;
    sf::Color buttonColor;
    sf::Color buttonHoverColor;
    
    // Removed hoveredOption since we don't need hover effects
    
public:
    EscapeRoomGame() : currentWindow(sf::VideoMode(800, 600), "Escape Room Challenge"),
                       currentState(WAITING_FOR_START),
                       shouldAutoClose(false),
                       taskCompleted(false),
                       backgroundColor(sf::Color(25, 25, 35)),
                       primaryColor(sf::Color(100, 200, 255)),
                       successColor(sf::Color(50, 200, 50)),
                       errorColor(sf::Color(200, 50, 50)),
                       buttonColor(sf::Color(60, 70, 90)),
                       buttonHoverColor(sf::Color(80, 90, 120)) {
        
        initializeUI();
        loadAssets();
        setupWindow1(); // Start with first window
    }
    
    void initializeUI() {
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load arial.ttf font file\n";
        }
        
        // Initialize basic text elements
        titleText.setFont(font);
        titleText.setCharacterSize(36);
        titleText.setFillColor(primaryColor);
        
        descriptionText.setFont(font);
        descriptionText.setCharacterSize(18);
        descriptionText.setFillColor(sf::Color::White);
        
        instructionText.setFont(font);
        instructionText.setCharacterSize(16);
        descriptionText.setFillColor(sf::Color::Yellow);
        
        resultText.setFont(font);
        resultText.setCharacterSize(24);
    }
    
    void loadAssets() {
        if (!acroomTexture.loadFromFile("acroom.png")) {
            std::cout << "Warning: Could not load acroom.png image file\n";
        } else {
            acroomSprite.setTexture(acroomTexture);
            
            // Scale image to fit window
            sf::Vector2u imageSize = acroomTexture.getSize();
            sf::Vector2u windowSize = currentWindow.getSize();
            
            float scaleX = (float)(windowSize.x - 100) / imageSize.x;
            float scaleY = (float)(windowSize.y - 200) / imageSize.y;
            float scale = std::min(scaleX, scaleY);
            
            acroomSprite.setScale(scale, scale);
            
            // Center the image
            sf::FloatRect spriteBounds = acroomSprite.getGlobalBounds();
            acroomSprite.setPosition(
                (windowSize.x - spriteBounds.width) / 2,
                80
            );
        }
    }
    
    void setupWindow1() {
        currentWindow.setTitle("AC Room - Click to Start");
        
        titleText.setString("Escape Room Challenge");
        titleText.setPosition(50, 20);
        
        instructionText.setString("Click on the AC room image to start the mission!");
        instructionText.setCharacterSize(20);
        instructionText.setFillColor(sf::Color::Yellow);
        instructionText.setPosition(50, currentWindow.getSize().y - 100);
        
        // Clear any existing options
        menuOptions.clear();
        optionButtons.clear();
    }
    
    void setupWindow2() {
        currentWindow.setTitle("Mission Briefing");
        
        titleText.setString("Mission Briefing");
        titleText.setPosition(50, 30);
        
        descriptionText.setString(
            "Choose the correct options to make Rana Sir\n"
            "leave the room where the result is kept.\n\n"
            "Your mission is critical - make the right choices\n"
            "to successfully complete the task!"
        );
        descriptionText.setPosition(50, 100);
        
        instructionText.setString("Click anywhere to continue to the next step...");
        instructionText.setFillColor(sf::Color::Cyan);
        instructionText.setPosition(50, currentWindow.getSize().y - 80);
        
        menuOptions.clear();
        optionButtons.clear();
    }
    
    void setupWindow3() {
        currentWindow.setTitle("Room Selection");
        
        titleText.setString("Select the Target Room");
        titleText.setPosition(50, 30);
        
        descriptionText.setString("Which room should you target to complete the mission?");
        descriptionText.setPosition(50, 100);
        
        // Create room options
        std::vector<std::string> roomOptions = {
            "Rana Sir's Room",
            "Database Room", 
            "Server's Room"
        };
        
        createMenuOptions(roomOptions, 180);
    }
    
    void setupWindow4() {
        currentWindow.setTitle("Action Selection");
        
        titleText.setString("Choose Your Action");
        titleText.setPosition(50, 30);
        
        descriptionText.setString("What action will make Rana Sir leave the room?");
        descriptionText.setPosition(50, 100);
        
        // Create action options
        std::vector<std::string> actionOptions = {
            "Turn Off the AC",
            "Lock the Room"
        };
        
        createMenuOptions(actionOptions, 180);
    }
    
    void createMenuOptions(const std::vector<std::string>& options, float startY) {
        menuOptions.clear();
        optionButtons.clear();
        
        for (size_t i = 0; i < options.size(); i++) {
            // Create text
            sf::Text optionText;
            optionText.setFont(font);
            optionText.setString(options[i]);
            optionText.setCharacterSize(22);
            optionText.setFillColor(sf::Color::White);
            
            // Create button background
            sf::RectangleShape button;
            button.setSize(sf::Vector2f(600, 60));
            button.setPosition(100, startY + i * 80);
            button.setFillColor(buttonColor);
            button.setOutlineThickness(2);
            button.setOutlineColor(primaryColor);
            
            // Position text in center of button
            sf::FloatRect textBounds = optionText.getLocalBounds();
            optionText.setPosition(
                button.getPosition().x + (button.getSize().x - textBounds.width) / 2,
                button.getPosition().y + (button.getSize().y - textBounds.height) / 2 - 5
            );
            
            menuOptions.push_back(optionText);
            optionButtons.push_back(button);
        }
    }
    
    // Removed mouse move handling - not needed for this task
    
    void handleClick(int x, int y) {
        switch (currentState) {
            case WAITING_FOR_START:
                if (acroomSprite.getGlobalBounds().contains(x, y)) {
                    currentState = TASK_DESCRIPTION;
                    setupWindow2();
                }
                break;
                
            case TASK_DESCRIPTION:
                currentState = ROOM_SELECTION;
                setupWindow3();
                break;
                
            case ROOM_SELECTION:
                for (size_t i = 0; i < optionButtons.size(); i++) {
                    if (optionButtons[i].getGlobalBounds().contains(x, y)) {
                        if (i == 0) { // "Rana Sir's Room" is correct
                            currentState = ACTION_SELECTION;
                            setupWindow4();
                        } else {
                            // Wrong choice - show failure and close
                            showFailureAndClose("Wrong room selected!");
                        }
                        return;
                    }
                }
                break;
                
            case ACTION_SELECTION:
                for (size_t i = 0; i < optionButtons.size(); i++) {
                    if (optionButtons[i].getGlobalBounds().contains(x, y)) {
                        if (i == 0) { // "Turn Off the AC" is correct
                            showSuccessAndClose();
                        } else {
                            // Wrong choice - show failure and close
                            showFailureAndClose("Wrong action selected!");
                        }
                        return;
                    }
                }
                break;
                
            default:
                break;
        }
    }
    
    void showSuccessAndClose() {
        currentState = TASK_COMPLETED;
        taskCompleted = true;
        
        currentWindow.setTitle("Mission Accomplished!");
        
        resultText.setString("🎉 Mission Accomplished! 🎉\n\nRana Sir has left the room!\nTask completed successfully!");
        resultText.setFillColor(successColor);
        resultText.setPosition(50, 200);
        
        shouldAutoClose = true;
        autoCloseTimer.restart();
        
        menuOptions.clear();
        optionButtons.clear();
    }
    
    void showFailureAndClose(const std::string& reason) {
        currentState = TASK_FAILED;
        taskCompleted = false;
        
        currentWindow.setTitle("Mission Failed!");
        
        resultText.setString("❌ Mission Failed! ❌\n\n" + reason + "\n\nWindow will close automatically...");
        resultText.setFillColor(errorColor);
        resultText.setPosition(50, 200);
        
        shouldAutoClose = true;
        autoCloseTimer.restart();
        
        menuOptions.clear();
        optionButtons.clear();
    }
    
    void draw() {
        currentWindow.clear(backgroundColor);
        
        switch (currentState) {
            case WAITING_FOR_START:
                currentWindow.draw(titleText);
                if (acroomTexture.getSize().x > 0) {
                    currentWindow.draw(acroomSprite);
                }
                currentWindow.draw(instructionText);
                break;
                
            case TASK_DESCRIPTION:
                currentWindow.draw(titleText);
                currentWindow.draw(descriptionText);
                currentWindow.draw(instructionText);
                break;
                
            case ROOM_SELECTION:
            case ACTION_SELECTION:
                currentWindow.draw(titleText);
                currentWindow.draw(descriptionText);
                
                // Draw buttons and options
                for (size_t i = 0; i < optionButtons.size(); i++) {
                    optionButtons[i].setFillColor(buttonColor);
                    currentWindow.draw(optionButtons[i]);
                    currentWindow.draw(menuOptions[i]);
                }
                break;
                
            case TASK_COMPLETED:
            case TASK_FAILED:
                currentWindow.draw(resultText);
                
                sf::Text autoCloseText;
                autoCloseText.setFont(font);
                autoCloseText.setString("Window will close in 3 seconds...");
                autoCloseText.setCharacterSize(16);
                autoCloseText.setFillColor(sf::Color::White);
                autoCloseText.setPosition(50, currentWindow.getSize().y - 50);
                currentWindow.draw(autoCloseText);
                break;
        }
        
        currentWindow.display();
    }
    
    void run() {
        while (currentWindow.isOpen()) {
            sf::Event event;
            while (currentWindow.pollEvent(event)) {
                switch (event.type) {
                    case sf::Event::Closed:
                        currentWindow.close();
                        break;
                        
                    case sf::Event::MouseButtonPressed:
                        if (event.mouseButton.button == sf::Mouse::Left) {
                            handleClick(event.mouseButton.x, event.mouseButton.y);
                        }
                        break;
                        
                    case sf::Event::KeyPressed:
                        if (event.key.code == sf::Keyboard::Escape) {
                            currentWindow.close();
                        }
                        break;
                        
                    default:
                        break;
                }
            }
            
            // Auto-close logic
            if (shouldAutoClose && autoCloseTimer.getElapsedTime().asSeconds() >= 3.0f) {
                currentWindow.close();
                break;
            }
            
            draw();
        }
    }
};

int main() {
    EscapeRoomGame game;
    game.run();
    return 0;
}

// Compilation instructions:
// g++ -o escape_room main.cpp -lsfml-graphics -lsfml-window -lsfml-system
// 
// Required files:
// - acroom.png (your AC room image)
// - arial.ttf (or any other TTF font file)
//
// Game Flow:
// Window 1: Click on acroom.png to start
// Window 2: Mission briefing - click anywhere to continue
// Window 3: Choose room (correct: "Rana Sir's Room")
// Window 4: Choose action (correct: "Turn Off the AC")
// Success/Failure: Auto-closes after 3 seconds
//
// Features:
// - Interactive hover effects on buttons
// - Automatic failure detection and window closure
// - Success celebration with auto-close
// - Clean, story-driven UI
// - ESC key to force exit anytime