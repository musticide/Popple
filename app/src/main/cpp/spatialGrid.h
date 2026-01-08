#pragma once
#include "Entity.h"
#include "Singleton.h"
#include "raylib.h"
#include <vector>

struct Bubble;

class SpatialGrid : public Entity, public Singleton<SpatialGrid> {
private:
    std::vector<std::vector<Bubble*>> m_Grid;
    const int GRID_SIZE = 100, GRID_COLS = (GetScreenWidth() + GRID_SIZE - 1) / GRID_SIZE,
              GRID_ROWS = (GetScreenHeight() + GRID_SIZE - 1) / GRID_SIZE;

    void ClearInternal();

    int GetGridIndexInternal(Vector3 position) const;

    void AddEntityInternal(Bubble* entity);

    std::vector<Bubble*> GetNearbyEntitiesInternal(Vector3 position) const;

public:
    SpatialGrid();

    static void Clear() { Get().ClearInternal(); }

    static int GetGridIndex(Vector3 position) { return Get().GetGridIndexInternal(position); }

    static void AddEntity(Bubble* entity) { Get().AddEntityInternal(entity); }

    static std::vector<Bubble*> GetNearbyEntities(Vector3 position)
    {
        return Get().GetNearbyEntitiesInternal(position);
    }
};
