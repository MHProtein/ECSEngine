#pragma once
#ifndef GAME_H
#define GAME_H
#define GLM_ENABLE_EXPERIMENTAL
#include <SDL.h>

#include "../ECS/ECS.h"
#include "..\Components\Transform.h"


namespace ECSEngine
{
    class AssetManager;
    class RenderSystem;
    constexpr uint32_t FPS = 30;
    constexpr uint32_t MILLISECS_PER_FRAME = 1000 / FPS;

    struct Time
    {
        static float deltaTime;    
    };
    
    class Engine
    {
    public:
        Engine(size_t windowWidth, size_t windowHeight);
        ~Engine();
    
        void Initialize();
        void Setup();
        void Run();
        void ProcessInput();
        void Update();
        void Render() const;
        void Destroy() const;

    private:
        void InitializeSDL();
        void ShutDownSDL() const;
        void RenderClear() const;
        
        bool isRunning;

        size_t windowWidth;
        size_t windowHeight;

        uint32_t millisecsPreviousFrame;

        SDL_Window* window;
        SDL_Renderer* renderer;
        SDL_DisplayMode displayMode;
        
        Entity tank;
        std::shared_ptr<LocalTransform> transform;
    };
}

#endif




