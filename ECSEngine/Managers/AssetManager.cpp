#include "AssetManager.h"

#include <SDL_image.h>

#include "..\Core\Sprite.h"
#include "../Logger/Logger.h"

ECSEngine::AssetManager::AssetManager() : renderer(nullptr)
{
}

void ECSEngine::AssetManager::Initialize(SDL_Renderer* rendererParam)
{
    this->renderer = rendererParam;
}

std::shared_ptr<ECSEngine::Sprite> ECSEngine::AssetManager::AddSprite(const std::string& name, const std::string& path)
{
    SDL_Surface* surface = IMG_Load(path.data());
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_FreeSurface(surface);
    if(texture == nullptr)
        Logger::Error("Failed To Load Sprite From Path : " + path);
    std::shared_ptr<Sprite> sprite = std::make_shared<Sprite>(path, surface, texture);
    sprites.insert(std::make_pair(name, sprite));
    return sprite;
}

bool ECSEngine::AssetManager::HasSprite(const std::string& name)
{
    return sprites.find(name) != sprites.cend();
}

void ECSEngine::AssetManager::RemoveSprite(const std::string& name)
{
    sprites.erase(name);
}

std::shared_ptr<ECSEngine::Sprite> ECSEngine::AssetManager::GetSprite(const std::string& name)
{
    return sprites.find(name)->second;
}
