#include "MovementSystem.h"

#include "../Components/Transform.h"
#include "../Components/RigidBody.h"
#include "../Engine/Engine.h"

ECSEngine::Movement::Movement()
{
    priority = 0;
    RequireComponent<LocalTransform>();
    RequireComponent<RigidBody>();
}

void ECSEngine::Movement::Update()
{
    System::Update();

    for (auto& entity : entities)
    {
        auto& rigidBody = entity.GetComponent<RigidBody>();
        
        auto& transform = entity.GetComponent<LocalTransform>();
        
        transform.position += rigidBody.velocity * Time::deltaTime;
    }
}
