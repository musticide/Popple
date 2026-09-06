#include "Globals.h"
#include "Log.h"
#include "Signal.h"
#include "raylib.h"
#include <raymob.h>

int Globals::screenWidth  = 0;
int Globals::screenHeight = 0;

const int Globals::baseScreenWidth = 1080, Globals::baseScreenHeight = 2340;
Vector2 Globals::uiScale = { 0.f, 0.f };

bool Globals::GameResults::gameCompleted;
bool Globals::GameResults::gameWon;
int Globals::GameResults::score;
int Globals::GameResults::health;
int Globals::GameResults::levelPlayed;
int Globals::GameResults::levelRating;

int Globals::state = 0;
Signal<int> Globals::gameStateChanged;

Callback Globals::onPauseCallBack = []() {
    if (!IsStateValid(APP_PAUSED)) {
        EnableState(APP_PAUSED);
    }
    LOGW("PAUSED");
};

Callback Globals::onResumeCallBack = []() {
    if (IsStateValid(APP_PAUSED)) DisableState(APP_PAUSED);
    LOGW("RESUMED");
};

void Globals::EnableState(int state) {
    if (!IsStateValid(state)) {
        Globals::state |= state;
        LOGW("Game State Enabled: %s, Current States: %s",
            GetStateNames(state).c_str(),
            GetStateNames(Globals::state).c_str());
        gameStateChanged.emit(Globals::state);
    }
}

void Globals::DisableState(int state) {
    if (IsStateValid(state)) {
        Globals::state &= ~state;
        LOGW("Game State Disabled: %s, Current States: %s",
            GetStateNames(state).c_str(),
            GetStateNames(Globals::state).c_str());
        gameStateChanged.emit(Globals::state);
    }
}

bool Globals::IsStateValid(int state) {
    return (Globals::state & state) != 0;
}

std::string Globals::GetStateNames(int state) {
    std::string result = "";

    if ((state & MAIN_MENU) != 0) {
        result += "MAIN_MENU | ";
    }
    if ((state & APP_PAUSED) != 0) {
        result += "APP_PAUSED | ";
    }
    if ((state & GAMEPLAY_PAUSED) != 0) {
        result += "GAMEPLAY_PAUSED | ";
    }
    if ((state & GAMEPLAY_RUNNING) != 0) {
        result += "GAMEPLAY_RUNNING | ";
    }
    if ((state & GAMEPLAY_ENDED) != 0) {
        result += "GAMEPLAY_ENDED | ";
    }

    // If result isn't empty, remove the trailing " | "
    if (!result.empty()) {
        result.erase(result.length() - 3);
    } else {
        result = "NONE"; // Fallback if no valid bits were set
    }

    return result;
}
