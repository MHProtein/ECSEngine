#pragma once
#ifndef RENDERER_SYSTEM_H
#define RENDERER_SYSTEM_H
#include <map>
#include <SDL.h>

#include "../ECS/ECS.h"

namespace ECSEngine
{
    class RenderSystem : public System
    {
    public:
        RenderSystem() = delete;
        RenderSystem(SDL_Renderer* renderer, SDL_Window* window);
        void AddEntityToSystem(Entity& entity) override;
        void RemoveEntityFromSystem(const Entity& entity) override;
        void Update() override;
        void Destroy() override;

    private:
        void Initialize();
        void Render();
        
        SDL_Renderer* renderer;
        SDL_Window* window;
        SDL_Rect srcRect;

        std::pmr::multimap<struct SortingLayerMask, Entity> renderSequence;
    };

}

#endif