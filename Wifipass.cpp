#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <algorithm>
#include <cstdio>

enum class Screen {
    WELCOME,
    MAIN_MENU,
    TEACHERS_INFO,
    DEPT_INFO,
    PASSWORD_ENTRY,
    HINTS_WINDOW,
    OTHERS_INFO,
    TASK_FAILED
};

class CSEDUWebsite {
private:
    sf::RenderWindow window;
    sf::Font font;
    Screen currentScreen;

    // Random number generation
    std::random_device rd;
    std::mt19937 gen;
    int invalidTeacherIndex;
    std::string correctPassword;

    // Overall timer for the entire task (2 minutes)
    sf::Clock overallTimer;
    float overallTimeLimit;
    bool taskStarted;

    // Timer for password entry
    sf::Clock passwordTimer;
    bool timerStarted;
    float timeLimit;

    // Password input
    std::string userInput;
    bool passwordCorrect;
    bool passwordFailed;

    // Task failed screen timer
    sf::Clock taskFailedDisplayTimer;
    bool taskFailedShown;

    // Welcome Screen Elements
    sf::Text welcomeText;
    sf::RectangleShape clickButton;
    sf::Text clickButtonText;
    sf::RectangleShape exitButton;
    sf::Text exitButtonText;

    // Main Menu Elements
    sf::Text mainTitle;
    sf::RectangleShape teachersButton;
    sf::RectangleShape deptButton;
    sf::RectangleShape othersButton;
    sf::Text teachersButtonText;
    sf::Text deptButtonText;
    sf::Text othersButtonText;
    sf::RectangleShape backButton;
    sf::Text backButtonText;
    sf::Text overallTimerText;

    // Teachers Info Elements
    sf::Text teachersTitle;
    sf::Text teachersListTitle;
    std::vector<sf::Text> teacherNames;
    std::vector<std::string> phoneNumbers;
    sf::RectangleShape teachersBackButton;
    sf::Text teachersBackText;

    // Dept Info Elements
    sf::Text deptTitle;
    sf::Text deptHistoryTitle;
    sf::Text deptContent;
    sf::RectangleShape deptBackButton;
    sf::Text deptBackText;

    // Password Entry Elements
    sf::Text passwordTitle;
    sf::Text passwordPrompt;
    sf::RectangleShape passwordInputBox;
    sf::Text passwordInputText;
    sf::Text timerText;
    sf::Text statusText;
    sf::RectangleShape passwordBackButton;
    sf::Text passwordBackText;
    sf::RectangleShape hintsButton;
    sf::Text hintsButtonText;

    // Hints Window Elements
    sf::Text hintsTitle;
    sf::Text hint1Text;
    sf::Text hint2Text;
    sf::RectangleShape hintsBackButton;
    sf::Text hintsBackText;

    // Others Info Elements (WiFi Password)
    sf::Text othersTitle;
    sf::RectangleShape wifiBox;
    sf::Text wifiTitle;
    sf::Text usernameText;
    sf::Text passwordText;
    sf::RectangleShape othersBackButton;
    sf::Text othersBackText;

    // Task Failed Elements
    sf::Text taskFailedTitle;
    sf::Text taskFailedMessage;
    sf::Text taskFailedCountdown;

public:
    CSEDUWebsite() : window(sf::VideoMode(1200, 800), "CSEDU's Private Info Website"),
                     currentScreen(Screen::WELCOME), gen(rd()), timerStarted(false),
                     timeLimit(60.0f), passwordCorrect(false), passwordFailed(false),
                     overallTimeLimit(120.0f), taskStarted(false), taskFailedShown(false) {
        
        // Try to load font with fallback options
        loadFont();

        generateRandomTeacherData();
        setupWelcomeScreen();
        setupMainMenu();
        setupTeachersInfo();
        setupDeptInfo();
        setupPasswordEntry();
        setupHintsWindow();
        setupOthersInfo();
        setupTaskFailedScreen();
    }

    bool loadFont() {
        // Try multiple font paths common on Linux systems
        std::vector<std::string> fontPaths = {
            "arial.ttf",
            "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
            "/usr/share/fonts/TTF/DejaVuSans.ttf",
            "/usr/share/fonts/truetype/ubuntu/Ubuntu-R.ttf",
            "/System/Library/Fonts/Arial.ttf",  // macOS
            "C:\\Windows\\Fonts\\arial.ttf"     // Windows
        };

        for (const auto& path : fontPaths) {
            if (font.loadFromFile(path)) {
                return true;
            }
        }
        return false;
    }

    void generateRandomTeacherData() {
        std::uniform_int_distribution<> dis(0, 29);
        invalidTeacherIndex = dis(gen);

        phoneNumbers.clear();
        std::uniform_int_distribution<> digitDis(0, 9);

        for (int i = 0; i < 30; i++) {
            std::string phoneNumber = "01";
            if (i == invalidTeacherIndex) {
                // Generate 11-digit number (invalid)
                for (int j = 0; j < 9; j++) {
                    phoneNumber += std::to_string(digitDis(gen));
                }
                // Get last 3 digits and reverse them
                std::string lastThree = phoneNumber.substr(phoneNumber.length() - 3);
                std::reverse(lastThree.begin(), lastThree.end());
                correctPassword = lastThree + "32"; // 32 years old (from dept info)
            } else {
                // Generate normal 10-digit number
                for (int j = 0; j < 8; j++) {
                    phoneNumber += std::to_string(digitDis(gen));
                }
            }
            phoneNumbers.push_back(phoneNumber);
        }
    }

    void setupWelcomeScreen() {
        welcomeText.setFont(font);
        welcomeText.setString("Welcome to CSEDU Private Info");
        welcomeText.setCharacterSize(32);
        welcomeText.setFillColor(sf::Color::White);
        welcomeText.setPosition(350, 300);

        clickButton.setSize(sf::Vector2f(150, 50));
        clickButton.setPosition(525, 400);
        clickButton.setFillColor(sf::Color(70, 130, 180));
        clickButton.setOutlineThickness(2);
        clickButton.setOutlineColor(sf::Color::White);

        clickButtonText.setFont(font);
        clickButtonText.setString("Enter Site");
        clickButtonText.setCharacterSize(18);
        clickButtonText.setFillColor(sf::Color::White);
        clickButtonText.setPosition(560, 420);

        exitButton.setSize(sf::Vector2f(150, 50));
        exitButton.setPosition(525, 470);
        exitButton.setFillColor(sf::Color(150, 50, 50));
        exitButton.setOutlineThickness(2);
        exitButton.setOutlineColor(sf::Color::White);

        exitButtonText.setFont(font);
        exitButtonText.setString("Exit");
        exitButtonText.setCharacterSize(18);
        exitButtonText.setFillColor(sf::Color::White);
        exitButtonText.setPosition(575, 490);
    }

    void setupMainMenu() {
        mainTitle.setFont(font);
        mainTitle.setString("CSEDU's Private Info");
        mainTitle.setCharacterSize(36);
        mainTitle.setFillColor(sf::Color::Cyan);
        mainTitle.setPosition(420, 100);

        overallTimerText.setFont(font);
        overallTimerText.setString("Overall Time Remaining: 2:00");
        overallTimerText.setCharacterSize(20);
        overallTimerText.setFillColor(sf::Color::Green);
        overallTimerText.setPosition(850, 50);

        teachersButton.setSize(sf::Vector2f(250, 60));
        teachersButton.setPosition(475, 220);
        teachersButton.setFillColor(sf::Color(100, 70, 130));
        teachersButton.setOutlineThickness(2);
        teachersButton.setOutlineColor(sf::Color::White);

        teachersButtonText.setFont(font);
        teachersButtonText.setString("1. Teachers Info");
        teachersButtonText.setCharacterSize(20);
        teachersButtonText.setFillColor(sf::Color::White);
        teachersButtonText.setPosition(540, 245);

        deptButton.setSize(sf::Vector2f(250, 60));
        deptButton.setPosition(475, 320);
        deptButton.setFillColor(sf::Color(70, 130, 100));
        deptButton.setOutlineThickness(2);
        deptButton.setOutlineColor(sf::Color::White);

        deptButtonText.setFont(font);
        deptButtonText.setString("2. Dept Info");
        deptButtonText.setCharacterSize(20);
        deptButtonText.setFillColor(sf::Color::White);
        deptButtonText.setPosition(555, 345);

        othersButton.setSize(sf::Vector2f(300, 60));
        othersButton.setPosition(450, 420);
        othersButton.setFillColor(sf::Color(130, 100, 70));
        othersButton.setOutlineThickness(2);
        othersButton.setOutlineColor(sf::Color::White);

        othersButtonText.setFont(font);
        othersButtonText.setString("3. Others (Lab, WiFi, Facilities)");
        othersButtonText.setCharacterSize(18);
        othersButtonText.setFillColor(sf::Color::White);
        othersButtonText.setPosition(480, 445);

        backButton.setSize(sf::Vector2f(100, 40));
        backButton.setPosition(50, 650);
        backButton.setFillColor(sf::Color(80, 80, 100));
        backButton.setOutlineThickness(2);
        backButton.setOutlineColor(sf::Color::White);

        backButtonText.setFont(font);
        backButtonText.setString("Back");
        backButtonText.setCharacterSize(16);
        backButtonText.setFillColor(sf::Color::White);
        backButtonText.setPosition(85, 665);
    }

    void setupTeachersInfo() {
        teachersTitle.setFont(font);
        teachersTitle.setString("Teachers Information");
        teachersTitle.setCharacterSize(32);
        teachersTitle.setFillColor(sf::Color::Yellow);
        teachersTitle.setPosition(450, 50);

        teachersListTitle.setFont(font);
        teachersListTitle.setString("List of Faculty Members (30 Teachers):");
        teachersListTitle.setCharacterSize(20);
        teachersListTitle.setFillColor(sf::Color::White);
        teachersListTitle.setPosition(50, 120);

        std::vector<std::string> teacherNamesList = {
            "Dr. Rashida Ahmed", "Prof. Karim Rahman", "Dr. Fatima Khan", "Prof. Alamgir Hossain",
            "Dr. Nasir Uddin", "Prof. Ruma Begum", "Dr. Mizanur Rahman", "Prof. Salma Khatun",
            "Dr. Tariq Islam", "Prof. Shahida Parveen", "Dr. Rafiqul Alam", "Prof. Nargis Sultana",
            "Dr. Habibur Rahman", "Prof. Farida Yasmin", "Dr. Aminul Haque", "Prof. Rokeya Begum",
            "Dr. Shamsul Huda", "Prof. Maksuda Khatun", "Dr. Jamal Uddin", "Prof. Rashida Sultana",
            "Dr. Nazrul Islam", "Prof. Dilruba Yasmin", "Dr. Mofizul Rahman", "Prof. Rahima Begum",
            "Dr. Golam Kibria", "Prof. Hosne Ara", "Dr. Rezaul Karim", "Prof. Shireen Akhter",
            "Dr. Anisul Haque", "Prof. Mahmuda Khatun"
        };

        teacherNames.clear();
        for (int i = 0; i < 30; i++) {
            sf::Text nameText;
            nameText.setFont(font);

            std::string displayText = std::to_string(i + 1) + ". " + teacherNamesList[i] + " - " + phoneNumbers[i];
            nameText.setString(displayText);
            nameText.setCharacterSize(14);
            nameText.setFillColor(sf::Color::White);

            int col = i / 15;
            int row = i % 15;
            nameText.setPosition(50 + col * 500, 160 + row * 30);

            teacherNames.push_back(nameText);
        }

        teachersBackButton.setSize(sf::Vector2f(100, 40));
        teachersBackButton.setPosition(50, 650);
        teachersBackButton.setFillColor(sf::Color(80, 80, 100));
        teachersBackButton.setOutlineThickness(2);
        teachersBackButton.setOutlineColor(sf::Color::White);

        teachersBackText.setFont(font);
        teachersBackText.setString("Back");
        teachersBackText.setCharacterSize(16);
        teachersBackText.setFillColor(sf::Color::White);
        teachersBackText.setPosition(85, 665);
    }

    void setupDeptInfo() {
        deptTitle.setFont(font);
        deptTitle.setString("Department Information");
        deptTitle.setCharacterSize(32);
        deptTitle.setFillColor(sf::Color::Green);
        deptTitle.setPosition(420, 50);

        deptHistoryTitle.setFont(font);
        deptHistoryTitle.setString("CSEDU's History");
        deptHistoryTitle.setCharacterSize(28);
        deptHistoryTitle.setFillColor(sf::Color::Yellow);
        deptHistoryTitle.setPosition(480, 120);

        deptContent.setFont(font);
        deptContent.setString(
            "The Department of Computer Science and Engineering at the University of Dhaka\n"
            "was established in 1992 with the vision of becoming a leading center for\n"
            "computer science education and research in Bangladesh.\n\n"
            "Founded 32 years ago, the department started with just 50 students\n"
            "and has now grown to accommodate over 800 students across different levels.\n\n"
            "The department has grown significantly over the years, now offering\n"
            "undergraduate and graduate programs in various areas of computer science\n"
            "and engineering, including software engineering, artificial intelligence,\n"
            "computer networks, and database systems.\n\n"
            "Key milestone: Established in '92, celebrating 32 years of excellence!"
        );
        deptContent.setCharacterSize(16);
        deptContent.setFillColor(sf::Color::White);
        deptContent.setPosition(100, 180);

        deptBackButton.setSize(sf::Vector2f(100, 40));
        deptBackButton.setPosition(50, 650);
        deptBackButton.setFillColor(sf::Color(80, 80, 100));
        deptBackButton.setOutlineThickness(2);
        deptBackButton.setOutlineColor(sf::Color::White);

        deptBackText.setFont(font);
        deptBackText.setString("Back");
        deptBackText.setCharacterSize(16);
        deptBackText.setFillColor(sf::Color::White);
        deptBackText.setPosition(85, 665);
    }

    void setupPasswordEntry() {
        passwordTitle.setFont(font);
        passwordTitle.setString("WiFi Access - Password Required");
        passwordTitle.setCharacterSize(28);
        passwordTitle.setFillColor(sf::Color::Red);
        passwordTitle.setPosition(400, 100);

        passwordPrompt.setFont(font);
        passwordPrompt.setString("Enter the password to access WiFi credentials:");
        passwordPrompt.setCharacterSize(18);
        passwordPrompt.setFillColor(sf::Color::White);
        passwordPrompt.setPosition(350, 200);

        passwordInputBox.setSize(sf::Vector2f(300, 40));
        passwordInputBox.setPosition(450, 250);
        passwordInputBox.setFillColor(sf::Color(40, 40, 60));
        passwordInputBox.setOutlineThickness(2);
        passwordInputBox.setOutlineColor(sf::Color::White);

        passwordInputText.setFont(font);
        passwordInputText.setString("");
        passwordInputText.setCharacterSize(18);
        passwordInputText.setFillColor(sf::Color::White);
        passwordInputText.setPosition(460, 265);

       /*timerText.setFont(font);
        timerText.setString("Time remaining: 60s");
        timerText.setCharacterSize(20);
        timerText.setFillColor(sf::Color::Yellow);
        timerText.setPosition(500, 320);*/

        statusText.setFont(font);
        statusText.setString("Need hints? Click the Hints button!");
        statusText.setCharacterSize(16);
        statusText.setFillColor(sf::Color::Cyan);
        statusText.setPosition(450, 360);

        passwordBackButton.setSize(sf::Vector2f(100, 40));
        passwordBackButton.setPosition(50, 650);
        passwordBackButton.setFillColor(sf::Color(80, 80, 100));
        passwordBackButton.setOutlineThickness(2);
        passwordBackButton.setOutlineColor(sf::Color::White);

        passwordBackText.setFont(font);
        passwordBackText.setString("Back");
        passwordBackText.setCharacterSize(16);
        passwordBackText.setFillColor(sf::Color::White);
        passwordBackText.setPosition(85, 665);

        hintsButton.setSize(sf::Vector2f(100, 40));
        hintsButton.setPosition(200, 650);
        hintsButton.setFillColor(sf::Color(70, 130, 180));
        hintsButton.setOutlineThickness(2);
        hintsButton.setOutlineColor(sf::Color::White);

        hintsButtonText.setFont(font);
        hintsButtonText.setString("Hints");
        hintsButtonText.setCharacterSize(16);
        hintsButtonText.setFillColor(sf::Color::White);
        hintsButtonText.setPosition(235, 665);
    }

    void setupHintsWindow() {
        hintsTitle.setFont(font);
        hintsTitle.setString("Password Hints");
        hintsTitle.setCharacterSize(32);
        hintsTitle.setFillColor(sf::Color::Yellow);
        hintsTitle.setPosition(500, 100);

        hint1Text.setFont(font);
        hint1Text.setString("1. Find the invalid phone number (11 digits instead of 10)\n   Take the last 3 digits and reverse them");
        hint1Text.setCharacterSize(18);
        hint1Text.setFillColor(sf::Color::White);
        hint1Text.setPosition(100, 200);

        hint2Text.setFont(font);
        hint2Text.setString("2. Add the department's age in years\n   (Check Dept Info for how old CSEDU is)");
        hint2Text.setCharacterSize(18);
        hint2Text.setFillColor(sf::Color::White);
        hint2Text.setPosition(100, 300);

        hintsBackButton.setSize(sf::Vector2f(100, 40));
        hintsBackButton.setPosition(50, 650);
        hintsBackButton.setFillColor(sf::Color(80, 80, 100));
        hintsBackButton.setOutlineThickness(2);
        hintsBackButton.setOutlineColor(sf::Color::White);

        hintsBackText.setFont(font);
        hintsBackText.setString("Back");
        hintsBackText.setCharacterSize(16);
        hintsBackText.setFillColor(sf::Color::White);
        hintsBackText.setPosition(85, 665);
    }

    void setupOthersInfo() {
        othersTitle.setFont(font);
        othersTitle.setString("WiFi & Lab Information");
        othersTitle.setCharacterSize(32);
        othersTitle.setFillColor(sf::Color::Magenta);
        othersTitle.setPosition(470, 50);

        wifiBox.setSize(sf::Vector2f(400, 200));
        wifiBox.setPosition(400, 200);
        wifiBox.setFillColor(sf::Color(40, 40, 60));
        wifiBox.setOutlineThickness(3);
        wifiBox.setOutlineColor(sf::Color::Cyan);

        wifiTitle.setFont(font);
        wifiTitle.setString("WiFi Access Credentials");
        wifiTitle.setCharacterSize(24);
        wifiTitle.setFillColor(sf::Color::Cyan);
        wifiTitle.setPosition(450, 220);

        usernameText.setFont(font);
        usernameText.setString("Network: CSEDU_Faculty");
        usernameText.setCharacterSize(20);
        usernameText.setFillColor(sf::Color::White);
        usernameText.setPosition(450, 280);

        passwordText.setFont(font);
        passwordText.setString("Password: CSEDU2024_Secure!");
        passwordText.setCharacterSize(20);
        passwordText.setFillColor(sf::Color::Yellow);
        passwordText.setPosition(450, 320);

        othersBackButton.setSize(sf::Vector2f(100, 40));
        othersBackButton.setPosition(50, 650);
        othersBackButton.setFillColor(sf::Color(80, 80, 100));
        othersBackButton.setOutlineThickness(2);
        othersBackButton.setOutlineColor(sf::Color::White);

        othersBackText.setFont(font);
        othersBackText.setString("Back");
        othersBackText.setCharacterSize(16);
        othersBackText.setFillColor(sf::Color::White);
        othersBackText.setPosition(85, 665);
    }

    void setupTaskFailedScreen() {
        taskFailedTitle.setFont(font);
        taskFailedTitle.setString("TASK FAILED");
        taskFailedTitle.setCharacterSize(64);
        taskFailedTitle.setFillColor(sf::Color::Red);
        taskFailedTitle.setPosition(450, 300);

        taskFailedMessage.setFont(font);
        taskFailedMessage.setString("Time's up! You couldn't complete the task in 2 minutes.");
        taskFailedMessage.setCharacterSize(24);
        taskFailedMessage.setFillColor(sf::Color::White);
        taskFailedMessage.setPosition(300, 400);

        taskFailedCountdown.setFont(font);
        taskFailedCountdown.setString("Closing in 3 seconds...");
        taskFailedCountdown.setCharacterSize(20);
        taskFailedCountdown.setFillColor(sf::Color::Yellow);
        taskFailedCountdown.setPosition(500, 450);
    }

    void resetPasswordEntry() {
        userInput.clear();
        passwordInputText.setString("");
        passwordCorrect = false;
        passwordFailed = false;
        timerStarted = true;
        passwordTimer.restart();
        statusText.setString("Need hints? Click the Hints button!");
        statusText.setFillColor(sf::Color::Cyan);
    }

    // Fix for the updateOverallTimer() function
    void updateOverallTimer() {
        if (taskStarted && currentScreen != Screen::TASK_FAILED) {
            float elapsed = overallTimer.getElapsedTime().asSeconds();
            float remaining = overallTimeLimit - elapsed;
            
            if (remaining <= 0) {
                // Time's up - show task failed screen
                currentScreen = Screen::TASK_FAILED;
                taskFailedShown = true;
                taskFailedDisplayTimer.restart();
            } else {
                int minutes = static_cast<int>(remaining) / 60;
                int seconds = static_cast<int>(remaining) % 60;
                
                // FIXED: Increased buffer size from 32 to 64 to avoid overflow
                char timeStr[64];  // Changed from 128 to 64 (sufficient for the string)
                std::sprintf(timeStr, "Overall Time Remaining: %d:%02d", minutes, seconds);
                overallTimerText.setString(timeStr);
                
                // Alternative solution using std::string (more C++ style):
                // std::string timeStr = "Overall Time Remaining: " + 
                //                      std::to_string(minutes) + ":" + 
                //                      (seconds < 10 ? "0" : "") + std::to_string(seconds);
                // overallTimerText.setString(timeStr);
                
                // Change color as time runs out
                if (remaining < 30) {
                    overallTimerText.setFillColor(sf::Color::Red);
                } else if (remaining < 60) {
                    overallTimerText.setFillColor(sf::Color::Yellow);
                } else {
                    overallTimerText.setFillColor(sf::Color::Green);
                }
            }
        }
    }
    void updateTimer() {
        if (timerStarted && !passwordCorrect && !passwordFailed) {
            float elapsed = passwordTimer.getElapsedTime().asSeconds();
            float remaining = timeLimit - elapsed;
            
            if (remaining <= 0) {
                // Password entry time's up - but overall timer still controls main closure
                timerText.setString("Password time expired!");
                timerText.setFillColor(sf::Color::Red);
                timerStarted = false;
            } else {
                timerText.setString("Time remaining: " + std::to_string(static_cast<int>(remaining)) + "s");
            }
        }
    }

    void updateTaskFailedScreen() {
        if (taskFailedShown) {
            float elapsed = taskFailedDisplayTimer.getElapsedTime().asSeconds();
            int remaining = 3 - static_cast<int>(elapsed);
            
            if (remaining <= 0) {
                // Close the window after 3 seconds
                window.close();
            } else {
                taskFailedCountdown.setString("Closing in " + std::to_string(remaining) + " seconds...");
            }
        }
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

            if (currentScreen == Screen::PASSWORD_ENTRY && event.type == sf::Event::TextEntered) {
                if (timerStarted && !passwordCorrect && !passwordFailed) {
                    if (event.text.unicode == 8) { // Backspace
                        if (!userInput.empty()) {
                            userInput.pop_back();
                            passwordInputText.setString(userInput);
                        }
                    } else if (event.text.unicode == 13) { // Enter
                        checkPassword();
                    } else if (event.text.unicode < 128 && userInput.length() < 20) {
                        userInput += static_cast<char>(event.text.unicode);
                        passwordInputText.setString(userInput);
                    }
                }
            }
        }
    }

    void checkPassword() {
        if (userInput == correctPassword) {
            passwordCorrect = true;
            statusText.setString("Correct! Accessing WiFi credentials...");
            statusText.setFillColor(sf::Color::Green);
            // Automatically switch to Others Info after 1 second
            sf::sleep(sf::milliseconds(1000));
            currentScreen = Screen::OTHERS_INFO;
        } else {
            passwordFailed = true;
            statusText.setString("Wrong password! Try again.");
            statusText.setFillColor(sf::Color::Red);
            userInput.clear();
            passwordInputText.setString("");
            // Reset after 2 seconds
            sf::sleep(sf::milliseconds(2000));
            passwordFailed = false;
            timerStarted = true;
            passwordTimer.restart();
            statusText.setString("Need hints? Click the Hints button!");
            statusText.setFillColor(sf::Color::Cyan);
        }
    }

    void handleMouseClick(const sf::Vector2i& mousePos) {
        float x = static_cast<float>(mousePos.x);
        float y = static_cast<float>(mousePos.y);

        if (currentScreen == Screen::WELCOME) {
            if (clickButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::MAIN_MENU;
                taskStarted = true;
                overallTimer.restart();
            } else if (exitButton.getGlobalBounds().contains(x, y)) {
                window.close();
            }
        }
        else if (currentScreen == Screen::MAIN_MENU) {
            if (backButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::WELCOME;
                taskStarted = false;
            } else if (teachersButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::TEACHERS_INFO;
            } else if (deptButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::DEPT_INFO;
            } else if (othersButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::PASSWORD_ENTRY;
                resetPasswordEntry();
            }
        }
        else if (currentScreen == Screen::TEACHERS_INFO) {
            if (teachersBackButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::MAIN_MENU;
            }
        }
        else if (currentScreen == Screen::DEPT_INFO) {
            if (deptBackButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::MAIN_MENU;
            }
        }else if (currentScreen == Screen::PASSWORD_ENTRY) {
        if (passwordBackButton.getGlobalBounds().contains(x, y)) {
            currentScreen = Screen::MAIN_MENU;
        } else if (hintsButton.getGlobalBounds().contains(x, y)) {
            currentScreen = Screen::HINTS_WINDOW;
        }
    }
    else if (currentScreen == Screen::HINTS_WINDOW) {
            if (hintsBackButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::PASSWORD_ENTRY;
            }
        }
        else if (currentScreen == Screen::OTHERS_INFO) {
            if (othersBackButton.getGlobalBounds().contains(x, y)) {
                currentScreen = Screen::MAIN_MENU;
            }
        }
    }

    void render() {
        window.clear(sf::Color::Black);

        switch (currentScreen) {
            case Screen::WELCOME:
                renderWelcomeScreen();
                break;
            case Screen::MAIN_MENU:
                renderMainMenu();
                break;
            case Screen::TEACHERS_INFO:
                renderTeachersInfo();
                break;
            case Screen::DEPT_INFO:
                renderDeptInfo();
                break;
            case Screen::PASSWORD_ENTRY:
                renderPasswordEntry();
                break;
            case Screen::HINTS_WINDOW:
                renderHintsWindow();
                break;
            case Screen::OTHERS_INFO:
                renderOthersInfo();
                break;
            case Screen::TASK_FAILED:
                renderTaskFailedScreen();
                break;
        }

        window.display();
    }

    void renderWelcomeScreen() {
        window.draw(welcomeText);
        window.draw(clickButton);
        window.draw(clickButtonText);
        window.draw(exitButton);
        window.draw(exitButtonText);
    }

    void renderMainMenu() {
        window.draw(mainTitle);
        window.draw(overallTimerText);
        window.draw(teachersButton);
        window.draw(teachersButtonText);
        window.draw(deptButton);
        window.draw(deptButtonText);
        window.draw(othersButton);
        window.draw(othersButtonText);
        window.draw(backButton);
        window.draw(backButtonText);
    }

    void renderTeachersInfo() {
        window.draw(teachersTitle);
        window.draw(teachersListTitle);
        
        for (const auto& teacherName : teacherNames) {
            window.draw(teacherName);
        }
        
        window.draw(teachersBackButton);
        window.draw(teachersBackText);
    }

    void renderDeptInfo() {
        window.draw(deptTitle);
        window.draw(deptHistoryTitle);
        window.draw(deptContent);
        window.draw(deptBackButton);
        window.draw(deptBackText);
    }

    void renderPasswordEntry() {
        window.draw(passwordTitle);
        window.draw(passwordPrompt);
        window.draw(passwordInputBox);
        window.draw(passwordInputText);
        window.draw(statusText);
        window.draw(passwordBackButton);
        window.draw(passwordBackText);
        window.draw(hintsButton);
        window.draw(hintsButtonText);
    }

    void renderHintsWindow() {
        window.draw(hintsTitle);
        window.draw(hint1Text);
        window.draw(hint2Text);
        window.draw(hintsBackButton);
        window.draw(hintsBackText);
    }

    void renderOthersInfo() {
        window.draw(othersTitle);
        window.draw(wifiBox);
        window.draw(wifiTitle);
        window.draw(usernameText);
        window.draw(passwordText);
        window.draw(othersBackButton);
        window.draw(othersBackText);
    }

    void renderTaskFailedScreen() {
        window.draw(taskFailedTitle);
        window.draw(taskFailedMessage);
        window.draw(taskFailedCountdown);
    }

    void run() {
        while (window.isOpen()) {
            handleEvents();
            updateOverallTimer();
            
            if (currentScreen == Screen::TASK_FAILED) {
                updateTaskFailedScreen();
            }
            
            render();
        }
    }
};

int main() {
    CSEDUWebsite game;
    game.run();
    return 0;
}