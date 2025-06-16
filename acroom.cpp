#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>

enum class GameState {
    WELCOME,
    ROOM_SELECTION,
    AC_CONTROL,
    RESULT
};

class ACServerGame {
private:
    sf::RenderWindow window;
    sf::Font font;
    GameState currentState;
    int selectedRoom;
    bool taskPassed;
    
    // UI Elements
    sf::Text titleText;
    sf::Text welcomeText;
    sf::Text instructionText;
    sf::Text resultText;
    sf::RectangleShape startButton;
    sf::Text startButtonText;
    
    // Room buttons (401-420)
    std::vector<sf::RectangleShape> roomButtons;
    std::vector<sf::Text> roomTexts;
    
    // AC control buttons
    sf::RectangleShape onButton;
    sf::RectangleShape offButton;
    sf::Text onButtonText;
    sf::Text offButtonText;
    sf::Text selectedRoomText;
    
    // Close button for result screen
    sf::RectangleShape closeButton;
    sf::Text closeButtonText;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color primaryColor;
    sf::Color buttonColor;
    sf::Color successColor;
    sf::Color failColor;
    
public:
    ACServerGame()
    : window(sf::VideoMode(900, 700), "CSEDU AC Server Control")
    , currentState(GameState::WELCOME)
    , selectedRoom(0)
    , taskPassed(false)
    , backgroundColor(sf::Color(20, 25, 35))
    , primaryColor(sf::Color(100, 200, 255))
    , buttonColor(sf::Color(60, 80, 120))
    , successColor(sf::Color(50, 200, 50))
    , failColor(sf::Color(200, 50, 50))
    {
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load arial.ttf\n";
        }
        
        setupWelcomeScreen();
        setupRoomSelection();
        setupACControl();
        setupResultScreen();
    }
    
    void setupWelcomeScreen() {
        titleText.setFont(font);
        titleText.setString("CSEDU's AC Server");
        titleText.setCharacterSize(42);
        titleText.setFillColor(primaryColor);
        titleText.setPosition(280, 150);

        welcomeText.setFont(font);
        welcomeText.setString("Wise people make wise decisions");
        welcomeText.setCharacterSize(28);
        welcomeText.setFillColor(sf::Color::Yellow);
        welcomeText.setPosition(250, 250);

        instructionText.setFont(font);
        instructionText.setString("Click Start to begin your mission");
        instructionText.setCharacterSize(20);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(320, 320);

        startButton.setSize({200, 60});
        startButton.setPosition(350, 400);
        startButton.setFillColor(buttonColor);
        startButton.setOutlineThickness(3);
        startButton.setOutlineColor(primaryColor);

        startButtonText.setFont(font);
        startButtonText.setString("START");
        startButtonText.setCharacterSize(24);
        startButtonText.setFillColor(sf::Color::White);
        startButtonText.setPosition(420, 425);
    }

    void setupRoomSelection() {
        roomButtons.clear();
        roomTexts.clear();
        
        for (int i = 0; i < 20; ++i) {
            int roomNumber = 401 + i;
            int col = i % 4;
            int row = i / 4;
            float x = 150 + col * 160;
            float y = 180 + row * 80;
            
            sf::RectangleShape button;
            button.setSize({140, 60});
            button.setPosition(x, y);
            button.setFillColor(buttonColor);
            button.setOutlineThickness(2);
            button.setOutlineColor(sf::Color::White);
            
            sf::Text text;
            text.setFont(font);
            text.setString("Room " + std::to_string(roomNumber));
            text.setCharacterSize(16);
            text.setFillColor(sf::Color::White);
            sf::FloatRect tb = text.getLocalBounds();
            text.setPosition(x + (140 - tb.width) / 2, y + (60 - tb.height) / 2 - 3);
            
            roomButtons.push_back(button);
            roomTexts.push_back(text);
        }
    }
    
    void setupACControl() {
        selectedRoomText.setFont(font);
        selectedRoomText.setCharacterSize(28);
        selectedRoomText.setFillColor(sf::Color::White);
        selectedRoomText.setPosition(300, 200);

        instructionText.setFont(font);
        instructionText.setString("Choose AC control action:");
        instructionText.setCharacterSize(20);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(320, 280);

        onButton.setSize({180, 80});
        onButton.setPosition(250, 350);
        onButton.setFillColor(sf::Color(200, 80, 80));
        onButton.setOutlineThickness(3);
        onButton.setOutlineColor(sf::Color::White);

        onButtonText.setFont(font);
        onButtonText.setString("TURN ON AC");
        onButtonText.setCharacterSize(20);
        onButtonText.setFillColor(sf::Color::White);
        onButtonText.setPosition(285, 380);

        offButton.setSize({180, 80});
        offButton.setPosition(470, 350);
        offButton.setFillColor(sf::Color(80, 200, 80));
        offButton.setOutlineThickness(3);
        offButton.setOutlineColor(sf::Color::White);

        offButtonText.setFont(font);
        offButtonText.setString("TURN OFF AC");
        offButtonText.setCharacterSize(20);
        offButtonText.setFillColor(sf::Color::White);
        offButtonText.setPosition(500, 380);
    }
    
    void setupResultScreen() {
        resultText.setFont(font);
        resultText.setCharacterSize(32);
        resultText.setPosition(250, 250);

        closeButton.setSize({150, 50});
        closeButton.setPosition(375, 450);
        closeButton.setFillColor(buttonColor);
        closeButton.setOutlineThickness(2);
        closeButton.setOutlineColor(sf::Color::White);

        closeButtonText.setFont(font);
        closeButtonText.setString("CLOSE");
        closeButtonText.setCharacterSize(18);
        closeButtonText.setFillColor(sf::Color::White);
        closeButtonText.setPosition(425, 470);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2i m = sf::Mouse::getPosition(window);
                handleMouseClick({static_cast<float>(m.x), static_cast<float>(m.y)});
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Escape) {
                window.close();
            }
        }
    }
    
    void handleMouseClick(const sf::Vector2f& pos) {
        float x = pos.x, y = pos.y;
        
        if (currentState == GameState::WELCOME) {
            if (startButton.getGlobalBounds().contains(x, y)) {
                currentState = GameState::ROOM_SELECTION;
            }
        }
        else if (currentState == GameState::ROOM_SELECTION) {
            for (size_t i = 0; i < roomButtons.size(); ++i) {
                if (roomButtons[i].getGlobalBounds().contains(x, y)) {
                    selectedRoom = 401 + static_cast<int>(i);
                    selectedRoomText.setString("Selected: Room " + std::to_string(selectedRoom));
                    currentState = GameState::AC_CONTROL;
                    break;
                }
            }
        }
        else if (currentState == GameState::AC_CONTROL) {
            if (onButton.getGlobalBounds().contains(x, y)) {
                taskPassed = false;
                showResult();
            }
            else if (offButton.getGlobalBounds().contains(x, y)) {
                taskPassed = (selectedRoom == 405);
                showResult();
            }
        }
        else if (currentState == GameState::RESULT) {
            if (closeButton.getGlobalBounds().contains(x, y)) {
                window.close();
            }
        }
    }
    
    void showResult() {
        currentState = GameState::RESULT;
        if (taskPassed) {
            resultText.setString("🎉 TASK PASSED! 🎉");
            resultText.setFillColor(successColor);
            window.setTitle("Mission Accomplished!");
            instructionText.setString("Perfect! You selected Room 405 and turned OFF the AC.");
        } else {
            resultText.setString("❌ TASK FAILED! ❌");
            resultText.setFillColor(failColor);
            window.setTitle("Mission Failed!");
            if (selectedRoom != 405) {
                instructionText.setString("Wrong room! The correct room was 405.");
            } else {
                instructionText.setString("Wrong action! You should have turned OFF the AC.");
            }
        }
        instructionText.setCharacterSize(18);
        instructionText.setFillColor(sf::Color::White);
        instructionText.setPosition(200, 320);
    }
    
    void render() {
        window.clear(backgroundColor);
        
        switch (currentState) {
            case GameState::WELCOME:
                window.draw(titleText);
                window.draw(welcomeText);
                window.draw(instructionText);
                window.draw(startButton);
                window.draw(startButtonText);
                break;
            
            case GameState::ROOM_SELECTION: {
                titleText.setString("CSEDU's AC Server");
                titleText.setPosition(280, 50);
                window.draw(titleText);
                
                // Scoped instruction text
                {
                    sf::Text roomInstruction;
                    roomInstruction.setFont(font);
                    roomInstruction.setString("Select a room to control its AC:");
                    roomInstruction.setCharacterSize(22);
                    roomInstruction.setFillColor(sf::Color::White);
                    roomInstruction.setPosition(280, 120);
                    window.draw(roomInstruction);
                }
                
                for (size_t i = 0; i < roomButtons.size(); ++i) {
                    window.draw(roomButtons[i]);
                    window.draw(roomTexts[i]);
                }
                break;
            }
            
            case GameState::AC_CONTROL:
                window.draw(titleText);
                window.draw(selectedRoomText);
                window.draw(instructionText);
                window.draw(onButton);
                window.draw(onButtonText);
                window.draw(offButton);
                window.draw(offButtonText);
                break;
            
            case GameState::RESULT:
                window.draw(resultText);
                window.draw(instructionText);
                window.draw(closeButton);
                window.draw(closeButtonText);
                break;
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
    ACServerGame game;
    game.run();
    return 0;
}
