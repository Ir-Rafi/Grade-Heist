#include "Pathfinding.h"
#include "Constants.h"
#include <queue>
#include <algorithm>

std::vector<Vec2f> findPathBFS(Vec2f start, Vec2f goal, const std::vector<bool>& grid) {
    int cols = WORLD_W / TILE_SIZE;
    int rows = WORLD_H / TILE_SIZE;

    auto inb = [&](int x,int y){ return x>=0 && x<cols && y>=0 && y<rows; };
    auto toG = [&](Vec2f v){ return std::make_pair(int(v.x/TILE_SIZE), int(v.y/TILE_SIZE)); };

    auto [sx, sy] = toG(start);
    auto [gx, gy] = toG(goal);

    if (!inb(sx, sy) || !inb(gx, gy) || grid[gy*cols + gx]) return {};

    std::queue<std::pair<int,int>> q;
    std::vector<int> parent(cols*rows, -1);

    auto idx = [&](int x,int y){ return y*cols + x; };

    q.push({sx, sy});
    parent[idx(sx, sy)] = idx(sx, sy);

    const int DX[4] = {1, -1, 0, 0};
    const int DY[4] = {0, 0, 1, -1};
    bool found = false;

    while (!q.empty()) {
        auto [x, y] = q.front();
        q.pop();
        if (x == gx && y == gy) {
            found = true;
            break;
        }
        for (int i = 0; i < 4; i++) {
            int nx = x + DX[i], ny = y + DY[i];
            if (inb(nx, ny) && parent[idx(nx, ny)] < 0 && !grid[ny*cols + nx]) {
                parent[idx(nx, ny)] = idx(x, y);
                q.push({nx, ny});
            }
        }
    }

    if (!found) return {};

    std::vector<Vec2f> path;
    int cx = gx, cy = gy;
    while (cx != sx || cy != sy) {
        path.emplace_back(cx * TILE_SIZE + TILE_SIZE/2.f,
                          cy * TILE_SIZE + TILE_SIZE/2.f);
        int pi = parent[idx(cx, cy)];
        cx = pi % cols;
        cy = pi / cols;
    }
    std::reverse(path.begin(), path.end());
    return path;
}
