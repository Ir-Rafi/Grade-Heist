#pragma once
#include <vector>
#include <utility> // for std::pair

using Tile = std::pair<int, int>;

std::vector<Tile> findPathBFS(
    Tile start,
    Tile goal,
    const std::vector<bool>& coarseGrid,
    int colsCoarse,
    int rowsCoarse
);
