#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <sstream>
#include <climits>

class IntegerOverflowTask {
private:
    sf::RenderWindow window;
    sf::Font font;
    
    // States
    enum State { INPUT_PHASE, SUCCESS_PHASE, FAILURE_PHASE };
    State currentState;
    
    // Input handling
    std::string inputText;
    
    // UI elements
    sf::RectangleShape inputBox;
    sf::RectangleShape usernameBox;
    sf::RectangleShape passwordBox;
    sf::Text messageText;
    sf::Text inputDisplayText;
    sf::Text usernameLabel;
    sf::Text usernameText;
    sf::Text passwordLabel;
    sf::Text passwordText;
    sf::Text successMessage;
    sf::Text failureMessage;
    
    // Background elements
    sf::RectangleShape backgroundGradient;
    std::vector<sf::CircleShape> backgroundCircles;
    sf::RectangleShape titleBar;
    
    const long long INT32_MAX_VALUE = 2147483647LL; // 2^31 - 1
    
public:
    IntegerOverflowTask()
    : window(sf::VideoMode(800, 600), "Integer Overflow Task")
    , currentState(INPUT_PHASE)
    {
        if (!font.loadFromFile("arial.ttf")) {
            std::cout << "Warning: Could not load font. Using default font.\n";
        }
        setupUI();
    }
    
    void setupUI() {
        setupBackground();
        
        // Title bar
        titleBar.setSize({800, 120});
        titleBar.setPosition(0, 0);
        titleBar.setFillColor(sf::Color(45, 52, 64, 200));
        
        // Input box
        inputBox.setSize({400, 50});
        inputBox.setPosition(200, 280);
        inputBox.setFillColor(sf::Color(255,255,255,240));
        inputBox.setOutlineColor(sf::Color(76,175,80));
        inputBox.setOutlineThickness(3);
        
        // Prompt text
        messageText.setFont(font);
        messageText.setString("Type as much as u can. I am bigger than ur thinking\n....Think bigger type bigger");
        messageText.setCharacterSize(24);
        messageText.setFillColor(sf::Color::White);
        messageText.setStyle(sf::Text::Bold);
        messageText.setPosition(80, 15);
        
        // Input display
        inputDisplayText.setFont(font);
        inputDisplayText.setCharacterSize(22);
        inputDisplayText.setFillColor(sf::Color(33,37,41));
        inputDisplayText.setPosition(210, 293);
        
        // Success UI
        usernameBox.setSize({320,45});
        usernameBox.setPosition(240,200);
        usernameBox.setFillColor(sf::Color(255,255,255,240));
        usernameBox.setOutlineColor(sf::Color(33,150,243));
        usernameBox.setOutlineThickness(3);
        
        passwordBox.setSize({320,45});
        passwordBox.setPosition(240,280);
        passwordBox.setFillColor(sf::Color(255,255,255,240));
        passwordBox.setOutlineColor(sf::Color(33,150,243));
        passwordBox.setOutlineThickness(3);
        
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
        passwordText.setString("924689093745");
        passwordText.setCharacterSize(20);
        passwordText.setFillColor(sf::Color(33,37,41));
        passwordText.setPosition(250,290);
        
        successMessage.setFont(font);
        successMessage.setString("Task passed!!! Press Enter to exit.");
        successMessage.setCharacterSize(22);
        successMessage.setFillColor(sf::Color(76,175,80));
        successMessage.setStyle(sf::Text::Bold);
        successMessage.setPosition(120,370);
        
        failureMessage.setFont(font);
        failureMessage.setString("Task failed!!!");
        failureMessage.setCharacterSize(24);
        failureMessage.setFillColor(sf::Color(244,67,54));
        failureMessage.setStyle(sf::Text::Bold);
        // center horizontally
        float x = (800 - failureMessage.getLocalBounds().width) / 2.f;
        failureMessage.setPosition(x, 300);
    }
    
    void setupBackground() {
        backgroundGradient.setSize({800,600});
        backgroundGradient.setPosition(0,0);
        
        backgroundCircles.clear();
        sf::CircleShape c1(80); c1.setPosition(-40,-40); c1.setFillColor(sf::Color(103,58,183,30)); backgroundCircles.push_back(c1);
        sf::CircleShape c2(60); c2.setPosition(700,50);   c2.setFillColor(sf::Color(233,30,99,25));  backgroundCircles.push_back(c2);
        sf::CircleShape c3(100);c3.setPosition(650,450);  c3.setFillColor(sf::Color(33,150,243,20)); backgroundCircles.push_back(c3);
        sf::CircleShape c4(45); c4.setPosition(50,450);   c4.setFillColor(sf::Color(76,175,80,35));  backgroundCircles.push_back(c4);
        sf::CircleShape c5(70); c5.setPosition(100,100);  c5.setFillColor(sf::Color(255,193,7,25));  backgroundCircles.push_back(c5);
    }
    
    bool isValidNumericInput(const std::string& s) {
        if (s.empty()) return false;
        size_t i = (s[0]=='+'||s[0]=='-')?1:0;
        if (i==1 && s.size()==1) return false;
        for (; i<s.size(); ++i) if (!isdigit(s[i])) return false;
        return true;
    }
    
    bool isIntegerOverflow(const std::string& s) {
        if (!isValidNumericInput(s)) return false;
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
    
    void handleInput(sf::Event& evt) {
        if (currentState == INPUT_PHASE) {
            if (evt.type == sf::Event::TextEntered) {
                if (evt.text.unicode == 8) { // backspace
                    if (!inputText.empty())
                        inputText.pop_back();
                }
                else if (evt.text.unicode == 13) { // enter
                    if (isValidNumericInput(inputText) && isIntegerOverflow(inputText))
                        currentState = SUCCESS_PHASE;
                    else
                        currentState = FAILURE_PHASE;
                }
                else if (evt.text.unicode >= 32 && evt.text.unicode < 127) {
                    inputText += static_cast<char>(evt.text.unicode);
                }
                inputDisplayText.setString(inputText);
            }
        }
        else {
            if (evt.type == sf::Event::KeyPressed && evt.key.code == sf::Keyboard::Enter) {
                window.close();
            }
        }
    }
    
    void render() {
        // background gradient
        sf::Vertex grad[] = {
            {{0,0},   sf::Color(67,56,202)},
            {{800,0}, sf::Color(79,70,229)},
            {{800,600},sf::Color(99,102,241)},
            {{0,600}, sf::Color(67,56,202)}
        };
        
        window.clear();
        window.draw(grad, 4, sf::Quads);
        for (auto& c : backgroundCircles) window.draw(c);
        
        if (currentState == INPUT_PHASE) {
            window.draw(titleBar);
            window.draw(messageText);
            window.draw(inputBox);
            window.draw(inputDisplayText);
        } else {
            // semi-transparent overlay
            sf::RectangleShape ov({800,600});
            ov.setFillColor(sf::Color(0,0,0,50));
            window.draw(ov);
            for (auto& c : backgroundCircles) window.draw(c);
            
            if (currentState == SUCCESS_PHASE) {
                window.draw(usernameLabel);
                window.draw(usernameBox);
                window.draw(usernameText);
                window.draw(passwordLabel);
                window.draw(passwordBox);
                window.draw(passwordText);
                window.draw(successMessage);
            } else { // FAILURE_PHASE
                window.draw(failureMessage);
            }
        }
        
        window.display();
    }
    
    void run() {
        while (window.isOpen()) {
            sf::Event evt;
            while (window.pollEvent(evt)) {
                if (evt.type == sf::Event::Closed)
                    window.close();
                handleInput(evt);
            }
            render();
        }
    }
};

int main() {
    IntegerOverflowTask task;
    task.run();
    return 0;
}
