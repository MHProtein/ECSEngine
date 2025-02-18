#pragma once
#ifndef HIERARCHY_SYSTEM_H
#define HIERARCHY_SYSTEM_H

#include <map>
#include <glm/vec2.hpp>

#include "../ECS/ECS.h"


namespace ECSEngine
{
    struct LocalTransform;
    struct GlobalTransform;
    
    void UpdateChildren(const Entity& parent, GlobalTransform& parentTransform);

    
    class HierarchySystem : public System
    {
    public:
        HierarchySystem();
        void AddEntityToSystem(Entity& entity) override;
        void RemoveEntityFromSystem(const Entity& entity) override;
        void Update() override;

    private:
        //Parents have children but no parent
        std::pmr::set<Entity> supremeParents;
    };
}

#endif
