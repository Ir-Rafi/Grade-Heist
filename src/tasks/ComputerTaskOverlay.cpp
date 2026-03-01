// ComputerTaskOverlay.cpp
#include "tasks/ComputerTaskOverlay.hpp"
#include <algorithm>

ComputerTaskOverlay::ComputerTaskOverlay(unsigned width, unsigned height)
: rng(std::random_device{}()), currentState(State::Inactive), taskPassed(false)
{
    m_rt.create(width, height);
    m_overlay.setTexture(m_rt.getTexture());
    setupUI();

    taskLines = {
        "INITIALIZE_REACTOR_CORE",
        "CALIBRATE_NAVIGATION_SYSTEM",
        "DOWNLOAD_ENGINE_DIAGNOSTICS",
        "SYNCHRONIZE_COMMUNICATIONS",
        "STABILIZE_OXYGEN_LEVELS",
        "UPLOAD_SECURITY_PROTOCOLS",
        "RESTART_WIFI_CONNECTION",
        "EMPTY_GARBAGE_CHUTE",
        "DIVERT_POWER_TO_SHIELDS",
        "ALIGN_ENGINE_OUTPUT",
        "PRIME_SHIELDS_SYSTEM",
        "SCAN_BOARDING_PASS"
    };
}

void ComputerTaskOverlay::setupUI() {
    font.loadFromFile("assets/arial.ttf");
    backgroundColor = sf::Color(15,15,25,200);
    screenBorder.setSize({static_cast<float>(m_rt.getSize().x - 20),
                          static_cast<float>(m_rt.getSize().y - 20)});
    screenBorder.setPosition(10,10);
    screenBorder.setFillColor(sf::Color(0,40,0));
    screenBorder.setOutlineThickness(3);
    screenBorder.setOutlineColor(sf::Color::Green);

    inputBox.setSize({600.f,45.f});
    inputBox.setPosition(100.f,400.f);
    inputBox.setFillColor(sf::Color(30,30,30));
    inputBox.setOutlineThickness(2);
    inputBox.setOutlineColor(sf::Color::Cyan);

    titleText.setFont(font);
    titleText.setString("=== COMPUTER TERMINAL ACCESS ===");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::Green);
    titleText.setPosition(100.f,30.f);

    promptText.setFont(font);
    promptText.setString("ENTER COMMAND:");
    promptText.setCharacterSize(20);
    promptText.setFillColor(sf::Color::Yellow);
    promptText.setPosition(100.f,350.f);

    taskText.setFont(font);
    taskText.setCharacterSize(24);
    taskText.setFillColor(sf::Color::Cyan);
    taskText.setPosition(100.f,150.f);

    inputText.setFont(font);
    inputText.setCharacterSize(22);
    inputText.setFillColor(sf::Color::White);
    inputText.setPosition(110.f,410.f);

    resultText.setFont(font);
    resultText.setCharacterSize(24);
    resultText.setPosition(100.f,480.f);

    exitText.setFont(font);
    exitText.setString("Press ESC to exit");
    exitText.setCharacterSize(20);
    exitText.setFillColor(sf::Color::Green);
    exitText.setPosition(100.f,520.f);
}

void ComputerTaskOverlay::start() {
    pickNewTask();
    userInput.clear();
    currentState = State::Playing;
    taskPassed   = false;
}

void ComputerTaskOverlay::pickNewTask() {
    std::uniform_int_distribution<> dist(0, taskLines.size()-1);
    currentTask = taskLines[dist(rng)];
    taskText.setString("REQUIRED: " + currentTask);
    inputText.setString("");
    resultText.setString("");
}

void ComputerTaskOverlay::handleEvent(const sf::Event& e) {
    if (currentState == State::Inactive) return;

    // During playing: ESC closes overlay
    if (currentState == State::Playing &&
        e.type == sf::Event::KeyPressed &&
        e.key.code == sf::Keyboard::Escape)
    {
        currentState = State::Inactive;
        return;
    }

    // After completion: Enter or ESC closes overlay
    if (currentState == State::Completed &&
        e.type == sf::Event::KeyPressed &&
        e.key.code == sf::Keyboard::Escape)
    {
        currentState = State::Inactive;
        return;
    }

    if (currentState != State::Playing)
        return;

    if (e.type == sf::Event::TextEntered) {
        if (e.text.unicode == 8 && !userInput.empty()) {
            userInput.pop_back();
        }
        else if (e.text.unicode == 13) {
            checkTaskCompletion();
        }
        else if (e.text.unicode >= 32 && e.text.unicode < 127) {
            userInput += static_cast<char>(e.text.unicode);
        }
        std::string up = userInput;
        std::transform(up.begin(), up.end(), up.begin(), ::toupper);
        inputText.setString(up);
    }
}

void ComputerTaskOverlay::checkTaskCompletion() {
    std::string up = userInput;
    std::transform(up.begin(), up.end(), up.begin(), ::toupper);
    taskPassed   = (up == currentTask);
    currentState = State::Completed;
    if (taskPassed) {
        resultText.setString("ACCESS GRANTED - TASK COMPLETED (7)");
        resultText.setFillColor(sf::Color::Green);
    } else {
        resultText.setString("ACCESS DENIED - INVALID COMMAND");
        resultText.setFillColor(sf::Color::Red);
    }
}

void ComputerTaskOverlay::update(float) {}

void ComputerTaskOverlay::draw(sf::RenderWindow& window) {
    if (currentState == State::Inactive) return;

    m_rt.clear(backgroundColor);
    m_rt.draw(screenBorder);
    m_rt.draw(titleText);

    if (currentState == State::Playing) {
        m_rt.draw(taskText);
        m_rt.draw(promptText);
        m_rt.draw(inputBox);
        m_rt.draw(inputText);
        sf::RectangleShape cursor({3.f,22.f});
        cursor.setFillColor(sf::Color::White);
        cursor.setPosition(inputText.getPosition().x +
                           inputText.getLocalBounds().width,
                           inputText.getPosition().y);
        static int cnt = 0; cnt++;
        if ((cnt/30)%2 == 0) m_rt.draw(cursor);
    } else {
        m_rt.draw(taskText);
        m_rt.draw(inputBox);
        m_rt.draw(inputText);
        m_rt.draw(resultText);
        m_rt.draw(exitText);
    }

    m_rt.display();
    auto ws = window.getSize();
    auto rs = m_rt.getSize();
    m_overlay.setPosition((ws.x - rs.x)*0.5f,
                          (ws.y - rs.y)*0.5f);
    window.draw(m_overlay);
}

bool ComputerTaskOverlay::isActive() const {
    return currentState == State::Playing;
}

bool ComputerTaskOverlay::isFinished() const {
    return currentState == State::Completed;
}

bool ComputerTaskOverlay::passed() const {
    return taskPassed;
}
