// CombinationLock.cpp
#include "locker.hpp"
#include <iostream>
#include <algorithm>
#include <cmath>

CombinationLock::CombinationLock(unsigned w, unsigned h)
: currentState(State::Inactive)
, autoClose(false)
, taskPassed(false)
, rng(std::random_device{}())
{
    // Create off-screen buffer
    if (!m_rt.create(w, h))
        std::cerr << "Error: cannot create CombinationLock overlay\n";
    m_overlay.setTexture(m_rt.getTexture());
    setupUI();
}

void CombinationLock::setupUI() {
    // Load font
    if (!font.loadFromFile("arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf\n";

    // Initialize game data
    correctCode = "745268";
    hintText = "Put all the numbers u got throughout the whole journey";
    maxDigits = 6;
    maxAttempts = 2;
    attemptsRemaining = maxAttempts;
    
    // Colors
    backgroundColor = sf::Color(15, 15, 25);
    successColor = sf::Color(0, 255, 100);
    failColor = sf::Color(255, 50, 50);
    
    setupBackground();
    setupVisualElements();
    setupTexts();
}

void CombinationLock::setupBackground() {
    backgroundParticles.clear();
    for (int i = 0; i < 20; i++) {
        sf::CircleShape p(2 + rng() % 4);
        p.setPosition(rng() % 800, rng() % 600);
        p.setFillColor(sf::Color(
            100 + rng() % 100,
            100 + rng() % 100,
            150 + rng() % 100,
            100 + rng() % 100
        ));
        backgroundParticles.push_back(p);
    }
}

void CombinationLock::setupVisualElements() {
    // Frame
    lockFrame.setSize({450.f, 400.f});
    lockFrame.setPosition(175.f, 80.f);
    lockFrame.setFillColor(sf::Color(40, 40, 60, 200));
    lockFrame.setOutlineThickness(6.f);
    lockFrame.setOutlineColor(sf::Color(120, 120, 140));
    
    // Dial
    lockDial.setRadius(60.f);
    lockDial.setPosition(340.f, 120.f);
    lockDial.setFillColor(sf::Color(50, 50, 70));
    lockDial.setOutlineThickness(4.f);
    lockDial.setOutlineColor(sf::Color::White);
    
    // Code display
    codeDisplay.setSize({350.f, 60.f});
    codeDisplay.setPosition(225.f, 220.f);
    codeDisplay.setFillColor(sf::Color(20, 20, 20));
    codeDisplay.setOutlineThickness(3.f);
    codeDisplay.setOutlineColor(sf::Color(0, 255, 0));
    
    // Digit boxes
    digitBoxes.clear();
    digitTexts.clear();
    for (int i = 0; i < maxDigits; i++) {
        sf::RectangleShape box({45.f, 40.f});
        box.setPosition(245.f + i * 50.f, 230.f);
        box.setFillColor(sf::Color(30, 30, 30));
        box.setOutlineThickness(2.f);
        box.setOutlineColor(sf::Color(0, 200, 0));
        digitBoxes.push_back(box);
        
        sf::Text t;
        t.setFont(font);
        t.setCharacterSize(24);
        t.setFillColor(sf::Color(0, 255, 0));
        t.setPosition(255.f + i * 50.f, 235.f);
        digitTexts.push_back(t);
    }
    
    // Control buttons
    enterButton.setSize({120.f, 50.f});
    enterButton.setPosition(250.f, 320.f);
    enterButton.setFillColor(sf::Color(0, 150, 0));
    enterButton.setOutlineThickness(3.f);
    enterButton.setOutlineColor(sf::Color::White);
    
    clearButton.setSize({120.f, 50.f});
    clearButton.setPosition(430.f, 320.f);
    clearButton.setFillColor(sf::Color(150, 0, 0));
    clearButton.setOutlineThickness(3.f);
    clearButton.setOutlineColor(sf::Color::White);
}

void CombinationLock::setupTexts() {
    // Title
    titleText.setFont(font);
    titleText.setString("Welcome to the Last Stage");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color(255, 100, 100));
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);
    titleText.setPosition(200.f, 20.f);
    
    // Hint display
    hintDisplayText.setFont(font);
    hintDisplayText.setString("HINT: " + hintText);
    hintDisplayText.setCharacterSize(16);
    hintDisplayText.setFillColor(sf::Color(255, 255, 100));
    hintDisplayText.setOutlineColor(sf::Color::Black);
    hintDisplayText.setOutlineThickness(1.f);
    hintDisplayText.setPosition(50.f, 120.f);
    
    // Code display
    codeDisplayText.setFont(font);
    codeDisplayText.setString("Enter 6-digit code:");
    codeDisplayText.setCharacterSize(18);
    codeDisplayText.setFillColor(sf::Color::White);
    codeDisplayText.setOutlineColor(sf::Color::Black);
    codeDisplayText.setOutlineThickness(1.f);
    codeDisplayText.setPosition(280.f, 200.f);
    
    // Instructions
    instructionText.setFont(font);
    instructionText.setString("Use keyboard to enter digits (0-9) - Order doesn't matter");
    instructionText.setCharacterSize(14);
    instructionText.setFillColor(sf::Color(100, 255, 255));
    instructionText.setOutlineColor(sf::Color::Black);
    ruct.setOutlineThickness(1.f);
    instructionText.setPosition(180.f, 290.f);
    
    // Keyboard instructions
    keyboardInstructions.setFont(font);
    keyboardInstructions.setString("Press Enter to submit | Backspace to delete");
    keyboardInstructions.setCharacterSize(12);
    keyboardInstructions.setFillColor(sf::Color(200, 200, 200));
    keyboardInstructions.setOutlineColor(sf::Color::Black);
    keyboardInstructions.setOutlineThickness(1.f);
    keyboardInstructions.setPosition(220.f, 380.f);
    
    // Button texts
    enterButtonText.setFont(font);
    enterButtonText.setString("ENTER");
    enterButtonText.setCharacterSize(16);
    enterButtonText.setFillColor(sf::Color::White);
    enterButtonText.setOutlineColor(sf::Color::Black);
    enterButtonText.setOutlineThickness(1.f);
    enterButtonText.setPosition(285.f, 335.f);
    
    clearButtonText.setFont(font);
    clearButtonText.setString("CLEAR");
    clearButtonText.setCharacterSize(16);
    clearButtonText.setFillColor(sf::Color::White);
    clearButtonText.setOutlineColor(sf::Color::Black);
    clearButtonText.setOutlineThickness(1.f);
    clearButtonText.setPosition(465.f, 335.f);
    
    // Attempts
    attemptText.setFont(font);
    attemptText.setCharacterSize(18);
    attemptText.setFillColor(sf::Color::Yellow);
    attemptText.setOutlineColor(sf::Color::Black);
    attemptText.setOutlineThickness(1.f);
    attemptText.setPosition(500.f, 280.f);
    
    // Result
    resultText.setFont(font);
    resultText.setCharacterSize(22);
    resultText.setOutlineColor(sf::Color::Black);
    resultText.setOutlineThickness(2.f);
    resultText.setPosition(150.f, 420.f);
    
    // Exit text
    exitText.setFont(font);
    exitText.setString("Press ESC to close");
    exitText.setCharacterSize(16);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1.f);
    exitText.setPosition(320.f, 500.f);
}

void CombinationLock::start() {
    enteredCode.clear();
    attemptsRemaining = maxAttempts;
    taskPassed = false;
    currentState = State::HintDisplay;
    autoClose = false;
    gameClock.restart();
    particleClock.restart();
    updateCodeDisplay();
}

void CombinationLock::handleEvent(const sf::Event& e) {
    // ESC closes overlay immediately
    if (e.type == sf::Event::KeyPressed &&
        e.key.code == sf::Keyboard::Escape &&
        currentState != State::Inactive)
    {
        currentState = State::Inactive;
        return;
    }
    
    if (e.type == sf::Event::MouseButtonPressed) {
        handleMouseClick(static_cast<float>(e.mouseButton.x), static_cast<float>(e.mouseButton.y));
    }
    
    if (e.type == sf::Event::KeyPressed) {
        handleKeyPress(e.key.code);
    }
}

void CombinationLock::handleMouseClick(float x, float y) {
    // Convert to overlay coordinates
    auto ws = m_rt.getSize();
    float overlayX = x;
    float overlayY = y;
    
    if (currentState == State::HintDisplay) {
        currentState = State::Playing;
        gameClock.restart();
        return;
    }
    
    if (currentState != State::Playing) return;
    
    if (enterButton.getGlobalBounds().contains(overlayX, overlayY)) {
        if (static_cast<int>(enteredCode.size()) == maxDigits) {
            checkCombination();
        }
        return;
    }
    
    if (clearButton.getGlobalBounds().contains(overlayX, overlayY)) {
        enteredCode.clear();
        updateCodeDisplay();
    }
}

void CombinationLock::handleKeyPress(sf::Keyboard::Key k) {
    if (currentState == State::HintDisplay) {
        currentState = State::Playing;
        gameClock.restart();
        return;
    }
    
    if (currentState != State::Playing) return;
    
    // Handle number keys (0-9)
    if (k >= sf::Keyboard::Num0 && k <= sf::Keyboard::Num9) {
        if (static_cast<int>(enteredCode.size()) < maxDigits) {
            char digit = '0' + (k - sf::Keyboard::Num0);
            enteredCode += digit;
            updateCodeDisplay();
        }
    }
    // Handle numpad keys (0-9)
    else if (k >= sf::Keyboard::Numpad0 && k <= sf::Keyboard::Numpad9) {
        if (static_cast<int>(enteredCode.size()) < maxDigits) {
            char digit = '0' + (k - sf::Keyboard::Numpad0);
            enteredCode += digit;
            updateCodeDisplay();
        }
    }
    // Handle Enter key
    else if (k == sf::Keyboard::Enter) {
        if (static_cast<int>(enteredCode.size()) == maxDigits) {
            checkCombination();
        }
    }
    // Handle Backspace or Delete
    else if (k == sf::Keyboard::BackSpace || k == sf::Keyboard::Delete) {
        if (!enteredCode.empty()) {
            enteredCode.pop_back();
            updateCodeDisplay();
        }
    }
}

void CombinationLock::updateCodeDisplay() {
    for (int i = 0; i < maxDigits; i++) {
        if (i < static_cast<int>(enteredCode.size())) {
            digitTexts[i].setString(std::string(1, enteredCode[i]));
        } else {
            digitTexts[i].setString("_");
        }
    }
}

bool CombinationLock::checkDigitsMatch(const std::string& entered, const std::string& correct) {
    std::string e = entered;
    std::string c = correct;
    std::sort(e.begin(), e.end());
    std::sort(c.begin(), c.end());
    return e == c;
}

void CombinationLock::checkCombination() {
    currentState = State::Completed;
    
    if (checkDigitsMatch(enteredCode, correctCode)) {
        taskPassed = true;
        resultText.setString("✅ PASS! (8)\nThe door is open..Now, Escape hopefully u will study well next time");
        resultText.setFillColor(successColor);
        lockFrame.setOutlineColor(successColor);
        lockDial.setOutlineColor(successColor);
        codeDisplay.setOutlineColor(successColor);
        for (auto& b : digitBoxes) {
            b.setOutlineColor(successColor);
        }
    } else {
        attemptsRemaining--;
        if (attemptsRemaining <= 0) {
            taskPassed = false;
            resultText.setString("❌ FAIL!\nTask failed try again...");
            resultText.setFillColor(failColor);
            lockFrame.setOutlineColor(failColor);
            lockDial.setOutlineColor(failColor);
            codeDisplay.setOutlineColor(failColor);
            for (auto& b : digitBoxes) {
                b.setOutlineColor(failColor);
            }
        } else {
            currentState = State::Playing;
            resultText.setString("Wrong code! " + std::to_string(attemptsRemaining) + " attempt(s) remaining");
            resultText.setFillColor(sf::Color::Yellow);
            lockFrame.setOutlineColor(failColor);
            lockDial.setOutlineColor(failColor);
            codeDisplay.setOutlineColor(failColor);
            for (auto& b : digitBoxes) {
                b.setOutlineColor(failColor);
            }
            enteredCode.clear();
            updateCodeDisplay();
            gameClock.restart();
            return;
        }
    }
    
    finishClock.restart();
    autoClose = true;
}

void CombinationLock::updateBackground() {
    float dt = particleClock.restart().asSeconds();
    for (auto& p : backgroundParticles) {
        sf::Vector2f pos = p.getPosition();
        pos.x += (30 + rng() % 20) * dt;
        pos.y += std::sin(pos.x * 0.01f) * 20 * dt;
        if (pos.x > 820) {
            pos.x = -20;
            pos.y = rng() % 600;
        }
        p.setPosition(pos);
    }
}

void CombinationLock::update(float) {
    updateBackground();
    
    if (currentState == State::Playing) {
        attemptText.setString("Attempts: " + std::to_string(attemptsRemaining) + "/" + std::to_string(maxAttempts));
        
        if (gameClock.getElapsedTime().asSeconds() > 1.0f && attemptsRemaining > 0) {
            lockFrame.setOutlineColor(sf::Color(120, 120, 140));
            lockDial.setOutlineColor(sf::Color::White);
            codeDisplay.setOutlineColor(sf::Color(0, 255, 0));
            for (auto& b : digitBoxes) {
                b.setOutlineColor(sf::Color(0, 200, 0));
            }
            resultText.setString("");
        }
    }
    
    if (currentState == State::Completed && autoClose) {
        if (finishClock.getElapsedTime().asSeconds() >= 6.f) {
            currentState = State::Inactive;
        }
    }
}

void CombinationLock::renderHintScreen() {
    sf::Text hintTitle("FINAL CHALLENGE - SECURITY LOCK", font, 32);
    hintTitle.setFillColor(sf::Color(255, 200, 100));
    hintTitle.setOutlineColor(sf::Color::Black);
    hintTitle.setOutlineThickness(2.f);
    hintTitle.setPosition(150.f, 150.f);
    m_rt.draw(hintTitle);
    
    sf::RectangleShape box({600.f, 120.f});
    box.setPosition(100.f, 220.f);
    box.setFillColor(sf::Color(30, 30, 50, 180));
    box.setOutlineThickness(3.f);
    box.setOutlineColor(sf::Color(100, 200, 255));
    m_rt.draw(box);
    
    sf::Text wrappedHint("HINT:\n" + hintText, font, 18);
    wrappedHint.setFillColor(sf::Color(150, 255, 150));
    wrappedHint.setOutlineColor(sf::Color::Black);
    wrappedHint.setOutlineThickness(1.f);
    wrappedHint.setPosition(120.f, 250.f);
    m_rt.draw(wrappedHint);
    
    sf::Text clickText("Click anywhere or press any key to start", font, 20);
    clickText.setFillColor(sf::Color::White);
    clickText.setOutlineColor(sf::Color::Black);
    clickText.setOutlineThickness(1.f);
    clickText.setPosition(200.f, 380.f);
    m_rt.draw(clickText);
}

void CombinationLock::renderGameScreen() {
    m_rt.draw(lockFrame);
    m_rt.draw(lockDial);
    m_rt.draw(codeDisplay);
    m_rt.draw(codeDisplayText);
    
    for (int i = 0; i < maxDigits; i++) {
        m_rt.draw(digitBoxes[i]);
        m_rt.draw(digitTexts[i]);
    }
    
    m_rt.draw(enterButton);
    m_rt.draw(clearButton);
    m_rt.draw(enterButtonText);
    m_rt.draw(clearButtonText);
    m_rt.draw(instructionText);
    m_rt.draw(keyboardInstructions);
    m_rt.draw(attemptText);
    
    if (currentState == State::Completed) {
        m_rt.draw(resultText);
        m_rt.draw(exitText);
    }
}

void CombinationLock::draw(sf::RenderWindow& window) {
    if (currentState == State::Inactive) return;
    
    m_rt.clear(sf::Color(0, 0, 0, 180));
    
    // Draw background particles
    for (auto& p : backgroundParticles) {
        m_rt.draw(p);
    }
    
    m_rt.draw(titleText);
    
    if (currentState == State::HintDisplay) {
        renderHintScreen();
    } else {
        renderGameScreen();
    }
    
    m_rt.display();
    
    auto ws = window.getSize();
    auto rs = m_rt.getSize();
    m_overlay.setPosition(
        (ws.x - rs.x) * 0.5f,
        (ws.y - rs.y) * 0.5f
    );
    window.draw(m_overlay);
}

bool CombinationLock::isActive() const {
    return currentState == State::Playing;
}

bool CombinationLock::isFinished() const {
    return currentState == State::Completed;
}

bool CombinationLock::passed() const {
    return taskPassed;
}