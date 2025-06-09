#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>

struct TeacherInfo {
    std::string name;
    std::string roomNumber;
    std::string submissionStatus;
    sf::Color statusColor;
};

enum class Screen {
    FILE_SELECTION,
    TEACHER_INFO_CHART,
    EMPTY_FILE
};

class TeacherInfoSystem {
private:
    sf::RenderWindow window;
    sf::Font font;
    Screen currentScreen;
    std::string selectedFile;
    
    // File Selection Screen Elements
    sf::RectangleShape background;
    sf::Text titleText;
    sf::RectangleShape deptInfoButton;
    sf::RectangleShape teacherInfoButton;
    sf::RectangleShape curriculumInfoButton;
    sf::Text deptInfoText;
    sf::Text teacherInfoText;
    sf::Text curriculumInfoText;
    sf::Text instructionText;
    
    // Teacher Info Chart Elements
    sf::RectangleShape chartBackground;
    sf::Text chartTitle;
    sf::Text backButtonText;
    sf::RectangleShape backButton;
    std::vector<TeacherInfo> teachers;
    std::vector<sf::Text> teacherTexts;
    std::vector<sf::Text> roomTexts;
    std::vector<sf::Text> statusTexts;
    sf::Text headerName, headerRoom, headerStatus;
    
    // Empty File Screen Elements
    sf::Text emptyFileText;
    sf::Text emptyBackText;
    sf::RectangleShape emptyBackButton;
    
public:
    TeacherInfoSystem() : window(sf::VideoMode(1000, 700), "Teacher Information System"), currentScreen(Screen::FILE_SELECTION) {
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load font file\n";
        }
        
        setupFileSelectionScreen();
        setupTeacherData();
        setupTeacherInfoChart();
        setupEmptyFileScreen();
    }
    
    void setupFileSelectionScreen() {
        // Background
        background.setSize(sf::Vector2f(1000, 700));
        background.setFillColor(sf::Color(25, 25, 35));
        
        // Title
        titleText.setFont(font);
        titleText.setString("TEACHER'S ROOM - FILE ACCESS");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(280, 100);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Select a file to access information:");
        instructionText.setCharacterSize(18);
        instructionText.setFillColor(sf::Color(200, 200, 200));
        instructionText.setPosition(350, 180);
        
        // File Buttons
        float buttonWidth = 200;
        float buttonHeight = 60;
        float startY = 250;
        float spacing = 100;
        
        // Dept Info Button
        deptInfoButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        deptInfoButton.setPosition(400, startY);
        deptInfoButton.setFillColor(sf::Color(70, 70, 100));
        deptInfoButton.setOutlineThickness(2);
        deptInfoButton.setOutlineColor(sf::Color::White);
        
        deptInfoText.setFont(font);
        deptInfoText.setString("Dept Info");
        deptInfoText.setCharacterSize(18);
        deptInfoText.setFillColor(sf::Color::White);
        deptInfoText.setPosition(460, startY + 20);
        
        // Teacher Info Button
        teacherInfoButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        teacherInfoButton.setPosition(400, startY + spacing);
        teacherInfoButton.setFillColor(sf::Color(70, 100, 70));
        teacherInfoButton.setOutlineThickness(2);
        teacherInfoButton.setOutlineColor(sf::Color::White);
        
        teacherInfoText.setFont(font);
        teacherInfoText.setString("Teacher's Info");
        teacherInfoText.setCharacterSize(18);
        teacherInfoText.setFillColor(sf::Color::White);
        teacherInfoText.setPosition(440, startY + spacing + 20);
        
        // Curriculum Info Button
        curriculumInfoButton.setSize(sf::Vector2f(buttonWidth, buttonHeight));
        curriculumInfoButton.setPosition(400, startY + spacing * 2);
        curriculumInfoButton.setFillColor(sf::Color(100, 70, 70));
        curriculumInfoButton.setOutlineThickness(2);
        curriculumInfoButton.setOutlineColor(sf::Color::White);
        
        curriculumInfoText.setFont(font);
        curriculumInfoText.setString("Curriculum Info");
        curriculumInfoText.setCharacterSize(18);
        curriculumInfoText.setFillColor(sf::Color::White);
        curriculumInfoText.setPosition(430, startY + spacing * 2 + 20);
    }
    
    void setupTeacherData() {
        teachers = {
            {"Sir1", "402", "Submitted", sf::Color::Green},
            {"Sir2", "301", "Not Submitted", sf::Color::Red},
            {"Sir3", "404", "Submitted", sf::Color::Green},
            {"Rana Sir", "405", "Not Submitted", sf::Color::Red},
            {"Sir4", "302", "Submitted", sf::Color::Green},
            {"Sir5", "403", "Not Submitted", sf::Color::Red},
            {"Sir6", "305", "Submitted", sf::Color::Green},
            {"Sir7", "401", "Not Submitted", sf::Color::Red},
            {"Sir8", "303", "Submitted", sf::Color::Green},
            {"Sir9", "304", "Not Submitted", sf::Color::Red}
        };
    }
    
    void setupTeacherInfoChart() {
        // Chart Background
        chartBackground.setSize(sf::Vector2f(900, 600));
        chartBackground.setPosition(50, 50);
        chartBackground.setFillColor(sf::Color(35, 35, 45));
        chartBackground.setOutlineThickness(3);
        chartBackground.setOutlineColor(sf::Color::Cyan);
        
        // Chart Title
        chartTitle.setFont(font);
        chartTitle.setString("TEACHER INFORMATION CHART");
        chartTitle.setCharacterSize(28);
        chartTitle.setFillColor(sf::Color::Cyan);
        chartTitle.setPosition(300, 70);
        
        // Back Button
        backButton.setSize(sf::Vector2f(100, 40));
        backButton.setPosition(80, 600);
        backButton.setFillColor(sf::Color(80, 80, 100));
        backButton.setOutlineThickness(2);
        backButton.setOutlineColor(sf::Color::White);
        
        backButtonText.setFont(font);
        backButtonText.setString("Back");
        backButtonText.setCharacterSize(16);
        backButtonText.setFillColor(sf::Color::White);
        backButtonText.setPosition(115, 615);
        
        // Headers
        headerName.setFont(font);
        headerName.setString("TEACHER NAME");
        headerName.setCharacterSize(18);
        headerName.setFillColor(sf::Color::Yellow);
        headerName.setPosition(100, 130);
        
        headerRoom.setFont(font);
        headerRoom.setString("ROOM NO");
        headerRoom.setCharacterSize(18);
        headerRoom.setFillColor(sf::Color::Yellow);
        headerRoom.setPosition(400, 130);
        
        headerStatus.setFont(font);
        headerStatus.setString("RESULT STATUS");
        headerStatus.setCharacterSize(18);
        headerStatus.setFillColor(sf::Color::Yellow);
        headerStatus.setPosition(600, 130);
        
        // Create text objects for each teacher
        teacherTexts.clear();
        roomTexts.clear();
        statusTexts.clear();
        
        for (int i = 0; i < teachers.size(); i++) {
            sf::Text nameText, roomText, statusText;
            
            nameText.setFont(font);
            nameText.setString(teachers[i].name);
            nameText.setCharacterSize(16);
            nameText.setFillColor(sf::Color::White);
            nameText.setPosition(100, 170 + i * 35);
            
            roomText.setFont(font);
            roomText.setString(teachers[i].roomNumber);
            roomText.setCharacterSize(16);
            roomText.setFillColor(sf::Color::White);
            roomText.setPosition(420, 170 + i * 35);
            
            statusText.setFont(font);
            statusText.setString(teachers[i].submissionStatus);
            statusText.setCharacterSize(16);
            statusText.setFillColor(teachers[i].statusColor);
            statusText.setPosition(620, 170 + i * 35);
            
            teacherTexts.push_back(nameText);
            roomTexts.push_back(roomText);
            statusTexts.push_back(statusText);
        }
    }
    
    void setupEmptyFileScreen() {
        emptyFileText.setFont(font);
        emptyFileText.setString("This file is currently empty.\nNo information available.");
        emptyFileText.setCharacterSize(24);
        emptyFileText.setFillColor(sf::Color::White);
        emptyFileText.setPosition(350, 300);
        
        emptyBackButton.setSize(sf::Vector2f(100, 40));
        emptyBackButton.setPosition(450, 400);
        emptyBackButton.setFillColor(sf::Color(80, 80, 100));
        emptyBackButton.setOutlineThickness(2);
        emptyBackButton.setOutlineColor(sf::Color::White);
        
        emptyBackText.setFont(font);
        emptyBackText.setString("Back");
        emptyBackText.setCharacterSize(16);
        emptyBackText.setFillColor(sf::Color::White);
        emptyBackText.setPosition(485, 415);
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
        }
    }
    
    void handleMouseClick(const sf::Vector2i& mousePos) {
        switch (currentScreen) {
            case Screen::FILE_SELECTION:
                if (teacherInfoButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentScreen = Screen::TEACHER_INFO_CHART;
                    selectedFile = "Teacher's Info";
                }
                else if (deptInfoButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentScreen = Screen::EMPTY_FILE;
                    selectedFile = "Dept Info";
                }
                else if (curriculumInfoButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentScreen = Screen::EMPTY_FILE;
                    selectedFile = "Curriculum Info";
                }
                break;
                
            case Screen::TEACHER_INFO_CHART:
                if (backButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentScreen = Screen::FILE_SELECTION;
                }
                break;
                
            case Screen::EMPTY_FILE:
                if (emptyBackButton.getGlobalBounds().contains(mousePos.x, mousePos.y)) {
                    currentScreen = Screen::FILE_SELECTION;
                }
                break;
        }
    }
    
    void render() {
        window.clear(sf::Color(20, 20, 30));
        
        switch (currentScreen) {
            case Screen::FILE_SELECTION:
                renderFileSelection();
                break;
            case Screen::TEACHER_INFO_CHART:
                renderTeacherChart();
                break;
            case Screen::EMPTY_FILE:
                renderEmptyFile();
                break;
        }
        
        window.display();
    }
    
    void renderFileSelection() {
        window.draw(background);
        window.draw(titleText);
        window.draw(instructionText);
        
        window.draw(deptInfoButton);
        window.draw(deptInfoText);
        
        window.draw(teacherInfoButton);
        window.draw(teacherInfoText);
        
        window.draw(curriculumInfoButton);
        window.draw(curriculumInfoText);
    }
    
    void renderTeacherChart() {
        window.draw(chartBackground);
        window.draw(chartTitle);
        window.draw(backButton);
        window.draw(backButtonText);
        
        window.draw(headerName);
        window.draw(headerRoom);
        window.draw(headerStatus);
        
        for (int i = 0; i < teachers.size(); i++) {
            window.draw(teacherTexts[i]);
            window.draw(roomTexts[i]);
            window.draw(statusTexts[i]);
        }
    }
    
    void renderEmptyFile() {
        window.draw(background);
        window.draw(emptyFileText);
        window.draw(emptyBackButton);
        window.draw(emptyBackText);
    }
    
    void run() {
        while (window.isOpen()) {
            handleEvents();
            render();
        }
    }
};

int main() {
    TeacherInfoSystem system;
    system.run();
    return 0;
}