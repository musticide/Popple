#pragma once
#include "Scene.h"
#include "StaticMesh.h"
#include "TestEntity.h"

class TestScene : public Scene {
  public:
    TestScene();
    ~TestScene();

    void Load() override;

  private:
    std::unique_ptr<TestEntity> testEntity;
};
