// GradeChangerOverlay.cpp
#include "GradeChangerOverlay.hpp"
#include <cctype>
#include <iostream>

GradeChangerOverlay::GradeChangerOverlay(unsigned w, unsigned h)
: currentPhase(Inactive)
, usernameActive(false)
, passwordActive(false)
, loginSuccess(false)
, nameActive(false)
, gradeActive(false)
, gradeChanged(false)
, closeDelay(2.f)
, shouldClose(false)
{
    if (!m_rt.create(w, h))
        std::cerr << "Error: cannot create GradeChangerOverlay\n";
    m_overlay.setTexture(m_rt.getTexture());
    if (!font.loadFromFile("arial.ttf"))
        std::cerr << "Warning: could not load arial.ttf\n";
    setupUI();
}

void GradeChangerOverlay::setupUI() {
    setupLoginUI();
    setupGradeUI();
}

void GradeChangerOverlay::setupLoginUI() {
    // Title
    loginTitleText.setFont(font);
    loginTitleText.setString("Admin Login");
    loginTitleText.setCharacterSize(28);
    loginTitleText.setFillColor(sf::Color::White);
    loginTitleText.setPosition(200, 30);

    // Username label & box
    usernameLabel.setFont(font);
    usernameLabel.setString("Username:");
    usernameLabel.setCharacterSize(18);
    usernameLabel.setFillColor(sf::Color::Yellow);
    usernameLabel.setPosition(50, 100);

    usernameBox.setSize({300, 40});
    usernameBox.setFillColor(sf::Color::White);
    usernameBox.setOutlineThickness(2);
    usernameBox.setOutlineColor(sf::Color::Blue);
    usernameBox.setPosition(50, 130);

    usernameText.setFont(font);
    usernameText.setCharacterSize(16);
    usernameText.setFillColor(sf::Color::Black);
    usernameText.setPosition(55, 140);

    // Password label & box
    passwordLabel = usernameLabel;
    passwordLabel.setString("Password:");
    passwordLabel.setPosition(50, 190);

    passwordBox = usernameBox;
    passwordBox.setPosition(50, 220);

    passwordText.setFont(font);
    passwordText.setCharacterSize(16);
    passwordText.setFillColor(sf::Color::Black);
    passwordText.setPosition(55, 230);

    // Login button
    loginButton.setSize({100, 40});
    loginButton.setFillColor(sf::Color::Green);
    loginButton.setOutlineThickness(2);
    loginButton.setOutlineColor(sf::Color::White);
    loginButton.setPosition(50, 280);

    loginButtonText.setFont(font);
    loginButtonText.setString("Login");
    loginButtonText.setCharacterSize(16);
    loginButtonText.setFillColor(sf::Color::White);
    loginButtonText.setPosition(75, 295);

    // Result
    loginResultText.setFont(font);
    loginResultText.setCharacterSize(18);
    loginResultText.setFillColor(sf::Color::Red);
    loginResultText.setPosition(50, 340);
}

void GradeChangerOverlay::setupGradeUI() {
    gradeTitleText.setFont(font);
    gradeTitleText.setString("Grade Changer");
    gradeTitleText.setCharacterSize(28);
    gradeTitleText.setFillColor(sf::Color::White);
    gradeTitleText.setPosition(180, 30);

    // Name label & box
    nameLabel.setFont(font);
    nameLabel.setString("Student Name:");
    nameLabel.setCharacterSize(18);
    nameLabel.setFillColor(sf::Color::Yellow);
    nameLabel.setPosition(50, 100);

    nameBox.setSize({300, 40});
    nameBox.setFillColor(sf::Color::White);
    nameBox.setOutlineThickness(2);
    nameBox.setOutlineColor(sf::Color::Blue);
    nameBox.setPosition(50, 130);

    nameText.setFont(font);
    nameText.setCharacterSize(16);
    nameText.setFillColor(sf::Color::Black);
    nameText.setPosition(55, 140);

    // Grade label & box
    gradeLabel.setFont(font);
    gradeLabel.setString("Grade:");
    gradeLabel.setCharacterSize(18);
    gradeLabel.setFillColor(sf::Color::Yellow);
    gradeLabel.setPosition(50, 190);

    gradeBox.setSize({300, 40});
    gradeBox.setFillColor(sf::Color::White);
    gradeBox.setOutlineThickness(2);
    gradeBox.setOutlineColor(sf::Color::Blue);
    gradeBox.setPosition(50, 220);

    gradeText.setFont(font);
    gradeText.setCharacterSize(16);
    gradeText.setFillColor(sf::Color::Black);
    gradeText.setPosition(55, 230);

    // Submit button
    submitButton.setSize({100, 40});
    submitButton.setFillColor(sf::Color::Green);
    submitButton.setOutlineThickness(2);
    submitButton.setOutlineColor(sf::Color::White);
    submitButton.setPosition(50, 280);

    submitButtonText.setFont(font);
    submitButtonText.setString("Submit");
    submitButtonText.setCharacterSize(16);
    submitButtonText.setFillColor(sf::Color::White);
    submitButtonText.setPosition(70, 295);

    // Submit result
    submitResultText.setFont(font);
    submitResultText.setCharacterSize(20);
    submitResultText.setFillColor(sf::Color::Green);
    submitResultText.setPosition(50, 340);
}

void GradeChangerOverlay::start() {
    loginUsername.clear();
    loginPassword.clear();
    usernameActive = true;
    passwordActive = false;
    loginSuccess   = false;
    loginResultText.setString("");

    studentName.clear();
    studentGrade.clear();
    nameActive    = false;
    gradeActive   = false;
    gradeChanged  = false;
    submitResultText.setString("");

    shouldClose    = false;
    currentPhase   = Login;
}

void GradeChangerOverlay::handleEvent(const sf::Event& e) {
    if (currentPhase == Inactive) return;

    // --- new: pressing Escape always closes the overlay ---
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
        currentPhase = Inactive;
        return;
    }

    // Text entry
    if (e.type == sf::Event::TextEntered) {
        char c = static_cast<char>(e.text.unicode);
        if (currentPhase == Login) {
            handleLoginTextInput(c);
        } else if (currentPhase == Grade && !shouldClose) {
            handleGradeTextInput(c);
        }
    }

    // Mouse click
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2i mp(e.mouseButton.x, e.mouseButton.y);
        // Convert into overlay-local coords
        sf::Vector2f offset = m_overlay.getPosition();
        sf::Vector2i local(
            mp.x - int(offset.x),
            mp.y - int(offset.y)
        );
        if (currentPhase == Login) {
            handleLoginMouseClick(local);
        } else if (currentPhase == Grade && !shouldClose) {
            handleGradeMouseClick(local);
        }
    }

    // Enter key
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
        if (currentPhase == Login) {
            checkLogin();
        } else if (currentPhase == Grade && !shouldClose) {
            submitGradeChange();
        }
    }
}

void GradeChangerOverlay::handleLoginTextInput(char c) {
    if (c == '\b') {
        if (usernameActive && !loginUsername.empty()) loginUsername.pop_back();
        else if (passwordActive && !loginPassword.empty()) loginPassword.pop_back();
    } else if (c == '\t') {
        usernameActive = !usernameActive;
        passwordActive = !passwordActive;
    } else if (std::isprint(c)) {
        if (usernameActive && loginUsername.size() < 20) loginUsername += c;
        else if (passwordActive && loginPassword.size() < 20) loginPassword += c;
    }
    usernameText.setString(loginUsername);
    passwordText.setString(std::string(loginPassword.size(), '*'));
    usernameBox.setOutlineColor(usernameActive ? sf::Color::Green : sf::Color::Blue);
    passwordBox.setOutlineColor(passwordActive ? sf::Color::Green : sf::Color::Blue);
}

void GradeChangerOverlay::handleGradeTextInput(char c) {
    if (c == '\b') {
        if (nameActive && !studentName.empty()) studentName.pop_back();
        else if (gradeActive && !studentGrade.empty()) studentGrade.pop_back();
    } else if (c == '\t') {
        nameActive  = !nameActive;
        gradeActive = !gradeActive;
    } else if (std::isprint(c)) {
        if (nameActive && studentName.size() < 30) studentName += c;
        else if (gradeActive && studentGrade.size() < 10) {
            if (std::isdigit(c) || c == '.' || std::isalpha(c) || c=='+'||c=='-')
                studentGrade += c;
        }
    }
    nameText.setString(studentName);
    gradeText.setString(studentGrade);
    nameBox.setOutlineColor(nameActive ? sf::Color::Green : sf::Color::Blue);
    gradeBox.setOutlineColor(gradeActive ? sf::Color::Green : sf::Color::Blue);
}

void GradeChangerOverlay::handleLoginMouseClick(sf::Vector2i pos) {
    if (usernameBox.getGlobalBounds().contains(pos.x, pos.y)) {
        usernameActive = true; passwordActive = false;
    } else if (passwordBox.getGlobalBounds().contains(pos.x, pos.y)) {
        usernameActive = false; passwordActive = true;
    } else if (loginButton.getGlobalBounds().contains(pos.x, pos.y)) {
        checkLogin();
    }
}

void GradeChangerOverlay::handleGradeMouseClick(sf::Vector2i pos) {
    if (nameBox.getGlobalBounds().contains(pos.x, pos.y)) {
        nameActive = true; gradeActive = false;
    } else if (gradeBox.getGlobalBounds().contains(pos.x, pos.y)) {
        nameActive = false; gradeActive = true;
    } else if (submitButton.getGlobalBounds().contains(pos.x, pos.y)) {
        submitGradeChange();
    }
}

void GradeChangerOverlay::checkLogin() {
    if (loginUsername == "Admin" && loginPassword == "9246890") {
        loginSuccess = true;
        currentPhase = Grade;
        loginResultText.setString("Login Successful!");
        loginResultText.setFillColor(sf::Color::Green);
    } else {
        loginResultText.setString("Invalid credentials!");
        loginResultText.setFillColor(sf::Color::Red);
    }
}

void GradeChangerOverlay::submitGradeChange() {
    if (!studentName.empty() && !studentGrade.empty()) {
        gradeChanged   = true;
        currentPhase   = Completed;
        submitResultText.setString("Grade Changed(8) – closing in 2s...");
        submitResultText.setFillColor(sf::Color::Green);
        shouldClose    = true;
        closeClock.restart();
    } else {
        submitResultText.setString("Please fill both fields!");
        submitResultText.setFillColor(sf::Color::Red);
    }
}

void GradeChangerOverlay::update(float dt) {
    if (currentPhase == Completed && shouldClose) {
        if (closeClock.getElapsedTime().asSeconds() >= closeDelay) {
            currentPhase = Inactive;
        }
    }
}

void GradeChangerOverlay::draw(sf::RenderWindow& window) {
    if (currentPhase == Inactive) return;

    m_rt.clear({0,0,0,180});

    if (currentPhase == Login) {
        m_rt.draw(loginTitleText);
        m_rt.draw(usernameLabel);
        m_rt.draw(usernameBox);
        m_rt.draw(usernameText);
        m_rt.draw(passwordLabel);
        m_rt.draw(passwordBox);
        m_rt.draw(passwordText);
        m_rt.draw(loginButton);
        m_rt.draw(loginButtonText);
        m_rt.draw(loginResultText);
    } else { // Grade or Completed
        m_rt.draw(gradeTitleText);
        m_rt.draw(nameLabel);
        m_rt.draw(nameBox);
        m_rt.draw(nameText);
        m_rt.draw(gradeLabel);
        m_rt.draw(gradeBox);
        m_rt.draw(gradeText);
        m_rt.draw(submitButton);
        m_rt.draw(submitButtonText);
        m_rt.draw(submitResultText);
    }

    m_rt.display();

    auto ws = window.getSize();
    auto rs = m_rt.getSize();
    m_overlay.setPosition((ws.x - rs.x)/2.f, (ws.y - rs.y)/2.f);
    window.draw(m_overlay);
}

bool GradeChangerOverlay::isActive() const {
    return currentPhase == Login || currentPhase == Grade;
}

bool GradeChangerOverlay::isFinished() const {
    return currentPhase == Completed;
}

bool GradeChangerOverlay::passed() const {
    return gradeChanged;
}
