#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iomanip>
#include <sstream>
#include <vector>
#include <cmath>

enum class TaskState {
    WaitingForInsert,
    Printing,
    WaitingForRemove,
    Failure,
    Success,
    TaskCompleted
};

struct Page {
    sf::RectangleShape shape;
    bool isInCopier;
    bool isProcessed;
    sf::Vector2f originalPosition;
    int pageNumber;
};

int main() {
    sf::RenderWindow window({1000, 700}, "Advanced Photocopier Task - 3 Pages");
    window.setFramerateLimit(60);

    // Load font
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) {
        return -1;
    }

    // Copier
    sf::RectangleShape copier({200, 150});
    copier.setFillColor(sf::Color(100, 100, 100));
    copier.setOutlineColor(sf::Color(150, 150, 150));
    copier.setOutlineThickness(3);
    copier.setPosition(400, 250);

    sf::RectangleShape copierSlot({160, 110});
    copierSlot.setFillColor(sf::Color(50, 50, 50));
    copierSlot.setPosition(420, 270);

    sf::Text copierLabel("COPIER", font, 16);
    copierLabel.setFillColor(sf::Color::White);
    copierLabel.setPosition(450, 220);

    // Pages
    std::vector<Page> pages(3);
    sf::Vector2f startPositions[3] = {{50,450},{150,450},{250,450}};
    sf::Color pageColors[3] = {
        sf::Color::White,
        sf::Color(255,255,200),
        sf::Color(200,255,200)
    };
    for (int i = 0; i < 3; i++) {
        pages[i].shape.setSize({80,100});
        pages[i].shape.setPosition(startPositions[i]);
        pages[i].originalPosition = startPositions[i];
        pages[i].isInCopier = pages[i].isProcessed = false;
        pages[i].pageNumber = i+1;
        pages[i].shape.setFillColor(pageColors[i]);
        pages[i].shape.setOutlineColor(sf::Color::Black);
        pages[i].shape.setOutlineThickness(2);
    }
    std::vector<sf::Text> pageLabels(3);
    for (int i = 0; i < 3; i++) {
        pageLabels[i].setFont(font);
        pageLabels[i].setString("Page " + std::to_string(i+1));
        pageLabels[i].setCharacterSize(14);
        pageLabels[i].setFillColor(sf::Color::White);
        pageLabels[i].setPosition(startPositions[i].x, startPositions[i].y - 25);
    }

    // Progress bar
    sf::RectangleShape barFrame({220,26});
    barFrame.setFillColor(sf::Color::Transparent);
    barFrame.setOutlineColor(sf::Color(200,200,200));
    barFrame.setOutlineThickness(2);
    barFrame.setPosition(390,450);

    sf::RectangleShape barFill({0,22});
    barFill.setFillColor(sf::Color(100,250,100));
    barFill.setPosition(barFrame.getPosition() + sf::Vector2f(2,2));

    // Text elements
    sf::Text instr("", font, 20);
    instr.setFillColor(sf::Color::White);
    instr.setPosition(300,150);

    sf::Text timerText("", font, 18);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(620,450);

    sf::Text pageCountText("", font, 18);
    pageCountText.setFillColor(sf::Color::Cyan);
    pageCountText.setPosition(50,50);

    sf::Text completionText("TASK COMPLETED!(4)", font, 48);
    completionText.setFillColor(sf::Color::Green);
    completionText.setPosition(300,300);

    // Background particles
    std::vector<sf::CircleShape> backgroundParticles;
    sf::Clock particleClock;
    {
        for (int i = 0; i < 20; i++) {
            sf::CircleShape p(2 + rand()%4);
            p.setPosition(rand()%1000, rand()%700);
            p.setFillColor(sf::Color(
                100+rand()%100,
                100+rand()%100,
                150+rand()%100,
                100+rand()%100
            ));
            backgroundParticles.push_back(p);
        }
    }

    // State
    TaskState state = TaskState::WaitingForInsert;
    int draggedPageIndex = -1;
    int currentPageInCopier = -1;
    int pagesProcessed = 0;
    sf::Clock printClock;
    sf::Clock completionClock;

    while (window.isOpen()) {
        // Events
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            // Exit on Enter after fail or completion
            if (ev.type == sf::Event::KeyPressed && ev.key.code == sf::Keyboard::Enter) {
                if (state == TaskState::Failure || state == TaskState::TaskCompleted) {
                    window.close();
                }
            }

            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mpos(ev.mouseButton.x, ev.mouseButton.y);
                if (state == TaskState::WaitingForInsert) {
                    for (int i = 0; i < 3; i++) {
                        if (!pages[i].isProcessed && !pages[i].isInCopier
                            && pages[i].shape.getGlobalBounds().contains(mpos)) {
                            draggedPageIndex = i;
                            break;
                        }
                    }
                }
                else if (state == TaskState::WaitingForRemove && currentPageInCopier != -1) {
                    // retrieve
                    pages[currentPageInCopier].isInCopier = false;
                    pages[currentPageInCopier].isProcessed = true;
                    pages[currentPageInCopier].shape.setPosition(
                        700 + pagesProcessed*100, 450
                    );
                    currentPageInCopier = -1;
                    pagesProcessed++;
                    if (pagesProcessed >= 3) {
                        state = TaskState::TaskCompleted;
                        completionClock.restart();
                    } else {
                        state = TaskState::WaitingForInsert;
                    }
                }
                else if (state == TaskState::Failure) {
                    // reset
                    if (currentPageInCopier != -1) {
                        pages[currentPageInCopier].shape.setPosition(
                            pages[currentPageInCopier].originalPosition
                        );
                        pages[currentPageInCopier].isInCopier = false;
                        currentPageInCopier = -1;
                    }
                    state = TaskState::WaitingForInsert;
                }
            }

            if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left) {
                if (draggedPageIndex != -1) {
                    if (state == TaskState::WaitingForInsert
                        && copierSlot.getGlobalBounds().intersects(
                            pages[draggedPageIndex].shape.getGlobalBounds()))
                    {
                        // snap in
                        sf::Vector2f slotCenter = copierSlot.getPosition() + sf::Vector2f(
                            (copierSlot.getSize().x - pages[draggedPageIndex].shape.getSize().x)/2,
                            (copierSlot.getSize().y - pages[draggedPageIndex].shape.getSize().y)/2
                        );
                        pages[draggedPageIndex].shape.setPosition(slotCenter);
                        pages[draggedPageIndex].isInCopier = true;
                        currentPageInCopier = draggedPageIndex;
                        state = TaskState::Printing;
                        printClock.restart();
                    } else {
                        pages[draggedPageIndex].shape.setPosition(
                            pages[draggedPageIndex].originalPosition
                        );
                    }
                    draggedPageIndex = -1;
                }
            }
        }

        // Drag
        if (draggedPageIndex != -1) {
            auto m = sf::Mouse::getPosition(window);
            pages[draggedPageIndex].shape.setPosition(
                m.x - pages[draggedPageIndex].shape.getSize().x/2,
                m.y - pages[draggedPageIndex].shape.getSize().y/2
            );
        }

        // Background update
        float dt = particleClock.restart().asSeconds();
        for (auto& p : backgroundParticles) {
            sf::Vector2f pos = p.getPosition();
            pos.x += (30 + rand()%20)*dt;
            pos.y += std::sin(pos.x*0.01f)*20*dt;
            if (pos.x > 1020) {
                pos.x = -20;
                pos.y = rand()%700;
            }
            p.setPosition(pos);
        }

        // State logic
        switch (state) {
            case TaskState::WaitingForInsert:
                instr.setString(
                    "Drag pages into copier slot (Processed: " +
                    std::to_string(pagesProcessed) + "/3)"
                );
                barFill.setSize({0,22});
                timerText.setString("");
                break;
            case TaskState::Printing: {
                instr.setString(
                    "Printing Page " +
                    std::to_string(pages[currentPageInCopier].pageNumber) +
                    "... hold steady"
                );
                float t = printClock.getElapsedTime().asSeconds();
                float pct = std::min(t/3.f, 1.f);
                barFill.setSize({(barFrame.getSize().x-4)*pct, 22});
                std::ostringstream ss;
                ss << std::fixed << std::setprecision(1) << (3.f - std::min(t,3.f)) << "s";
                timerText.setString(ss.str());
                if (pct >= 1.f) {
                    state = TaskState::WaitingForRemove;
                }
                break;
            }
            case TaskState::WaitingForRemove:
                instr.setString(
                    "Click to retrieve Page " +
                    std::to_string(pages[currentPageInCopier].pageNumber)
                );
                break;
            case TaskState::Failure:
                instr.setString("Too soon! Press Enter to reset.");
                timerText.setString("");
                break;
            case TaskState::Success:
                instr.setString("Page completed! Continue.");
                timerText.setString("");
                break;
            case TaskState::TaskCompleted:
                instr.setString("All pages processed! Press Enter to exit.");
                timerText.setString("");
                break;
        }

        pageCountText.setString(
            "Pages Completed: " + std::to_string(pagesProcessed) + "/3"
        );

        // Render
        window.clear(sf::Color::Black);
        window.draw(copier);
        window.draw(copierSlot);
        window.draw(copierLabel);

        // Draw pages
        for (int i = 0; i < 3; i++) {
            if (!pages[i].isProcessed || pages[i].isInCopier)
                window.draw(pages[i].shape);
        }
        // Completed area
        for (int i = 0; i < 3; i++) {
            if (pages[i].isProcessed && !pages[i].isInCopier)
                window.draw(pages[i].shape);
        }

        window.draw(barFrame);
        window.draw(barFill);
        window.draw(instr);
        window.draw(timerText);
        window.draw(pageCountText);

        for (int i = 0; i < 3; i++) {
            if (!pages[i].isProcessed && !pages[i].isInCopier)
                window.draw(pageLabels[i]);
        }

        if (state == TaskState::TaskCompleted) {
            float pulse = std::sin(completionClock.getElapsedTime().asSeconds()*4)*0.3f + 0.7f;
            completionText.setFillColor(sf::Color(0, (sf::Uint8)(255*pulse), 0));
            window.draw(completionText);
        }

        if (pagesProcessed > 0) {
            sf::Text completedLabel("Completed Pages:", font, 16);
            completedLabel.setFillColor(sf::Color::Green);
            completedLabel.setPosition(650,420);
            window.draw(completedLabel);
        }

        window.display();
    }

    return 0;
}
