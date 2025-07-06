#include <SFML/Graphics.hpp>
#include <vector>
#include <functional>
#include <iostream>
#include <thread>
#include <atomic>
#include <cmath>

 #include "Constants.h"             // WORLD_W, WORLD_H, TILE_SIZE, Vec2f, WAYPOINT_REACH_THRESHOLD
// #include "Collision.h"             // checkCollision(...)
// #include "Enemy.h"                 // Enemy(walls, grid, circuit)
// #include "Player.h"                // Player class
// #include "Minimap.h"               // Minimap HUD
// #include "MapLoader.hpp"           // Tiled map loader (with getTaskRects)
// #include "codechallenge.h"         // CodeChallenge overlay
// #include "WiFiServerTask.hpp"      // WiFiServerTask overlay (wifioff)
// #include "WiFiLoginTask.hpp"       // WiFiLoginTask overlay (wifilin)
// #include "TypingChallenge.hpp"     // TypingChallenge overlay
// #include "PhotocopierTask.hpp"     // PhotocopierTask overlay
// #include "UI.h"

// // Function to check the orientation of the triplet (p, q, r)
// int orientation(const sf::Vector2f& p, const sf::Vector2f& q, const sf::Vector2f& r) {
//     float val = (q.y - p.y) * (r.x - q.x) - (q.x - p.x) * (r.y - q.y);
//     if (val == 0) return 0;  // Collinear
//     return (val > 0) ? 1 : 2;  // 1 = Clockwise, 2 = Counterclockwise
// }

// // Function to check if point q lies on segment pr
// bool onSegment(const sf::Vector2f& p, const sf::Vector2f& q, const sf::Vector2f& r) {
//     if (q.x <= std::max(p.x, r.x) && q.x >= std::min(p.x, r.x) &&
//         q.y <= std::max(p.y, r.y) && q.y >= std::min(p.y, r.y)) {
//         return true;
//     }
//     return false;
// }

// // Function to check if two segments (p1-q1 and p2-q2) intersect
// bool doIntersect(const sf::Vector2f& p1, const sf::Vector2f& q1, const sf::Vector2f& p2, const sf::Vector2f& q2) {
//     // Find the 4 orientations needed for the general and special cases
//     int o1 = orientation(p1, q1, p2);
//     int o2 = orientation(p1, q1, q2);
//     int o3 = orientation(p2, q2, p1);
//     int o4 = orientation(p2, q2, q1);

//     // General case
//     if (o1 != o2 && o3 != o4) {
//         return true;
//     }

//     // Special cases: p1, q1, p2, q2 are collinear, and p2 or q2 lies on the segment p1q1
//     if (o1 == 0 && onSegment(p1, p2, q1)) return true;
//     if (o2 == 0 && onSegment(p1, q2, q1)) return true;
//     if (o3 == 0 && onSegment(p2, p1, q2)) return true;
//     if (o4 == 0 && onSegment(p2, q1, q2)) return true;

//     // Otherwise the segments don't intersect
//     return false;
// }

// Function to create a thin FloatRect (line) between two points
sf::FloatRect createLineFloatRect(const sf::Vector2f& p1, const sf::Vector2f& p2) {
    // Calculate the vector between p1 and p2
    sf::Vector2f diff = p2 - p1;

    // Calculate the length of the line segment (distance between p1 and p2)
    float length = std::sqrt(diff.x * diff.x + diff.y * diff.y);

    // Create the rectangle (thin line)
    sf::FloatRect lineRect(p1.x, p1.y, length, 2.f);  // 1.f is the height (thickness)

    // Set the rotation angle to align the rectangle with the line
    float angle = std::atan2(diff.y, diff.x) * 180.f / 3.14159265f;  // atan2 gives the angle in radians

    // You can store rotation or use the angle to calculate the correct position during drawing
    lineRect.left -= length / 2.f;  // Shift left to center the rectangle (optional)

    return lineRect;
}

bool isend(const std::vector<sf::RectangleShape>& walls, const sf::Vector2f& pos_player, const sf::Vector2f& pos_enemy){
    if(dist(pos_enemy, pos_player) >= 100.f) return false;

    for (auto& w : walls) {
        sf::FloatRect rect = w.getGlobalBounds();
        // sf::Vector2f p1(rect.left, rect.top);
        // sf::Vector2f p2(rect.left + rect.width, rect.top);
        // sf::Vector2f p3(rect.left + rect.width, rect.top + rect.height);
        // sf::Vector2f p4(rect.left, rect.top + rect.height);
        // bool t = false;
        // t |= doIntersect(pos_player, pos_enemy, p1, p2);
        // t |= doIntersect(pos_player, pos_enemy, p1, p3);
        // t |= doIntersect(pos_player, pos_enemy, p1, p4);

        sf::FloatRect line = createLineFloatRect(pos_enemy, pos_player);
        bool t = line.intersects(rect);

        if(t) return t;
    }

    return false;
}