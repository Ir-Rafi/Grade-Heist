// MapLoader.cpp
#include "world/MapLoader.hpp"
#include <iostream>

// Helper for empty-vector return
static const std::vector<sf::FloatRect> EMPTY_RECTS;

MapLoader::MapLoader(const std::string& mapPath,
                     const std::string& tilesetPath,
                     int tileWidth,
                     int tileHeight)
: mapFile(mapPath)
, tilesetFile(tilesetPath)
, tileW(tileWidth)
, tileH(tileHeight)
{
}

bool MapLoader::load()
{
    tson::Tileson parser;
    std::unique_ptr<tson::Map> map = parser.parse(mapFile);
    if (map->getStatus() != tson::ParseStatus::OK) {
        std::cerr << "Tileson error: Failed to parse map\n";
        return false;
    }

    if (!tilesetTexture.loadFromFile(tilesetFile)) {
        std::cerr << "Failed to load tileset: " << tilesetFile << "\n";
        return false;
    }

    auto* layer = map->getLayer("Tile Layer 1");
    if (!layer || layer->getType() != tson::LayerType::TileLayer) {
        std::cerr << "Invalid or missing tile layer\n";
        return false;
    }

    const tson::Tileset& tileset = map->getTilesets()[0];
    int columns = tileset.getColumns();
    auto& tiles = layer->getTileObjects();

    vertices.setPrimitiveType(sf::Quads);
    vertices.resize(tiles.size() * 4);

    int idx = 0;
    for (auto& tileObj : tiles) {
        tson::TileObject& tile = tileObj.second;
        if (!tile.getTile()) continue;

        // World position
        tson::Vector2i gridPos = tile.getPositionInTileUnits();
        float x = float(gridPos.x * tileW);
        float y = float(gridPos.y * tileH);

        // Texture coords indices
        int id = tile.getTile()->getId();
        int tu = id % columns;
        int tv = id / columns;

        sf::Vertex* quad = &vertices[idx * 4];
        quad[0].position = { x,          y };
        quad[1].position = { x + tileW,  y };
        quad[2].position = { x + tileW,  y + tileH };
        quad[3].position = { x,          y + tileH };

        // **Updated**: use sf::Vector2f ctor to avoid narrowing
        quad[0].texCoords = sf::Vector2f(tu * tileW,      tv * tileH);
        quad[1].texCoords = sf::Vector2f((tu + 1) * tileW, tv * tileH);
        quad[2].texCoords = sf::Vector2f((tu + 1) * tileW, (tv + 1) * tileH);
        quad[3].texCoords = sf::Vector2f(tu * tileW,      (tv + 1) * tileH);

        // Read the custom "type" property (empty if missing)
        std::string type = tile.getTile()
            ->getProperties()
            .getValue<std::string>("type");

        if (type == "coll") {
            collisionRects.emplace_back(x, y, float(tileW), float(tileH));
        }
        else if (!type.empty()) {
            taskRects[type].emplace_back(x, y, float(tileW), float(tileH));
        }

        ++idx;
    }

    return true;
}

const std::vector<sf::FloatRect>&
MapLoader::getCollisionRects() const
{
    return collisionRects;
}

const std::vector<sf::FloatRect>&
MapLoader::getTaskRects(const std::string& type) const
{
    auto it = taskRects.find(type);
    return (it != taskRects.end() ? it->second : EMPTY_RECTS);
}

void MapLoader::draw(sf::RenderTarget& target,
                     sf::RenderStates states) const
{
    states.transform *= getTransform();
    states.texture   = &tilesetTexture;
    target.draw(vertices, states);
}
