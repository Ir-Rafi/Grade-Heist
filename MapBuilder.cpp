#include "MapBuilder.h"
#include <algorithm>

std::vector<sf::RectangleShape> buildWalls() {
    std::vector<sf::RectangleShape> walls;
    const float T = 20.f;

    walls.emplace_back(sf::Vector2f(WORLD_W, T)); walls.back().setPosition(0, 0);
    walls.emplace_back(sf::Vector2f(WORLD_W, T)); walls.back().setPosition(0, WORLD_H - T);
    walls.emplace_back(sf::Vector2f(T, WORLD_H)); walls.back().setPosition(0, 0);
    walls.emplace_back(sf::Vector2f(T, WORLD_H)); walls.back().setPosition(WORLD_W - T, 0);

    float yDiv = TILE_SIZE * 12;
    float leftW = TILE_SIZE * 18;
    float corridorW = TILE_SIZE * 4;
    float rightW = WORLD_W - leftW - corridorW;
    float doorW = TILE_SIZE * 2;

    std::vector<float> dx = {
        leftW/2.f,
        leftW + corridorW/2.f,
        leftW + corridorW + rightW/2.f
    };
    std::vector<float> hcuts = {0};
    for(auto c: dx) hcuts.push_back(c - doorW/2);
    for(auto c: dx) hcuts.push_back(c + doorW/2);
    hcuts.push_back(WORLD_W);
    std::sort(hcuts.begin(), hcuts.end());

    for(int i=0; i+1<int(hcuts.size()); i+=2){
        float x0 = hcuts[i], x1 = hcuts[i+1];
        sf::RectangleShape r({x1 - x0, T});
        r.setPosition(x0, yDiv);
        walls.push_back(r);
    }

    {
        float xDiv = leftW;
        float y1 = yDiv/2.f;
        float y2 = yDiv + (WORLD_H - yDiv)/2;
        float doorH = TILE_SIZE * 2;
        std::vector<float> cuts = {0,
            y1 - doorH/2, y1 + doorH/2,
            y2 - doorH/2, y2 + doorH/2,
            float(WORLD_H)
        };
        for(int i=0; i+1<int(cuts.size()); i+=2){
            float y0 = cuts[i], y1 = cuts[i+1];
            sf::RectangleShape r({T, y1 - y0});
            r.setPosition(xDiv, y0);
            walls.push_back(r);
        }
    }
    {
        float xDiv = leftW + corridorW;
        float y0 = yDiv + (WORLD_H - yDiv)/2 - TILE_SIZE;
        float y1 = y0 + TILE_SIZE * 2;
        std::vector<std::pair<float,float>> segs = {
            {0, yDiv}, {yDiv, y0}, {y1, float(WORLD_H)}
        };
        for(auto u: segs){
            float a = u.first, b = u.second;
            sf::RectangleShape r({T, b - a});
            r.setPosition(xDiv, a);
            walls.push_back(r);
        }
    }
    return walls;
}

std::vector<bool> buildCollisionGrid(const std::vector<sf::RectangleShape>& walls) {
    int cols = WORLD_W / TILE_SIZE, rows = WORLD_H / TILE_SIZE;
    std::vector<bool> grid(cols * rows, false);
    for(int y = 0; y < rows; y++){
        for(int x = 0; x < cols; x++){
            sf::FloatRect cell(x * TILE_SIZE, y * TILE_SIZE, TILE_SIZE, TILE_SIZE);
            for(auto& w: walls){
                if(cell.intersects(w.getGlobalBounds())){
                    grid[y * cols + x] = true;
                    break;
                }
            }
        }
    }
    return grid;
}

std::vector<Vec2f> getPatrolCircuit(const std::vector<sf::RectangleShape>& walls) {
    return {
        // 1. Top‐right room entry (moved down from ceiling and in from right wall)
        {1450.f, 200.f},  

        // 2. Move left along the upper corridor
        {900.f, 200.f},
        {450.f, 200.f},
        {150.f, 200.f},  // Top‐left corner area

        // 3. Drop down into top‐left room
        {150.f, 400.f},
        {150.f, 600.f},  // Bottom of top‐left room

        // 4. Move right through middle corridor
        {400.f, 600.f},  
        {600.f, 600.f},  
        {900.f, 600.f},  
        {1200.f, 600.f}, // Entering top‐right of middle area

        // 5. Drop down into middle‐right room
        {1200.f, 900.f}, // Near ceiling of bottom‐right section
        {1200.f, 1100.f},// Bottom edge of that room

        // 6. Sweep left across bottom corridor
        {900.f, 1100.f},
        {500.f, 1100.f},
        {200.f, 1100.f}, // Bottom‐left corner

        // 7. Move up into bottom‐left room
        {200.f, 800.f},
        {200.f, 500.f},  // Midway up that room

        // 8. Cross back to center
        {450.f, 350.f},  // Center‐left
        {700.f, 350.f},  // Center‐middle
        {1000.f, 350.f}, // Center‐right

        // 9. Return to top‐right starting zone
        {1300.f, 350.f},
        {1450.f, 200.f}  // Back to entry point
    };
}