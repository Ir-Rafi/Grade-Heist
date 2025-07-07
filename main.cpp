#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>

#include "Constants.h"             // WORLD_W, WORLD_H, TILE_SIZE, Vec2f, WAYPOINT_REACH_THRESHOLD
#include "Collision.h"             // checkCollision(...)
#include "Enemy.h"                 // Enemy(walls, grid, circuit)
#include "Player.h"                // Player class
#include "Minimap.h"               // Minimap HUD
#include "MapLoader.hpp"           // Tiled map loader (with getTaskRects)
#include "codechallenge.h"         // CodeChallenge overlay
#include "WiFiServerTask.hpp"      // WiFiServerTask overlay (wifioff)
#include "WiFiLoginTask.hpp"       // WiFiLoginTask overlay (wifilin)
#include "TypingChallenge.hpp"     // TypingChallenge overlay
#include "PhotocopierTask.hpp"     // PhotocopierTask overlay
#include "ComputerTaskOverlay.hpp" // Computer terminal overlay
#include"ACServerOverlay.hpp"
#include "IntegerOverflowOverlay.hpp"
#include "CombinationLockOverlay.hpp"
#include "PCSimulatorOverlay.hpp"
#include "GradeChangerOverlay.hpp"
#include "UI.h"
#include "isend.h"

int main()
{    int uniqueCompleted = 0;
    // --- Window & view setup ---
    sf::RenderWindow window(
        sf::VideoMode(WORLD_W/2, WORLD_H/2),
        "Grade-Heist"
    );
    window.setFramerateLimit(60);
    sf::View view({0.f, 0.f, WORLD_W/2.f, WORLD_H/2.f});

    // // --- Loading screen sprite ---
    // sf::Texture loadingTex;
    // if (!loadingTex.loadFromFile("loading.png")) {
    //     std::cerr << "Failed to load loading.png\n";
    //     return -1;
    // }
    // sf::Sprite loadingSprite(loadingTex);
    // loadingSprite.setPosition(
    //     (window.getSize().x - loadingTex.getSize().x) / 2.f,
    //     (window.getSize().y - loadingTex.getSize().y) / 2.f
    // );
    // window.clear();
    // window.draw(loadingSprite);
    // window.display();

    // --- Loading screen sprite ---
    sf::Texture loadingTex;
    if (!loadingTex.loadFromFile("loading.png")) {
        std::cerr << "Failed to load loading.png\n";
        return -1;
    }
    sf::Sprite loadingSprite;

    loadingSprite.setTexture(loadingTex);

    loadingSprite.setPosition(
        0.f, 0.0f
    );
    window.clear();
    window.draw(loadingSprite);
    window.display();

    //start music
    sf::Music loadingMusic;
    if (!loadingMusic.openFromFile("loading_music.ogg")) {
        std::cerr << "Failed to load music.\n";
        return -1;
    }
    loadingMusic.setLoop(true);  // Loop the music
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

    // Stop music after loading is finished
    loadingMusic.stop();
      // --- Gameplay music setup ---
    sf::Music gameplayMusic;
    if (!gameplayMusic.openFromFile("gameplay_music.ogg")) {
        std::cerr << "Failed to load gameplay music.\n";
        return -1;
    }
    gameplayMusic.setLoop(true);  // Loop the gameplay music
    gameplayMusic.play();         // Start gameplay music

    if (!loadSuccess) {
        std::cerr << "Map load failed\n";
        return -1;
    }

    // --- Extract collision walls ---
    const auto& collisionRects = map.getCollisionRects();
    std::vector<sf::RectangleShape> walls;
    walls.reserve(collisionRects.size());
    for (auto& r : collisionRects) {
        sf::RectangleShape w;
        w.setPosition(r.left, r.top);
        w.setSize({r.width, r.height});
        walls.push_back(w);
    }

    // --- Build walkable grid ---
    int cols = WORLD_W / TILE_SIZE;
    int rows = WORLD_H / TILE_SIZE;
    std::vector<bool> grid(cols * rows);
    for (int y = 0; y < rows; ++y) {
        for (int x = 0; x < cols; ++x) {
            sf::FloatRect cell(x * TILE_SIZE, y * TILE_SIZE,
                               TILE_SIZE, TILE_SIZE);
            for (auto& w : walls) {
                if (cell.intersects(w.getGlobalBounds())) {
                    grid[y*cols + x] = true;
                    break;
                }
            }
        }
    }

    // --- Patrol circuit setup ---
    std::vector<Vec2f> rawCircuit = {
        {950.f, 600.f}, {900.f, 600.f}, {900.f, 660.f},
        {900.f, 700.f}, {800.f, 700.f}, {300.f, 700.f},
        {300.f, 900.f}, {500.f, 900.f}, {600.f, 1500.f},
        {387.f, 1512.f}, {681.f, 1543.f}, {782.f, 1583.f},
        {911.f, 1552.f}, {934.f, 1469.f}, {911.f, 1279.f},
        {986.f, 1279.f}, {486.f, 1893.f}, {314.f, 2071.f},
        {190.f, 2021.f}, {397.f, 2455.f}, {431.f, 2287.f},
        {725.f, 2287.f}, {794.f, 2174.f}, {794.f, 1998.f},
        {960.f, 2124.f}, {1129.f, 2124.f}, {1129.f, 2377.f},
        {1290.f, 2373.f}, {1290.f, 1895.f}, {1466.f, 1539.f},
        {1770.f, 1539.f}, {1808.f, 1636.f}, {1770.f, 1941.f},
        {1909.f, 2140.f}, {2016.f, 2285.f}, {2238.f, 2261.f},
        {2321.f, 2178.f}, {2210.f, 2014.f}, {2222.f, 1814.f},
        {1835.f, 1814.f}, {1835.f, 1270.f}, {1909.f, 1237.f},
        {2010.f, 1344.f}, {2110.f, 1220.f}, {2346.f, 1220.f},
        {2170.f, 1158.f}, {1902.f, 1251.f}, {1663.f, 1125.f},
        {1748.f, 931.f},  {1758.f, 743.f},  {2244.f, 743.f},
        {2400.f, 663.f},  {2247.f, 571.f},  {2125.f, 733.f},
        {1774.f, 743.f},  {1734.f, 606.f},  {1489.f, 675.f},
        {1311.f, 675.f},  {1311.f, 1400.f}, {1206.f, 1400.f},
        {1189.f, 422.f},  {1301.f, 422.f},  {1301.f, 674.f},
        {974.f, 674.f}
    };
    std::vector<Vec2f> circuit;
    const float ENEMY_SIZE = 36.f;
    for (auto& p : rawCircuit) {
        sf::FloatRect box(p.x - ENEMY_SIZE/2, p.y - ENEMY_SIZE/2,
                          ENEMY_SIZE, ENEMY_SIZE);
        if (!checkCollision(box, walls))
            circuit.push_back(p);
    }
    if (circuit.empty()) circuit = rawCircuit;

    // Build coarse grid for enemy pathfinding
    int pp = ENEMY_SIZE / TILE_SIZE;
    //int cols = WORLD_W / TILE_SIZE;
    //int rows = WORLD_H / TILE_SIZE;
    int colsCoarse = (WORLD_W / TILE_SIZE) - (pp - 1);
    int rowsCoarse = (WORLD_H / TILE_SIZE) - (pp - 1);

    // Create coarse grid: true=free, false=blocked
    std::vector<bool> coarseGrid(colsCoarse * rowsCoarse, true);
    for (int cy = 0; cy < rowsCoarse; ++cy) {
        for (int cx = 0; cx < colsCoarse; ++cx) {
            bool free = true;
            for (int dy = 0; dy < pp && free; ++dy) {
                for (int dx = 0; dx < pp; ++dx) {
                    int ry = cy + dy;
                    int rx = cx + dx;
                    if (grid[ry * cols + rx]) {
                        free = false;
                        break;
                    }
                }
            }
            coarseGrid[cy * colsCoarse + cx] = free;
        }
    }

    // Convert circuit patrol points to coarse tile indices
    std::vector<Tile> patrolTiles;
    for (auto& pt : circuit) {
        int cx = int(pt.x / TILE_SIZE) - (pp / 2);
        int cy = int(pt.y / TILE_SIZE) - (pp / 2);
        // Clamp to coarse grid (defensive)
        cx = std::clamp(cx, 0, colsCoarse - 1);
        cy = std::clamp(cy, 0, rowsCoarse - 1);
        patrolTiles.emplace_back(cx, cy);
    }

    // Create enemy
    Enemy enemy(walls, coarseGrid, colsCoarse, rowsCoarse, patrolTiles);

    // --- Player setup ---
    sf::Texture playerTex;
    if (!playerTex.loadFromFile("sprite.png"))
        return -1;
    Player player(playerTex, walls);
    player.setPosition(WORLD_W/2.f, WORLD_H/2.f);

    // --- HUD & UI setup ---
    sf::RectangleShape zone({200,150});
    zone.setPosition(WORLD_W-200, WORLD_H-150);
    zone.setFillColor({0,255,0,100});

    sf::Font font;
    if (!font.loadFromFile("arial.ttf"))
        return -1;
       sf::Text uniqueText("Task done: 0", font, 24);
    uniqueText.setFillColor(sf::Color::White);

    Minimap minimap(WORLD_W, WORLD_H, font);
    sf::Clock clk;

    // --- Overlays construction ---
    CodeChallenge       codeChallenge(800,600);
    WiFiServerTask      wifiOffTask(800,600);   // existing "wifioff"
    WiFiLoginTask       wifilinTask(800,600);   // new "wifilin"
    TypingChallenge     typingTask(800,600);
    PhotocopierTask     photocopyTask(800,600);
    ComputerTaskOverlay compTermTask(800,600);  // new computer terminal task
    //ACServerOverlay   acTask(800,600);
    IntegerOverflowOverlay overflowTask(800,600);
    CombinationLockOverlay lockerTask(800, 600);
    PCSimulatorOverlay servTask(800,600);
    GradeChangerOverlay gradeChanger(800, 600);
    // --- Button sprite shared by tasks ---
    sf::Texture btnTex;
    if (!btnTex.loadFromFile("buttonimg.png"))
        return -1;
    sf::Sprite btn(btnTex);
    btn.setPosition(750,520);
    bool showButton = false;

    // --- Task integration setup ---
    enum class TaskType {
        None,
        Cout,      // "cout" → codeChallenge
        WiFiOff,   // "wifioff" → wifiOffTask
        Wifilin,   // "wifilin" → wifilinTask
        Tyac,      // "tyac"  → typingTask
        Printer,   // "printer" → photocopyTask
        CompTerm,   // "compterm" → compTermTask
        //Ac,
        overflowTask,
        Locker,
        servTask,
        Gradechg
    };
    TaskType pendingTask = TaskType::None;

    // Pull rect-lists from the map by your Tiled "type" string
    const auto& coutRects     = map.getTaskRects("cout");
    const auto& wifiOffRects  = map.getTaskRects("wifioff");
    const auto& wifilinRects  = map.getTaskRects("wifilin");
    const auto& tyacRects     = map.getTaskRects("tyac");
    const auto& printerRects  = map.getTaskRects("printer");
    const auto& compTermRects = map.getTaskRects("typing");
    //const auto& acRects       = map.getTaskRects("ac");
    const auto& overflowRects = map.getTaskRects("laptop");
    const auto& lockerRects   = map.getTaskRects("locker");
    const auto& servRects = map.getTaskRects("serv");
    const auto& gradechgRects = map.getTaskRects("grade");

    const auto& endtRects           = map.getTaskRects("endt");
    bool       hasWon              = false;

    struct TaskDef {
        TaskType                         type;
        const std::vector<sf::FloatRect>* rects;
        std::function<void()>            startFunc;
         std::function<bool()>              isDone;    // <-- must be provided
        bool                               counted = false;
    };

    std::vector<TaskDef> tasks = {
        { TaskType::Cout,      &coutRects, [&](){ codeChallenge.start(); }, [&](){ return codeChallenge.isFinished(); } },

        { TaskType::WiFiOff,   &wifiOffRects,   [&](){ wifiOffTask.start();   }, [&](){return wifiOffTask.isFinished();   } },
        { TaskType::Wifilin,   &wifilinRects,   [&](){ wifilinTask.start();   }, [&](){return wifilinTask.isFinished();   } },
        { TaskType::Tyac,      &tyacRects,      [&](){ typingTask.start();    }, [&](){return typingTask.isFinished();    } },
        { TaskType::Printer,   &printerRects,   [&](){ photocopyTask.start(); }, [&](){return photocopyTask.isFinished(); } },
        { TaskType::CompTerm,  &compTermRects,  [&](){ compTermTask.start();  }, [&](){return compTermTask.isFinished();  } },
       // { TaskType::Ac,       &acRects,         [&](){ acTask.start();        }, [&](){return acTask.isFinished();        } },
        { TaskType::overflowTask,&overflowRects,[&](){ overflowTask.start();  }, [&](){return overflowTask.isFinished();  } },
        { TaskType::Locker,   &lockerRects,     [&](){ lockerTask.start();    }, [&](){return lockerTask.isFinished();    } },
        { TaskType::servTask,   &servRects,     [&](){ servTask.start();      }, [&](){return servTask.isFinished();    } },
        { TaskType::Gradechg,   &gradechgRects, [&](){ gradeChanger.start();  }, [&](){return gradeChanger.isFinished();    } },
        
    };

    // --- Main game loop ---
    while (window.isOpen()) {
        sf::Event e;
        while (window.pollEvent(e)) {
            if (e.type == sf::Event::Closed) {
                window.close();
            }
            // 1) Dispatch to any active overlay
            else if (codeChallenge.isActive()) {
                codeChallenge.handleEvent(e);
            }
            else if (wifiOffTask.isActive()) {
                wifiOffTask.handleEvent(e);
            }
            else if (wifilinTask.isActive()) {
                wifilinTask.handleEvent(e);
            }
            else if (typingTask.isActive()) {
                typingTask.handleEvent(e);
            }
            else if (photocopyTask.isActive()) {
                photocopyTask.handleEvent(e);
            }
            else if (compTermTask.isActive() || compTermTask.isFinished()) {
                compTermTask.handleEvent(e);
            }
            // else if (acTask.isActive()) {
            //     acTask.handleEvent(e);
            // }
             else if (overflowTask.isActive()) {
                overflowTask.handleEvent(e);
            }
              else if (lockerTask.isActive()) {
                lockerTask.handleEvent(e);
            }
            else if (servTask.isActive()) {
                servTask.handleEvent(e);
            }
            else if (gradeChanger.isActive()) {
                gradeChanger.handleEvent(e);
            }
            // 2) Otherwise handle game input & button-click
            else {
                minimap.handleEvent(e, window);

                if (showButton
                    && e.type == sf::Event::MouseButtonPressed
                    && e.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2f mp(e.mouseButton.x, e.mouseButton.y);
                    if (btn.getGlobalBounds().contains(mp)
                        && pendingTask != TaskType::None)
                    {
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
        if (!codeChallenge.isActive() &&
            !wifiOffTask.isActive()    &&
            !wifilinTask.isActive()    &&
            !typingTask.isActive()     &&
            !photocopyTask.isActive()  &&
            !compTermTask.isActive()&&
            // !acTask.isActive()&&
            !overflowTask.isActive()&&
            !lockerTask.isActive()&&
            !servTask.isActive()&&
            !gradeChanger.isActive())
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
        // Clamp to coarse grid
        playerCx = std::clamp(playerCx, 0, colsCoarse - 1);
        playerCy = std::clamp(playerCy, 0, rowsCoarse - 1);

        enemy.update({playerCx, playerCy}, dt);


        if (isend(walls, player.getPosition(), enemy.shape().getPosition())) {
            // Load the game-over texture
            sf::Texture gameOverTexture;
            if (!gameOverTexture.loadFromFile("gameover.png")) {
                std::cerr << "Failed to load gameover.png\n";
                return -1;
            }
            sf::Sprite gameOverSprite(gameOverTexture);

            // Freeze on the game-over screen until window close or any key press
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

            // Exit after the game-over screen is dismissed
            return 0;
        }


      
        // Determine which task tile we're on
        pendingTask = TaskType::None;
        if (!codeChallenge.isActive() &&
            !wifiOffTask.isActive()    &&
            !wifilinTask.isActive()    &&
            !typingTask.isActive()     &&
            !photocopyTask.isActive()  &&
            !compTermTask.isActive()&&
            // !acTask.isActive()&&
            !overflowTask.isActive()&&
            !lockerTask.isActive()&&
            !servTask.isActive()&&
            !gradeChanger.isActive())
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


        codeChallenge.update(dt);
        wifiOffTask.update(dt);
        wifilinTask.update(dt);
        typingTask.update(dt);
        photocopyTask.update(dt);
        compTermTask.update(dt);
        // acTask.update(dt);
        overflowTask.update(dt);
        lockerTask.update(dt);
        servTask.update(dt);
        gradeChanger.update(dt);
         for (auto &td : tasks) {
        if (!td.counted && td.isDone()) {
            td.counted = true;
            uniqueCompleted++;
            uniqueText.setString("Tasks done: " + std::to_string(uniqueCompleted));

            if (!hasWon && uniqueCompleted == 10) {
                // get your player’s bounds (adjust to however you expose them)
                sf::FloatRect playerBounds = player.getGlobalBounds();

                for (const auto& rect : endtRects) {
                    if (rect.intersects(playerBounds)) {
                        hasWon = true;
                        // → your win handler: could be a new overlay, message, window.close(), etc.
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
        drawScene(window,
               walls,
               enemy,
               player,
               uniqueText);

        // HUD
        sf::Vector2f tl = view.getCenter() - view.getSize()/2.f;
        uniqueText.setPosition(tl + Vec2f(10,10));
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
        // if (acTask.isActive() || acTask.isFinished())
        //     acTask.draw(window);
         if (overflowTask.isActive() || overflowTask.isFinished())
            overflowTask.draw(window);
         if (lockerTask.isActive()   || lockerTask.isFinished())
            lockerTask.draw(window);
         if (servTask.isActive()   || servTask.isFinished())
            servTask.draw(window);
        if (gradeChanger.isActive() || gradeChanger.isFinished())
            gradeChanger.draw(window); 
            window.display();
    }

    return 0;
}

