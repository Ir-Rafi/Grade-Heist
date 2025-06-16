#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <sstream>

struct Room {
    int roomNumber;
    sf::RectangleShape roomPanel;
    sf::Text roomText;
    sf::RectangleShape offButton;
    sf::RectangleShape freezeButton;
    sf::Text offButtonText;
    sf::Text freezeButtonText;
    bool cctvOff;
    bool cctvFrozen;
    
    Room(int num) : roomNumber(num), cctvOff(false), cctvFrozen(false) {}
};

class CCTVManipulationTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // Game state
    bool showDisclaimer;
    bool taskStarted;
    bool taskCompleted;
    bool taskFailed;
    bool actionTaken;
    std::string failureReason;
    
    std::vector<Room> rooms;
    const int RANA_SIR_ROOM = 405;
    
    // UI elements
    sf::Text disclaimerTitle;
    sf::Text disclaimerText;
    sf::RectangleShape continueButton;
    sf::Text continueButtonText;
    
    sf::Text taskTitle;
    sf::Text instructionText;
    sf::Text resultText;
    sf::Text warningText;
    
    // Scroll functionality
    float scrollOffset;
    const float SCROLL_SPEED = 50.0f;
    
public:
    CCTVManipulationTask() : window(sf::VideoMode(1000, 700), "CCTV Manipulation Task"),
                             showDisclaimer(true), taskStarted(false), taskCompleted(false),
                             taskFailed(false), actionTaken(false), scrollOffset(0.0f) {
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf" << std::endl;
        }
        
        setupDisclaimer();
        setupRooms();
        setupUI();
    }
    
    void setupDisclaimer() {
        disclaimerTitle.setFont(font);
        disclaimerTitle.setCharacterSize(32);
        disclaimerTitle.setFillColor(sf::Color::Red);
        disclaimerTitle.setPosition(300, 100);
        disclaimerTitle.setString("⚠️ WARNING ⚠️");
        
        disclaimerText.setFont(font);
        disclaimerText.setCharacterSize(20);
        disclaimerText.setFillColor(sf::Color::Yellow);
        disclaimerText.setPosition(50, 200);
        std::string disclaimer = "THINK TWICE BEFORE CLICKING ANY SWITCH!\n\n"
                               "• You can choose ONLY ONE room\n"
                               "• You can select ONLY ONE option\n"
                               "• You got the room number from previous task"
                               "• Choose carefully - wrong choice means FAILURE!\n\n"
                               //"Mission: Freeze Rana Sir's room CCTV to avoid detection\n"
                              // "Rana Sir's Room: 405\n\n"
                               "Remember: If you  choose the wrong room or wrong option, you would might get caught!";
                               "REmemeber: Sir has the access of his room's CCTV and He is keeping his eye on it 24/7";
        disclaimerText.setString(disclaimer);
        continueButton.setSize(sf::Vector2f(200, 50));
        continueButton.setPosition(400, 550);
        continueButton.setFillColor(sf::Color::Green);
        continueButton.setOutlineThickness(2);
        continueButton.setOutlineColor(sf::Color::White);
        
        continueButtonText.setFont(font);
        continueButtonText.setCharacterSize(18);
        continueButtonText.setFillColor(sf::Color::White);
        continueButtonText.setPosition(450, 565);
        continueButtonText.setString("START MISSION");
    }
    
    void setupRooms() {
        for (int i = 401; i <= 420; i++) {
            rooms.push_back(Room(i));
        }
        
        // Setup room UI elements
        int roomsPerRow = 4;
        float startX = 50;
        float startY = 150;
        float roomWidth = 220;
        float roomHeight = 120;
        float spacing = 20;
        
        for (size_t i = 0; i < rooms.size(); i++) {
            int row = i / roomsPerRow;
            int col = i % roomsPerRow;
            
            float x = startX + col * (roomWidth + spacing);
            float y = startY + row * (roomHeight + spacing);
            
            Room& room = rooms[i];
            
            // Room panel
            room.roomPanel.setSize(sf::Vector2f(roomWidth, roomHeight));
            room.roomPanel.setPosition(x, y);
            room.roomPanel.setFillColor(sf::Color(40, 40, 60));
            room.roomPanel.setOutlineThickness(2);
            
            // Special highlighting for Rana sir's room
            if (room.roomNumber == RANA_SIR_ROOM) {
                room.roomPanel.setOutlineColor(sf::Color::Yellow);
            } else {
                room.roomPanel.setOutlineColor(sf::Color::White);
            }
            
            // Room number text
            room.roomText.setFont(font);
            room.roomText.setCharacterSize(18);
            room.roomText.setFillColor(sf::Color::White);
            room.roomText.setPosition(x + 10, y + 10);
            
            if (room.roomNumber == RANA_SIR_ROOM) {
                room.roomText.setString("Room " + std::to_string(room.roomNumber) + "\n(Rana Sir's Room)");
                room.roomText.setFillColor(sf::Color::Yellow);
            } else {
                room.roomText.setString("Room " + std::to_string(room.roomNumber));
            }
            
            // Off button
            room.offButton.setSize(sf::Vector2f(90, 30));
            room.offButton.setPosition(x + 10, y + 60);
            room.offButton.setFillColor(sf::Color::Red);
            room.offButton.setOutlineThickness(1);
            room.offButton.setOutlineColor(sf::Color::White);
            
            room.offButtonText.setFont(font);
            room.offButtonText.setCharacterSize(12);
            room.offButtonText.setFillColor(sf::Color::White);
            room.offButtonText.setPosition(x + 25, y + 70);
            room.offButtonText.setString("OFF CCTV");
            
            // Freeze button
            room.freezeButton.setSize(sf::Vector2f(90, 30));
            room.freezeButton.setPosition(x + 110, y + 60);
            room.freezeButton.setFillColor(sf::Color::Blue);
            room.freezeButton.setOutlineThickness(1);
            room.freezeButton.setOutlineColor(sf::Color::White);
            
            room.freezeButtonText.setFont(font);
            room.freezeButtonText.setCharacterSize(12);
            room.freezeButtonText.setFillColor(sf::Color::White);
            room.freezeButtonText.setPosition(x + 115, y + 70);
            room.freezeButtonText.setString("FREEZE CCTV");
        }
    }
    
    void setupUI() {
        taskTitle.setFont(font);
        taskTitle.setCharacterSize(28);
        taskTitle.setFillColor(sf::Color::White);
        taskTitle.setPosition(350, 20);
        taskTitle.setString("CCTV Manipulation");
        
        instructionText.setFont(font);
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::Cyan);
        instructionText.setPosition(50, 80);
        instructionText.setString("Select Rana Sir's room (405) and FREEZE its CCTV to avoid detection!");
        
        warningText.setFont(font);
        warningText.setCharacterSize(14);
        warningText.setFillColor(sf::Color::Red);
        warningText.setPosition(50, 110);
        warningText.setString("⚠️ ONE CHOICE ONLY - Choose wisely!");
        
        resultText.setFont(font);
        resultText.setCharacterSize(20);
        resultText.setPosition(50, 650);
    }
    
    void handleEvents() {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            if (event.type == sf::Event::MouseButtonPressed) {
                sf::Vector2i mousePos = sf::Mouse::getPosition(window);
                
                if (showDisclaimer) {
                    // Check continue button
                    if (continueButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                        showDisclaimer = false;
                        taskStarted = true;
                    }
                } else if (taskStarted && !actionTaken) {
                    // Check room buttons
                    for (auto& room : rooms) {
                        // Adjust for scroll offset
                        sf::Vector2f adjustedMousePos(mousePos.x, mousePos.y + scrollOffset);
                        
                        if (room.offButton.getGlobalBounds().contains(adjustedMousePos.x, adjustedMousePos.y)) {
                            handleRoomAction(room.roomNumber, false); // Off CCTV
                            break;
                        }
                        
                        if (room.freezeButton.getGlobalBounds().contains(adjustedMousePos.x, adjustedMousePos.y)) {
                            handleRoomAction(room.roomNumber, true); // Freeze CCTV
                            break;
                        }
                    }
                }
            }
            
            // Scroll functionality
            if (event.type == sf::Event::MouseWheelScrolled && taskStarted) {
                scrollOffset -= event.mouseWheelScroll.delta * SCROLL_SPEED;
                if (scrollOffset < 0) scrollOffset = 0;
                if (scrollOffset > 300) scrollOffset = 300; // Max scroll
            }
            
            // Restart with R key
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R) {
                if (taskCompleted || taskFailed) {
                    resetTask();
                }
            }
        }
    }
    
    void handleRoomAction(int roomNumber, bool freeze) {
        actionTaken = true;
        
        if (roomNumber == RANA_SIR_ROOM && freeze) {
            // Correct choice - freeze Rana sir's room
            taskCompleted = true;
            resultText.setString("SUCCESS! Rana Sir's CCTV frozen - You're safe to enter!");
            resultText.setFillColor(sf::Color::Green);
            
            // Update room visual
            for (auto& room : rooms) {
                if (room.roomNumber == RANA_SIR_ROOM) {
                    room.cctvFrozen = true;
                    room.freezeButton.setFillColor(sf::Color::Green);
                    room.roomPanel.setFillColor(sf::Color(0, 60, 0));
                    break;
                }
            }
        } else {
            // Wrong choice
            taskFailed = true;
            
            if (roomNumber != RANA_SIR_ROOM) {
                failureReason = "WRONG ROOM! You selected room " + std::to_string(roomNumber) + 
                               " instead of Rana Sir's room (405)";
            } else if (!freeze) {
                failureReason = "WRONG ACTION! You turned OFF the CCTV instead of FREEZING it!\n" +
                               std::string("Rana Sir noticed the camera went black and got suspicious!");
            }
            
            resultText.setString("MISSION FAILED!\n" + failureReason + "\nYou've been caught!");
            resultText.setFillColor(sf::Color::Red);
            
            // Update room visual for wrong choice
            for (auto& room : rooms) {
                if (room.roomNumber == roomNumber) {
                    room.roomPanel.setFillColor(sf::Color(60, 0, 0));
                    if (freeze) {
                        room.cctvFrozen = true;
                        room.freezeButton.setFillColor(sf::Color::Red);
                    } else {
                        room.cctvOff = true;
                        room.offButton.setFillColor(sf::Color::Red);
                    }
                    break;
                }
            }
        }
    }
    
    void resetTask() {
        showDisclaimer = true;
        taskStarted = false;
        taskCompleted = false;
        taskFailed = false;
        actionTaken = false;
        scrollOffset = 0.0f;
        failureReason = "";
        
        // Reset all rooms
        for (auto& room : rooms) {
            room.cctvOff = false;
            room.cctvFrozen = false;
            room.roomPanel.setFillColor(sf::Color(40, 40, 60));
            room.offButton.setFillColor(sf::Color::Red);
            room.freezeButton.setFillColor(sf::Color::Blue);
        }
        
        resultText.setString("");
    }
    
    void render() {
        window.clear(sf::Color(20, 20, 30));
        
        if (showDisclaimer) {
            // Draw disclaimer screen
            window.draw(disclaimerTitle);
            window.draw(disclaimerText);
            window.draw(continueButton);
            window.draw(continueButtonText);
        } else {
            // Draw main task interface
            window.draw(taskTitle);
            window.draw(instructionText);
            window.draw(warningText);
            
            // Create scrollable view for rooms
            sf::View originalView = window.getView();
            sf::View scrollView = originalView;
            scrollView.move(0, -scrollOffset);
            window.setView(scrollView);
            
            // Draw rooms
            for (const auto& room : rooms) {
                window.draw(room.roomPanel);
                window.draw(room.roomText);
                window.draw(room.offButton);
                window.draw(room.freezeButton);
                window.draw(room.offButtonText);
                window.draw(room.freezeButtonText);
            }
            
            // Reset view for UI elements
            window.setView(originalView);
            window.draw(resultText);
            
            // Draw restart instruction
            if (taskCompleted || taskFailed) {
                sf::Text restartText;
                restartText.setFont(font);
                restartText.setCharacterSize(16);
                restartText.setFillColor(sf::Color::White);
                restartText.setPosition(400, 620);
                restartText.setString("Press R to restart");
                window.draw(restartText);
            }
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
    CCTVManipulationTask game;
    game.run();
    
    return 0;
}