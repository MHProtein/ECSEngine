#include "HierarchySystem.h"

#include <iostream>
#include <ostream>

#include "../Components/Transform.h"

ECSEngine::HierarchySystem::HierarchySystem()
{
    RequireComponent<LocalTransform>();
    RequireComponent<Parent>();
    priority = 1;
}

void ECSEngine::HierarchySystem::AddEntityToSystem(Entity& entity)
{
    if(!entity.ContainsComponent<GlobalTransform>())
        entity.AddComponent<GlobalTransform>();
    
    auto& parent = entity.GetComponent<Parent>().entity;
    
    if(!entity.ContainsComponent<FormerParent>())
        entity.AddComponent<FormerParent>(parent);
    else
        entity.GetComponent<FormerParent>().entity = parent;
    
    //If a supreme parent is a child, remove it
    auto it = supremeParents.find(entity);
    if(it != supremeParents.cend())
        supremeParents.erase(it);
    

    if(!parent.ContainsComponent<GlobalTransform>())
        parent.AddComponent<GlobalTransform>();
    
    if(parent.ContainsComponent<Children>())
    {
        Children& children = parent.GetComponent<Children>();
        children.children.push_back(entity);
        return;
    }
    Children& children = parent.AddComponent<Children>();
    children.children.push_back(entity);
    
    if(!parent.ContainsComponent<Parent>())
        supremeParents.insert(parent);
}

void ECSEngine::HierarchySystem::RemoveEntityFromSystem(const Entity& entity)
{
    if(entity.ContainsComponent<Children>())
        supremeParents.insert(entity);
    else
    {
        if(entity.ContainsComponent<GlobalTransform>())
            entity.RemoveComponent<GlobalTransform>();
    }

    if(entity.ContainsComponent<FormerParent>())
    {
        auto& parent = entity.GetComponent<FormerParent>().entity;

        auto& children = parent.GetComponent<Children>().children;

        children.erase(std::find(children.cbegin(), children.cend(), parent));

        if(children.empty())
            parent.RemoveComponent<Children>();

        if(supremeParents.find(parent) != supremeParents.cend())
            supremeParents.erase(parent);
    }
}

void ECSEngine::HierarchySystem::Update()
{
    System::Update();

    for (auto& parent : supremeParents)
    {
        auto& globalTransform = parent.GetComponent<GlobalTransform>();
        globalTransform.matrix = parent.GetComponent<LocalTransform>().GetMatrix();
        UpdateChildren(parent, globalTransform);
    }

    std::cout<<std::endl;
}

void ECSEngine::UpdateChildren(const Entity& parent, GlobalTransform& parentTransform)
{
    if(!parent.ContainsComponent<Children>())
        return;
    auto& children = parent.GetComponent<Children>().children;

    for (auto& child : children)
    {
        auto& globalTransform = child.GetComponent<GlobalTransform>();
        auto& transform = child.GetComponent<LocalTransform>();

        globalTransform.matrix = parentTransform.matrix * transform.GetMatrix();

        UpdateChildren(child, globalTransform);
    }
}
