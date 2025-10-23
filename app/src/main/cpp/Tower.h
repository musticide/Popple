#pragma once

#include "Entity.h"
#include "raylib.h"
#include "raymath.h"
#include <raymob.h>

class Tower : public Entity{
private:
    Model m_Model;
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
