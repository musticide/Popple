#include "StaticMesh.h"
#include "DrawableEntity.h"
#include "ResourceManager.h"
#include "raylib.h"

StaticMesh::StaticMesh(const char* filepath, RenderQueue type)
    : DrawableEntity(type),
    m_Filepath(filepath)
{
    m_Model = LoadModelFromMesh(ResourceManager::GetModel(filepath)->meshes[0]);
}

StaticMesh::~StaticMesh() {  }

void StaticMesh::Draw() const
{
    DrawModelEx(m_Model, position, Vector3Normalize(rotation), Vector3Length(rotation), scale, WHITE);
}
