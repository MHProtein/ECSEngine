#include "AnimationSystem.h"

#include <glm/gtx/string_cast.hpp>

#include "../Components/Animation.h"
#include "../Components/SpriteRenderer.h"
#include "../Components/Transform.h"


ECSEngine::AnimationSystem::AnimationSystem()
{
    RequireComponent<LocalTransform>();
    RequireComponent<Animation>();
    priority = 3;
}

void ECSEngine::AnimationSystem::AddEntityToSystem(Entity& entity)
{
    System::AddEntityToSystem(entity);
    
    auto& animation = entity.GetComponent<Animation>();
    if(!entity.ContainsComponent<SpriteRenderer>())
        entity.AddComponent<SpriteRenderer>(animation.sprite);

    SpriteRenderer& renderer = entity.GetComponent<SpriteRenderer>();

    renderer.rect = animation.rects[animation.currentRect];
} 

void ECSEngine::AnimationSystem::Update()
{
    System::Update();

    for (auto& entity : entities)
    {
        auto& animation = entity.GetComponent<Animation>();
        auto& renderer = entity.GetComponent<SpriteRenderer>();
        if(++animation.currentFrame > animation.intervalFrames)
        {
            animation.currentFrame = 0;
            if(++animation.currentRect >= animation.rects.size())
            {
                if(animation.loop)
                    animation.currentRect =0;
                else
                    --animation.currentRect;
            }
            renderer.rect = animation.rects[animation.currentRect];
            Logger::Log(glm::to_string(renderer.rect.center) + " " + glm::to_string(renderer.rect.extent));
        }
    }
    
}

void ECSEngine::AnimationSystem::RemoveEntityFromSystem(const Entity& entity)
{
    System::RemoveEntityFromSystem(entity);
}
