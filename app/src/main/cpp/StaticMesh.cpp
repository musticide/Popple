#include "StaticMesh.h"
#include "raylib.h"

StaticMesh::StaticMesh(const char* filepath)
    : m_Filepath(filepath)
{
    m_Model = LoadModel(filepath);
}

StaticMesh::~StaticMesh() { UnloadModel(m_Model); }

void StaticMesh::Draw() const
{
    DrawModelEx(m_Model, position, Vector3Normalize(rotation), Vector3Length(rotation), scale, WHITE);
}
