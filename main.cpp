#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <thread>
#include <atomic>
#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

// Project headers
#include "Constants.h"         // WORLD_W, WORLD_H, TILE_SIZE, Vec2f, etc.
#include "Collision.h"         // checkCollision(...)
#include "Enemy.h"             // Enemy class
#include "Player.h"            // Player class
#include "Minimap.h"           // Minimap HUD
#include "MapLoader.hpp"       // Tiled map loader
#include "codechallenge.h"
#include "WiFiServerTask.hpp"
#include "WiFiLoginTask.hpp"
#include "TypingChallenge.hpp"
#include "PhotocopierTask.hpp"
#include "ComputerTaskOverlay.hpp"
#include "ACServerOverlay.hpp"
#include "IntegerOverflowOverlay.hpp"
#include "CombinationLockOverlay.hpp"
#include "PCSimulatorOverlay.hpp"
#include "GradeChangerOverlay.hpp"
#include "UI.h"
#include "isend.h"

// Modularized environment setup
#include "CollisionWalls.h"
#include "WalkableGrid.h"
#include "PatrolCircuit.h"   // <-- rawCircuit & filterPatrolCircuit
#include "CoarseGrid.h"
#include "PatrolTiles.h"

int main()
{
    int uniqueCompleted = 0;

    // --- Window & view setup ---
    sf::RenderWindow window(
        sf::VideoMode(WORLD_W / 2, WORLD_H / 2),
        "Grade-Heist"
    );
    window.setFramerateLimit(60);
    sf::View view({0.f, 0.f, WORLD_W / 2.f, WORLD_H / 2.f});

    // --- Loading screen sprite ---
    sf::Texture loadingTex;
    if (!loadingTex.loadFromFile("loading.png")) {
        std::cerr << "Failed to load loading.png\n";
        return -1;
    }
    sf::Sprite loadingSprite;
    loadingSprite.setTexture(loadingTex);
    loadingSprite.setPosition(0.f, 0.0f);

    window.clear();
    window.draw(loadingSprite);
    window.display();

    // --- Loading music ---
    sf::Music loadingMusic;
    if (!loadingMusic.openFromFile("loading_music.ogg")) {
        std::cerr << "Failed to load music.\n";
        return -1;
    }
    loadingMusic.setLoop(true);
    loadingMusic.play();

    // --- Async map loading ---
    std::atomic<bool> done{false};
    bool loadSuccess = false;
    MapLoader map("eita.json", "eita.png", 12, 12);
    window.setActive(false);
    std::thread loaderThread([&]() {
        sf::Context ctx;
        loadSuccess = map.load();
        done = true;
    });
    window.setActive(true);

    sf::Event evt;
    while (window.isOpen() && !done) {
        while (window.pollEvent(evt)) {
            if (evt.type == sf::Event::Closed) {
                window.close();
                done = true;
            }
        }
        window.clear();
        window.draw(loadingSprite);
        window.display();
        sf::sleep(sf::milliseconds(10));
    }
    loaderThread.join();

    loadingMusic.stop();

    // --- Gameplay music setup ---
    sf::Music gameplayMusic;
    if (!gameplayMusic.openFromFile("gameplay_music.ogg")) {
        std::cerr << "Failed to load gameplay music.\n";
        return -1;
    }
    gameplayMusic.setLoop(true);
    gameplayMusic.play();

    if (!loadSuccess) {
        std::cerr << "Map load failed\n";
        return -1;
    }

    // --- Environment & enemy setup (modularized) ---
    auto walls = extractCollisionWalls(map);
    auto grid = buildWalkableGrid(walls, WORLD_W, WORLD_H, TILE_SIZE);

    // --- Patrol circuit setup (modularized) ---
    const float ENEMY_SIZE = 36.f;
    auto rawCircuit = getRawPatrolCircuit(); // From PatrolCircuit.h
    auto circuit = filterPatrolCircuit(rawCircuit, ENEMY_SIZE, walls, checkCollision);

    int pp = ENEMY_SIZE / TILE_SIZE;
    int cols = WORLD_W / TILE_SIZE;
    int rows = WORLD_H / TILE_SIZE;
    int colsCoarse = cols - (pp - 1);
    int rowsCoarse = rows - (pp - 1);

    auto coarseGrid = buildCoarseGrid(grid, WORLD_W, WORLD_H, TILE_SIZE, ENEMY_SIZE);
    auto patrolTiles = convertPatrolPointsToTiles(circuit, TILE_SIZE, ENEMY_SIZE, colsCoarse, rowsCoarse);

    // --- Enemy creation (remains here) ---
    Enemy enemy(walls, coarseGrid, colsCoarse, rowsCoarse, patrolTiles);

    // --- Player setup ---
    sf::Texture playerTex;
    if (!playerTex.loadFromFile("sprite.png"))
        return -1;
    Player player(playerTex, walls);
    player.setPosition(WORLD_W / 2.f, WORLD_H / 2.f);

    // --- HUD & UI setup ---
    sf::RectangleShape zone({200, 150});
    zone.setPosition(WORLD_W - 200, WORLD_H - 150);
    zone.setFillColor({0, 255, 0, 100});
    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return -1;
    sf::Text uniqueText("Tasks done: 0", font, 24);
    uniqueText.setFillColor(sf::Color::White);
    Minimap minimap(WORLD_W, WORLD_H, font);
    sf::Clock clk;

    // --- Overlays construction ---
    CodeChallenge codeChallenge(800, 600);
    WiFiServerTask wifiOffTask(800, 600);
    WiFiLoginTask wifilinTask(800, 600);
    TypingChallenge typingTask(800, 600);
    PhotocopierTask photocopyTask(800, 600);
    ComputerTaskOverlay compTermTask(800, 600);
    IntegerOverflowOverlay overflowTask(800, 600);
    CombinationLockOverlay lockerTask(800, 600);
    PCSimulatorOverlay servTask(800, 600);
    GradeChangerOverlay gradeChanger(800, 600);

    // --- Button sprite shared by tasks ---
    sf::Texture btnTex;
    if (!btnTex.loadFromFile("buttonimg.png"))
        return -1;
    sf::Sprite btn(btnTex);
    btn.setPosition(750, 520);
    bool showButton = false;

    // --- Task integration setup ---
    enum class TaskType {
        None, Cout, WiFiOff, Wifilin, Tyac, Printer, CompTerm,
        overflowTask, Locker, servTask, Gradechg
    };

    // Pull rect-lists from the map by your Tiled "type" string
    const auto& coutRects = map.getTaskRects("cout");
    const auto& wifiOffRects = map.getTaskRects("wifioff");
    const auto& wifilinRects = map.getTaskRects("wifilin");
    const auto& tyacRects = map.getTaskRects("tyac");
    const auto& printerRects = map.getTaskRects("printer");
    const auto& compTermRects = map.getTaskRects("typing");
    const auto& overflowRects = map.getTaskRects("laptop");
    const auto& lockerRects = map.getTaskRects("locker");
    const auto& servRects = map.getTaskRects("serv");
    const auto& gradechgRects = map.getTaskRects("grade");
    const auto& endtRects = map.getTaskRects("endt");

    bool hasWon = false;

    struct TaskDef {
        TaskType type;
        const std::vector<sf::FloatRect>* rects;
        std::function<void()> startFunc;
        std::function<bool()> isDone;
        bool counted = false;
    };

    std::vector<TaskDef> tasks = {
        { TaskType::Cout, &coutRects, [&]() { codeChallenge.start(); }, [&]() { return codeChallenge.isFinished(); } },
        { TaskType::WiFiOff, &wifiOffRects, [&]() { wifiOffTask.start(); }, [&]() { return wifiOffTask.isFinished(); } },
        { TaskType::Wifilin, &wifilinRects, [&]() { wifilinTask.start(); }, [&]() { return wifilinTask.isFinished(); } },
        { TaskType::Tyac, &tyacRects, [&]() { typingTask.start(); }, [&]() { return typingTask.isFinished(); } },
        { TaskType::Printer, &printerRects, [&]() { photocopyTask.start(); }, [&]() { return photocopyTask.isFinished(); } },
        { TaskType::CompTerm, &compTermRects, [&]() { compTermTask.start(); }, [&]() { return compTermTask.isFinished(); } },
        { TaskType::overflowTask, &overflowRects, [&]() { overflowTask.start(); }, [&]() { return overflowTask.isFinished(); } },
        { TaskType::Locker, &lockerRects, [&]() { lockerTask.start(); }, [&]() { return lockerTask.isFinished(); } },
        { TaskType::servTask, &servRects, [&]() { servTask.start(); }, [&]() { return servTask.isFinished(); } },
        { TaskType::Gradechg, &gradechgRects, [&]() { gradeChanger.start(); }, [&]() { return gradeChanger.isFinished(); } },
    };

    TaskType pendingTask = TaskType::None;

    // --- Main game loop ---
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            // 1) Dispatch to any active overlay
            else if (codeChallenge.isActive()) codeChallenge.handleEvent(e);
            else if (wifiOffTask.isActive()) wifiOffTask.handleEvent(e);
            else if (wifilinTask.isActive()) wifilinTask.handleEvent(e);
            else if (typingTask.isActive()) typingTask.handleEvent(e);
            else if (photocopyTask.isActive()) photocopyTask.handleEvent(e);
            else if (compTermTask.isActive() || compTermTask.isFinished()) compTermTask.handleEvent(e);
            else if (overflowTask.isActive()) overflowTask.handleEvent(e);
            else if (lockerTask.isActive()) lockerTask.handleEvent(e);
            else if (servTask.isActive()) servTask.handleEvent(e);
            else if (gradeChanger.isActive()) gradeChanger.handleEvent(e);
            // 2) Otherwise handle game input & button-click
            else {
                minimap.handleEvent(e, window);
                if (showButton && e.type == sf::Event::MouseButtonPressed && e.mouseButton.button == sf::Mouse::Left) {
                    sf::Vector2f mp(e.mouseButton.x, e.mouseButton.y);
                    if (btn.getGlobalBounds().contains(mp) && pendingTask != TaskType::None) {
                        for (auto& td : tasks) {
                            if (td.type == pendingTask) {
                                td.startFunc();
                                break;
                            }
                        }
                    }
                }
            }
        }

        // --- Updates ---
        float dt = clk.restart().asSeconds();
        bool chase = false;
        if (!codeChallenge.isActive() && !wifiOffTask.isActive() && !wifilinTask.isActive() &&
            !typingTask.isActive() && !photocopyTask.isActive() && !compTermTask.isActive() &&
            !overflowTask.isActive() && !lockerTask.isActive() && !servTask.isActive() && !gradeChanger.isActive())
        {
            player.handleInput(dt);
        }
        else chase = true;

        player.update();
        enemy.setChasing(chase);

        // Update enemy using player's coarse tile
        sf::Vector2f playerPos = player.getPosition();
        int playerCx = int(playerPos.x / TILE_SIZE) - (pp / 2);
        int playerCy = int(playerPos.y / TILE_SIZE) - (pp / 2);
        playerCx = std::clamp(playerCx, 0, colsCoarse - 1);
        playerCy = std::clamp(playerCy, 0, rowsCoarse - 1);
        enemy.update({playerCx, playerCy}, dt);

        if (isend(walls, player.getPosition(), enemy.shape().getPosition())) {
            sf::Texture gameOverTexture;
            if (!gameOverTexture.loadFromFile("gameover.png")) {
                std::cerr << "Failed to load gameover.png\n";
                return -1;
            }
            sf::Sprite gameOverSprite(gameOverTexture);
            while (window.isOpen()) {
                sf::Event evt;
                while (window.pollEvent(evt)) {
                    if (evt.type == sf::Event::Closed) {
                        window.close();
                    }
                }
                window.clear();
                window.draw(gameOverSprite);
                window.display();
            }
            gameplayMusic.stop();
            return 0;
        }

        // Determine which task tile we're on
        pendingTask = TaskType::None;
        if (!codeChallenge.isActive() && !wifiOffTask.isActive() && !wifilinTask.isActive() &&
            !typingTask.isActive() && !photocopyTask.isActive() && !compTermTask.isActive() &&
            !overflowTask.isActive() && !lockerTask.isActive() && !servTask.isActive() && !gradeChanger.isActive())
        {
            sf::FloatRect pb = player.getGlobalBounds();
            for (auto& td : tasks) {
                for (auto& r : *td.rects) {
                    if (r.intersects(pb)) {
                        pendingTask = td.type;
                        break;
                    }
                }
                if (pendingTask != TaskType::None)
                    break;
            }
        }
        showButton = (pendingTask != TaskType::None);

        // Update overlays
        codeChallenge.update(dt);
        wifiOffTask.update(dt);
        wifilinTask.update(dt);
        typingTask.update(dt);
        photocopyTask.update(dt);
        compTermTask.update(dt);
        overflowTask.update(dt);
        lockerTask.update(dt);
        servTask.update(dt);
        gradeChanger.update(dt);

        // Count completed tasks
        for (auto& td : tasks) {
            if (!td.counted && td.isDone()) {
                td.counted = true;
                uniqueCompleted++;
                uniqueText.setString("Tasks done: " + std::to_string(uniqueCompleted));
                if (!hasWon && uniqueCompleted == 10) {
                    sf::FloatRect playerBounds = player.getGlobalBounds();
                    for (const auto& rect : endtRects) {
                        if (rect.intersects(playerBounds)) {
                            hasWon = true;
                            std::cout << "🏆 All tasks complete! You win!\n";
                            window.close();
                            break;
                        }
                    }
                }
            }
        }

        // --- Drawing ---
        window.clear();
        view.setCenter(player.getPosition());
        window.setView(view);
        window.draw(map);
        window.draw(zone);
        window.draw(enemy.shape());
        player.draw(window);
        drawScene(window, walls, enemy, player, uniqueText);

        // HUD
        sf::Vector2f tl = view.getCenter() - view.getSize() / 2.f;
        uniqueText.setPosition(tl + Vec2f(10, 10));
        window.draw(uniqueText);

        window.setView(window.getDefaultView());
        minimap.draw(window, map, walls, player, enemy);

        if (showButton)
            window.draw(btn);

        // Draw overlays on top
        if (codeChallenge.isActive())
            codeChallenge.draw(window);
        if (wifiOffTask.isActive())
            wifiOffTask.draw(window);
        if (wifilinTask.isActive() || wifilinTask.isFinished())
            wifilinTask.draw(window);
        if (typingTask.isActive() || typingTask.isFinished())
            typingTask.draw(window);
        if (photocopyTask.isActive() || photocopyTask.isFinished())
            photocopyTask.draw(window);
        if (compTermTask.isActive() || compTermTask.isFinished())
            compTermTask.draw(window);
        if (overflowTask.isActive() || overflowTask.isFinished())
            overflowTask.draw(window);
        if (lockerTask.isActive() || lockerTask.isFinished())
            lockerTask.draw(window);
        if (servTask.isActive() || servTask.isFinished())
            servTask.draw(window);
        if (gradeChanger.isActive() || gradeChanger.isFinished())
            gradeChanger.draw(window);

        window.display();
    }
    return 0;
}
