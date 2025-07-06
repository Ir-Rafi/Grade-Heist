#include "WiFiLoginTask.hpp"
#include <ctime>
#include <algorithm>
#include <cctype>

WiFiLoginTask::WiFiLoginTask(unsigned width, unsigned height)
: state(State::Inactive)
, w(width), h(height)
, userActive(false), passActive(false), showError(false)
, rng(static_cast<unsigned>(std::time(nullptr)))
, credIndex(0)
{
    rt.create(w, h);
    overlay.setTexture(rt.getTexture());
    font.loadFromFile("arial.ttf"); // fallback if it fails

    generateCredentials();
    setupUI();
}

void WiFiLoginTask::generateCredentials() {
    std::vector<std::string> users   = {"CSEDU_Student","CSEDU_Admin","WiFi_User","Campus_Guest","Network_Member"};
    std::vector<std::string> prefs   = {"Secure_","Access_","Login_","Key_"};
    std::vector<std::string> sufs    = {"1234!","5678#","ABCD@","WXYZ$"};
    std::uniform_int_distribution<> ud(0, users.size()-1),
                                   pd(0, prefs.size()-1),
                                   sd(0, sufs.size()-1),
                                   nd(1000,9999);
    for (int i = 0; i < 5; ++i) {
        WiFiCredentials c;
        c.username = users[ud(rng)] + std::to_string(i+1);
        c.password = prefs[pd(rng)] + std::to_string(nd(rng)) + sufs[sd(rng)];
        creds.push_back(c);
    }
    std::shuffle(creds.begin(), creds.end(), rng);
}

WiFiCredentials WiFiLoginTask::currentCred() const {
    return creds[credIndex];
}

void WiFiLoginTask::nextCred() {
    credIndex = (credIndex + 1) % creds.size();
}

void WiFiLoginTask::setupUI() {
    auto mkText = [&](sf::Text& t, const std::string& s, unsigned sz, float x, float y){
        t.setFont(font);
        t.setString(s);
        t.setCharacterSize(sz);
        t.setFillColor(sf::Color::White);
        t.setPosition(x,y);
    };

    // Login screen header
    mkText(header, "Linux Wi-Fi Login", 30, w/2.f - 120, 50);

    // Login box
    loginBox = sf::RectangleShape({500,300});
    loginBox.setPosition(w/2.f - 250, 100);
    loginBox.setFillColor({40,40,40});
    loginBox.setOutlineThickness(2);

    // Username field
    mkText(userLabel, "Username:", 20, w/2.f - 230, 150);
    userField = sf::RectangleShape({300,40});
    userField.setPosition(w/2.f - 230, 180);
    mkText(userText, "", 18, w/2.f - 225, 185);
    userText.setFillColor(sf::Color::Black);    // input text in black

    // Password field
    mkText(passLabel, "Password:", 20, w/2.f - 230, 240);
    passField = sf::RectangleShape({300,40});
    passField.setPosition(w/2.f - 230, 270);
    mkText(passText, "", 18, w/2.f - 225, 275);
    passText.setFillColor(sf::Color::Black);    // masked password in black

    // Login button
    loginBtn = sf::RectangleShape({120,45});
    loginBtn.setPosition(w/2.f - 60, 330);
    loginBtn.setFillColor(sf::Color::Blue);
    mkText(btnText, "Login", 20, w/2.f - 30, 340);

    // Error text
    mkText(errText, "Invalid!", 16, w/2.f - 30, 385);
    errText.setFillColor(sf::Color::Red);

    // Info screen header
    mkText(infoHeader, "Wi-Fi Credentials", 30, w/2.f - 140, 50);

    // Info box
    infoBox = sf::RectangleShape({500,300});
    infoBox.setPosition(w/2.f - 250, 100);
    infoBox.setFillColor({30,30,30});
    infoBox.setOutlineThickness(2);

    // Displayed credentials
    mkText(infoUserLabel, "User:", 20, w/2.f - 230, 150);
    mkText(infoUserVal, "", 20, w/2.f - 230, 180);
    infoUserVal.setFillColor(sf::Color::Black); // credentials in black

    mkText(infoPassLabel, "Pass:", 20, w/2.f - 230, 230);
    mkText(infoPassVal, "", 20, w/2.f - 230, 260);
    infoPassVal.setFillColor(sf::Color::Black); // credentials in black

    mkText(doneText, "Success!", 24, w/2.f - 60, 330);
    mkText(exitText, "Press Enter to close", 18, w/2.f - 100, 370);
}

void WiFiLoginTask::start() {
    resetLogin();
    state = State::Login;
}

void WiFiLoginTask::resetLogin() {
    username.clear();
    password.clear();
    userText.setString("");
    passText.setString("");
    userActive = passActive = showError = false;
}

void WiFiLoginTask::handleEvent(const sf::Event& e) {
    // Escape always closes
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
        state = State::Inactive;
        return;
    }
    if (state == State::Inactive) return;

    // Enter key
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter) {
        if (state == State::Login) {
            attemptLogin();
        }
        else if (state == State::Info) {
            nextCred();
            state = State::Completed;
        }
        else if (state == State::Completed) {
            state = State::Inactive;
        }
        return;
    }

    // Mouse click (overlay-local)
    if (state == State::Login && e.type == sf::Event::MouseButtonPressed) {
        sf::Vector2f winMp(e.mouseButton.x, e.mouseButton.y);
        sf::Vector2f overlayPos = overlay.getPosition();
        sf::Vector2f mp = winMp - overlayPos;

        if (userField.getGlobalBounds().contains(mp)) {
            userActive = true;  passActive = false;
        }
        else if (passField.getGlobalBounds().contains(mp)) {
            passActive = true;  userActive = false;
        }
        return;
    }

    // Text entry
    if (state == State::Login && e.type == sf::Event::TextEntered) {
        char c = static_cast<char>(e.text.unicode);
        if (userActive) {
            if (c == 8 && !username.empty())       username.pop_back();
            else if (std::isprint(static_cast<unsigned char>(c)))
                                                   username.push_back(c);
            userText.setString(username);
        }
        else if (passActive) {
            if (c == 8 && !password.empty())       password.pop_back();
            else if (std::isprint(static_cast<unsigned char>(c)))
                                                   password.push_back(c);
            passText.setString(std::string(password.size(), '*'));
        }
    }
}

void WiFiLoginTask::attemptLogin() {
    if (username == "student" && password == "student") {
        auto cr = currentCred();
        infoUserVal.setString(cr.username);
        infoPassVal.setString(cr.password);
        state = State::Info;
        showError = false;
    } else {
        showError = true;
    }
}

void WiFiLoginTask::update(float /*dt*/) {
    // no per-frame updates
}

void WiFiLoginTask::draw(sf::RenderWindow& window) {
    if (state == State::Inactive) return;

    rt.clear({0,0,0,180});

    if (state == State::Login) {
        rt.draw(header);
        rt.draw(loginBox);
        rt.draw(userLabel);
        rt.draw(userField);
        rt.draw(userText);
        rt.draw(passLabel);
        rt.draw(passField);
        rt.draw(passText);
        rt.draw(loginBtn);
        rt.draw(btnText);
        if (showError) rt.draw(errText);
    }
    else if (state == State::Info) {
        rt.draw(infoHeader);
        rt.draw(infoBox);
        rt.draw(infoUserLabel);
        rt.draw(infoUserVal);
        rt.draw(infoPassLabel);
        rt.draw(infoPassVal);
        rt.draw(doneText);
        rt.draw(exitText);
    }
    else if (state == State::Completed) {
        state = State::Inactive;
    }

    rt.display();

    auto ws  = window.getSize();
    auto rsz = rt.getSize();
    overlay.setPosition((ws.x - rsz.x)/2.f, (ws.y - rsz.y)/2.f);
    window.draw(overlay);
}

bool WiFiLoginTask::isActive()   const { return state != State::Inactive; }
bool WiFiLoginTask::isFinished() const { return state == State::Info; }
