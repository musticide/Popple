#include "scheduler.h"
#include "Log.h"

Scheduler::Scheduler() {
}

Scheduler::~Scheduler() {
}

void Scheduler::Update(float dT) {
    // for (DelayedCall dC : delayedCalls) {
    //     if (dC.remainingTime -= dT <= 0.0) {
    //         dC.callback();
    //     }
    // }
    for (auto it = delayedCalls.begin(); it != delayedCalls.end();) {
        it->remainingTime -= dT;

        if (it->remainingTime <= 0.0f) {
            it->callback();
            it = delayedCalls.erase(it);
        } else {
            ++it;
        }
    }
}

void Scheduler::ScheduleCall(float time, std::function<void()> func) {
    delayedCalls.push_back({func, time});
}
