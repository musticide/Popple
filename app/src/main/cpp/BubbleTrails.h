#pragma once
#include "DrawableEntity.h"
#include "raylib.h"

class BubbleTrails : public DrawableEntity {
  public:
    BubbleTrails(Scene* parentScene);
    ~BubbleTrails();

    void Draw() override;

    std::shared_ptr<Model> trailMesh = nullptr;

  private:
    int tfrcId = -1;
    Vector4 tfrc = {0, 12.f, 8.f, 2.f};
};
