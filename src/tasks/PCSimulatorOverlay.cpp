#include "tasks/PCSimulatorOverlay.hpp"
#include <iostream>

PCSimulatorOverlay::PCSimulatorOverlay(unsigned width, unsigned height)
: currentState(Inactive)
, switchOn(false)
, showInfoWindow(false)
{
    // Create off-screen render texture
    m_rt.create(width, height);
    m_overlay.setTexture(m_rt.getTexture());

    if (!font.loadFromFile("assets/arial.ttf")) {
        std::cerr << "Warning: could not load arial.ttf\n";
    }

    setupUI();
}

void PCSimulatorOverlay::start() {
    switchOn       = false;
    showInfoWindow = false;
    currentState   = WaitingFlip;

    // Reset toggle to OFF position
    switchToggle.setPosition(342.f, 252.f);
    switchToggle.setFillColor(sf::Color::Red);
}

void PCSimulatorOverlay::handleEvent(const sf::Event& e) {
    if (currentState == Inactive)
        return;

    // Always allow ESC to close immediately
    if (e.type == sf::Event::KeyPressed &&
        e.key.code == sf::Keyboard::Escape)
    {
        currentState = Inactive;
        return;
    }

    // Info screen: Enter closes
    if (currentState == Info &&
        e.type == sf::Event::KeyPressed &&
        e.key.code == sf::Keyboard::Return)
    {
        currentState = Inactive;
        return;
    }

    // WaitingFlip: map window coords → overlay coords, then test click
    if (currentState == WaitingFlip &&
        e.type == sf::Event::MouseButtonPressed &&
        e.mouseButton.button == sf::Mouse::Left)
    {
        // 1) global click in window space
        sf::Vector2f globalClick(float(e.mouseButton.x),
                                 float(e.mouseButton.y));
        // 2) subtract overlay position to get RT-local coords
        sf::Vector2f offset = m_overlay.getPosition();
        sf::Vector2f localClick = globalClick - offset;

        // 3) test within switchBackground
        if (!switchOn &&
            switchBackground.getGlobalBounds().contains(localClick))
        {
            switchOn       = true;
            showInfoWindow = true;

            // Move toggle to ON position and change color
            switchToggle.setPosition(402.f, 252.f);
            switchToggle.setFillColor(sf::Color::Green);
            currentState = Info;
        }
    }
}

void PCSimulatorOverlay::update(float /*dt*/) {
    // no dynamic updates
}

void PCSimulatorOverlay::draw(sf::RenderWindow& window) {
    if (currentState == Inactive)
        return;

    m_rt.clear(sf::Color::Black);

    if (currentState == WaitingFlip) {
        m_rt.draw(switchMessage);
        m_rt.draw(switchBackground);
        m_rt.draw(switchToggle);
        m_rt.draw(onText);
        m_rt.draw(offText);
    }
    else if (currentState == Info) {
        m_rt.draw(infoWindow);
        m_rt.draw(infoBox);
        m_rt.draw(infoTitle);
        m_rt.draw(usernameLabel);
        m_rt.draw(usernameField);
        m_rt.draw(usernameValue);
        m_rt.draw(passwordLabel);
        m_rt.draw(passwordField);
        m_rt.draw(passwordValue);
        m_rt.draw(exitMessage);
    }

    m_rt.display();

    // Center on main window
    auto ws = window.getSize();
    auto rs = m_rt.getSize();
    m_overlay.setPosition((ws.x - rs.x) * 0.5f,
                          (ws.y - rs.y) * 0.5f);
    window.draw(m_overlay);
}

bool PCSimulatorOverlay::isActive() const {
    return currentState != Inactive;
}

bool PCSimulatorOverlay::isFinished() const {
    return false;
}

void PCSimulatorOverlay::setupUI() {
    // --- Switch UI ---
    switchMessage.setFont(font);
    switchMessage.setString("Turn on the switch");
    switchMessage.setCharacterSize(24);
    switchMessage.setFillColor(sf::Color::White);
    switchMessage.setPosition(300.f, 200.f);

    switchBackground.setSize({120.f,60.f});
    switchBackground.setPosition(340.f,250.f);
    switchBackground.setFillColor(sf::Color(100,100,100));
    switchBackground.setOutlineThickness(2.f);
    switchBackground.setOutlineColor(sf::Color::White);

    switchToggle.setSize({56.f,56.f});
    switchToggle.setPosition(342.f,252.f);
    switchToggle.setFillColor(sf::Color::Red);

    onText.setFont(font);
    onText.setString("ON");
    onText.setCharacterSize(14);
    onText.setFillColor(sf::Color::White);
    onText.setPosition(420.f,270.f);

    offText.setFont(font);
    offText.setString("OFF");
    offText.setCharacterSize(14);
    offText.setFillColor(sf::Color::White);
    offText.setPosition(350.f,270.f);

    // --- Info UI ---
    infoWindow.setSize({600.f,400.f});
    infoWindow.setPosition(100.f,100.f);
    infoWindow.setFillColor(sf::Color(40,40,40));
    infoWindow.setOutlineThickness(3.f);
    infoWindow.setOutlineColor(sf::Color::White);

    infoBox.setSize({500.f,300.f});
    infoBox.setPosition(150.f,150.f);
    infoBox.setFillColor(sf::Color(60,60,60));
    infoBox.setOutlineThickness(2.f);
    infoBox.setOutlineColor(sf::Color::Cyan);

    infoTitle.setFont(font);
    infoTitle.setString("Here is the username and password of PC");
    infoTitle.setCharacterSize(22);
    infoTitle.setFillColor(sf::Color::White);
    infoTitle.setPosition(180.f,170.f);

    usernameLabel.setFont(font);
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(20);
    usernameLabel.setFillColor(sf::Color::White);
    usernameLabel.setPosition(200.f,230.f);

    usernameField.setSize({250.f,40.f});
    usernameField.setPosition(200.f,260.f);
    usernameField.setFillColor(sf::Color::White);
    usernameField.setOutlineThickness(2.f);
    usernameField.setOutlineColor(sf::Color::Black);

    usernameValue.setFont(font);
    usernameValue.setString("student");
    usernameValue.setCharacterSize(18);
    usernameValue.setFillColor(sf::Color::Black);
    usernameValue.setPosition(210.f,270.f);

    passwordLabel.setFont(font);
    passwordLabel.setString("Password:");
    passwordLabel.setCharacterSize(20);
    passwordLabel.setFillColor(sf::Color::White);
    passwordLabel.setPosition(200.f,320.f);

    passwordField.setSize({250.f,40.f});
    passwordField.setPosition(200.f,350.f);
    passwordField.setFillColor(sf::Color::White);
    passwordField.setOutlineThickness(2.f);
    passwordField.setOutlineColor(sf::Color::Black);

    passwordValue.setFont(font);
    passwordValue.setString("student");
    passwordValue.setCharacterSize(18);
    passwordValue.setFillColor(sf::Color::Black);
    passwordValue.setPosition(210.f,360.f);

    exitMessage.setFont(font);
    exitMessage.setString("Press Enter or Escape to exit the task");
    exitMessage.setCharacterSize(18);
    exitMessage.setFillColor(sf::Color::Yellow);
    exitMessage.setPosition(230.f,410.f);
}
