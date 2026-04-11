#pragma once
#include "Entity.h"
#include "Singleton.h"
#include <functional>
#include <vector>

struct DelayedCall {
    float remainingTime;
    std::function<void()> callback;

    DelayedCall(std::function<void()> func, float time) : callback(func), remainingTime(time) {
    }
};

class Scheduler : public Singleton<Scheduler>{
  public:
    Scheduler();
    ~Scheduler();

    std::vector<DelayedCall> delayedCalls;

    void ScheduleCall(float time, std::function<void()> func);
    void Update(float dT = 1.0f);

  private:
};

///Example usage
// Scheduler::Get().ScheduleCall(2.f, [this]() {
//     if (this->m_AnemoShieldMesh) {
//         LOGI("EM: Deactivate Anemo Shield");
//         this->m_AnemoShieldMesh->scale = { 10.f, 10.f, 10.f };
//         this->m_AnemoShieldMesh->SetActive(false);
//     }
// });
