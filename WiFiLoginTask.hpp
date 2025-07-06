#ifndef WIFI_LOGIN_TASK_HPP
#define WIFI_LOGIN_TASK_HPP

#include <SFML/Graphics.hpp>
#include <vector>
#include <string>
#include <random>

struct WiFiCredentials {
    std::string username;
    std::string password;
};

class WiFiLoginTask {
public:
    WiFiLoginTask(unsigned width, unsigned height);

    // Trigger the login UI
    void start();

    // Handle SFML events while active
    void handleEvent(const sf::Event& e);

    // Update per-frame (dt in seconds)
    void update(float dt);

    // Draw the overlay
    void draw(sf::RenderWindow& window);

    bool isActive()   const;
    bool isFinished() const;

private:
    enum class State { Inactive, Login, Info, Completed };

    // UI & credential helpers
    void setupUI();
    void generateCredentials();
    void attemptLogin();
    void nextCred();
    void resetLogin();

    // Current credential
    WiFiCredentials currentCred() const;

    // Members
    State               state;
    unsigned            w, h;
    sf::RenderTexture   rt;
    sf::Sprite          overlay;
    sf::Font            font;

    // Login screen
    sf::Text            header;
    sf::RectangleShape  loginBox;
    sf::Text            userLabel, passLabel;
    sf::RectangleShape  userField, passField, loginBtn;
    sf::Text            userText, passText, btnText, errText;

    // Info screen
    sf::Text            infoHeader, infoUserLabel, infoPassLabel;
    sf::Text            infoUserVal, infoPassVal, doneText, exitText;
    sf::RectangleShape  infoBox;

    // Input
    std::string         username, password;
    bool                userActive, passActive, showError;

    // Credentials pool
    std::vector<WiFiCredentials> creds;
    std::mt19937                  rng;
    int                           credIndex;
};

#endif // WIFI_LOGIN_TASK_HPP
