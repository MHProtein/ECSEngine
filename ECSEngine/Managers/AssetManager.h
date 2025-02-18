#pragma once
#include <memory>
#include <SDL.h>
#include <string>
#include <unordered_map>

#include "../Core/Singleton.h"

namespace ECSEngine
{
    struct Sprite;
    class AssetManager : public Singleton<AssetManager>
    {
    public:
        AssetManager();
        void Initialize(SDL_Renderer* renderer);

        std::shared_ptr<Sprite> AddSprite(const std::string& name, const std::string& path);
        bool HasSprite(const std::string& name);
        void RemoveSprite(const std::string& name);
        std::shared_ptr<Sprite> GetSprite(const std::string& name);
    
    private:
        SDL_Renderer* renderer;
        std::unordered_map<std::string, std::shared_ptr<Sprite>> sprites;
    };  
}


