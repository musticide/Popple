#pragma once
#include "popple.h"
#include "raylib.h"
#include <vector>
#include "bubble.h"

class SpatialGrid {
private:
    std::vector<std::vector<Bubble*>> m_Grid;
    const int GRID_SIZE = 100, GRID_COLS = (GetScreenWidth() + GRID_SIZE - 1) / GRID_SIZE,
              GRID_ROWS = (GetScreenHeight() + GRID_SIZE - 1) / GRID_SIZE;

    SpatialGrid();
    ~SpatialGrid();

    SpatialGrid(SpatialGrid const&) = delete;
    void operator=(SpatialGrid const&) = delete;

    void ClearInternal();

    int GetGridIndexInternal(Vector3 position) const;

    void AddEntityInternal(Bubble* entity);

    std::vector<Bubble*> GetNearbyEntitiesInternal(Vector3 position) const;

public:
    static SpatialGrid& GetInstance()
    {
        static SpatialGrid instance;
        return instance;
    }

    static void Clear() { GetInstance().ClearInternal(); }

    static int GetGridIndex(Vector3 position) { return GetInstance().GetGridIndexInternal(position); }

    static void AddEntity(Bubble* entity) { GetInstance().AddEntityInternal(entity); }

    static std::vector<Bubble*> GetNearbyEntities(Vector3 position) {
        return GetInstance().GetNearbyEntitiesInternal(position);
    }
};
