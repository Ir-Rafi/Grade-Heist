#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdlib>
#include <iostream>
#include <vector>

// --- Menu choices ---
enum MenuChoice {
    BeginHeist    = 0,
    GradeList     = 1,
    AboutCreators = 2,
};

// --- Forward declarations ---
void showGameScreen(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& backSprite);
void showLeaderboardScreen(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& backSprite);
void showAboutScreen(sf::RenderWindow& window, const sf::Sprite& aboutSprite, const sf::Sprite& backSprite);

// --- 1) Splash screen ---
void showSplash(sf::RenderWindow& window,
                const sf::Sprite& splashSprite,
                sf::Text& pressText)
{
    float pulse = 1.0f;
    bool growing = true;
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) window.close();
            if (e.type == sf::Event::MouseButtonPressed ||
                e.type == sf::Event::KeyPressed)
                return;
        }
        pulse += (growing ? 0.004f : -0.004f);
        if (pulse >= 1.1f) growing = false;
        if (pulse <= 0.9f) growing = true;
        pressText.setScale(pulse, pulse);

        window.clear();
        window.draw(splashSprite);
        window.draw(pressText);
        window.display();
    }
}

// --- 2) Menu with texture-space clicks ---
MenuChoice showImageMenu(sf::RenderWindow& window,
                         const sf::Texture& menuTex)
{
    sf::Sprite menuSprite(menuTex);
    menuSprite.setPosition(0, 0);
    sf::Vector2f scale(
        float(window.getSize().x) / menuTex.getSize().x,
        float(window.getSize().y) / menuTex.getSize().y
    );
    menuSprite.setScale(scale);

    // button bounds in texture coordinates
    sf::IntRect beginRect(876, 356, 573, 138);
    sf::IntRect gradeRect(950, 533, 445, 134);
    sf::IntRect aboutRect(909, 708, 505, 126);

    while (true) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) std::exit(0);

            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mp{e.mouseButton.x, e.mouseButton.y};
                sf::Vector2f world = window.mapPixelToCoords(mp);
                sf::Vector2f local = world - menuSprite.getPosition();
                sf::Vector2i tex{
                    int(local.x / scale.x),
                    int(local.y / scale.y)
                };
                if (beginRect.contains(tex))   return BeginHeist;
                if (gradeRect.contains(tex))   return GradeList;
                if (aboutRect.contains(tex))   return AboutCreators;
            }
        }
        window.clear();
        window.draw(menuSprite);
        window.display();
    }
}

// --- 3a) Game screen (with back button) ---
void showGameScreen(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& backSprite) {
    sf::Text t("Game would start here...", font, 36);
    t.setPosition(50, 50);

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mp{e.mouseButton.x, e.mouseButton.y};
                sf::Vector2f world = window.mapPixelToCoords(mp);
                if (backSprite.getGlobalBounds().contains(world))
                    return;  // back to menu
            }
        }

        window.clear(sf::Color::Black);
        window.draw(t);
        window.draw(backSprite);
        window.display();
    }
}

// --- 3b) Leaderboard screen (with back button) ---
void showLeaderboardScreen(sf::RenderWindow& window, const sf::Font& font, const sf::Sprite& backSprite) {
    std::vector<std::string> scores = {
        "1. Alice – 12345",
        "2. Bob   –  9876",
        "3. Carol –  5432",
    };
    std::vector<sf::Text> lines;
    for (size_t i = 0; i < scores.size(); ++i) {
        sf::Text line(scores[i], font, 28);
        line.setPosition(50, 50 + i * 40);
        lines.push_back(line);
    }

    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mp{e.mouseButton.x, e.mouseButton.y};
                sf::Vector2f world = window.mapPixelToCoords(mp);
                if (backSprite.getGlobalBounds().contains(world))
                    return;  // back to menu
            }
        }

        window.clear(sf::Color(30, 30, 30));
        for (auto& line : lines) window.draw(line);
        window.draw(backSprite);
        window.display();
    }
}

// --- 3c) About screen (image + back button) ---
void showAboutScreen(sf::RenderWindow& window, const sf::Sprite& aboutSprite, const sf::Sprite& backSprite) {
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (e.type == sf::Event::MouseButtonPressed &&
                e.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2i mp{e.mouseButton.x, e.mouseButton.y};
                sf::Vector2f world = window.mapPixelToCoords(mp);
                if (backSprite.getGlobalBounds().contains(world))
                    return;  // back to menu
            }
        }

        window.clear(sf::Color(30, 30, 30));
        window.draw(aboutSprite);
        window.draw(backSprite);
        window.display();
    }
}

// --- main entry point ---
int main(){
    const int W = 1200, H = 800;

    // Load & play music
    sf::Music bgMusic;
    if (!bgMusic.openFromFile("gameplay_begin.mp3")) {
        std::cerr << "Failed to load music\n"; 
        return -1;
    }
    bgMusic.setLoop(true);
    bgMusic.setVolume(100);
    bgMusic.play();

    // Load splash & font
    sf::Texture splashTex;
    if (!splashTex.loadFromFile("grade_heist.png")) {
        std::cerr << "Failed to load splash\n"; 
        return -1;
    }
    sf::Font font;
    if (!font.loadFromFile("ComicNeue-Bold.ttf")) {
        std::cerr << "Failed to load font\n"; 
        return -1;
    }

    // Load menu background
    sf::Texture menuTex;
    if (!menuTex.loadFromFile("menu_background.png")) {
        std::cerr << "Failed to load menu image\n"; 
        return -1;
    }

    // Load about image
    sf::Texture aboutTex;
    if (!aboutTex.loadFromFile("about.png")) {
        std::cerr << "Failed to load about.png\n"; 
        return -1;
    }
    sf::Sprite aboutSprite(aboutTex);
    aboutSprite.setScale(
        float(W) / aboutTex.getSize().x,
        float(H) / aboutTex.getSize().y
    );

    // Load back button image
    sf::Texture backTex;
    if (!backTex.loadFromFile("back.png")) {
        std::cerr << "Failed to load back.jpeg\n";
        return -1;
    }
    sf::Sprite backSprite(backTex);
    // position in top-left corner
    backSprite.setPosition(1130.f, 01.f);
    // (optional) scale if you want it smaller than 225×225:
     float scaleFactor = 80.f / backTex.getSize().x;
     backSprite.setScale(scaleFactor, scaleFactor);

    // Create the main window
    sf::RenderWindow window({ W, H }, "Grade Heist");
    window.setFramerateLimit(60);

    // Prepare splash sprite
    sf::Sprite splashSprite(splashTex);
    splashSprite.setScale(float(W)/splashTex.getSize().x,
                          float(H)/splashTex.getSize().y);

    // Prepare "Press Anywhere" text
    sf::Text pressText("Press Anywhere To Start", font, 48);
    pressText.setFillColor(sf::Color(139, 57, 17));
    pressText.setOutlineColor(sf::Color::Black);
    pressText.setOutlineThickness(3);
    {
        auto b = pressText.getLocalBounds();
        pressText.setOrigin(b.left + b.width/2, 0);
        pressText.setPosition(W/2.f, H - 80.f);
    }

    // 1) Splash
    showSplash(window, splashSprite, pressText);

    // 2) Menu & choice
    MenuChoice choice = showImageMenu(window, menuTex);

    // 3) Dispatch selected screen
    switch (choice) {
        case BeginHeist:
            showGameScreen(window, font, backSprite);
            break;
        case GradeList:
            showLeaderboardScreen(window, font, backSprite);
            break;
        case AboutCreators:
            showAboutScreen(window, aboutSprite, backSprite);
            break;
    }

    return 0;
}
//g++ *.cpp -lsfml-graphics -lsfml-window -lsfml-system