#pragma once
#include "../Scene.h"
#include "NetworkReceiver.h">
#include <memory>
#include "../ParticleSystem.h"

class EditorScene : public Scene{
    public:
        EditorScene();
        ~EditorScene();
    #ifdef DEBUG
    NetworkReceiver* netReceiver = nullptr;
    #endif

    std::unique_ptr<ParticleSystem> particleSystem;
};
