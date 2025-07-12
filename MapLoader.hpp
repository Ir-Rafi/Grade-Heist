// MapLoader.hpp
#pragma once

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>
#include <unordered_map>
#include "tileson/include/tileson.hpp"

class MapLoader : public sf::Drawable, public sf::Transformable {
public:
    MapLoader(const std::string& mapPath,
              const std::string& tilesetPath,
              int tileWidth,
              int tileHeight);

    bool load();

    // Collision-only tiles (type == "coll")
    const std::vector<sf::FloatRect>& getCollisionRects() const;

    // Any other custom tile-type (e.g. "cout", "wifioff", etc.)
    const std::vector<sf::FloatRect>& getTaskRects(const std::string& type) const;

    virtual void draw(sf::RenderTarget& target,
                      sf::RenderStates states) const override;

private:
    std::string mapFile;
    std::string tilesetFile;
    int tileW, tileH;

    sf::Texture tilesetTexture;
    sf::VertexArray vertices;

    // Rects for collision tiles
    std::vector<sf::FloatRect> collisionRects;

    // Rects for all other tile-types keyed by their "type" property
    std::unordered_map<std::string, std::vector<sf::FloatRect>> taskRects;
};
