#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>

class CombinationLockWindow {
private:
    sf::RenderWindow lockWindow;
    sf::Font font;
    
    // Lock data
    std::string correctCode;
    std::string enteredCode;
    std::string hintText;
    
    // UI elements
    sf::Text titleText;
    sf::Text hintDisplayText;
    sf::Text codeDisplayText;
    sf::Text instructionText;
    sf::Text resultText;
    sf::Text attemptText;
    
    // Visual elements
    sf::RectangleShape lockFrame;
    sf::CircleShape lockDial;
    sf::RectangleShape codeDisplay;
    std::vector<sf::RectangleShape> digitBoxes;
    std::vector<sf::Text> digitTexts;
    std::vector<sf::RectangleShape> numberButtons;
    std::vector<sf::Text> buttonTexts;
    sf::RectangleShape enterButton;
    sf::RectangleShape clearButton;
    sf::Text enterButtonText;
    sf::Text clearButtonText;
    
    // Background elements
    std::vector<sf::CircleShape> backgroundParticles;
    
    // Timer
    sf::Clock gameClock;
    sf::Clock particleClock;
    
    // Colors
    sf::Color backgroundColor;
    sf::Color successColor;
    sf::Color failColor;
    
    // Game state
    enum LockState { PLAYING, SUCCESS, FAILED, HINT_DISPLAY };
    LockState currentState;
    
    bool taskCompleted;
    bool windowShouldClose;
    bool waitingForExit;
    int maxDigits;
    int attemptsRemaining;
    int maxAttempts;

public:
    CombinationLockWindow()
    : lockWindow(sf::VideoMode(1000, 800), "Welcome to the Last Stage", sf::Style::Close)
    {
        initialize();
    }
    
    void initialize() {
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Error loading arial.ttf\n";
        }
        
        // 6-digit code including '8'
        correctCode = "745268";
        hintText    = "Put all the numbers u got throughout the whole journey";
        maxDigits   = 6;
        maxAttempts = 2;
        attemptsRemaining = maxAttempts;
        
        backgroundColor = sf::Color(15, 15, 25);
        successColor    = sf::Color(0, 255, 100);
        failColor       = sf::Color(255, 50, 50);
        
        setupBackground();
        setupVisualElements();
        setupTexts();
        
        currentState    = HINT_DISPLAY;
        taskCompleted   = false;
        windowShouldClose = false;
        waitingForExit  = false;
        enteredCode.clear();
        
        gameClock.restart();
        particleClock.restart();
    }
    
    void setupBackground() {
        backgroundParticles.clear();
        for (int i = 0; i < 20; i++) {
            sf::CircleShape p(2 + rand() % 4);
            p.setPosition(rand() % 1000, rand() % 800);
            p.setFillColor(sf::Color(
                100 + rand()%100,
                100 + rand()%100,
                150 + rand()%100,
                100 + rand()%100
            ));
            backgroundParticles.push_back(p);
        }
    }
    
    void updateBackground() {
        float dt = particleClock.restart().asSeconds();
        for (auto& p : backgroundParticles) {
            sf::Vector2f pos = p.getPosition();
            pos.x += (30 + rand()%20) * dt;
            pos.y += std::sin(pos.x * 0.01f) * 20 * dt;
            if (pos.x > 1020) {
                pos.x = -20;
                pos.y = rand() % 800;
            }
            p.setPosition(pos);
        }
    }
    
    void setupVisualElements() {
        // Frame
        lockFrame.setSize({450,550});
        lockFrame.setPosition(275,120);
        lockFrame.setFillColor(sf::Color(40,40,60,200));
        lockFrame.setOutlineThickness(6);
        lockFrame.setOutlineColor(sf::Color(120,120,140));
        
        // Dial
        lockDial.setRadius(90);
        lockDial.setPosition(405,180);
        lockDial.setFillColor(sf::Color(50,50,70));
        lockDial.setOutlineThickness(4);
        lockDial.setOutlineColor(sf::Color::White);
        
        // Code display
        codeDisplay.setSize({350,60});
        codeDisplay.setPosition(325,340);
        codeDisplay.setFillColor(sf::Color(20,20,20));
        codeDisplay.setOutlineThickness(3);
        codeDisplay.setOutlineColor(sf::Color(0,255,0));
        
        // Digit boxes
        digitBoxes.clear();
        digitTexts.clear();
        for (int i = 0; i < maxDigits; i++) {
            sf::RectangleShape box({55,45});
            box.setPosition(345 + i*65, 350);
            box.setFillColor(sf::Color(30,30,30));
            box.setOutlineThickness(2);
            box.setOutlineColor(sf::Color(0,200,0));
            digitBoxes.push_back(box);
            
            sf::Text t;
            t.setFont(font);
            t.setCharacterSize(28);
            t.setFillColor(sf::Color(0,255,0));
            t.setPosition(360 + i*65, 355);
            digitTexts.push_back(t);
        }
        
        // Number buttons (1–9)
        numberButtons.clear();
        buttonTexts.clear();
        for (int i = 1; i <= 9; i++) {
            sf::RectangleShape btn({70,55});
            int row = (i-1)/3, col = (i-1)%3;
            btn.setPosition(80 + col*80, 450 + row*65);
            btn.setFillColor(sf::Color(60,80,100));
            btn.setOutlineThickness(3);
            btn.setOutlineColor(sf::Color(150,150,150));
            numberButtons.push_back(btn);
            
            sf::Text txt;
            txt.setFont(font);
            txt.setCharacterSize(24);
            txt.setFillColor(sf::Color::White);
            txt.setString(std::to_string(i));
            auto b = btn.getGlobalBounds();
            auto tb = txt.getLocalBounds();
            txt.setPosition(
                b.left + b.width/2  - tb.width/2,
                b.top  + b.height/2 - tb.height/2 - 5
            );
            buttonTexts.push_back(txt);
        }
        // Button 0
        {
            sf::RectangleShape btn({70,55});
            btn.setPosition(80+80, 645);
            btn.setFillColor(sf::Color(60,80,100));
            btn.setOutlineThickness(3);
            btn.setOutlineColor(sf::Color(150,150,150));
            numberButtons.push_back(btn);
            
            sf::Text txt;
            txt.setFont(font);
            txt.setCharacterSize(24);
            txt.setFillColor(sf::Color::White);
            txt.setString("0");
            auto b = btn.getGlobalBounds();
            auto tb = txt.getLocalBounds();
            txt.setPosition(
                b.left + b.width/2  - tb.width/2,
                b.top  + b.height/2 - tb.height/2 - 5
            );
            buttonTexts.push_back(txt);
        }
        
        // Control buttons
        enterButton.setSize({100,60});
        enterButton.setPosition(750,480);
        enterButton.setFillColor(sf::Color(0,150,0));
        enterButton.setOutlineThickness(3);
        enterButton.setOutlineColor(sf::Color::White);
        
        clearButton.setSize({100,60});
        clearButton.setPosition(750,560);
        clearButton.setFillColor(sf::Color(150,0,0));
        clearButton.setOutlineThickness(3);
        clearButton.setOutlineColor(sf::Color::White);
    }
    
    void setupTexts() {
        titleText.setFont(font);
        titleText.setString("Welcome to the Last Stage");
        titleText.setCharacterSize(32);
        titleText.setFillColor(sf::Color(255,100,100));
        titleText.setPosition(280,30);
        
        hintDisplayText.setFont(font);
        hintDisplayText.setString("HINT: " + hintText);
        hintDisplayText.setCharacterSize(18);
        hintDisplayText.setFillColor(sf::Color(255,255,100));
        hintDisplayText.setPosition(50,150);
        
        codeDisplayText.setFont(font);
        codeDisplayText.setString("Enter 6-digit code:");
        codeDisplayText.setCharacterSize(20);
        codeDisplayText.setFillColor(sf::Color::White);
        codeDisplayText.setPosition(380,315);
        
        instructionText.setFont(font);
        instructionText.setString("Click numbers to enter code (order doesn't matter)");
        instructionText.setCharacterSize(16);
        instructionText.setFillColor(sf::Color(100,255,255));
        instructionText.setPosition(50,420);
        
        enterButtonText.setFont(font);
        enterButtonText.setString("ENTER");
        enterButtonText.setCharacterSize(18);
        enterButtonText.setFillColor(sf::Color::White);
        enterButtonText.setPosition(775,500);
        
        clearButtonText.setFont(font);
        clearButtonText.setString("CLEAR");
        clearButtonText.setCharacterSize(18);
        clearButtonText.setFillColor(sf::Color::White);
        clearButtonText.setPosition(775,580);
        
        attemptText.setFont(font);
        attemptText.setCharacterSize(20);
        attemptText.setFillColor(sf::Color::Yellow);
        attemptText.setPosition(600,420);
        
        resultText.setFont(font);
        resultText.setCharacterSize(24);
        resultText.setPosition(200,720);
    }
    
    void handleMouseClick(int x, int y) {
        if (currentState == HINT_DISPLAY) {
            currentState = PLAYING;
            gameClock.restart();
            return;
        }
        if (currentState != PLAYING) return;
        
        for (int i = 0; i < 10; ++i) {
            if (numberButtons[i].getGlobalBounds().contains(x,y)) {
                if ((int)enteredCode.size() < maxDigits) {
                    std::string d = (i < 9 ? std::to_string(i+1) : "0");
                    enteredCode += d;
                    updateCodeDisplay();
                }
                return;
            }
        }
        if (enterButton.getGlobalBounds().contains(x,y)) {
            if ((int)enteredCode.size() == maxDigits) checkCombination();
            return;
        }
        if (clearButton.getGlobalBounds().contains(x,y)) {
            enteredCode.clear();
            updateCodeDisplay();
        }
    }
    
    void handleKeyPress(sf::Keyboard::Key k) {
        if (waitingForExit && k == sf::Keyboard::Enter) {
            windowShouldClose = true;
        }
    }
    
    void updateCodeDisplay() {
        for (int i = 0; i < maxDigits; i++) {
            if (i < (int)enteredCode.size())
                digitTexts[i].setString(std::string(1, enteredCode[i]));
            else
                digitTexts[i].setString("_");
        }
    }
    
    bool checkDigitsMatch(std::string e, std::string c) {
        std::sort(e.begin(), e.end());
        std::sort(c.begin(), c.end());
        return e == c;
    }
    
    void checkCombination() {
        if (checkDigitsMatch(enteredCode, correctCode)) {
            taskCompleted = true;
            currentState = SUCCESS;
            waitingForExit = true;
            resultText.setString(
                "The door is open..Now, Escape hopefully u will study well next time"
            );
            resultText.setFillColor(successColor);
            lockFrame.setOutlineColor(successColor);
            lockDial.setOutlineColor(successColor);
            codeDisplay.setOutlineColor(successColor);
            for (auto &b : digitBoxes) b.setOutlineColor(successColor);
        } else {
            attemptsRemaining--;
            if (attemptsRemaining <= 0) {
                currentState = FAILED;
                waitingForExit = true;
                resultText.setString("Task failed try again...");
                resultText.setFillColor(failColor);
                lockFrame.setOutlineColor(failColor);
                lockDial.setOutlineColor(failColor);
                codeDisplay.setOutlineColor(failColor);
                for (auto &b : digitBoxes) b.setOutlineColor(failColor);
            } else {
                resultText.setString(
                    "Wrong code! " + std::to_string(attemptsRemaining) + " attempt(s) remaining"
                );
                resultText.setFillColor(sf::Color::Yellow);
                lockFrame.setOutlineColor(failColor);
                lockDial.setOutlineColor(failColor);
                codeDisplay.setOutlineColor(failColor);
                for (auto &b : digitBoxes) b.setOutlineColor(failColor);
                enteredCode.clear();
                updateCodeDisplay();
                gameClock.restart();
            }
        }
    }
    
    void update() {
        updateBackground();
        if (currentState == PLAYING) {
            attemptText.setString(
                "Attempts: " + std::to_string(attemptsRemaining) + "/" + std::to_string(maxAttempts)
            );
            if (gameClock.getElapsedTime().asSeconds() > 1.0f
             && attemptsRemaining > 0
             && currentState == PLAYING)
            {
                lockFrame.setOutlineColor(sf::Color(120,120,140));
                lockDial.setOutlineColor(sf::Color::White);
                codeDisplay.setOutlineColor(sf::Color(0,255,0));
                for (auto &b : digitBoxes) b.setOutlineColor(sf::Color(0,200,0));
                resultText.setString("");
            }
        }
    }
    
    void render() {
        lockWindow.clear(backgroundColor);
        for (auto &p : backgroundParticles) lockWindow.draw(p);
        lockWindow.draw(titleText);
        
        if (currentState == HINT_DISPLAY) {
            sf::Text hintTitle("FINAL CHALLENGE - SECURITY LOCK", font, 36);
            hintTitle.setFillColor(sf::Color(255,200,100));
            hintTitle.setPosition(200,250);
            lockWindow.draw(hintTitle);
            
            sf::RectangleShape box({700,150});
            box.setPosition(150,320);
            box.setFillColor(sf::Color(30,30,50,180));
            box.setOutlineThickness(3);
            box.setOutlineColor(sf::Color(100,200,255));
            lockWindow.draw(box);
            
            sf::Text wrappedHint("HINT:\n" + hintText, font, 20);
            wrappedHint.setFillColor(sf::Color(150,255,150));
            wrappedHint.setPosition(180,350);
            lockWindow.draw(wrappedHint);
            
            sf::Text clickText("Click anywhere to start the challenge", font, 22);
            clickText.setFillColor(sf::Color::White);
            clickText.setPosition(300,550);
            lockWindow.draw(clickText);
            
        } else {
            lockWindow.draw(lockFrame);
            lockWindow.draw(lockDial);
            lockWindow.draw(codeDisplay);
            lockWindow.draw(codeDisplayText);
            
            for (int i = 0; i < maxDigits; i++) {
                lockWindow.draw(digitBoxes[i]);
                lockWindow.draw(digitTexts[i]);
            }
            for (int i = 0; i < 10; i++) {
                lockWindow.draw(numberButtons[i]);
                lockWindow.draw(buttonTexts[i]);
            }
            lockWindow.draw(enterButton);
            lockWindow.draw(clearButton);
            lockWindow.draw(enterButtonText);
            lockWindow.draw(clearButtonText);
            lockWindow.draw(instructionText);
            lockWindow.draw(attemptText);
            lockWindow.draw(resultText);
            
            if (waitingForExit) {
                sf::Text exitText("Press \"Enter\" key to close the program", font, 18);
                exitText.setFillColor(sf::Color(255,255,100));
                exitText.setPosition(320,750);
                lockWindow.draw(exitText);
            }
        }
        
        lockWindow.display();
    }
    
    bool run() {
        // initialize display
        updateCodeDisplay();
        
        while (lockWindow.isOpen() && !windowShouldClose) {
            sf::Event e;
            while (lockWindow.pollEvent(e)) {
                if (e.type == sf::Event::Closed) {
                    lockWindow.close();
                    return false;
                }
                if (e.type == sf::Event::MouseButtonPressed) {
                    handleMouseClick(e.mouseButton.x, e.mouseButton.y);
                }
                if (e.type == sf::Event::KeyPressed) {
                    handleKeyPress(e.key.code);
                }
            }
            update();
            render();
        }
        lockWindow.close();
        return taskCompleted;
    }
};

int main() {
    CombinationLockWindow game;
    game.run();
    return 0;
}
