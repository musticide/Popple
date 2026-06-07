#pragma once

#include "PlayerProfile.h"
#include "firebase.h"
#include <cstdint>
#include <firebase/database/database_reference.h>
#include <firebase/variant.h>
#include "Log.h"
#include <map>
#include <string>


firebase::Variant GetFormattedPlayerProfile() {
    std::map<firebase::Variant, firebase::Variant> rootUserNode;
    rootUserNode[PlayerProfile.username.key]            = PlayerProfile.username.value;
    rootUserNode[PlayerProfile.deviceId.key]            = PlayerProfile.deviceId.value;
    rootUserNode[PlayerProfile.highestScore.key]        = PlayerProfile.highestScore.value;
    rootUserNode[PlayerProfile.longestTimeSurvived.key] = PlayerProfile.longestTimeSurvived.value;
    rootUserNode[PlayerProfile.highestLevelCleared.key] = PlayerProfile.highestLevelCleared.value;
    rootUserNode[PlayerProfile.platform.key]            = PlayerProfile.platform.value;
    rootUserNode[PlayerProfile.appVersion.key]          = PlayerProfile.appVersion.value;
    rootUserNode[PlayerProfile.lastUpdated.key]         = PlayerProfile.lastUpdated.value;

    return firebase::Variant(rootUserNode);
}

void SavePlayerProfile() {
    if (g_firebaseDB == nullptr) 
    {
        LOGE("Save Failed. Could not find fireDB");
        return;
    }
    if(g_UserId.empty()) {
        LOGE("Save Failed. User id is empty");
        return;
    }

    firebase::database::DatabaseReference recordRoute = g_firebaseDB->GetReference("users").Child(g_UserId);

    recordRoute.SetValue(GetFormattedPlayerProfile());
    LOGI("Save Complete.");
}
