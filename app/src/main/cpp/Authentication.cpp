#include "Authentication.h"
#include "Android.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "firebase.h"
#include "raymob.h"
#include <firebase/database/common.h>
#include <firebase/database/data_snapshot.h>
#include <firebase/database/database_reference.h>
#include <firebase/database/query.h>
#include <firebase/future.h>
#include <string>

#define firedb firebase::database

enum LoginState {
    IDLE,
    CHECKING,
    SUCCESS,
    FAILED
};

LoginState g_LoginState = IDLE;

firebase::Future<firedb::DataSnapshot> g_LookupFuture;

void StartUserLoginFlow(const std::string& currentDeviceId) {
    PlayerProfile.deviceId.value = currentDeviceId;
    if (g_firebaseDB == nullptr) {
        LOGE("AUTH: Could not Start Login flow: DB not initialized");
        return;
    }

    g_LoginState = CHECKING;
    // 1. Primary Strategy: Direct Lookup if a stored userId exists
    if (!g_UserId.empty()) {
        LOGI("AUTH: Stored User ID found (%s). Fetching profile directly...", g_UserId.c_str());

        // Direct path lookup: targets "/users/YOUR_USER_ID" instantly
        g_LookupFuture = g_firebaseDB->GetReference("users").Child(g_UserId).GetValue();
    }
    // 2. Fallback Strategy: Device ID Search if no userId is stored locally
    else {
        LOGI("AUTH: No stored User ID. Falling back to Device ID query...");

        firedb::Query deviceQuery = g_firebaseDB->GetReference("users").OrderByChild("device_id").EqualTo(currentDeviceId);

        g_LookupFuture = deviceQuery.GetValue();
    }
}

void UpdateUserLoginLoop() {
    if (g_LoginState != CHECKING) return;

    if (g_LookupFuture.error() != firedb::kErrorNone) {
        LOGE("AUTH: Database search failed. Error: %s", g_LookupFuture.error_message());
        g_LoginState = FAILED;
        return;
    }

    firedb::DataSnapshot snapshot = *g_LookupFuture.result();

    if (snapshot.exists() && snapshot.children_count() > 0) {
        // Existing User
        auto children                 = snapshot.children();
        firedb::DataSnapshot userNode = children[0];

        g_UserId = userNode.key();
        LOGI("AUTH: Existing User found. UserId: %s", g_UserId.c_str());

        PlayerProfile.username.value = userNode.Child(PlayerProfile.username.key).value().string_value();
        PlayerProfile.platform.value = userNode.Child(PlayerProfile.platform.key).value().string_value();
        PlayerProfile.appVersion.value = userNode.Child(PlayerProfile.appVersion.key).value().string_value();

        PlayerProfile.highestScore.value = (int)userNode.Child(PlayerProfile.highestScore.key).value().int64_value();
        PlayerProfile.longestTimeSurvived.value = (int)userNode.Child(PlayerProfile.longestTimeSurvived.key).value().int64_value();
        PlayerProfile.highestLevelCleared.value = (int)userNode.Child(PlayerProfile.highestLevelCleared.key).value().int64_value();

        g_LoginState = SUCCESS;
        WriteToAppStorage("user_id.dat", &g_UserId, g_UserId.size());
    } else {
        // New User
        LOGI("AUTH: User not found, creating new user");

        firedb::DatabaseReference newUserRef = g_firebaseDB->GetReference("users").PushChild();
        g_UserId                             = newUserRef.key();

        // PlayerProfile.username.value = ""; // TODO: Get User Name
        // PlayerProfile.lastUpdated = getcurrent


        newUserRef.SetValue(GetFormattedPlayerProfile());

        LOGI("AUTH: New User created with userId: %s", g_UserId.c_str());
        g_LoginState = SUCCESS;
        WriteToAppStorage("user_id.dat", &g_UserId, g_UserId.size());
    }
}
void RunGameBootAuthentication() {
    std::string user_hardware_id = GetAndroidDeviceID();
    LOGI("SYSTEM: Launching game check with Device ID: %s", user_hardware_id.c_str());
    StartUserLoginFlow(user_hardware_id);
}

void GetLocalUserId() {
    int size = 0;

    void* data = ReadFromAppStorage("user_id.dat", &size);
    if (data && size > 0) {
        g_UserId.assign(static_cast<const char*>(data), size);
        LOGI("Retrieved user Id from storage: %s", g_UserId.c_str());
    } else {
        LOGI("AUTH: No user id found in storage");
    }
    free(data);
}
