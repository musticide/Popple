#include "EntityManager.h"

EntityManager::EntityManager() { }

EntityManager::~EntityManager() { }

void EntityManager::SortEntitiesByRenderModeInternal()
{
    std::sort(m_Entities.begin(), m_Entities.end(),
        [](Entity* a, Entity* b) { return a->GetRenderMode() < b->GetRenderMode(); });
}

void EntityManager::RemoveInternal(Entity* e)
{
    auto it = std::find(m_Entities.begin(), m_Entities.end(), e);
    if (it != m_Entities.end()) {
        m_Entities.erase(it);
    }
}

