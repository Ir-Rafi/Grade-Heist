// PatrolCircuit.h
#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

// Returns the default patrol circuit points for the enemy's patrol path
inline std::vector<Vec2f> getRawPatrolCircuit() {
    return {
        {950.f, 600.f}, {900.f, 600.f}, {900.f, 660.f}, {900.f, 700.f}, {800.f, 700.f}, {300.f, 700.f},
        {300.f, 900.f}, {500.f, 900.f}, {600.f, 1500.f}, {387.f, 1512.f}, {681.f, 1543.f}, {782.f, 1583.f},
        {911.f, 1552.f}, {934.f, 1469.f}, {911.f, 1279.f}, {986.f, 1279.f}, {486.f, 1893.f}, {314.f, 2071.f},
        {190.f, 2021.f}, {397.f, 2455.f}, {431.f, 2287.f}, {725.f, 2287.f}, {794.f, 2174.f}, {794.f, 1998.f},
        {960.f, 2124.f}, {1129.f, 2124.f}, {1129.f, 2377.f}, {1290.f, 2373.f}, {1290.f, 1895.f}, {1466.f, 1539.f},
        {1770.f, 1539.f}, {1808.f, 1636.f}, {1770.f, 1941.f}, {1909.f, 2140.f}, {2016.f, 2285.f}, {2238.f, 2261.f},
        {2321.f, 2178.f}, {2210.f, 2014.f}, {2222.f, 1814.f}, {1835.f, 1814.f}, {1835.f, 1270.f}, {1909.f, 1237.f},
        {2010.f, 1344.f}, {2110.f, 1220.f}, {2346.f, 1220.f}, {2170.f, 1158.f}, {1902.f, 1251.f}, {1663.f, 1125.f},
        {1748.f, 931.f},  {1758.f, 743.f},  {2244.f, 743.f},  {2400.f, 663.f},  {2247.f, 571.f},  {2125.f, 733.f},
        {1774.f, 743.f},  {1734.f, 606.f},  {1489.f, 675.f},  {1311.f, 675.f},  {1311.f, 1400.f}, {1206.f, 1400.f},
        {1189.f, 422.f},  {1301.f, 422.f},  {1301.f, 674.f},  {974.f, 674.f}
    };
}

// Filters patrol points to remove those that collide with walls
inline std::vector<Vec2f> filterPatrolCircuit(
    const std::vector<Vec2f>& rawCircuit,
    float enemySize,
    const std::vector<sf::RectangleShape>& walls,
    bool (*checkCollision)(const sf::FloatRect&, const std::vector<sf::RectangleShape>&)
) {
    std::vector<Vec2f> circuit;
    for (const auto& p : rawCircuit) {
        sf::FloatRect box(p.x - enemySize / 2, p.y - enemySize / 2, enemySize, enemySize);
        if (!checkCollision(box, walls))
            circuit.push_back(p);
    }
    if (circuit.empty()) return rawCircuit;
    return circuit;
}
