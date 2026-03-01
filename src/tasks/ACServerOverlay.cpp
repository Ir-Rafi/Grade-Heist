// ACServerOverlay.cpp
#include "tasks/ACServerOverlay.hpp"

ACServerOverlay::ACServerOverlay(unsigned width, unsigned height)
: currentState(State::Inactive)
, selectedRoom(0)
, taskPassed(false)
, backgroundColor(20,25,35)
, primaryColor(100,200,255)
, buttonColor(60,80,120)
, successColor(50,200,50)
, failColor(200,50,50)
{
    m_rt.create(width, height);
    m_overlay.setTexture(m_rt.getTexture());
    font.loadFromFile("assets/arial.ttf");

    setupWelcomeScreen();
    setupRoomSelection();
    setupACControl();
    setupResultScreen();
}

void ACServerOverlay::start() {
    currentState = State::Welcome;
    taskPassed = false;
}

void ACServerOverlay::handleEvent(const sf::Event& event) {
    if (currentState == State::Inactive)
        return;

    // Close overlay on ESC anytime
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Escape)
    {
        currentState = State::Inactive;
        return;
    }

    // Handle mouse click
    if (event.type == sf::Event::MouseButtonPressed &&
        event.mouseButton.button == sf::Mouse::Left)
    {
        sf::Vector2f pos((float)event.mouseButton.x,
                         (float)event.mouseButton.y);
        handleMouseClick(pos);
    }

    // Handle Enter to exit on result success
    if (event.type == sf::Event::KeyPressed &&
        event.key.code == sf::Keyboard::Return &&
        currentState == State::Result &&
        taskPassed)
    {
        currentState = State::Inactive;
    }
}

void ACServerOverlay::update(float) {
    // No periodic updates
}

void ACServerOverlay::draw(sf::RenderWindow& window) {
    if (currentState == State::Inactive)
        return;

    m_rt.clear(backgroundColor);

    switch (currentState) {
        case State::Welcome:
            m_rt.draw(titleText);
            m_rt.draw(welcomeText);
            m_rt.draw(instructionText);
            m_rt.draw(startButton);
            m_rt.draw(startButtonText);
            break;

        case State::RoomSelection:
            m_rt.draw(titleText);
            {
                sf::Text t = instructionText;
                t.setString("Select a room to control its AC:");
                t.setPosition(280,120);
                m_rt.draw(t);
            }
            for (size_t i=0; i<roomButtons.size(); ++i) {
                m_rt.draw(roomButtons[i]);
                m_rt.draw(roomTexts[i]);
            }
            break;

        case State::ACControl:
            m_rt.draw(titleText);
            m_rt.draw(selectedRoomText);
            m_rt.draw(instructionText);
            m_rt.draw(onButton);
            m_rt.draw(onButtonText);
            m_rt.draw(offButton);
            m_rt.draw(offButtonText);
            break;

        case State::Result:
            m_rt.draw(resultText);
            m_rt.draw(instructionText);
            m_rt.draw(closeButton);
            m_rt.draw(closeButtonText);
            break;

        default:
            break;
    }

    m_rt.display();
    // Center and draw overlay
    auto ws = window.getSize();
    auto rs = m_rt.getSize();
    m_overlay.setPosition((ws.x-rs.x)*0.5f, (ws.y-rs.y)*0.5f);
    window.draw(m_overlay);
}

bool ACServerOverlay::isActive() const    { return currentState != State::Inactive; }
bool ACServerOverlay::isFinished() const  { return currentState == State::Result; }
bool ACServerOverlay::passed() const      { return taskPassed; }

void ACServerOverlay::setupWelcomeScreen() {
    titleText.setFont(font);
    titleText.setString("CSEDU's AC Server");
    titleText.setCharacterSize(42);
    titleText.setFillColor(primaryColor);
    titleText.setPosition(280,150);

    welcomeText.setFont(font);
    welcomeText.setString("Wise people make wise decisions");
    welcomeText.setCharacterSize(28);
    welcomeText.setFillColor(sf::Color::Yellow);
    welcomeText.setPosition(250,250);

    instructionText.setFont(font);
    instructionText.setString("Click Start to begin your mission");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(320,320);

    startButton.setSize({200,60});
    startButton.setPosition(350,400);
    startButton.setFillColor(buttonColor);
    startButton.setOutlineThickness(3);
    startButton.setOutlineColor(primaryColor);

    startButtonText.setFont(font);
    startButtonText.setString("START");
    startButtonText.setCharacterSize(24);
    startButtonText.setFillColor(sf::Color::White);
    startButtonText.setPosition(420,425);
}

void ACServerOverlay::setupRoomSelection() {
    roomButtons.clear(); roomTexts.clear();
    for (int i=0; i<20; ++i) {
        int roomNumber=401+i;
        int col=i%4, row=i/4;
        float x=150+col*160, y=180+row*80;
        sf::RectangleShape btn({140,60});
        btn.setPosition(x,y);
        btn.setFillColor(buttonColor);
        btn.setOutlineThickness(2);
        btn.setOutlineColor(sf::Color::White);
        sf::Text t; t.setFont(font);
        t.setString("Room "+std::to_string(roomNumber));
        t.setCharacterSize(16);
        t.setFillColor(sf::Color::White);
        auto bounds=t.getLocalBounds();
        t.setPosition(x+(140-bounds.width)/2, y+(60-bounds.height)/2-3);
        roomButtons.push_back(btn);
        roomTexts.push_back(t);
    }
}

void ACServerOverlay::setupACControl() {
    selectedRoomText.setFont(font);
    selectedRoomText.setCharacterSize(28);
    selectedRoomText.setFillColor(sf::Color::White);
    selectedRoomText.setPosition(300,200);

    instructionText.setFont(font);
    instructionText.setString("Choose AC control action:");
    instructionText.setCharacterSize(20);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(320,280);

    onButton.setSize({180,80}); onButton.setPosition(250,350);
    onButton.setFillColor(sf::Color(200,80,80));
    onButton.setOutlineThickness(3); onButton.setOutlineColor(sf::Color::White);
    onButtonText.setFont(font); onButtonText.setString("TURN ON AC");
    onButtonText.setCharacterSize(20); onButtonText.setFillColor(sf::Color::White);
    onButtonText.setPosition(285,380);

    offButton.setSize({180,80}); offButton.setPosition(470,350);
    offButton.setFillColor(sf::Color(80,200,80));
    offButton.setOutlineThickness(3); offButton.setOutlineColor(sf::Color::White);
    offButtonText.setFont(font); offButtonText.setString("TURN OFF AC");
    offButtonText.setCharacterSize(20); offButtonText.setFillColor(sf::Color::White);
    offButtonText.setPosition(500,380);
}

void ACServerOverlay::setupResultScreen() {
    resultText.setFont(font); resultText.setCharacterSize(32);
    resultText.setPosition(250,250);

    closeButton.setSize({150,50}); closeButton.setPosition(375,450);
    closeButton.setFillColor(buttonColor);
    closeButton.setOutlineThickness(2); closeButton.setOutlineColor(sf::Color::White);
    closeButtonText.setFont(font); closeButtonText.setString("CLOSE");
    closeButtonText.setCharacterSize(18); closeButtonText.setFillColor(sf::Color::White);
    closeButtonText.setPosition(425,470);
}

void ACServerOverlay::handleMouseClick(const sf::Vector2f& pos) {
    if (currentState == State::Welcome) {
        if (startButton.getGlobalBounds().contains(pos)) {
            currentState = State::RoomSelection;
        }
    } else if (currentState == State::RoomSelection) {
        for (size_t i=0; i<roomButtons.size(); ++i) {
            if (roomButtons[i].getGlobalBounds().contains(pos)) {
                selectedRoom = 401 + (int)i;
                selectedRoomText.setString("Selected: Room "+std::to_string(selectedRoom));
                currentState = State::ACControl;
                break;
            }
        }
    } else if (currentState == State::ACControl) {
        if (onButton.getGlobalBounds().contains(pos)) {
            taskPassed = false; showResult();
        } else if (offButton.getGlobalBounds().contains(pos)) {
            taskPassed = (selectedRoom == 405);
            showResult();
        }
    } else if (currentState == State::Result) {
        if (closeButton.getGlobalBounds().contains(pos)) {
            currentState = State::Inactive;
        }
    }
}

void ACServerOverlay::showResult() {
    currentState = State::Result;
    if (taskPassed) {
        resultText.setString("🎉 TASK PASSED! 🎉");
        resultText.setFillColor(successColor);
        instructionText.setString("Task passed!!! Press Enter to exit the task");
        instructionText.setFillColor(successColor);
        // hide close button
        closeButton.setFillColor(sf::Color::Transparent);
        closeButton.setOutlineColor(sf::Color::Transparent);
        closeButtonText.setFillColor(sf::Color::Transparent);
    } else {
        resultText.setString("❌ TASK FAILED! ❌");
        resultText.setFillColor(failColor);
        if (selectedRoom != 405)
            instructionText.setString("Wrong room!");
        else
            instructionText.setString("Wrong action!");
        instructionText.setFillColor(sf::Color::White);
    }
}
