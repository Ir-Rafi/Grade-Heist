// ComputerTaskOverlay.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <random>

class ComputerTaskOverlay {
public:
    enum class State { Inactive, Playing, Completed };

    ComputerTaskOverlay(unsigned width, unsigned height);

    void start();
    void handleEvent(const sf::Event& e);
    void update(float dt);
    void draw(sf::RenderWindow& window);

    bool isActive() const;
    bool isFinished() const;
    bool passed() const;

private:
    void setupUI();
    void pickNewTask();
    void checkTaskCompletion();

    sf::RenderTexture m_rt;
    sf::Sprite       m_overlay;

    sf::Font          font;
    sf::Text          titleText;
    sf::Text          taskText;
    sf::Text          promptText;
    sf::Text          inputText;
    sf::Text          resultText;
    sf::Text          exitText;
    sf::RectangleShape screenBorder;
    sf::RectangleShape inputBox;
    sf::Color         backgroundColor;

    std::vector<std::string> taskLines;
    std::string              currentTask;
    std::string              userInput;

    std::mt19937      rng;
    State             currentState;
    bool              taskPassed;
};
