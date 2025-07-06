#include "PhotocopierTask.hpp"
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <sstream>
#include <iomanip>

PhotocopierTask::PhotocopierTask(unsigned w, unsigned h)
: state(State::Inactive)
{
    std::srand(unsigned(std::time(nullptr)));
    m_rt.create(w, h);
    m_overlay.setTexture(m_rt.getTexture());

    // Load font
    font.loadFromFile("arial.ttf");

    setupUI();
    // Don't call resetPages() here; leave Inactive until start()

    // Build background particles
    for (int i = 0; i < 20; ++i) {
        sf::CircleShape p(2 + std::rand() % 4);
        p.setPosition(std::rand() % w, std::rand() % h);
        p.setFillColor(
            sf::Color(
                100 + std::rand() % 100,
                100 + std::rand() % 100,
                150 + std::rand() % 100,
                100 + std::rand() % 100
            )
        );
        backgroundParticles.push_back(p);
    }
}

void PhotocopierTask::setupUI() {
    // Copier box
    copier.setSize({200,150});
    copier.setFillColor({100,100,100});
    copier.setOutlineColor({150,150,150});
    copier.setOutlineThickness(3);
    copier.setPosition(400,250);

    copierSlot.setSize({160,110});
    copierSlot.setFillColor({50,50,50});
    copierSlot.setPosition(420,270);

    copierLabel.setFont(font);
    copierLabel.setString("COPIER");
    copierLabel.setCharacterSize(16);
    copierLabel.setFillColor(sf::Color::White);
    copierLabel.setPosition(450,220);

    // Progress bar
    barFrame.setSize({220,26});
    barFrame.setFillColor(sf::Color::Transparent);
    barFrame.setOutlineColor({200,200,200});
    barFrame.setOutlineThickness(2);
    barFrame.setPosition(390,450);

    barFill.setSize({0,22});
    barFill.setFillColor({100,250,100});
    barFill.setPosition(barFrame.getPosition() + sf::Vector2f(2,2));

    // Instruction text
    instrText.setFont(font);
    instrText.setCharacterSize(20);
    instrText.setFillColor(sf::Color::White);
    instrText.setPosition(300,150);

    timerText.setFont(font);
    timerText.setCharacterSize(18);
    timerText.setFillColor(sf::Color::White);
    timerText.setPosition(620,450);

    pageCountText.setFont(font);
    pageCountText.setCharacterSize(18);
    pageCountText.setFillColor(sf::Color::Cyan);
    pageCountText.setPosition(50,50);

    completionText.setFont(font);
    completionText.setString("TASK COMPLETED!(4)");
    completionText.setCharacterSize(48);
    completionText.setFillColor(sf::Color::Green);
    completionText.setPosition(300,300);
}

void PhotocopierTask::resetPages() {
    pages.clear();
    pageLabels.clear();
    sf::Vector2f starts[3] = {{50,450},{150,450},{250,450}};
    sf::Color colors[3] = { sf::Color::White, sf::Color(255,255,200), sf::Color(200,255,200) };
    for (int i = 0; i < 3; ++i) {
        Page pg;
        pg.shape.setSize({80,100});
        pg.shape.setPosition(starts[i]);
        pg.originalPosition = starts[i];
        pg.pageNumber = i+1;
        pg.shape.setFillColor(colors[i]);
        pg.shape.setOutlineColor(sf::Color::Black);
        pg.shape.setOutlineThickness(2);
        pages.push_back(pg);

        sf::Text label;
        label.setFont(font);
        label.setString("Page " + std::to_string(i+1));
        label.setCharacterSize(14);
        label.setFillColor(sf::Color::White);
        label.setPosition(starts[i].x, starts[i].y - 25);
        pageLabels.push_back(label);
    }
    pagesProcessed = 0;
    draggedPageIndex = currentPageInCopier = -1;
    state = State::WaitingForInsert;
}

void PhotocopierTask::start() {
    resetPages();
    completionClock.restart();
    printClock.restart();
    particleClock.restart();
    state = State::WaitingForInsert;
}

void PhotocopierTask::handleEvent(const sf::Event& e) {
    if (state == State::Inactive) return;

    // ESC closes overlay
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Escape) {
        state = State::Inactive;
        return;
    }

    // Enter to dismiss after completion
    if (e.type == sf::Event::KeyPressed && e.key.code == sf::Keyboard::Enter && state == State::TaskCompleted) {
        state = State::Inactive;
        return;
    }

    // Convert window to overlay-local coords
    auto toLocal = [&](float x, float y) {
        sf::Vector2f win(x,y);
        return win - m_overlay.getPosition();
    };

    // Mouse press: start drag
    if (e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
        sf::Vector2f mpos = toLocal(e.mouseButton.x, e.mouseButton.y);
        if (state == State::WaitingForInsert) {
            for (int i = 0; i < (int)pages.size(); ++i) {
                auto& pg = pages[i];
                if (!pg.isProcessed && !pg.isInCopier && pg.shape.getGlobalBounds().contains(mpos)) {
                    draggedPageIndex = i;
                    break;
                }
            }
        } else if (state == State::WaitingForRemove && currentPageInCopier >= 0) {
            auto& pg = pages[currentPageInCopier];
            pg.isInCopier   = false;
            pg.isProcessed  = true;
            pg.shape.setPosition(700 + pagesProcessed*100, 450);
            pagesProcessed++;
            currentPageInCopier = -1;
            state = (pagesProcessed >= 3 ? State::TaskCompleted : State::WaitingForInsert);
        }
    }

    // Mouse move: drag shape
    if (e.type == sf::Event::MouseMoved && draggedPageIndex >= 0 && state == State::WaitingForInsert) {
        sf::Vector2f mpos = toLocal(e.mouseMove.x, e.mouseMove.y);
        auto& pg = pages[draggedPageIndex];
        // center under cursor
        sf::Vector2f half = pg.shape.getSize() * 0.5f;
        pg.shape.setPosition(mpos - half);
    }

    // Mouse release: drop
    if (e.type == sf::Event::MouseButtonReleased && e.mouseButton.button == sf::Mouse::Left && draggedPageIndex >= 0) {
        sf::Vector2f mpos = toLocal(e.mouseButton.x, e.mouseButton.y);
        auto& pg = pages[draggedPageIndex];
        if (state == State::WaitingForInsert && copierSlot.getGlobalBounds().intersects(pg.shape.getGlobalBounds())) {
            // snap into copier
            sf::Vector2f slotPos = copierSlot.getPosition();
            slotPos += {(copierSlot.getSize().x - pg.shape.getSize().x)/2,
                        (copierSlot.getSize().y - pg.shape.getSize().y)/2};
            pg.shape.setPosition(slotPos);
            pg.isInCopier = true;
            currentPageInCopier = draggedPageIndex;
            state = State::Printing;
            printClock.restart();
        } else {
            // return home
            pg.shape.setPosition(pg.originalPosition);
        }
        draggedPageIndex = -1;
    }
}

void PhotocopierTask::update(float dt) {
    if (state == State::Inactive) return;
    updateBackground(dt);
    if (state == State::Printing) {
        float t = printClock.getElapsedTime().asSeconds();
        float pct = std::min(t/3.f, 1.f);
        barFill.setSize({(barFrame.getSize().x - 4)*pct, 22});
        std::ostringstream ss;
        ss << std::fixed << std::setprecision(1) << (3.f - std::min(t,3.f)) << "s";
        timerText.setString(ss.str());
        if (pct >= 1.f) state = State::WaitingForRemove;
    }
}

void PhotocopierTask::updateBackground(float dt) {
    for (auto& p : backgroundParticles) {
        auto pos = p.getPosition();
        pos.x += (30 + std::rand()%20)*dt;
        pos.y += std::sin(pos.x*0.01f)*20*dt;
        if (pos.x > m_rt.getSize().x+20) {
            pos.x = -20;
            pos.y = std::rand()%m_rt.getSize().y;
        }
        p.setPosition(pos);
    }
}

void PhotocopierTask::draw(sf::RenderWindow& window) {
    if (state == State::Inactive) return;
    m_rt.clear({0,0,0,180});
    for (auto& p : backgroundParticles) m_rt.draw(p);
    m_rt.draw(copier);
    m_rt.draw(copierSlot);
    m_rt.draw(copierLabel);
    for (auto& pg : pages) if (!pg.isProcessed || pg.isInCopier) m_rt.draw(pg.shape);
    for (auto& pg : pages) if (pg.isProcessed && !pg.isInCopier) m_rt.draw(pg.shape);
    for (int i = 0; i < (int)pageLabels.size(); ++i) if (!pages[i].isProcessed && !pages[i].isInCopier) m_rt.draw(pageLabels[i]);
    m_rt.draw(barFrame); m_rt.draw(barFill);
    switch (state) {
        case State::WaitingForInsert:
            instrText.setString("Drag pages into copier slot  [" + std::to_string(pagesProcessed) + "/3]");
            timerText.setString(""); break;
        case State::WaitingForRemove:
            instrText.setString("Click to retrieve Page " + std::to_string(pages[currentPageInCopier].pageNumber)); break;
        default: break;
    }
    pageCountText.setString("Pages Completed: " + std::to_string(pagesProcessed) + "/3");
    m_rt.draw(instrText); m_rt.draw(timerText); m_rt.draw(pageCountText);
    if (state == State::TaskCompleted) m_rt.draw(completionText);
    m_rt.display();
    auto ws = window.getSize(), rs = m_rt.getSize();
    m_overlay.setPosition((ws.x-rs.x)*0.5f, (ws.y-rs.y)*0.5f);
    window.draw(m_overlay);
}

bool PhotocopierTask::isActive() const  { return state != State::Inactive; }
bool PhotocopierTask::isFinished() const { return state == State::TaskCompleted; }
