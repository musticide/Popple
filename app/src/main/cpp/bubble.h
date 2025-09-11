#pragma once

#include <raymob.h>

class Bubble {
public:
    Bubble();
    ~Bubble();

    Vector2 position;
    float rotation;
    float radius;
    Color color;
    Vector2 moveSpeed;

    void Update();

    void Move();

private:
};

