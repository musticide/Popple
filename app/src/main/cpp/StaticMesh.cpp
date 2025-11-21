#include "StaticMesh.h"
#include "ResourceManager.h"
#include "raylib.h"

StaticMesh::StaticMesh(const char* filepath)
    : m_Filepath(filepath)
{
    m_Model = ResourceManager::GetModel(filepath);
}

StaticMesh::~StaticMesh() {  }

void StaticMesh::Draw() const
{
    DrawModelEx(*m_Model, position, Vector3Normalize(rotation), Vector3Length(rotation), scale, WHITE);
}
