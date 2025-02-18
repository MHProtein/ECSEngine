#pragma once
#ifndef TAIL_MAP
#define TAIL_MAP
#include <memory>
#include <string>
#include <vector>

#include "../Components/SpriteRenderer.h"
#include "../ECS/ECS.h"
#include <glm/glm.hpp>

namespace ECSEngine
{
    class Registry;
    class AssetManager;
    class TileMap
    {
        typedef size_t index_type;
    public:

        Entity entity;
        
        TileMap(const std::string& spriteName, const std::string& spritePath,
            const std::string& mapPath, size_t width, size_t height, const glm::vec2& tileSize,
            const glm::vec3& position, const SortingLayerMask& mask = {0});
        TileMap(std::shared_ptr<Sprite> sprite, const std::string& mapPath,
            size_t width, size_t height, const glm::vec2& tileSize, const glm::vec3& position, const SortingLayerMask& mask = {0});
    private:
        
        float GetOffset(float size, int medium, int index, bool isEven);
        void Initialize(std::ifstream& ifs, size_t width, size_t height, const glm::vec2& tileSize, const glm::vec3& position);

        std::shared_ptr<Sprite> sprite;
        std::pmr::vector<std::pmr::vector<std::pair<index_type, Entity>>> map;
        std::pmr::vector<Rect> rects;
        SortingLayerMask mask;
    };
}

#endif