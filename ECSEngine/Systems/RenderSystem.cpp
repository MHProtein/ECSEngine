#include "RenderSystem.h"

#include "../Components/SpriteRenderer.h"
#include "..\Components\Transform.h"
#include "../Core/Sprite.h"

ECSEngine::RenderSystem::RenderSystem(SDL_Renderer* renderer, SDL_Window* window) : renderer(renderer), window(window)
{
    Initialize();
}

void ECSEngine::RenderSystem::Initialize()
{
    priority = 10000;
    RequireComponent<LocalTransform>();
    RequireComponent<SpriteRenderer>();
}

void ECSEngine::RenderSystem::AddEntityToSystem(Entity& entity)
{
    System::AddEntityToSystem(entity);

    auto rend = entity.GetComponent<SpriteRenderer>();
    renderSequence.emplace(rend.layer, entity);
}

void ECSEngine::RenderSystem::RemoveEntityFromSystem(const Entity& entity)
{
    System::RemoveEntityFromSystem(entity);
    auto rend = entity.GetComponent<SpriteRenderer>();
    renderSequence.erase(rend.layer);
}

void ECSEngine::RenderSystem::Update()
{
    System::Update();
    Render();
}

void ECSEngine::RenderSystem::Destroy()
{
    System::Destroy();
}

void ECSEngine::RenderSystem::Render()
{
    for (auto& entityEntry : renderSequence)
    {
        auto entity = entityEntry.second;
        glm::vec3 position;
        glm::vec3 rotation;
        glm::vec3 scale;
        if (entity.ContainsComponent<Parent>())
        {
            GlobalTransform& transform = entity.GetComponent<GlobalTransform>();
            rotation = glm::degrees(transform.GetRotation());
            scale = transform.GetScale();
            position = transform.GetPosition();
        }
        else
        {
            LocalTransform& transform = entity.GetComponent<LocalTransform>();
            rotation = transform.rotation;
            scale = transform.scale;
            position = transform.position;
        }

        int w,h;
        SDL_GetWindowSize(window, &w, &h);
        
        const SpriteRenderer& spriteRenderer = entity.GetComponent<SpriteRenderer>();
        auto rectW = spriteRenderer.size.x * scale.x;
        auto rectH = spriteRenderer.size.y * scale.y;
        
        SDL_Rect destRect = {
            static_cast<int>(position.x - rectW / 2.0f + w / 2.0f),
            static_cast<int>(-position.y - rectH / 2.0f + h / 2.0f),
            static_cast<int>(rectW),
            static_cast<int>(rectH) };
        
        srcRect.x = static_cast<int>(spriteRenderer.rect.center.x);
        srcRect.y = static_cast<int>(spriteRenderer.rect.center.y);
        srcRect.w = static_cast<int>(spriteRenderer.rect.extent.x);
        srcRect.h = static_cast<int>(spriteRenderer.rect.extent.y);
        
        SDL_Point center = { static_cast<int>(destRect.w / 2.0f), static_cast<int>(destRect.h / 2.0f) };
        SDL_RenderCopyEx(renderer,
            spriteRenderer.sprite->texture,
            &srcRect, &destRect, -rotation.z , &center , SDL_FLIP_NONE);
    }
}
