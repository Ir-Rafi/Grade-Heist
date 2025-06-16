#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <iomanip>
#include <sstream>
#include <vector>
#include<cmath>

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
        // Handle font loading failure
        return -1;
    }

    // Create copier as a rectangle shape
    sf::RectangleShape copier({200, 150});
    copier.setFillColor(sf::Color(100, 100, 100));
    copier.setOutlineColor(sf::Color(150, 150, 150));
    copier.setOutlineThickness(3);
    copier.setPosition(400, 250);

    // Create copier slot (darker rectangle inside)
    sf::RectangleShape copierSlot({160, 110});
    copierSlot.setFillColor(sf::Color(50, 50, 50));
    copierSlot.setPosition(420, 270);

    // Create 3 pages as rectangle shapes
    std::vector<Page> pages(3);
    sf::Vector2f startPositions[3] = {
        {50, 450},   // Page 1
        {150, 450},  // Page 2  
        {250, 450}   // Page 3
    };
    
    for (int i = 0; i < 3; i++) {
        pages[i].shape.setSize({80, 100});
        pages[i].shape.setPosition(startPositions[i]);
        pages[i].originalPosition = startPositions[i];
        pages[i].isInCopier = false;
        pages[i].isProcessed = false;
        pages[i].pageNumber = i + 1;
        
        // Color pages differently for visual distinction
        sf::Color pageColors[3] = {
            sf::Color::White, 
            sf::Color(255, 255, 200), 
            sf::Color(200, 255, 200)
        };
        pages[i].shape.setFillColor(pageColors[i]);
        pages[i].shape.setOutlineColor(sf::Color::Black);
        pages[i].shape.setOutlineThickness(2);
    }

    // Progress bar frame
    sf::RectangleShape barFrame({220, 26});
    barFrame.setFillColor(sf::Color::Transparent);
    barFrame.setOutlineColor(sf::Color(200,200,200));
    barFrame.setOutlineThickness(2);
    barFrame.setPosition(390, 450);

    // Progress fill
    sf::RectangleShape barFill({0, 22});
    barFill.setFillColor(sf::Color(100,250,100));
    barFill.setPosition(barFrame.getPosition() + sf::Vector2f(2,2));

    // Instruction text
    sf::Text instr("", font, 20);
    instr.setFillColor(sf::Color::White);
    instr.setPosition(300, 150);

    // Timer text
    sf::Text timerText("", font, 18);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(620, 450);

    // Page counter text
    sf::Text pageCountText("", font, 18);
    pageCountText.setFillColor(sf::Color::Cyan);
    pageCountText.setPosition(50, 50);

    // Task completion text
    sf::Text completionText("TASK COMPLETED!", font, 48);
    completionText.setFillColor(sf::Color::Green);
    completionText.setPosition(300, 300);

    // Page labels
    std::vector<sf::Text> pageLabels(3);
    for (int i = 0; i < 3; i++) {
        pageLabels[i].setFont(font);
        pageLabels[i].setString("Page " + std::to_string(i + 1));
        pageLabels[i].setCharacterSize(14);
        pageLabels[i].setFillColor(sf::Color::White);
        pageLabels[i].setPosition(startPositions[i].x, startPositions[i].y - 25);
    }

    // Copier label
    sf::Text copierLabel("COPIER", font, 16);
    copierLabel.setFillColor(sf::Color::White);
    copierLabel.setPosition(450, 220);

    TaskState state = TaskState::WaitingForInsert;
    int draggedPageIndex = -1;
    int currentPageInCopier = -1;
    int pagesProcessed = 0;
    sf::Clock printClock;
    sf::Clock completionClock;

    while (window.isOpen()) {
        // --- Event Handling ---
        sf::Event ev;
        while (window.pollEvent(ev)) {
            if (ev.type == sf::Event::Closed)
                window.close();

            if (ev.type == sf::Event::MouseButtonPressed && ev.mouseButton.button == sf::Mouse::Left) {
                sf::Vector2f mpos(ev.mouseButton.x, ev.mouseButton.y);

                if (state == TaskState::WaitingForInsert) {
                    // Check if clicking on any unprocessed page
                    for (int i = 0; i < 3; i++) {
                        if (!pages[i].isProcessed && !pages[i].isInCopier && 
                            pages[i].shape.getGlobalBounds().contains(mpos)) {
                            draggedPageIndex = i;
                            break;
                        }
                    }
                }
                else if (state == TaskState::WaitingForRemove && currentPageInCopier != -1) {
                    // Remove current page from copier
                    pages[currentPageInCopier].isInCopier = false;
                    pages[currentPageInCopier].isProcessed = true;
                    pages[currentPageInCopier].shape.setPosition(700 + pagesProcessed * 100, 450); // Move to "completed" area
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
                    // Reset current page
                    if (currentPageInCopier != -1) {
                        pages[currentPageInCopier].shape.setPosition(pages[currentPageInCopier].originalPosition);
                        pages[currentPageInCopier].isInCopier = false;
                        currentPageInCopier = -1;
                    }
                    state = TaskState::WaitingForInsert;
                }
            }
            
            if (ev.type == sf::Event::MouseButtonReleased && ev.mouseButton.button == sf::Mouse::Left) {
                if (draggedPageIndex != -1) {
                    // Check if dropped on copier slot
                    if (state == TaskState::WaitingForInsert &&
                        copierSlot.getGlobalBounds().intersects(pages[draggedPageIndex].shape.getGlobalBounds())) {
                        
                        // Snap to copier slot center
                        sf::Vector2f slotCenter = copierSlot.getPosition() + sf::Vector2f(
                            copierSlot.getSize().x / 2 - pages[draggedPageIndex].shape.getSize().x / 2,
                            copierSlot.getSize().y / 2 - pages[draggedPageIndex].shape.getSize().y / 2
                        );
                        pages[draggedPageIndex].shape.setPosition(slotCenter);
                        
                        pages[draggedPageIndex].isInCopier = true;
                        currentPageInCopier = draggedPageIndex;
                        state = TaskState::Printing;
                        printClock.restart();
                    } else {
                        // Return to original position
                        pages[draggedPageIndex].shape.setPosition(pages[draggedPageIndex].originalPosition);
                    }
                    draggedPageIndex = -1;
                }
            }
        }

        // --- Dragging Logic ---
        if (draggedPageIndex != -1) {
            auto m = sf::Mouse::getPosition(window);
            pages[draggedPageIndex].shape.setPosition(
                m.x - pages[draggedPageIndex].shape.getSize().x/2,
                m.y - pages[draggedPageIndex].shape.getSize().y/2
            );
        }

        // --- State Updates & UI Text ---
        switch (state) {
        case TaskState::WaitingForInsert:
            instr.setString("Drag pages into copier slot (Pages processed: " + std::to_string(pagesProcessed) + "/3)");
            barFill.setSize({0,22});
            timerText.setString("");
            break;

        case TaskState::Printing: {
            instr.setString("Printing Page " + std::to_string(pages[currentPageInCopier].pageNumber) + "... hold steady");
            float t = printClock.getElapsedTime().asSeconds();
            float pct = std::min(t/3.f, 1.f);  // 3 seconds print time
            barFill.setSize({(barFrame.getSize().x-4)*pct, 22});

            // Show countdown
            std::ostringstream ss;
            ss << std::fixed << std::setprecision(1) << (3.f - std::min(t,3.f)) << "s";
            timerText.setString(ss.str());

            if (pct >= 1.f) {
                state = TaskState::WaitingForRemove;
            }
            break;
        }

        case TaskState::WaitingForRemove:
            instr.setString("Click to retrieve Page " + std::to_string(pages[currentPageInCopier].pageNumber));
            break;

        case TaskState::Failure:
            instr.setString("Too soon! Resetting page...");
            timerText.setString("");
            break;

        case TaskState::Success:
            instr.setString("Page completed! Continue with remaining pages");
            timerText.setString("");
            break;
            
        case TaskState::TaskCompleted:
            instr.setString("All pages processed successfully!");
            timerText.setString("");
            // Auto-close after 3 seconds
            if (completionClock.getElapsedTime().asSeconds() > 3.0f) {
                window.close();
            }
            break;
        }

        // Update page counter
        pageCountText.setString("Pages Completed: " + std::to_string(pagesProcessed) + "/3");

        // --- Rendering ---
        window.clear(sf::Color::Black);

        // Draw copier and slot
        window.draw(copier);
        window.draw(copierSlot);
        window.draw(copierLabel);

        // Draw all pages
        for (int i = 0; i < 3; i++) {
            if (!pages[i].isProcessed || pages[i].isInCopier) {
                window.draw(pages[i].shape);
            }
        }

        // Draw processed pages in completion area
        for (int i = 0; i < 3; i++) {
            if (pages[i].isProcessed && !pages[i].isInCopier) {
                window.draw(pages[i].shape);
            }
        }

        // Draw UI elements
        window.draw(barFrame);
        window.draw(barFill);
        window.draw(instr);
        window.draw(timerText);
        window.draw(pageCountText);

        // Draw page labels for unprocessed pages
        for (int i = 0; i < 3; i++) {
            if (!pages[i].isProcessed && !pages[i].isInCopier) {
                window.draw(pageLabels[i]);
            }
        }

        // Draw completion text
        if (state == TaskState::TaskCompleted) {
            // Add pulsing effect
            float pulse = std::sin(completionClock.getElapsedTime().asSeconds() * 4) * 0.3f + 0.7f;
            completionText.setFillColor(sf::Color(0, 255 * pulse, 0));
            window.draw(completionText);
        }

        // Draw "Completed Pages" label
        if (pagesProcessed > 0) {
            sf::Text completedLabel("Completed Pages:", font, 16);
            completedLabel.setFillColor(sf::Color::Green);
            completedLabel.setPosition(650, 420);
            window.draw(completedLabel);
        }

        window.display();
    }

    return 0;
}