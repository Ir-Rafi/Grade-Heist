#include "GameLogic.h"
#include "Constants.h"
#include <cmath>

// Helper: check if segment [A,B] intersects any wall edge
static bool isOccluded(const sf::Vector2f& A, const sf::Vector2f& B, const std::vector<sf::RectangleShape>& walls) {
    auto intersectSeg = [&](const sf::Vector2f& P, const sf::Vector2f& Q,
                            const sf::Vector2f& R, const sf::Vector2f& S) {
        // Segment P->Q and R->S intersection (excluding endpoints)
        sf::Vector2f PQ = Q - P;
        sf::Vector2f RS = S - R;
        float det = PQ.x * RS.y - PQ.y * RS.x;
        if (std::fabs(det) < 1e-6f) return false; // parallel or collinear
        sf::Vector2f PR = R - P;
        float t = (RS.x * PR.y - RS.y * PR.x) / det;
        float u = (PQ.x * PR.y - PQ.y * PR.x) / det;
        return (t > 0.0f && t < 1.0f && u > 0.0f && u < 1.0f);
    };

    for (auto& w : walls) {
        sf::FloatRect rect = w.getGlobalBounds();
        sf::Vector2f p1(rect.left, rect.top);
        sf::Vector2f p2(rect.left + rect.width, rect.top);
        sf::Vector2f p3(rect.left + rect.width, rect.top + rect.height);
        sf::Vector2f p4(rect.left, rect.top + rect.height);
        if (intersectSeg(A, B, p1, p2)) return true;
        if (intersectSeg(A, B, p2, p3)) return true;
        if (intersectSeg(A, B, p3, p4)) return true;
        if (intersectSeg(A, B, p4, p1)) return true;
    }
    return false;
}

void updateGameLogic(Player& player,
                     Enemy& enemy,
                     const std::vector<sf::RectangleShape>& walls,
                     float dt,
                     int& score,
                     sf::Text& scoreText)
{
    // 1) Player movement & update
    player.handleInput(dt);
    player.update();

    // 2) Enemy chasing / patrol logic
    // Use the zone as before (if you still want zone-based chasing, otherwise skip)
    // Example: enemy chases if in zone, else continues patrol
    // (Assuming 'zone' logic is no longer passed; using walls only)
    // If you still want zone-based chase, pass zone to this function and do:
    // enemy.setChasing(zone.getGlobalBounds().intersects(player.getGlobalBounds()));
    // For now, we simply let enemy always patrol unless player is within light or some condition.
    // Here, assume Enemy.update handles both patrol and chase based on internal state.
    enemy.update(player.getPosition(), dt); // patrol by default; if chasing state set elsewhere

    // 3) Light collision: award only when player touches visible light area
    static bool wasInLightLastFrame = false;

    // Enemy center
    sf::FloatRect eB = enemy.shape().getGlobalBounds();
    sf::Vector2f eCenter(eB.left + eB.width * 0.5f,
                         eB.top  + eB.height * 0.5f);

    // Player AABB
    sf::FloatRect pB = player.getGlobalBounds();
    // Find closest point on player rect to enemy center
    float closestX = std::max(pB.left, std::min(eCenter.x, pB.left + pB.width));
    float closestY = std::max(pB.top,  std::min(eCenter.y, pB.top  + pB.height));
    sf::Vector2f closestPoint(closestX, closestY);

    // Distance squared to that closest point
    float dx = eCenter.x - closestPoint.x;
    float dy = eCenter.y - closestPoint.y;
    float distSq = dx*dx + dy*dy;

    bool withinCircle = (distSq <= LIGHT_RADIUS * LIGHT_RADIUS);
    bool occluded = false;
    if (withinCircle) {
        occluded = isOccluded(eCenter, closestPoint, walls);
    }
    bool isNowInLight = (withinCircle && !occluded);
    if (isNowInLight && !wasInLightLastFrame) {
        score++;
        scoreText.setString("Score: " + std::to_string(score));
    }
    wasInLightLastFrame = isNowInLight;
}
