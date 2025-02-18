#include "ECS.h"
#include <algorithm>

#include "../Logger/Logger.h"

int ECSEngine::IComponent::nextID;

ECSEngine::Entity::Entity() : registry(nullptr), id(0)
{
}

ECSEngine::Entity::Entity(int id) : registry(nullptr), id(id)
{
}

int ECSEngine::Entity::GetID() const
{
    return id;
}

bool ECSEngine::operator==(const Entity& lhs, const Entity& rhs)
{
    return lhs.GetID() == rhs.GetID();
}

bool ECSEngine::operator!=(const Entity& lhs, const Entity& rhs)
{
    return !(lhs == rhs);
}

bool ECSEngine::operator<(const Entity& lhs, const Entity& rhs)
{
    return lhs.GetID() < rhs.GetID();
}

void ECSEngine::System::AddEntityToSystem(Entity& entity)
{
    entities.push_back(entity);
}

void ECSEngine::System::RemoveEntityFromSystem(const Entity& entity)
{
    entities.erase(std::remove_if(entities.begin(), entities.end(),
        [&entity](const Entity& other) { return entity == other; }), entities.end());
}

const std::pmr::vector<ECSEngine::Entity>& ECSEngine::System::GetSystemEntities() const
{
    return entities;
}

const ECSEngine::Signature& ECSEngine::System::GetComponentSignature() const
{
    return componentSignature;
}

void ECSEngine::System::Update()
{
    
}

void ECSEngine::System::Destroy()
{
    
}

bool ECSEngine::operator<(const System& lhs, const System& rhs)
{
    return lhs.priority < rhs.priority;
}

bool ECSEngine::operator<(std::shared_ptr<System> lhs, std::shared_ptr<System> rhs)
{
    return operator<(*lhs, *rhs);
}

bool ECSEngine::System::HasEntity(const Entity& entity) const
{
    return std::find(entities.cbegin(), entities.cend(), entity) != entities.cend();
}

void ECSEngine::Registry::Update()
{
    //todo : Add entities in the to be created list
    //todo : Remove entities in the to be removed list

    for(const auto& entity : entitiesToBeAddedToSystems)
    {
        AddEntityToSystems(entity);
    }
    entitiesToBeAddedToSystems.clear();
    for (const auto& entity : entitiesToBeRemovedFromSystems)
    {
        RemoveEntityFromSystems(entity);
    }
    entitiesToBeRemovedFromSystems.clear();

    for (auto& entity : entitiesToBeDestroyed)
    {
        RemoveEntityFromRegistry(entity);
    }
    entitiesToBeDestroyed.clear();  
}

void ECSEngine::Registry::UpdateSystems() const
{
    for (auto& system : systemsUpdateQueue)
    {
        system->Update();
    }
}

void ECSEngine::Registry::Destroy() const
{
    for (auto& system : systems)
    {
        system.second->Destroy();
    }
}

ECSEngine::Registry::Registry() = default;

ECSEngine::Entity& ECSEngine::Registry::CreateEntity()
{
    Entity& entity = const_cast<Entity&>(*entitiesToBeAddedToSystems.emplace(entityCount++).first);
    if(entity.GetID() >= entityComponentSignatures.size())
        entityComponentSignatures.resize(entity.GetID() + 1);

    entity.registry = this;
    
    Logger::Log("Entities Created with id = " + std::to_string(entity.GetID()));
    
    return entity;
}

void ECSEngine::Registry::DestroyEntity(const Entity& entity)
{
    if(!HasEntity(entity.GetID()))
        return;
    entitiesToBeRemovedFromSystems.insert(entity);
    entitiesToBeDestroyed.insert(entity);
}

bool ECSEngine::Registry::HasEntity(const Entity& entity) const
{
    return entity.GetID() < entityComponentSignatures.size();
}

void ECSEngine::Registry::AddEntityToSystems(const Entity& entity) const
{
    const auto entityID = entity.GetID();
    auto& sig = entityComponentSignatures[entityID];
     
    for (auto& system : systems)
    {
        const uint64_t systemSig = system.second->GetComponentSignature().to_ulong();
        if((sig.to_ulong() & systemSig) == systemSig)
        {
            system.second->AddEntityToSystem(const_cast<Entity&>(entity));
        }
    }
}

void ECSEngine::Registry::RemoveEntityFromSystems(const Entity& entity) const
{
    const auto entityID = entity.GetID();
    const auto& sig = entityComponentSignatures[entityID];

    for (auto& system : systems)
    {
        if(!system.second->HasEntity(entity))
            continue;
        const auto& systemSig = system.second->GetComponentSignature().to_ulong();
        if((sig.to_ulong() & systemSig) == systemSig)
            return;
        system.second->RemoveEntityFromSystem(entity);
    }
}

void ECSEngine::Registry::RemoveEntityFromRegistry(const Entity& entity)
{
    const auto entityID = entity.GetID();
    auto& sig = entityComponentSignatures[entityID];
    for(int i = 0; i != componentPools.size(); ++i)
    {
        if(!sig.test(i))
            return;
        componentPools[i]->RemoveAt(entityID);
        sig.set(i, false);
    }
}
