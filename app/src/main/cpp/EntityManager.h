#pragma once
#include "Entity.h"
#include <algorithm>
#include <vector>

class EntityManager {
// public:
private:
    EntityManager(EntityManager&&) = delete;
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    static void AddEntity(Entity* e) { Get().m_Entities.push_back(e); }

    static void Remove(Entity* e) { Get().RemoveInternal(e);}

    static Entity* GetEntityAt(int index) { return Get().m_Entities[index]; }

    static int GetEntityCount() {return Get().m_Entities.size();}

    static void SortEntitiesByRenderMode() { Get().SortEntitiesByRenderModeInternal(); }

private:
    EntityManager();
    ~EntityManager();

    std::vector<Entity*> m_Entities;

    static EntityManager& Get()
    {
        static EntityManager instance;
        return instance;
    }


    void SortEntitiesByRenderModeInternal();

    void RemoveInternal(Entity* e);
};
