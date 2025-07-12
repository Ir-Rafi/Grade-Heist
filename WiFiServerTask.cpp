// WiFiServerTask.cpp
#include "WiFiServerTask.hpp"
#include <algorithm>
#include <iostream>

WiFiServerTask::WiFiServerTask(unsigned w, unsigned h)
: currentState(State::Inactive)
{
    // Create off-screen buffer
    if (!m_rt.create(w, h)) {
        std::cerr << "Error: cannot create WiFi overlay of size "
                  << w << "×" << h << "\n";
    }
    m_overlay.setTexture(m_rt.getTexture());

    // Load font
    if (!font.loadFromFile("arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf\n";

    initializeCredentials();
    initializeUI();
}

void WiFiServerTask::initializeCredentials() {
    validUsernames = {"CSEDU_Admin"};
    // std::vector<std::string> prefixes = {"Access_","Login_","Key_"};
    // std::vector<std::string> suffixes = {"1234!","5678#","ABCD@","WXYZ$"};
    // for (auto& pre : prefixes)
    //     for (auto& suf : suffixes)
    //         validPasswords.push_back(pre + suf);
    validPasswords = {"Access1234!"};
}

void WiFiServerTask::initializeUI() {
    // Title
    titleText.setFont(font);
    titleText.setString("WiFi Server Task");
    titleText.setCharacterSize(28);
    titleText.setFillColor(sf::Color::White);
    titleText.setPosition(20.f, 20.f);

    // Username label & box
    usernameLabel = titleText;
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(20);
    usernameLabel.setPosition(50.f, 100.f);

    usernameBox.setSize({300.f, 35.f});
    usernameBox.setPosition(50.f, 130.f);
    usernameBox.setFillColor({255,255,255,200});
    usernameBox.setOutlineThickness(2.f);
    usernameBox.setOutlineColor({100,149,237});

    usernameInputText.setFont(font);
    usernameInputText.setCharacterSize(18);
    usernameInputText.setFillColor(sf::Color::Black);
    usernameInputText.setPosition(55.f, 135.f);

    // Password label & box
    passwordLabel = usernameLabel;
    passwordLabel.setString("Password:");
    passwordLabel.setPosition(50.f, 190.f);

    passwordBox = usernameBox;
    passwordBox.setPosition(50.f, 220.f);

    passwordInputText = usernameInputText;
    passwordInputText.setPosition(55.f, 225.f);

    // Login button
    loginBtnBox.setSize({100.f, 40.f});
    loginBtnBox.setPosition(200.f, 280.f);
    loginBtnBox.setFillColor({72,61,139});
    loginBtnBox.setOutlineThickness(2.f);
    loginBtnBox.setOutlineColor({255,255,255,150});

    loginButtonText.setFont(font);
    loginButtonText.setString("Login");
    loginButtonText.setCharacterSize(20);
    loginButtonText.setFillColor(sf::Color::White);
    loginButtonText.setPosition(225.f, 285.f);

    // ON / OFF buttons (Option screen)
    onBtnBox = loginBtnBox;
    onBtnBox.setPosition(150.f, 130.f);
    onBtnBox.setFillColor({34,139,34});
    onButtonText = loginButtonText;
    onButtonText.setString("ON");
    onButtonText.setPosition(185.f, 135.f);

    offBtnBox = onBtnBox;
    offBtnBox.setPosition(300.f, 130.f);
    offBtnBox.setFillColor({220,20,60});
    offButtonText = onButtonText;
    offButtonText.setString("OFF");
    offButtonText.setPosition(335.f, 135.f);

    // Result & exit prompt
    resultText.setFont(font);
    resultText.setCharacterSize(24);
    resultText.setPosition(50.f, 200.f);

    exitText.setFont(font);
    exitText.setString("Press Enter to continue");
    exitText.setCharacterSize(16);
    exitText.setFillColor(sf::Color::White);
    exitText.setPosition(50.f, 260.f);
}

bool WiFiServerTask::isValidCredentials() const {
    bool uv = std::find(validUsernames.begin(),
                        validUsernames.end(),
                        usernameStr) != validUsernames.end();
    bool pv = std::find(validPasswords.begin(),
                        validPasswords.end(),
                        passwordStr) != validPasswords.end();
    return uv && pv;
}

void WiFiServerTask::start() {
    currentState = State::Login;
    usernameActive = passwordActive = false;
    usernameStr.clear();
    passwordStr.clear();
    passwordDisplay.clear();
    usernameInputText.setString("");
    passwordInputText.setString("");
    resultText.setString("");
}

void WiFiServerTask::handleEvent(const sf::Event& e) {
    if (currentState == State::Inactive) return;

    // ESC cancels immediately
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
        currentState = State::Inactive;
        return;
    }

    // Convert mouse pos to overlay-local
    auto toLocal = [&](int mx, int my){
        auto pos = m_overlay.getPosition();
        return sf::Vector2f(float(mx) - pos.x, float(my) - pos.y);
    };

    switch (currentState) {
    case State::Login:
        if (e.type == sf::Event::MouseButtonPressed &&
            e.mouseButton.button == sf::Mouse::Left)
        {
            auto lp = toLocal(e.mouseButton.x, e.mouseButton.y);
            // Username click
            if (usernameBox.getGlobalBounds().contains(lp)) {
                usernameActive = true; passwordActive = false;
                usernameBox.setOutlineColor(sf::Color::Cyan);
                passwordBox.setOutlineColor({100,149,237});
            }
            // Password click
            else if (passwordBox.getGlobalBounds().contains(lp)) {
                passwordActive = true; usernameActive = false;
                passwordBox.setOutlineColor(sf::Color::Cyan);
                usernameBox.setOutlineColor({100,149,237});
            }
            // Login click
            else if (loginBtnBox.getGlobalBounds().contains(lp)) {
                if (isValidCredentials()) {
                    currentState = State::Option;
                } else {
                    taskPassed = false;
                    currentState = State::Result;
                    resultText.setString("❌ Invalid credentials");
                    resultText.setFillColor(sf::Color::Red);
                }
            }
            else {
                // clicked outside
                usernameActive = passwordActive = false;
                usernameBox.setOutlineColor({100,149,237});
                passwordBox.setOutlineColor({100,149,237});
            }
        }
        else if (e.type == sf::Event::TextEntered) {
            if (usernameActive) {
                if (e.text.unicode == 8 && !usernameStr.empty())
                    usernameStr.pop_back();
                else if (e.text.unicode >= 32 && e.text.unicode < 127)
                    usernameStr += char(e.text.unicode);
                usernameInputText.setString(usernameStr);
            }
            if (passwordActive) {
                if (e.text.unicode == 8 && !passwordStr.empty()) {
                    passwordStr.pop_back();
                    passwordDisplay.pop_back();
                }
                else if (e.text.unicode >= 32 && e.text.unicode < 127) {
                    passwordStr += char(e.text.unicode);
                    passwordDisplay += '*';
                }
                passwordInputText.setString(passwordDisplay);
            }
        }
        break;

    case State::Option:
        if (e.type == sf::Event::MouseButtonPressed &&
            e.mouseButton.button == sf::Mouse::Left)
        {
            auto lp = toLocal(e.mouseButton.x, e.mouseButton.y);
            if (onBtnBox.getGlobalBounds().contains(lp)) {
                taskPassed = false;
                currentState = State::Result;
                resultText.setString("❌ Task Failed");
                resultText.setFillColor(sf::Color::Red);
            }
            else if (offBtnBox.getGlobalBounds().contains(lp)) {
                taskPassed = true;
                currentState = State::Result;
                resultText.setString("✅ Task Passed");
                resultText.setFillColor(sf::Color::Green);
            }
        }
        break;

    case State::Result:
        if (e.type == sf::Event::KeyPressed &&
            e.key.code == sf::Keyboard::Enter)
        {
            currentState = State::Inactive;
        }
        break;

    case State::Inactive:
    default:
        break;
    }
}

void WiFiServerTask::update(float) {
    // no per-frame animations here
}

void WiFiServerTask::draw(sf::RenderWindow& window) {
    if (currentState == State::Inactive) return;

    // 1) Draw UI into m_rt
    m_rt.clear({30,30,30,200});  // translucent background

    m_rt.draw(titleText);
    if (currentState == State::Login) {
        m_rt.draw(usernameBox);
        m_rt.draw(usernameLabel);
        m_rt.draw(usernameInputText);
        m_rt.draw(passwordBox);
        m_rt.draw(passwordLabel);
        m_rt.draw(passwordInputText);
        m_rt.draw(loginBtnBox);
        m_rt.draw(loginButtonText);
    }
    else if (currentState == State::Option) {
        m_rt.draw(onBtnBox);
        m_rt.draw(onButtonText);
        m_rt.draw(offBtnBox);
        m_rt.draw(offButtonText);
    }
    else if (currentState == State::Result) {
        m_rt.draw(resultText);
        m_rt.draw(exitText);
    }

    m_rt.display();

    // 2) Center it in the window
    auto ws = window.getSize();
    auto rs = m_rt.getSize();
    m_overlay.setPosition(
        float(ws.x - rs.x) * 0.5f,
        float(ws.y - rs.y) * 0.5f
    );

    // 3) Finally draw the sprite
    window.draw(m_overlay);
}

bool WiFiServerTask::isActive() const {
    return currentState != State::Inactive;
}

bool WiFiServerTask::isFinished() const {
    return currentState == State::Inactive;
}
