#include "TileMap.h"

#include <fstream>
#include <sstream>
#include <utility>

#include "..\Components\Transform.h"
#include "../ECS/ECS.h"
#include "../Managers/AssetManager.h"


ECSEngine::TileMap::TileMap(const std::string& spriteName, const std::string& spritePath,
    const std::string& mapPath, size_t width, size_t height,
    const glm::vec2& tileSize, const glm::vec3& position, const SortingLayerMask& mask) : 
    sprite(AssetManager::Instance()->AddSprite(spriteName, spritePath)),
    mask(mask)
{
    sprite = AssetManager::Instance()->AddSprite(spriteName, spritePath);
    auto ifs = std::ifstream(mapPath, std::ifstream::in);
    Initialize(ifs, width, height, tileSize, position);  
}

ECSEngine::TileMap::TileMap(std::shared_ptr<Sprite> sprite, const std::string& mapPath,
        size_t width, size_t height, const glm::vec2& tileSize, const glm::vec3& position, const SortingLayerMask& mask) :
        sprite(std::move(sprite)),
        mask(mask)
{
    auto ifs = std::ifstream(mapPath, std::ifstream::in);
    Initialize(ifs, width, height, tileSize, position);  
}

float ECSEngine::TileMap::GetOffset(float size, int medium, int index, bool isEven)
{
    if(isEven)
    {
        auto halfSize = size / 2.0f;
        if(index == medium)
            return halfSize;
        else if (index == medium - 1)
            return -halfSize;
        else
        {
                    
            if(index < medium)
                return (index - medium + 1) * size - halfSize;
            else
                return (index - medium) * size + halfSize;
        }
    }
    else
    {
        if(index == medium)
            return 0;
        else
            return (index - medium) * size;
    }
}


void ECSEngine::TileMap::Initialize(std::ifstream& ifs, size_t width, size_t height, const glm::vec2& tileSize, const glm::vec3& position)
{
    std::string line;
    std::string value;
    
    entity = Registry::Instance()->CreateEntity();
    entity.AddComponent<LocalTransform>(position, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{1.0f, 1.0f, 1.0f});
    size_t tileWidth = sprite->surface->w / width;
    size_t tileHeight = sprite->surface->h / height;



    for (size_t i = 0; i < height; ++i)
    {
        for (size_t j = 0; j < width; ++j)
        {
            rects.emplace_back(glm::vec2{j * tileWidth, i * tileHeight}, glm::vec2{tileWidth, tileHeight});
        }
    }

    //Create entities
    size_t y = 0;
    while (std::getline(ifs, line))
    {
        std::stringstream ss(line);
        map.push_back({});
        while (std::getline(ss, value, ','))
        {
            auto& e = Registry::Instance()->CreateEntity();
            e.AddComponent<Parent>(entity);
            map[y].emplace_back(std::stoull(value), e);
        }
        y++;
    }
    int x = map[0].size();
    bool hEven = x % 2 == 0;
    bool vEven = y % 2 == 0;

    int hMid = x / 2;
    int vMid = y / 2;
    
    //Add components
    for (int i = 0; i < map.size(); ++i)
    {
        for (int j = 0; j < map[0].size(); ++j)
        {
            auto& pair = map[i][j];
            auto& transform = pair.second.AddComponent<LocalTransform>();

            float offsetX = GetOffset(tileSize.x, hMid, j, hEven);
            float offsetY = GetOffset(tileSize.y, vMid, i, vEven);
            
            transform.position = {offsetX, -offsetY, 0};
            transform.position += position;
            
            auto& renderer = pair.second.AddComponent<SpriteRenderer>(tileSize, sprite);
            renderer.rect = rects[pair.first];
            renderer.layer = mask;
        }
    }
}


