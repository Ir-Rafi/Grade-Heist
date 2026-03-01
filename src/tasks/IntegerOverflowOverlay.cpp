
// IntegerOverflowOverlay.cpp
#include "tasks/IntegerOverflowOverlay.hpp"
#include <algorithm>
#include <cctype>
#include <sstream>

IntegerOverflowOverlay::IntegerOverflowOverlay(unsigned width, unsigned height)
: currentState(State::Inactive)
, taskPassed(false)
{
    m_rt.create(width, height);
    m_overlay.setTexture(m_rt.getTexture());
    setupBackground();
    setupUI();
}

void IntegerOverflowOverlay::setupBackground() {
    backgroundGradient.setSize({(float)m_rt.getSize().x, (float)m_rt.getSize().y});
    backgroundGradient.setPosition(0,0);

    backgroundCircles.clear();
    sf::CircleShape c1(80); c1.setPosition(-40,-40); c1.setFillColor(sf::Color(103,58,183,30)); backgroundCircles.push_back(c1);
    sf::CircleShape c2(60); c2.setPosition(700,50);   c2.setFillColor(sf::Color(233,30,99,25));  backgroundCircles.push_back(c2);
    sf::CircleShape c3(100);c3.setPosition(650,450);  c3.setFillColor(sf::Color(33,150,243,20)); backgroundCircles.push_back(c3);
    sf::CircleShape c4(45); c4.setPosition(50,450);   c4.setFillColor(sf::Color(76,175,80,35));  backgroundCircles.push_back(c4);
    sf::CircleShape c5(70); c5.setPosition(100,100);  c5.setFillColor(sf::Color(255,193,7,25));  backgroundCircles.push_back(c5);
}

void IntegerOverflowOverlay::setupUI() {
    font.loadFromFile("assets/arial.ttf");

    // Title bar
    titleBar.setSize({(float)m_rt.getSize().x,120.f});
    titleBar.setPosition(0,0);
    titleBar.setFillColor(sf::Color(45,52,64,200));

    // Message
    messageText.setFont(font);
    messageText.setString("Type as much as u can. I am bigger than ur thinking\n....Think bigger type bigger");
    messageText.setCharacterSize(24);
    messageText.setFillColor(sf::Color::White);
    messageText.setStyle(sf::Text::Bold);
    messageText.setPosition(80,15);

    // Input box
    inputBox.setSize({400,50});
    inputBox.setPosition(200,280);
    inputBox.setFillColor(sf::Color(255,255,255,240));
    inputBox.setOutlineThickness(3);
    inputBox.setOutlineColor(sf::Color(76,175,80));

    inputDisplayText.setFont(font);
    inputDisplayText.setCharacterSize(22);
    inputDisplayText.setFillColor(sf::Color(33,37,41));
    inputDisplayText.setPosition(210,293);

    // Success UI
    usernameBox.setSize({320,45});
    usernameBox.setPosition(240,200);
    usernameBox.setFillColor(sf::Color(255,255,255,240));
    usernameBox.setOutlineThickness(3);
    usernameBox.setOutlineColor(sf::Color(33,150,243));

    passwordBox.setSize({320,45});
    passwordBox.setPosition(240,280);
    passwordBox.setFillColor(sf::Color(255,255,255,240));
    passwordBox.setOutlineThickness(3);
    passwordBox.setOutlineColor(sf::Color(33,150,243));

    usernameLabel.setFont(font);
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(20);
    usernameLabel.setFillColor(sf::Color::White);
    usernameLabel.setStyle(sf::Text::Bold);
    usernameLabel.setPosition(130,210);

    usernameText.setFont(font);
    usernameText.setString("Admin");
    usernameText.setCharacterSize(20);
    usernameText.setFillColor(sf::Color(33,37,41));
    usernameText.setPosition(250,210);

    passwordLabel.setFont(font);
    passwordLabel.setString("Password:");
    passwordLabel.setCharacterSize(20);
    passwordLabel.setFillColor(sf::Color::White);
    passwordLabel.setStyle(sf::Text::Bold);
    passwordLabel.setPosition(130,290);

    passwordText.setFont(font);
    passwordText.setString("9246890");
    passwordText.setCharacterSize(20);
    passwordText.setFillColor(sf::Color(33,37,41));
    passwordText.setPosition(250,290);

    successMessage.setFont(font);
    successMessage.setString("Task passed!!! Press Enter or ESC to exit.");
    successMessage.setCharacterSize(22);
    successMessage.setFillColor(sf::Color(76,175,80));
    successMessage.setStyle(sf::Text::Bold);
    successMessage.setPosition(120,370);

    failureMessage.setFont(font);
    failureMessage.setString("Task failed!!!");
    failureMessage.setCharacterSize(24);
    failureMessage.setFillColor(sf::Color(244,67,54));
    failureMessage.setStyle(sf::Text::Bold);
    float x = ((float)m_rt.getSize().x - failureMessage.getLocalBounds().width) / 2.f;
    failureMessage.setPosition(x,300);
}

void IntegerOverflowOverlay::start() {
    inputText.clear();
    inputDisplayText.setString("");
    currentState = State::InputPhase;
    taskPassed = false;
}

void IntegerOverflowOverlay::handleEvent(const sf::Event& e) {
    if (currentState == State::Inactive)
        return;

    if (currentState == State::InputPhase) {
        if (e.type == sf::Event::TextEntered) {
            auto u = e.text.unicode;
            if (u == 8) {
                if (!inputText.empty()) inputText.pop_back();
            } else if (u == 13) {
                checkInput();
            } else if (u >= 32 && u < 127) {
                inputText += static_cast<char>(u);
            }
            inputDisplayText.setString(inputText);
        }
    } else {
        if (e.type == sf::Event::KeyPressed &&
           (e.key.code == sf::Keyboard::Return || e.key.code == sf::Keyboard::Escape))
        {
            currentState = State::Inactive;
        }
    }
}

void IntegerOverflowOverlay::update(float) {}

void IntegerOverflowOverlay::draw(sf::RenderWindow& window) {
    if (currentState == State::Inactive) return;

    m_rt.clear();
    // draw gradient background
    sf::Vertex quad[] = {
        {{0,0}, sf::Color(67,56,202)},
        {{(float)m_rt.getSize().x,0}, sf::Color(79,70,229)},
        {{(float)m_rt.getSize().x,(float)m_rt.getSize().y}, sf::Color(99,102,241)},
        {{0,(float)m_rt.getSize().y}, sf::Color(67,56,202)}
    };
    m_rt.draw(quad,4,sf::Quads);
    for (auto& c : backgroundCircles) m_rt.draw(c);

    if (currentState == State::InputPhase) {
        m_rt.draw(titleBar);
        m_rt.draw(messageText);
        m_rt.draw(inputBox);
        m_rt.draw(inputDisplayText);
    } else if (currentState == State::SuccessPhase) {
        sf::RectangleShape ov({(float)m_rt.getSize().x,(float)m_rt.getSize().y});
        ov.setFillColor(sf::Color(0,0,0,50)); m_rt.draw(ov);
        for (auto& c : backgroundCircles) m_rt.draw(c);
        m_rt.draw(usernameLabel);
        m_rt.draw(usernameBox);
        m_rt.draw(usernameText);
        m_rt.draw(passwordLabel);
        m_rt.draw(passwordBox);
        m_rt.draw(passwordText);
        m_rt.draw(successMessage);
    } else {
        sf::RectangleShape ov({(float)m_rt.getSize().x,(float)m_rt.getSize().y});
        ov.setFillColor(sf::Color(0,0,0,50)); m_rt.draw(ov);
        m_rt.draw(failureMessage);
    }

    m_rt.display();
    auto ws = window.getSize(); auto rs = m_rt.getSize();
    m_overlay.setPosition((ws.x-rs.x)*0.5f,(ws.y-rs.y)*0.5f);
    window.draw(m_overlay);
}

bool IntegerOverflowOverlay::isActive() const {
    return currentState != State::Inactive;
}

bool IntegerOverflowOverlay::isFinished() const {
    return currentState == State::SuccessPhase || currentState == State::FailurePhase;
}

bool IntegerOverflowOverlay::passed() const {
    return taskPassed;
}

void IntegerOverflowOverlay::checkInput() {
    if (!isValidNumericInput(inputText)) {
        currentState = State::FailurePhase;
        return;
    }
    if (isIntegerOverflow(inputText)) {
        currentState = State::SuccessPhase;
        taskPassed = true;
    } else {
        currentState = State::FailurePhase;
    }
}

bool IntegerOverflowOverlay::isValidNumericInput(const std::string& s) {
    if (s.empty()) return false;
    size_t i = (s[0]=='+'||s[0]=='-')?1:0;
    if (i==1 && s.size()==1) return false;
    for (; i<s.size(); ++i) if (!std::isdigit((unsigned char)s[i])) return false;
    return true;
}

bool IntegerOverflowOverlay::isIntegerOverflow(const std::string& s) {
    try {
        long long v = std::stoll(s);
        return v > INT32_MAX_VALUE;
    } catch (...) {
        std::string num = s;
        if (num[0]=='+'||num[0]=='-') num = num.substr(1);
        if (num.size()>10) return true;
        if (num.size()==10 && num > "2147483647") return true;
        return false;
    }
}
