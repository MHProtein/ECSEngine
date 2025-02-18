#pragma once
#ifndef ECS_H
#define ECS_H
#include <bitset>
#include <memory>
#include <memory>
#include <optional>
#include <set>
#include <typeindex>
#include <unordered_map>
#include <vector>
#include <queue>

#include "../Core/Singleton.h"
#include "../Logger/Logger.h"

namespace ECSEngine
{
    const unsigned int MAX_COMPONENTS = 32;

    /// \brief
    /// We use a bitset to keep track of which components an entity has,
    /// and also helps keep track of which entities a system is interest in.
    typedef std::bitset<MAX_COMPONENTS> Signature;

    struct IComponent
    {
    protected:
        static int nextID;
    };

    template <class TComponent>
    class Component : public IComponent
    {
    public:
        static int GetID();
    };

    class Entity
    {
    public:
        Entity();
        Entity(int id);
        Entity(const Entity& other) = default;
        Entity& operator=(const Entity& other) = default;
        int GetID() const;
    
        template<class TComponent, typename ...TArgs> TComponent& AddComponent(TArgs&& ...args);
        template<class TComponent> void RemoveComponent() const;
        template<class TComponent> bool ContainsComponent() const;
        template<class TComponent> TComponent& GetComponent() const;
    
        class Registry* registry;
    
    protected:
        int id;
    };

    bool operator==(const Entity& lhs, const Entity& rhs);
    bool operator!=(const Entity& lhs, const Entity& rhs);
    bool operator<(const Entity& lhs, const Entity& rhs);

    /// \brief
    /// System
    /// Process entities containing a specific signature
    class System
    {
    public:
        System() = default;
        System(const System&) = default;
        virtual ~System() = default;

        virtual void AddEntityToSystem(Entity& entity);
        virtual void RemoveEntityFromSystem(const Entity& entity);
        virtual const std::pmr::vector<Entity>& GetSystemEntities() const;
        virtual const Signature& GetComponentSignature() const;
        bool HasEntity(const Entity& entity) const;
        
        template<class TComponent>
        void RequireComponent();

        virtual void Update();
        virtual void Destroy();
        
        int priority;
        
    protected:
        Signature componentSignature;
        std::pmr::vector<Entity> entities;
    };

    bool operator<(const System& lhs, const System& rhs);
    bool operator<(std::shared_ptr<System> lhs, std::shared_ptr<System> rhs);

    class IPool
    {
    public:
        virtual void RemoveAt(size_t index) = 0;
        virtual ~IPool() = default;
    };

    template <class T>
    class Pool : public IPool
    {
    public:
        Pool(size_t size)
        {
            data.resize(size);
        }
        ~Pool() override = default;
    
        std::pmr::vector<std::optional<T>>& GetData() { return data; }

        bool IsEmpty() const { return data.empty(); }
        size_t Size() const { return data.size(); }
        size_t Capacity() const { return data.capacity(); }

        void Resize(size_t newCapacity) { data.resize(newCapacity); }
        void Add(const T& object) { data.push_back(std::optional<T>(object)); }
        void Set(size_t index, const T& object) { data[index] = object; }
        T& Get(size_t index) { return data[index].value(); }
        T& operator[](size_t index) { return data.operator[](index).value(); }

        void RemoveAt(size_t index) override;
    
    private:
        std::pmr::vector<std::optional<T>> data;
    };

    class Registry : public Singleton<Registry>
    {
    public:
        Registry();
        
        Entity& CreateEntity();
        void DestroyEntity(const Entity& entity);
        bool HasEntity(const Entity& entity) const;

        template<class TComponent, typename ...TArgs> TComponent&  AddComponent(const Entity& entity, TArgs&& ...args);
        template<class TComponent> void RemoveComponent(const Entity& entity);
        template<class TComponent> bool ContainsComponent(const Entity& entity) const;
        template<class TComponent> TComponent& GetComponent(const Entity& entity) const;
    
        template<class TSystem, class ...TArgs> std::shared_ptr<TSystem> AddSystem(TArgs&& ...args);
        template<class TSystem> void RemoveSystem();
        template<class TSystem> bool HasSystem() const;
        template<class TSystem> TSystem& GetSystem() const;
        
        void Update();
        void UpdateSystems() const;

        void Destroy() const;
        
    private:
        void AddEntityToSystems(const Entity& entity) const;
        void RemoveEntityFromSystems(const Entity& entity) const;
        void RemoveEntityFromRegistry(const Entity& entity);
    
        size_t entityCount = 0;
    
        std::pmr::vector<std::shared_ptr<IPool>> componentPools;
        std::pmr::vector<Signature> entityComponentSignatures;
        
        std::pmr::unordered_map<std::type_index, std::shared_ptr<System>> systems;
        std::pmr::set<std::shared_ptr<System>> systemsUpdateQueue;

        std::pmr::set<Entity> entitiesToBeAddedToSystems;
        std::pmr::set<Entity> entitiesToBeRemovedFromSystems;
        std::pmr::set<Entity> entitiesToBeDestroyed;
    };




    /*
     * ////////////////////   Component   ////////////////////
     */
    template <class TComponent>
    int Component<TComponent>::GetID()
    {
        static auto id = nextID++;
        return id;
    }

    /*
     * ////////////////////   Entity   ////////////////////
     */

    template <class TComponent, typename ... TArgs>
    TComponent& Entity::AddComponent(TArgs&&... args)
    {
        return registry->AddComponent<TComponent>(*this, std::forward<TArgs>(args)...);
    }

    template <class TComponent>
    void Entity::RemoveComponent() const
    {
        registry->RemoveComponent<TComponent>(*this);
    }

    template <class TComponent>
    bool Entity::ContainsComponent() const
    {
        return registry->ContainsComponent<TComponent>(*this);
    }

    template <class TComponent>
    TComponent& Entity::GetComponent() const
    {
        return registry->GetComponent<TComponent>(*this);
    }

    /*
     * ////////////////////   System   ////////////////////
     */

    template <class TComponent>
    void System::RequireComponent()
    {
        const auto componentID = Component<TComponent>::GetID();
        componentSignature.set(componentID);
    }

    /*
     * ////////////////////   Registry   ////////////////////
     */

    template <class T>
    void Pool<T>::RemoveAt(size_t index)
    {
        data[index] = std::nullopt;
    }

    template <class TComponent, typename ... TArgs>
    TComponent& Registry::AddComponent(const Entity& entity, TArgs&&... args)
    {
        const auto componentID = Component<TComponent>::GetID();
        const auto entityID = entity.GetID();
        if(componentID >= componentPools.size())
        {
            componentPools.resize(componentID + 1, nullptr);
        }

        if(!componentPools[componentID])
        {
            std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>(MAX_COMPONENTS);
            componentPools[componentID] = newComponentPool;
        }

        std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentID]);

        if(entityID >= componentPool->Size())
            componentPool->Resize(entityCount);

        componentPool->Set(entityID, {std::forward<TArgs>(args)...});
        entityComponentSignatures[entityID].set(componentID);
        entitiesToBeAddedToSystems.insert(entity);
        Logger::Log("Component ID = " + std::to_string(componentID) + " was added to entity " + std::to_string(entityID));

        return componentPool->Get(entityID);
    }

    template <class TComponent>
    void Registry::RemoveComponent(const Entity& entity)
    {
        if(!ContainsComponent<TComponent>(entity))
            return;
        const auto componentID = Component<TComponent>::GetID();
        const auto entityID = entity.GetID();

        componentPools[componentID]->RemoveAt(entityID);
    
        entityComponentSignatures[entityID].set(componentID, false);
        entitiesToBeRemovedFromSystems.insert(entity);
        Logger::Log("Component ID = " + std::to_string(componentID) + " was removed from entity " + std::to_string(entityID));
    }

    template <class TComponent>
    bool Registry::ContainsComponent(const Entity& entity) const
    {
        return entityComponentSignatures[entity.GetID()].test(Component<TComponent>::GetID());
    }

    template <class TComponent>
    TComponent& Registry::GetComponent(const Entity& entity) const
    {
        auto pool = std::static_pointer_cast<Pool<TComponent>>(componentPools[Component<TComponent>::GetID()]);
        return pool->Get(entity.GetID());
    }

    template <class TSystem, class ... TArgs>
    std::shared_ptr<TSystem> Registry::AddSystem(TArgs&&... args)
    {
        std::shared_ptr<TSystem> ret = std::make_shared<TSystem>(std::forward<TArgs>(args)...);
        std::shared_ptr<System> newSystem = std::dynamic_pointer_cast<System>(ret);
        systems.insert(std::make_pair(std::type_index(typeid(TSystem)), newSystem));
        systemsUpdateQueue.insert(newSystem);
        Logger::Log(std::string(typeid(TSystem).name()) + " System is added");
        return ret;
    }

    template <class TSystem>
    void Registry::RemoveSystem()
    {
        systems.erase(systems.find(std::type_index(typeid(TSystem))));
        systemsUpdateQueue.erase(systemsUpdateQueue.find(std::type_index(typeid(TSystem))));
    }

    template <class TSystem>
    bool Registry::HasSystem() const
    {
        return systems.find(std::type_index(typeid(TSystem))) != systems.cend();
    }

    template <class TSystem>
    TSystem& Registry::GetSystem() const
    {
        return *std::static_pointer_cast<TSystem>(systems.find(std::type_index(typeid(TSystem)))->second);
    }
}

namespace std
{
    template<> struct hash<ECSEngine::System>
    {
        size_t operator()(const ECSEngine::System& system) const noexcept
        {
            return std::hash<string>()(system.GetComponentSignature().to_string());
        }
    };
}

    
#endif