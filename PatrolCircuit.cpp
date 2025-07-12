#include "PatrolCircuit.h"

std::vector<Vec2f> filterPatrolCircuit(const std::vector<Vec2f>& rawCircuit, float enemySize, const std::vector<sf::RectangleShape>& walls, bool (*checkCollision)(const sf::FloatRect&, const std::vector<sf::RectangleShape>&)) {
    std::vector<Vec2f> circuit;
    for (auto& p : rawCircuit) {
        sf::FloatRect box(p.x - enemySize/2, p.y - enemySize/2, enemySize, enemySize);
        if (!checkCollision(box, walls))
            circuit.push_back(p);
    }
    if (circuit.empty()) return rawCircuit;
    return circuit;
}
