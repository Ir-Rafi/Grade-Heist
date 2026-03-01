#pragma once

#include <SFML/Graphics.hpp>
#include <vector>

class PhotocopierTask {
public:
    // w,h = overlay size
    PhotocopierTask(unsigned w, unsigned h);

    // Called when player clicks the button on a "photocopier" tile
    void start();

    // Forward SFML events from your main loop
    void handleEvent(const sf::Event& e);

    // Update per-frame logic (background animation, printing progress)
    void update(float dt);

    // Draw the overlay into your existing RenderWindow
    void draw(sf::RenderWindow& window);

    // Queries for your main loop
    bool isActive()   const;  // overlay showing (either playing or completed)
    bool isFinished() const;  // task done, waiting to be dismissed

private:
    enum class State {
        Inactive,
        WaitingForInsert,
        Printing,
        WaitingForRemove,
        TaskCompleted
    };
    State state;

    // Off-screen buffer + sprite
    sf::RenderTexture    m_rt;
    sf::Sprite           m_overlay;

    // Shared font
    sf::Font             font;

    // Copier UI
    sf::RectangleShape   copier;
    sf::RectangleShape   copierSlot;
    sf::Text             copierLabel;

    // Pages
    struct Page {
        sf::RectangleShape shape;
        bool               isInCopier    = false;
        bool               isProcessed   = false;
        sf::Vector2f       originalPosition;
        int                pageNumber    = 0;
    };
    std::vector<Page>    pages;
    std::vector<sf::Text> pageLabels;

    // Progress bar
    sf::RectangleShape   barFrame;
    sf::RectangleShape   barFill;

    // Text prompts
    sf::Text             instrText;
    sf::Text             timerText;
    sf::Text             pageCountText;
    sf::Text             completionText;

    // Background particles
    std::vector<sf::CircleShape> backgroundParticles;
    sf::Clock                   particleClock;

    // Drag & print logic
    int       draggedPageIndex     = -1;
    int       currentPageInCopier   = -1;
    int       pagesProcessed       = 0;
    sf::Clock printClock;
    sf::Clock completionClock;

    // Helpers
    void setupUI();
    void resetPages();
    void updateBackground(float dt);
    void updateStateLogic();
};
