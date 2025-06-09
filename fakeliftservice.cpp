#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>

class Task {
public:
    virtual ~Task() = default;
    virtual void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) = 0;
    virtual void update() = 0;
    virtual void draw(sf::RenderWindow& window) = 0;
    virtual bool isCompleted() const = 0;
    virtual std::string getName() const = 0;
};

class WaterSupplyTask : public Task {
private:
    sf::RectangleShape background;
    sf::RectangleShape switchButton;
    sf::Text titleText;
    sf::Text statusText;
    sf::Text instructionText;
    sf::Font font;
    bool isOn;
    bool completed;
    
public:
    WaterSupplyTask() : isOn(true), completed(false) {
        // Load font (you might need to provide a font file path)
        if (!font.loadFromFile("arial.ttf")) {
            // If font loading fails, use default font
            std::cout << "Warning: Could not load font file\n";
        }
        
        // Setup background
        background.setSize(sf::Vector2f(400, 300));
        background.setPosition(50, 50);
        background.setFillColor(sf::Color(40, 60, 120));
        background.setOutlineThickness(3);
        background.setOutlineColor(sf::Color::White);
        
        // Setup switch button
        switchButton.setSize(sf::Vector2f(80, 40));
        switchButton.setPosition(210, 200);
        updateSwitchAppearance();
        
        // Setup texts
        titleText.setFont(font);
        titleText.setString("WATER SUPPLY CONTROL");
        titleText.setCharacterSize(24);
        titleText.setFillColor(sf::Color::White);
        titleText.setPosition(60, 70);
        
        statusText.setFont(font);
        statusText.setCharacterSize(18);
        statusText.setFillColor(sf::Color::Yellow);
        statusText.setPosition(150, 130);
        updateStatusText();
        
        instructionText.setFont(font);
        instructionText.setString("Click the switch to turn OFF water supply");
        instructionText.setCharacterSize(14);
        instructionText.setFillColor(sf::Color(200, 200, 200));
        instructionText.setPosition(80, 280);
    }
    
    void updateSwitchAppearance() {
        if (isOn) {
            switchButton.setFillColor(sf::Color::Green);
            switchButton.setOutlineColor(sf::Color(0, 150, 0));
        } else {
            switchButton.setFillColor(sf::Color::Red);
            switchButton.setOutlineColor(sf::Color(150, 0, 0));
        }
        switchButton.setOutlineThickness(2);
    }
    
    void updateStatusText() {
        if (isOn) {
            statusText.setString("Water Supply: ON");
            statusText.setFillColor(sf::Color::Green);
        } else {
            statusText.setString("Water Supply: OFF");
            statusText.setFillColor(sf::Color::Red);
        }
    }
    
    void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) override {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (switchButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && isOn) {
                isOn = false;
                completed = true;
                updateSwitchAppearance();
                updateStatusText();
                instructionText.setString("Task Completed!");
                instructionText.setFillColor(sf::Color::Green);
            }
        }
    }
    
    void update() override {
        // Animation or other updates can go here
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(background);
        window.draw(switchButton);
        window.draw(titleText);
        window.draw(statusText);
        window.draw(instructionText);
    }
    
    bool isCompleted() const override {
        return completed;
    }
    
    std::string getName() const override {
        return "Off the Water Supply";
    }
};

class LiftServiceTask : public Task {
private:
    sf::RectangleShape background;
    sf::RectangleShape switchButton;
    sf::RectangleShape statusScreen;
    sf::Text titleText;
    sf::Text statusText;
    sf::Text instructionText;
    sf::Text switchLabelText;
    sf::Font font;
    bool isOn;
    bool completed;
    
public:
    LiftServiceTask() : isOn(true), completed(false) {
        // Load font
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load font file\n";
        }
        
        // Setup background with different color scheme
        background.setSize(sf::Vector2f(400, 300));
        background.setPosition(500, 50);
        background.setFillColor(sf::Color(60, 40, 80));
        background.setOutlineThickness(3);
        background.setOutlineColor(sf::Color::Cyan);
        
        // Setup status screen (small screen above button)
        statusScreen.setSize(sf::Vector2f(180, 50));
        statusScreen.setPosition(610, 140);
        statusScreen.setFillColor(sf::Color::Black);
        statusScreen.setOutlineThickness(2);
        statusScreen.setOutlineColor(sf::Color::Green);
        
        // Setup switch button (different shape - circular-ish)
        switchButton.setSize(sf::Vector2f(60, 60));
        switchButton.setPosition(670, 210);
        updateSwitchAppearance();
        
        // Setup texts
        titleText.setFont(font);
        titleText.setString("LIFT SERVICE CONTROL");
        titleText.setCharacterSize(24);
        titleText.setFillColor(sf::Color::Cyan);
        titleText.setPosition(520, 70);
        
        statusText.setFont(font);
        statusText.setCharacterSize(14);
        statusText.setFillColor(sf::Color::White);
        statusText.setPosition(620, 155);
        updateStatusText();
        
        switchLabelText.setFont(font);
        switchLabelText.setString("POWER");
        switchLabelText.setCharacterSize(12);
        switchLabelText.setFillColor(sf::Color::White);
        switchLabelText.setPosition(680, 280);
        
        instructionText.setFont(font);
        instructionText.setString("Click the power button to turn OFF lift service");
        instructionText.setCharacterSize(14);
        instructionText.setFillColor(sf::Color(200, 200, 200));
        instructionText.setPosition(520, 320);
    }
    
    void updateSwitchAppearance() {
        if (isOn) {
            switchButton.setFillColor(sf::Color(0, 255, 100));
            switchButton.setOutlineColor(sf::Color(0, 200, 50));
        } else {
            switchButton.setFillColor(sf::Color(80, 80, 80));
            switchButton.setOutlineColor(sf::Color(40, 40, 40));
        }
        switchButton.setOutlineThickness(3);
    }
    
    void updateStatusText() {
        if (isOn) {
            statusText.setString("Lift Service is ON");
            statusText.setFillColor(sf::Color::Green);
            statusScreen.setOutlineColor(sf::Color::Green);
        } else {
            statusText.setString("Lift Service is OFF");
            statusText.setFillColor(sf::Color::Red);
            statusScreen.setOutlineColor(sf::Color::Red);
        }
    }
    
    void handleEvent(const sf::Event& event, const sf::Vector2i& mousePos) override {
        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            if (switchButton.getGlobalBounds().contains(mousePos.x, mousePos.y) && isOn) {
                isOn = false;
                completed = true;
                updateSwitchAppearance();
                updateStatusText();
                instructionText.setString("Task Completed!");
                instructionText.setFillColor(sf::Color::Green);
            }
        }
    }
    
    void update() override {
        // Animation or other updates can go here
    }
    
    void draw(sf::RenderWindow& window) override {
        window.draw(background);
        window.draw(statusScreen);
        window.draw(switchButton);
        window.draw(titleText);
        window.draw(statusText);
        window.draw(switchLabelText);
        window.draw(instructionText);
    }
    
    bool isCompleted() const override {
        return completed;
    }
    
    std::string getName() const override {
        return "Off the Lift Service";
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(1000, 400), "Fake Tasks - Water Supply & Lift Service");
    
    WaterSupplyTask waterTask;
    LiftServiceTask liftTask;
    
    while (window.isOpen()) {
        sf::Event event;
        sf::Vector2i mousePos = sf::Mouse::getPosition(window);
        
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            
            waterTask.handleEvent(event, mousePos);
            liftTask.handleEvent(event, mousePos);
        }
        
        waterTask.update();
        liftTask.update();
        
        window.clear(sf::Color(20, 20, 30));
        
        waterTask.draw(window);
        liftTask.draw(window);
        
        window.display();
    }
    
    return 0;
}