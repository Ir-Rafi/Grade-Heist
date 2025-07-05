#include "UI.h"
#include "Constants.h"
#include <cmath>

static float intersectRaySegment(const sf::Vector2f& rayO, const sf::Vector2f& rayD,
                                 const sf::Vector2f& A, const sf::Vector2f& B)
{
    sf::Vector2f v1 = rayO - A;
    sf::Vector2f v2 = B - A;
    sf::Vector2f v3(-rayD.y, rayD.x);
    float dot = v2.x * v3.x + v2.y * v3.y;
    if (std::fabs(dot) < 1e-6f) return INFINITY;
    float t1 = (v2.x * v1.y - v2.y * v1.x) / dot;
    float t2 = (v1.x * v3.x + v1.y * v3.y) / dot;
    if (t1 >= 0.0f && t2 >= 0.0f && t2 <= 1.0f) {
        return t1;
    }
    return INFINITY;
}

void updateView(sf::View& view, const Player& player) {
    sf::FloatRect pb = player.getGlobalBounds();
    view.setCenter(player.getPosition() + sf::Vector2f(pb.width, pb.height) / 2.f);
}

void drawScene(sf::RenderWindow& window,
               const std::vector<sf::RectangleShape>& walls,
               const sf::RectangleShape& zone,
               Enemy& enemy,
               Player& player,
               const sf::Text& scoreText)
{
    for (auto& w : walls) window.draw(w);
    window.draw(zone);

    const int RAY_COUNT = 120;
    std::vector<sf::Vertex> vertices;
    vertices.reserve(RAY_COUNT + 2);
    sf::Vector2f center = enemy.shape().getPosition();
    vertices.push_back(sf::Vertex(center, LIGHT_COLOR));

    for (int i = 0; i <= RAY_COUNT; ++i) {
        float angle = (2 * 3.14159265f * i) / RAY_COUNT;
        sf::Vector2f dir(std::cos(angle), std::sin(angle));
        sf::Vector2f farPoint = center + dir * LIGHT_RADIUS;

        float minT = LIGHT_RADIUS;
        for (auto& w : walls) {
            sf::FloatRect rect = w.getGlobalBounds();
            sf::Vector2f p1(rect.left, rect.top);
            sf::Vector2f p2(rect.left + rect.width, rect.top);
            sf::Vector2f p3(rect.left + rect.width, rect.top + rect.height);
            sf::Vector2f p4(rect.left, rect.top + rect.height);
            float t;
            t = intersectRaySegment(center, dir, p1, p2);
            if (t < minT) minT = t;
            t = intersectRaySegment(center, dir, p2, p3);
            if (t < minT) minT = t;
            t = intersectRaySegment(center, dir, p3, p4);
            if (t < minT) minT = t;
            t = intersectRaySegment(center, dir, p4, p1);
            if (t < minT) minT = t;
        }
        sf::Vector2f finalPoint = center + dir * minT;
        vertices.push_back(sf::Vertex(finalPoint, LIGHT_COLOR));
    }

    sf::VertexArray lightFan(sf::TriangleFan, vertices.size());
    for (size_t i = 0; i < vertices.size(); ++i) {
        lightFan[i] = vertices[i];
    }
    window.draw(lightFan);

    window.draw(enemy.shape());
    player.draw(window);

    sf::Vector2f tl = window.getView().getCenter() - window.getView().getSize() / 2.f;
    sf::Text sc = scoreText;
    sc.setPosition(tl + sf::Vector2f(10, 10));
    window.draw(sc);
}