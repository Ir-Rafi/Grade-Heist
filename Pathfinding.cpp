#include "Pathfinding.h"
#include "Constants.h"
#include <queue>
#include <algorithm>

std::vector<Tile> findPathBFS(
    Tile start,
    Tile goal,
    const std::vector<bool>& coarseGrid,
    int colsCoarse,
    int rowsCoarse
) {
    auto inb = [&](int x, int y) { return x >= 0 && x < colsCoarse && y >= 0 && y < rowsCoarse; };
    auto idx = [&](int x, int y) { return y * colsCoarse + x; };

    int sx = start.first, sy = start.second;
    int gx = goal.first, gy = goal.second;

    if (!inb(sx, sy) || !inb(gx, gy) || !coarseGrid[idx(sx, sy)] || !coarseGrid[idx(gx, gy)])
        return {};

    std::queue<Tile> q;
    std::vector<int> parent(colsCoarse * rowsCoarse, -1);
    q.push(start);
    parent[idx(sx, sy)] = idx(sx, sy);

    const int DX[4] = {1, -1, 0, 0};
    const int DY[4] = {0, 0, 1, -1};
    bool found = false;

    while (!q.empty()) {
        auto [x, y] = q.front(); q.pop();
        if (x == gx && y == gy) { found = true; break; }

        for (int d = 0; d < 4; ++d) {
            int nx = x + DX[d], ny = y + DY[d];
            if (inb(nx, ny) && parent[idx(nx, ny)] < 0 && coarseGrid[idx(nx, ny)]) {
                parent[idx(nx, ny)] = idx(x, y);
                q.push({nx, ny});
            }
        }
    }

    if (!found) return {};

    std::vector<Tile> path;
    int cx = gx, cy = gy;
    while (cx != sx || cy != sy) {
        path.emplace_back(cx, cy);
        int p = parent[idx(cx, cy)];
        cx = p % colsCoarse;
        cy = p / colsCoarse;
    }
    path.emplace_back(sx, sy);
    std::reverse(path.begin(), path.end());
    return path;
}
