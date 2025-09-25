#pragma once
#include "Entity.h"
#include <algorithm>
#include <vector>

class EntityManager {
public:
    EntityManager(EntityManager&&) = delete;
    EntityManager(const EntityManager&) = delete;
    EntityManager& operator=(EntityManager&&) = delete;
    EntityManager& operator=(const EntityManager&) = delete;

    static void AddEntity(Entity* e) { Get().m_Entities.push_back(e); }

    static void Remove(Entity* e) { Get().RemoveInternal(e);}

    static Entity* GetEntityAt(int index) { return Get().m_Entities[index]; }

    static int GetEntityCount() {return Get().m_Entities.size();}

private:
    EntityManager();
    ~EntityManager();

    std::vector<Entity*> m_Entities;

    static EntityManager& Get()
    {
        static EntityManager instance;
        return instance;
    }

    void RemoveInternal(Entity* e)
    {
        auto it = std::find(m_Entities.begin(), m_Entities.end(), e);
        if (it != m_Entities.end()) {
            m_Entities.erase(it);
        }
    }
};
