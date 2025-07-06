#pragma once

#include <SFML/Graphics.hpp>
#include <string>

class GradeChangerOverlay {
public:
    // Construct with overlay width & height
    GradeChangerOverlay(unsigned w, unsigned h);

    // Start the overlay (when player steps on the tile)
    void start();

    // Forward SFML events
    void handleEvent(const sf::Event& e);

    // Per-frame update (handles auto-close timing)
    void update(float dt);

    // Draw onto your main RenderWindow
    void draw(sf::RenderWindow& window);

    // State queries
    bool isActive()   const;  // true during login or grade phases
    bool isFinished() const;  // true after grade submission, before auto-close
    bool passed()     const;  // true if grade was successfully changed

private:
    enum Phase { Inactive, Login, Grade, Completed };
    Phase            currentPhase;

    // Offscreen render-texture & overlay sprite
    sf::RenderTexture m_rt;
    sf::Sprite        m_overlay;

    // Shared font
    sf::Font          font;

    // --- LOGIN UI ---
    sf::Text           loginTitleText;
    sf::Text           usernameLabel;
    sf::Text           passwordLabel;
    sf::RectangleShape usernameBox;
    sf::RectangleShape passwordBox;
    sf::RectangleShape loginButton;
    sf::Text           usernameText;
    sf::Text           passwordText;
    sf::Text           loginButtonText;
    sf::Text           loginResultText;
    std::string        loginUsername;
    std::string        loginPassword;
    bool               usernameActive;
    bool               passwordActive;
    bool               loginSuccess;

    // --- GRADE-CHANGE UI ---
    sf::Text           gradeTitleText;
    sf::Text           nameLabel;
    sf::Text           gradeLabel;
    sf::RectangleShape nameBox;
    sf::RectangleShape gradeBox;
    sf::RectangleShape submitButton;
    sf::Text           nameText;
    sf::Text           gradeText;
    sf::Text           submitButtonText;
    sf::Text           submitResultText;
    std::string        studentName;
    std::string        studentGrade;
    bool               nameActive;
    bool               gradeActive;
    bool               gradeChanged;

    // Auto-close timer
    sf::Clock          closeClock;
    float              closeDelay;
    bool               shouldClose;

    // UI setup
    void setupUI();
    void setupLoginUI();
    void setupGradeUI();

    // Input handling
    void handleLoginTextInput(char c);
    void handleGradeTextInput(char c);
    void handleLoginMouseClick(sf::Vector2i pos);
    void handleGradeMouseClick (sf::Vector2i pos);

    // Actions
    void checkLogin();
    void submitGradeChange();
};
