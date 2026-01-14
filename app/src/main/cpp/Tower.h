#pragma once

#include "Entity.h"
#include "DrawableEntity.h"
#include "raylib.h"
#include "raymath.h"
#include <raymob.h>
#include <memory>

class Tower : public DrawableEntity {
private:
    std::shared_ptr<Model> m_Model;
    float rotate;

public:
    Vector3 position;
    Vector3 rotation;
    Vector3 scale;
    Color color;

    Tower();
    Tower(Tower &&) = default;
    Tower(const Tower &) = default;
    Tower &operator=(Tower &&) = default;
    Tower &operator=(const Tower &) = default;
    ~Tower();

    void Start() override;
    void Update(float dT = 1.0f) override;
    void Draw() const override;

private:

};
