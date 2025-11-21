#pragma once

#include "Entity.h"
#include "raylib.h"
#include "raymath.h"
#include <memory>

class StaticMesh : public Entity {
private:
    const char* m_Filepath;
    std::shared_ptr<Model> m_Model;
    Material m_Material;

public:
    StaticMesh(const char* filepath);
    StaticMesh(StaticMesh&&) = default;
    StaticMesh(const StaticMesh&) = default;
    StaticMesh& operator=(StaticMesh&&) = default;
    StaticMesh& operator=(const StaticMesh&) = default;
    ~StaticMesh();

    Vector3 position = { 0, 0, 0 };
    Vector3 rotation = { 0, 0, 0 };
    Vector3 scale = { 1, 1, 1 };

    Model& GetModel() { return *m_Model; }

    void Draw() const override;
};
