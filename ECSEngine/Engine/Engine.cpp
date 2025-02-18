#include "Engine.h"

#include <SDL_image.h>

#include "../Components/Animation.h"
#include "../Components/RigidBody.h"
#include "../Components/SpriteRenderer.h"
#include "../Core/TileMap.h"
#include "../Managers/AssetManager.h"
#include "../Systems/AnimationSystem.h"
#include "../Systems/HierarchySystem.h"
#include "../Systems/RenderSystem.h"
#include "../Systems/MovementSystem.h"
#include "glm/gtx/string_cast.hpp"


float ECSEngine::Time::deltaTime;   

ECSEngine::Engine::Engine(size_t windowWidth, size_t windowHeight) : windowWidth(windowWidth),
                                                                     windowHeight(windowHeight),
                                                                     millisecsPreviousFrame(0), window(nullptr),
                                                                     renderer(nullptr),
                                                                     displayMode(),
                                                                     tank(Entity(-1)),
                                                                     transform(nullptr)
{
    isRunning = false;
}

ECSEngine::Engine::~Engine()
{
    
}

void ECSEngine::Engine::Initialize()
{
    InitializeSDL();
    isRunning = true;

    AssetManager::Instance()->Initialize(renderer);
}

void ECSEngine::Engine::Setup()
{
    auto bgLayer = TagsLayersManager::Instance()->AddSortingLayer("BackGround");
    auto groundVehicleLayer = TagsLayersManager::Instance()->AddSortingLayer("GroundVehicle");
    auto airVehicleLayer = TagsLayersManager::Instance()->AddSortingLayer("airVehicleLayer");
    Registry::Instance()->AddSystem<Movement>();
    Registry::Instance()->AddSystem<RenderSystem>(renderer, window);
    Registry::Instance()->AddSystem<HierarchySystem>();
    Registry::Instance()->AddSystem<AnimationSystem>();
    
    tank = Registry::Instance()->CreateEntity();
    tank.AddComponent<LocalTransform>(glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.0f, 1.0f, 1.0f));
    //tank.AddComponent<RigidBody>(glm::vec3(20.0f, 20.0f, 0.0f));
    tank.AddComponent<SpriteRenderer>(glm::vec2(64.0f, 64.0f),
        AssetManager::Instance()->AddSprite("tank", "Assets/Sprites/tank.png"), groundVehicleLayer);

    
    auto& bird = Registry::Instance()->CreateEntity();
    bird.AddComponent<LocalTransform>(glm::vec3(0.0f, 50.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.f, 1.f, 1.0f));
    //bird.AddComponent<RigidBody>(glm::vec2(-20.0f, -20.0f));
    bird.AddComponent<SpriteRenderer>(glm::vec2(32.0f, 32.0f),
        AssetManager::Instance()->AddSprite("bird", "Assets/Sprites/Bird.png"), airVehicleLayer);
    bird.AddComponent<Parent>(tank);
    bird.AddComponent<GlobalTransform>();

    auto& chopper = Registry::Instance()->CreateEntity();
    chopper.AddComponent<LocalTransform>(glm::vec3(50.0f, 50.0f, 0.0f),
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(1.f, 1.f, 1.0f));
    //bird.AddComponent<RigidBody>(glm::vec2(-20.0f, -20.0f));
    chopper.AddComponent<SpriteRenderer>(glm::vec2(128.0f, 128.0f),
        AssetManager::Instance()->AddSprite("chopper", "Assets/Sprites/chopper.png"), airVehicleLayer);

    auto& anim = chopper.AddComponent<Animation>(AssetManager::Instance()->GetSprite("chopper"), true, 4);
    anim.rects.emplace_back(0, 0, 32, 32);
    anim.rects.emplace_back(32, 0, 32, 32);
    
    TileMap map(AssetManager::Instance()->AddSprite("jungle", "Assets/TileMaps/jungle.png"),
     "Assets/TileMaps/jungle.map", 10, 3, {64, 64}, {0, 0, 0}, bgLayer);
    //map.entity.AddComponent<RigidBody>(glm::vec3{10.0f, 10.0f, 0.0f});
    //transform = &map.entity.GetComponent<LocalTransform>();
    //transform->scale = {0.5f, 0.5f, 1.0f};
    transform.reset(&tank.GetComponent<LocalTransform>());
}

void ECSEngine::Engine::Run()
{
    Setup();
    while (isRunning)
    {
        ProcessInput();
        RenderClear();
        Update();
        Render();
    }
}

void ECSEngine::Engine::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event) != 0)
    {
        switch (event.type)
        {
            case SDL_QUIT:
                isRunning = false;
                break;

            case SDL_KEYDOWN:
                if(event.key.keysym.sym == SDLK_ESCAPE)
                    isRunning = false;
                break;
            
            default:
                break;
        }
    }
}

void ECSEngine::Engine::Update()
{
    auto timeout = MILLISECS_PER_FRAME - (SDL_GetTicks() - millisecsPreviousFrame);
    if(timeout > 0 && timeout <= MILLISECS_PER_FRAME)
        SDL_Delay(timeout);
    Time::deltaTime = (SDL_GetTicks() - millisecsPreviousFrame) / 1000.0f;
    millisecsPreviousFrame = SDL_GetTicks();
    
    Registry::Instance()->UpdateSystems();
    Registry::Instance()->Update();
    
    transform->rotation.z -= 50.0f * Time::deltaTime;
}

void ECSEngine::Engine::Render() const
{

    SDL_RenderPresent(renderer);
}

void ECSEngine::Engine::Destroy() const
{
    Registry::Instance()->Destroy();
    ShutDownSDL();
}

void ECSEngine::Engine::InitializeSDL()
{
    if(SDL_Init(SDL_INIT_EVERYTHING) != 0)
    { 
        Logger::Error("Error initializing SDL.");
        return;
    }

    SDL_GetCurrentDisplayMode(0, &displayMode);
    
    window = SDL_CreateWindow("ECS Engine", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        windowWidth, windowHeight, SDL_WINDOW_BORDERLESS);
    
    if(!window)
    {
        Logger::Error("Error creating SDL window.");
        return; 
    }
    
    //-1 -- Default Display
    renderer = SDL_CreateRenderer(window, -1, 0);
    if(!renderer)
    {
        Logger::Error("Error creating SDL renderer.");
        return; 
    }
    SDL_SetWindowBordered(window, SDL_TRUE);
}

void ECSEngine::Engine::ShutDownSDL() const
{
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

void ECSEngine::Engine::RenderClear() const
{
    SDL_SetRenderDrawColor(renderer, 100, 46, 43, 255);
    SDL_RenderClear(renderer);
}
