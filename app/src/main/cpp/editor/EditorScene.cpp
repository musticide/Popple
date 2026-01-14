#include "EditorScene.h"
#include "NetworkReceiver.h"
EditorScene::EditorScene()
    : Scene("EditorScene")
{
    particleSystem = CreateEntity<ParticleSystem>(true, 50);
    particleSystem->particleProperties.lifetime = 0.6f;
    particleSystem->particleProperties.size = 0.08f;
    particleSystem->particleProperties.sizeVariation = 0.05f;
    particleSystem->emitType = EmitType::CONTINUOUS;
    particleSystem->shape = EmitShape::CIRCLE;
    particleSystem->particleProperties.initialSpeed = 0.3f;
    particleSystem->particleProperties.startColor = WHITE;
    particleSystem->particleProperties.endColor = BLACK;

#ifdef DEBUG
    netReceiver = new NetworkReceiver(8888);
    netReceiver->SetOnDataReceived([this](const json& data) {
        if (particleSystem)
            particleSystem->ApplyEditorParams(data);
    });
    netReceiver->Start();

    if (particleSystem)
        netReceiver->SendCurrentState(particleSystem->GetCurrentParams());
#endif
}

EditorScene::~EditorScene()
{
#ifdef DEBUG
    if (netReceiver) {
        netReceiver->Stop();
        delete netReceiver;
    }
#endif
}
