// IntegerOverflowOverlay.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

class IntegerOverflowOverlay {
public:
    enum class State { Inactive, InputPhase, SuccessPhase, FailurePhase };

    IntegerOverflowOverlay(unsigned width, unsigned height);

    void start();
    void handleEvent(const sf::Event& e);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isActive() const;
    bool isFinished() const;
    bool passed() const;

private:
    void setupBackground();
    void setupUI();
    void checkInput();
    bool isValidNumericInput(const std::string& s);
    bool isIntegerOverflow(const std::string& s);

    sf::RenderTexture              m_rt;
    sf::Sprite                     m_overlay;
    sf::Font                       font;

    State                          currentState;
    std::string                    inputText;
    bool                           taskPassed;

    // UI elements
    sf::RectangleShape             backgroundGradient;
    std::vector<sf::CircleShape>   backgroundCircles;
    sf::RectangleShape             titleBar;
    sf::Text                       messageText;
    sf::RectangleShape             inputBox;
    sf::Text                       inputDisplayText;

    sf::RectangleShape             usernameBox;
    sf::RectangleShape             passwordBox;
    sf::Text                       usernameLabel;
    sf::Text                       usernameText;
    sf::Text                       passwordLabel;
    sf::Text                       passwordText;
    sf::Text                       successMessage;
    sf::Text                       failureMessage;

    static constexpr long long     INT32_MAX_VALUE = 2147483647LL;
};
