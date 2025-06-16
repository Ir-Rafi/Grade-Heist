#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>
#include <sstream>

class DualSortingChallenge {
private:
    sf::RenderWindow window;
    sf::Font font;
    sf::Text titleText;
    sf::Text timerText;
    sf::Text numbersLabelText;
    sf::Text stringsLabelText;
    sf::Text numbersInputText;
    sf::Text stringsInputText;
    sf::Text resultText;
    sf::Text instructionText;
    sf::RectangleShape numbersInputBox;
    sf::RectangleShape stringsInputBox;
    
    std::vector<int> randomNumbers;
    std::vector<std::string> randomStrings;
    std::string numbersInput;
    std::string stringsInput;
    std::vector<int> sortedNumbers;
    std::vector<std::string> sortedStrings;
    
    sf::Clock gameClock;
    float timeLimit;
    bool gameActive;
    bool gameCompleted;
    bool taskPassed;
    bool numbersBoxActive; // true for numbers, false for strings
    
    std::mt19937 rng;
    
public:
    DualSortingChallenge() : window(sf::VideoMode(1000, 700), "Dual Sorting Challenge"),
                            timeLimit(60.0f), gameActive(false), gameCompleted(false), 
                            taskPassed(false), numbersBoxActive(true), rng(std::random_device{}()) {
        
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load arial.ttf. Using default font." << std::endl;
        }
        
        setupUI();
        generateRandomData();
        startGame();
    }
    
    void setupUI() {
        // Title
        titleText.setFont(font);
        titleText.setString("Dual Sorting Challenge");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(300, 30);
        
        // Instructions
        instructionText.setFont(font);
        instructionText.setString("Sort numbers (ascending) and letters in each string (alphabetically).\nClick boxes to switch focus. Press Enter to submit. You have 1 minute!");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color::Yellow);
        instructionText.setPosition(50, 80);
        
        // Timer
        timerText.setFont(font);
        timerText.setCharacterSize(24);
        timerText.setFillColor(sf::Color::Green);
        timerText.setPosition(50, 130);
        
        // Numbers section
        numbersLabelText.setFont(font);
        numbersLabelText.setCharacterSize(20);
        numbersLabelText.setFillColor(sf::Color::Cyan);
        numbersLabelText.setPosition(50, 180);
        
        numbersInputBox.setSize(sf::Vector2f(400, 40));
        numbersInputBox.setPosition(50, 250);
        numbersInputBox.setFillColor(sf::Color(50, 50, 50));
        numbersInputBox.setOutlineThickness(3);
        numbersInputBox.setOutlineColor(sf::Color::Green); // Active by default
        
        numbersInputText.setFont(font);
        numbersInputText.setCharacterSize(18);
        numbersInputText.setFillColor(sf::Color::White);
        numbersInputText.setPosition(55, 255);
        
        // Strings section
        stringsLabelText.setFont(font);
        stringsLabelText.setCharacterSize(20);
        stringsLabelText.setFillColor(sf::Color::Magenta);
        stringsLabelText.setPosition(50, 320);
        
        stringsInputBox.setSize(sf::Vector2f(400, 40));
        stringsInputBox.setPosition(50, 390);
        stringsInputBox.setFillColor(sf::Color(50, 50, 50));
        stringsInputBox.setOutlineThickness(3);
        stringsInputBox.setOutlineColor(sf::Color::White); // Inactive by default
        
        stringsInputText.setFont(font);
        stringsInputText.setCharacterSize(18);
        stringsInputText.setFillColor(sf::Color::White);
        stringsInputText.setPosition(55, 395);
        
        // Result text
        resultText.setFont(font);
        resultText.setCharacterSize(28);
        resultText.setPosition(50, 450);
    }
    
    void generateRandomData() {
        // Generate random numbers (5-8 numbers between 1-100)
        randomNumbers.clear();
        int numCount = 5 + (rng() % 4); // 5-8 numbers
        for (int i = 0; i < numCount; i++) {
            randomNumbers.push_back(1 + (rng() % 100));
        }
        
        // Create sorted version for comparison
        sortedNumbers = randomNumbers;
        std::sort(sortedNumbers.begin(), sortedNumbers.end());
        
        // Generate one random string with scrambled letters
        randomStrings.clear();
        std::vector<std::string> baseWords = {
            "programming", "computer", "keyboard", "monitor", "software",
            "hardware", "internet", "website", "database", "algorithm",
            "challenge", "sorting", "letters", "scramble", "puzzle",
            "game", "screen", "window", "function", "variable"
        };
        
        // Pick one random word
        std::string word = baseWords[rng() % baseWords.size()];
        // Take first 6-9 characters and scramble them
        int length = std::min(6 + (int)(rng() % 4), (int)word.length()); // 6-9 chars
        word = word.substr(0, length);
        
        // Scramble the letters
        std::shuffle(word.begin(), word.end(), rng);
        randomStrings.push_back(word);
        
        // Create sorted versions for comparison (each string's letters sorted)
        sortedStrings.clear();
        for (const std::string& str : randomStrings) {
            std::string sortedStr = str;
            std::sort(sortedStr.begin(), sortedStr.end());
            sortedStrings.push_back(sortedStr);
        }
        
        // Update display labels
        std::string numbersDisplay = "Numbers to sort: ";
        for (size_t i = 0; i < randomNumbers.size(); i++) {
            if (i > 0) numbersDisplay += ", ";
            numbersDisplay += std::to_string(randomNumbers[i]);
        }
        numbersLabelText.setString(numbersDisplay);
        
        std::string stringsDisplay = "Sort letters in string: " + randomStrings[0];
        stringsLabelText.setString(stringsDisplay);
    }
    
    void startGame() {
        gameClock.restart();
        gameActive = true;
        gameCompleted = false;
        numbersInput = "";
        stringsInput = "";
        numbersInputText.setString("");
        stringsInputText.setString("");
        resultText.setString("");
        numbersBoxActive = true;
        updateBoxColors();
    }
    
    void updateBoxColors() {
        if (numbersBoxActive) {
            numbersInputBox.setOutlineColor(sf::Color::Green);
            stringsInputBox.setOutlineColor(sf::Color::White);
        } else {
            numbersInputBox.setOutlineColor(sf::Color::White);
            stringsInputBox.setOutlineColor(sf::Color::Green);
        }
    }
    
    void handleInput(sf::Event& event) {
        if (!gameActive || gameCompleted) return;
        
        if (event.type == sf::Event::MouseButtonPressed) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            
            // Check if clicked on numbers box
            if (numbersInputBox.getGlobalBounds().contains(mousePos)) {
                numbersBoxActive = true;
                updateBoxColors();
            }
            // Check if clicked on strings box
            else if (stringsInputBox.getGlobalBounds().contains(mousePos)) {
                numbersBoxActive = false;
                updateBoxColors();
            }
        }
        
        if (event.type == sf::Event::KeyPressed) {
            if (event.key.code == sf::Keyboard::Tab) {
                // Switch between boxes with Tab
                numbersBoxActive = !numbersBoxActive;
                updateBoxColors();
            }
            else if (event.key.code == sf::Keyboard::Enter) {
                checkAnswer();
            }
        }
        
        if (event.type == sf::Event::TextEntered) {
            if (event.text.unicode == 8) { // Backspace
                if (numbersBoxActive && !numbersInput.empty()) {
                    numbersInput.pop_back();
                    numbersInputText.setString(numbersInput);
                } else if (!numbersBoxActive && !stringsInput.empty()) {
                    stringsInput.pop_back();
                    stringsInputText.setString(stringsInput);
                }
            } else if (event.text.unicode >= 32 && event.text.unicode < 127) {
                char inputChar = static_cast<char>(event.text.unicode);
                
                if (numbersBoxActive) {
                    numbersInput += inputChar;
                    numbersInputText.setString(numbersInput);
                } else {
                    stringsInput += inputChar;
                    stringsInputText.setString(stringsInput);
                }
            }
        }
    }
    
    std::vector<int> parseNumbers(const std::string& input) {
        std::vector<int> numbers;
        std::stringstream ss(input);
        std::string token;
        
        while (std::getline(ss, token, ',')) {
            // Trim whitespace
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            
            try {
                if (!token.empty()) {
                    numbers.push_back(std::stoi(token));
                }
            } catch (const std::exception&) {
                // Invalid number, ignore
            }
        }
        
        return numbers;
    }
    
    std::vector<std::string> parseStrings(const std::string& input) {
        std::vector<std::string> strings;
        std::stringstream ss(input);
        std::string token;
        
        while (std::getline(ss, token, ',')) {
            // Trim whitespace
            token.erase(0, token.find_first_not_of(" \t"));
            token.erase(token.find_last_not_of(" \t") + 1);
            
            if (!token.empty()) {
                strings.push_back(token);
            }
        }
        
        return strings;
    }
    
    void checkAnswer() {
        if (!gameActive) return;
        
        gameActive = false;
        gameCompleted = true;
        
        // Parse user inputs
        std::vector<int> userNumbers = parseNumbers(numbersInput);
        std::vector<std::string> userStrings = parseStrings(stringsInput);
        
        // Check if both answers are correct
        bool numbersCorrect = (userNumbers == sortedNumbers);
        bool stringsCorrect = (userStrings == sortedStrings);
        
        if (numbersCorrect && stringsCorrect) {
            taskPassed = true;
            resultText.setString("TASK PASSED!");
            resultText.setFillColor(sf::Color::Green);
        } else {
            taskPassed = false;
            std::string failMessage = "TASK FAILED!\n";
            if (!numbersCorrect) {
                failMessage += "Numbers incorrect. Correct: ";
                for (size_t i = 0; i < sortedNumbers.size(); i++) {
                    if (i > 0) failMessage += ",";
                    failMessage += std::to_string(sortedNumbers[i]);
                }
                failMessage += "\n";
            }
            if (!stringsCorrect) {
                failMessage += "Strings incorrect. Correct: ";
                for (size_t i = 0; i < sortedStrings.size(); i++) {
                    if (i > 0) failMessage += ",";
                    failMessage += sortedStrings[i];
                }
            }
            resultText.setString(failMessage);
            resultText.setFillColor(sf::Color::Red);
        }
    }
    
    void update() {
        if (!gameActive || gameCompleted) return;
        
        float elapsedTime = gameClock.getElapsedTime().asSeconds();
        float remainingTime = timeLimit - elapsedTime;
        
        if (remainingTime <= 0) {
            // Time's up!
            gameActive = false;
            gameCompleted = true;
            taskPassed = false;
            
            std::string timeUpMessage = "TASK FAILED!\nTime's up!\nCorrect answers:\nNumbers: ";
            for (size_t i = 0; i < sortedNumbers.size(); i++) {
                if (i > 0) timeUpMessage += ",";
                timeUpMessage += std::to_string(sortedNumbers[i]);
            }
            timeUpMessage += "\nStrings: ";
            for (size_t i = 0; i < sortedStrings.size(); i++) {
                if (i > 0) timeUpMessage += ",";
                timeUpMessage += sortedStrings[i];
            }
            
            resultText.setString(timeUpMessage);
            resultText.setFillColor(sf::Color::Red);
            remainingTime = 0;
        }
        
        timerText.setString("Time remaining: " + std::to_string(static_cast<int>(remainingTime)) + "s");
        
        // Change timer color as time runs out
        if (remainingTime < 20) {
            timerText.setFillColor(sf::Color::Red);
        } else if (remainingTime < 40) {
            timerText.setFillColor(sf::Color::Yellow);
        } else {
            timerText.setFillColor(sf::Color::Green);
        }
    }
    
    void render() {
        window.clear(sf::Color::Black);
        
        window.draw(titleText);
        window.draw(instructionText);
        window.draw(timerText);
        
        window.draw(numbersLabelText);
        window.draw(numbersInputBox);
        window.draw(numbersInputText);
        
        window.draw(stringsLabelText);
        window.draw(stringsInputBox);
        window.draw(stringsInputText);
        
        if (gameCompleted) {
            window.draw(resultText);
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
                
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space && gameCompleted) {
                        // Restart the game
                        generateRandomData();
                        startGame();
                    }
                    if (event.key.code == sf::Keyboard::Escape) {
                        window.close();
                    }
                }
                
                handleInput(event);
            }
            
            update();
            render();
        }
    }
};

int main() {
    try {
        DualSortingChallenge game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    
    return 0;
}