// TypingChallenge.cpp
#include "tasks/TypingChallenge.hpp"
#include <iostream>
#include <algorithm>
#include <cctype>
#include <sstream>

TypingChallenge::TypingChallenge(unsigned w, unsigned h)
: currentState(State::Inactive)
, autoClose(false)
, taskPassed(false)
, rng(std::random_device{}())
{
    // Create off-screen buffer
    if (!m_rt.create(w, h))
        std::cerr << "Error: cannot create TypingChallenge overlay\n";
    m_overlay.setTexture(m_rt.getTexture());
    setupUI();
}

void TypingChallenge::setupUI() {
    // Load font
    if (!font.loadFromFile("assets/arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf\n";

    // Title
    titleText.setFont(font);
    titleText.setString("Letter → Number Challenge");
    titleText.setCharacterSize(32);
    titleText.setFillColor(sf::Color::White);
    titleText.setOutlineColor(sf::Color::Black);
    titleText.setOutlineThickness(2.f);
    titleText.setPosition(20.f, 20.f);

    // Instructions
    instructionText.setFont(font);
    instructionText.setString(
        "Convert letters to A=1 … Z=26\n"
        "Type numbers separated by spaces, then press Enter"
    );
    instructionText.setCharacterSize(18);
    instructionText.setFillColor(sf::Color::Yellow);
    instructionText.setOutlineColor(sf::Color::Black);
    instructionText.setOutlineThickness(1.f);
    instructionText.setPosition(20.f, 80.f);

    // String to convert
    stringText.setFont(font);
    stringText.setCharacterSize(36);
    stringText.setFillColor(sf::Color::Cyan);
    stringText.setOutlineColor(sf::Color::Black);
    stringText.setOutlineThickness(1.f);
    stringText.setPosition(20.f, 150.f);

    // Input box & text
    inputBox.setSize({760.f, 40.f});
    inputBox.setPosition(20.f, 210.f);
    inputBox.setFillColor({30,30,30,200});
    inputBox.setOutlineThickness(2.f);
    inputBox.setOutlineColor(sf::Color::White);

    inputText.setFont(font);
    inputText.setCharacterSize(22);
    inputText.setFillColor(sf::Color::White);
    inputText.setOutlineColor(sf::Color::Black);
    inputText.setOutlineThickness(1.f);
    inputText.setPosition(25.f, 215.f);

    // Result message
    resultText.setFont(font);
    resultText.setCharacterSize(28);
    resultText.setOutlineColor(sf::Color::Black);
    resultText.setOutlineThickness(2.f);
    resultText.setPosition(20.f, 300.f);

    // Exit prompt
    exitText.setFont(font);
    exitText.setString("Press ESC to close");
    exitText.setCharacterSize(18);
    exitText.setFillColor(sf::Color::White);
    exitText.setOutlineColor(sf::Color::Black);
    exitText.setOutlineThickness(1.f);
    exitText.setPosition(20.f, 350.f);
}

void TypingChallenge::generateRandomString() {
    std::uniform_int_distribution<int> lenDist(4, 7);
    std::uniform_int_distribution<int> letterDist(0, 25);
    std::uniform_int_distribution<int> caseDist(0,1);

    int length = lenDist(rng);
    targetString.clear();
    correctAnswer.clear();

    for (int i = 0; i < length; ++i) {
        char c = char('A' + letterDist(rng));
        if (caseDist(rng)) c = static_cast<char>(std::tolower(c));
        targetString += c;

        int val = std::toupper(c) - 'A' + 1;
        if (i) correctAnswer += ' ';
        correctAnswer += std::to_string(val);
    }
    stringText.setString("String: " + targetString);
}

void TypingChallenge::start() {
    generateRandomString();
    userInput.clear();
    inputText.setString("");
    resultText.setString("");
    taskPassed   = false;
    currentState = State::Playing;
    autoClose    = false;
}

void TypingChallenge::handleEvent(const sf::Event& e) {
    // ESC closes overlay immediately
    if (e.type == sf::Event::KeyPressed &&
        e.key.code == sf::Keyboard::Escape &&
        currentState != State::Inactive)
    {
        currentState = State::Inactive;
        return;
    }

    if (currentState != State::Playing) return;

    if (e.type == sf::Event::TextEntered) {
        if (e.text.unicode == 8) { // backspace
            if (!userInput.empty()) {
                userInput.pop_back();
                inputText.setString(userInput);
            }
        }
        else if (e.text.unicode == 13) { // enter
            checkAnswer();
        }
        else if (e.text.unicode >= 32 && e.text.unicode < 127) {
            userInput += static_cast<char>(e.text.unicode);
            inputText.setString(userInput);
        }
    }
}

void TypingChallenge::checkAnswer() {
    currentState = State::Completed;
    taskPassed   = false;
    std::string ans = trim(userInput);
    if (ans == correctAnswer) {
        taskPassed = true;
        resultText.setString("✅ PASS! (6)\nRoom 405: Turn off AC");
        resultText.setFillColor(sf::Color::Green);
    } else {
        resultText.setString("❌ FAIL!\nAnswer: " + correctAnswer);
        resultText.setFillColor(sf::Color::Red);
    }
    finishClock.restart();
    autoClose = true;
}

void TypingChallenge::update(float) {
    if (currentState == State::Completed && autoClose) {
        if (finishClock.getElapsedTime().asSeconds() >= 6.f) {
            currentState = State::Inactive;
        }
    }
}

void TypingChallenge::draw(sf::RenderWindow& window) {
    if (currentState == State::Inactive) return;

    m_rt.clear({0,0,0,180});
    m_rt.draw(titleText);
    m_rt.draw(instructionText);
    m_rt.draw(stringText);
    m_rt.draw(inputBox);
    m_rt.draw(inputText);

    if (currentState == State::Completed) {
        m_rt.draw(resultText);
        m_rt.draw(exitText);
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

bool TypingChallenge::isActive() const {
    return currentState == State::Playing;
}

bool TypingChallenge::isFinished() const {
    return currentState == State::Completed;
}

bool TypingChallenge::passed() const {
    return taskPassed;
}

std::string TypingChallenge::trim(const std::string& s) {
    auto l = s.find_first_not_of(" \t");
    auto r = s.find_last_not_of(" \t");
    return (l == std::string::npos
            ? ""
            : s.substr(l, r - l + 1));
}
