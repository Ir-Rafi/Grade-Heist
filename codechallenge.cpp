/* codechallenge.cpp */
#include "codechallenge.h"
#include <iostream>

CodeChallenge::CodeChallenge(unsigned w, unsigned h)
: m_rt(), currentState(CHALLENGE_INACTIVE), attemptsLeft(2), shouldAutoClose(false), gameWon(false),
  correctAnswer("7.6"),
  backgroundColor(40, 44, 52), primaryColor(97, 218, 251), successColor(152, 195, 121),
  errorColor(224, 108, 117), codeBackgroundColor(30, 34, 40)
{
    // Prepare off-screen buffer
    if (!m_rt.create(w, h)) {
        std::cerr << "Error: Failed to create render texture for CodeChallenge overlay.\n";
    }
    m_overlay.setTexture(m_rt.getTexture());
    // Initial position; will be centered each draw
    m_overlay.setPosition(0.f, 0.f);

    // Define the challenge code
    challengeCode =
        "#include <iostream>\n"
        "using namespace std;\n"
        "#define ll long long\n\n"
        "int main() {\n"
        "    float i = 2.8000006;\n"
        "    i = i++ + ++i;\n"
        "    cout << i << endl;\n"
        "    return 0;\n"
        "}";

    initializeUI();
}

void CodeChallenge::start() {
    currentState = CHALLENGE_ACTIVE;
    attemptsLeft = 2;
    shouldAutoClose = false;
    gameWon = false;
    userInput.clear();
    updateAttemptsText();
    resultText.setString("");
    correctAnswerText.setString("");
}

void CodeChallenge::handleEvent(const sf::Event& evt) {
    // -- ESC closes the overlay at any time --
    if (evt.type == sf::Event::KeyPressed &&
        evt.key.code == sf::Keyboard::Escape)
    {
        currentState = CHALLENGE_INACTIVE;
        return;
    }

    if (currentState != CHALLENGE_ACTIVE) return;

    switch (evt.type) {
        case sf::Event::TextEntered:
            processTextInput(evt.text.unicode);
            break;
        case sf::Event::KeyPressed:
            handleKeyPress(evt.key.code);
            break;
        default:
            break;
    }
}

void CodeChallenge::update(float dt) {
    if (currentState != CHALLENGE_ACTIVE) return;
    if (shouldAutoClose && autoCloseTimer.getElapsedTime().asSeconds() >= 2.0f) {
        currentState = CHALLENGE_FINISHED;
    }
}

void CodeChallenge::draw(sf::RenderWindow& window) {
    if (currentState == CHALLENGE_INACTIVE) return;

    // Clear and draw UI into the off-screen buffer
    m_rt.clear(backgroundColor);
    m_rt.draw(codeBackground);
    m_rt.draw(inputBackground);
    m_rt.draw(titleText);
    m_rt.draw(instructionText);
    m_rt.draw(codeText);
    m_rt.draw(inputPromptText);
    m_rt.draw(userInputText);
    m_rt.draw(attemptsText);
    m_rt.draw(resultText);
    m_rt.draw(correctAnswerText);

    if (shouldAutoClose) {
        sf::Text autoCloseText;
        autoCloseText.setFont(font);
        autoCloseText.setString("Window will auto-close in 2 seconds...");
        autoCloseText.setCharacterSize(16);
        autoCloseText.setFillColor(sf::Color::White);
        autoCloseText.setPosition(20.f, m_rt.getSize().y - 30.f);
        m_rt.draw(autoCloseText);
    }

    m_rt.display();

    // Center the overlay on the window
    sf::Vector2u winSize = window.getSize();
    sf::Vector2u rtSize  = m_rt.getSize();
    m_overlay.setPosition(
        (winSize.x - rtSize.x) * 0.5f,
        (winSize.y - rtSize.y) * 0.5f
    );

    // Draw the centered overlay
    window.draw(m_overlay);
}

bool CodeChallenge::isFinished() const {
    return currentState == CHALLENGE_FINISHED;
}

bool CodeChallenge::isActive() const {
    return currentState == CHALLENGE_ACTIVE;
}

void CodeChallenge::initializeUI() {
    // Load font
    if (!font.loadFromFile("arial.ttf")) {
        std::cerr << "Warning: Could not load arial.ttf\n";
    }

    // Title
    titleText.setFont(font);
    titleText.setString("C++ Code Output Challenge");
    titleText.setCharacterSize(24);
    titleText.setFillColor(primaryColor);
    titleText.setPosition(20.f, 20.f);

    // Instructions
    instructionText.setFont(font);
    instructionText.setString("Guess the output. 2 attempts!");
    instructionText.setCharacterSize(16);
    instructionText.setFillColor(sf::Color::White);
    instructionText.setPosition(20.f, 60.f);

    // Code display
    codeText.setFont(font);
    codeText.setString(challengeCode);
    codeText.setCharacterSize(12);
    codeText.setFillColor(sf::Color(171,178,191));
    codeText.setPosition(20.f, 100.f);

    // Code background
    codeBackground.setSize(sf::Vector2f(m_rt.getSize().x - 40.f, 200.f));
    codeBackground.setPosition(20.f, 90.f);
    codeBackground.setFillColor(codeBackgroundColor);
    codeBackground.setOutlineThickness(2.f);
    codeBackground.setOutlineColor(primaryColor);

    // Input prompt
    inputPromptText.setFont(font);
    inputPromptText.setString("Enter your answer:");
    inputPromptText.setCharacterSize(16);
    inputPromptText.setFillColor(sf::Color::White);
    inputPromptText.setPosition(20.f, 310.f);

    // Input background
    inputBackground.setSize(sf::Vector2f(200.f, 30.f));
    inputBackground.setPosition(175.f, 305.f);
    inputBackground.setFillColor(sf::Color(60,64,72));
    inputBackground.setOutlineThickness(2.f);
    inputBackground.setOutlineColor(primaryColor);

    // User input display
    userInputText.setFont(font);
    userInputText.setCharacterSize(16);
    userInputText.setFillColor(sf::Color::White);
    userInputText.setPosition(
        inputBackground.getPosition().x + 5.f,
        inputBackground.getPosition().y + 5.f
    );
    userInputText.setString("_");

    // Attempts text
    attemptsText.setFont(font);
    attemptsText.setCharacterSize(14);
    attemptsText.setFillColor(sf::Color::Yellow);
    attemptsText.setPosition(20.f, 350.f);
    updateAttemptsText();

    // Result text
    resultText.setFont(font);
    resultText.setCharacterSize(18);
    resultText.setPosition(20.f, 380.f);

    // Correct answer text
    correctAnswerText.setFont(font);
    correctAnswerText.setCharacterSize(16);
    correctAnswerText.setPosition(20.f, 420.f);
}

void CodeChallenge::updateAttemptsText() {
    attemptsText.setString("Attempts left: " + std::to_string(attemptsLeft));
}

void CodeChallenge::processTextInput(sf::Uint32 unicode) {
    if (currentState != CHALLENGE_ACTIVE) return;
    if (unicode == 8) { // Backspace
        if (!userInput.empty()) userInput.pop_back();
    } else if (unicode == 13) { // Enter
        if (!userInput.empty()) {
            if (userInput == correctAnswer) {
                gameWon = true;
                resultText.setString("🎉 Correct! Press ESC to close.");
                resultText.setFillColor(successColor);
                currentState = CHALLENGE_FINISHED;
            } else {
                attemptsLeft--;
                updateAttemptsText();
                if (attemptsLeft <= 0) {
                    resultText.setString("❌ Game Over");
                    resultText.setFillColor(errorColor);
                    shouldAutoClose = true;
                    autoCloseTimer.restart();
                } else {
                    resultText.setString("❌ Wrong! Try again.");
                    resultText.setFillColor(errorColor);
                }
            }
            userInput.clear();
        }
    } else if (unicode >= 32 && unicode < 127) {
        userInput.push_back(static_cast<char>(unicode));
    }
    userInputText.setString(userInput + "_");
}

void CodeChallenge::handleKeyPress(sf::Keyboard::Key key) {
    // (Escape handled earlier in handleEvent)
    if (currentState == CHALLENGE_FINISHED && key == sf::Keyboard::Enter) {
        // Optionally restart or handle Enter at finish
    }
}
