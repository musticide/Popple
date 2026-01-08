#include "spatialGrid.h"
#include "bubbleManager.h"

SpatialGrid::SpatialGrid() { m_Grid.resize(GRID_COLS * GRID_ROWS); }


void SpatialGrid::AddEntityInternal(Bubble* entity)
{
    int gridIndex = GetGridIndex(entity->position);
    m_Grid[gridIndex].push_back(entity);
}

int SpatialGrid::GetGridIndexInternal(Vector3 position) const
{
    int col = (int)(position.x / GRID_SIZE);
    int row = (int)(position.z / GRID_SIZE);

    col = std::max(0, std::min(col, GRID_COLS - 1));
    row = std::max(0, std::min(row, GRID_ROWS - 1));

    return row * GRID_COLS + col;
}

void SpatialGrid::ClearInternal()
{
    for (auto& cell : m_Grid) {
        cell.clear();
    }
}

std::vector<Bubble*> SpatialGrid::GetNearbyEntitiesInternal(Vector3 position) const
{
    std::vector<Bubble*> result;

    int centerCol = (int)(position.x / GRID_SIZE);
    int centerRow = (int)(position.z / GRID_SIZE);

    for (int row = centerRow - 1; row <= centerRow + 1; row++) {
        for (int col = centerCol - 1; col <= centerCol + 1; col++) {
            if (col >= 0 && col < GRID_COLS && row >= 0 && row < GRID_ROWS) {
                int index = row * GRID_COLS + col;
                for (auto& entityIndex : m_Grid[index]) {
                    result.push_back(entityIndex);
                }
            }
        }
    }
    return result;
}
