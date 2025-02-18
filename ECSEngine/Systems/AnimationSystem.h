#pragma once
#include "../ECS/ECS.h"

#ifndef ANIMATION_SYSTEM_H
#define ANIMATION_SYSTEM_H

#define GLM_ENABLE_EXPERIMENTAL
namespace ECSEngine
{

    class AnimationSystem : public System
    {
    public:
        AnimationSystem();

        void AddEntityToSystem(Entity& entity) override;
        void Update() override;
        void RemoveEntityFromSystem(const Entity& entity) override;
        
    };    
}

#endif