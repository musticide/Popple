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
    CHECKING_USER_ID,   // State 1: Trying direct ID lookup
    CHECKING_DEVICE_ID, // State 2: Trying fallback device ID query
    SUCCESS,
    FAILED
};

LoginState g_LoginState = IDLE;
firebase::Future<firedb::DataSnapshot> g_LookupFuture;

void LoadProfileFromSnapshot(const firedb::DataSnapshot& userNode) {
    g_UserId = userNode.key();

    PlayerProfile.username.value   = userNode.Child(PlayerProfile.username.key).value().string_value();
    PlayerProfile.platform.value   = userNode.Child(PlayerProfile.platform.key).value().string_value();
    PlayerProfile.appVersion.value = userNode.Child(PlayerProfile.appVersion.key).value().string_value();

    PlayerProfile.highestScore.value = (int)userNode.Child(PlayerProfile.highestScore.key).value().int64_value();
    PlayerProfile.longestTimeSurvived.value =
        (int)userNode.Child(PlayerProfile.longestTimeSurvived.key).value().int64_value();
    PlayerProfile.highestLevelCleared.value =
        (int)userNode.Child(PlayerProfile.highestLevelCleared.key).value().int64_value();

    LoadLevelsFromFirebase();
    WriteToAppStorage("user_id.dat", &g_UserId, g_UserId.size());
}

void StartUserLoginFlow(const std::string& currentDeviceId) {
    PlayerProfile.deviceId.value = currentDeviceId;

    if (g_firebaseDB == nullptr) {
        LOGE("AUTH: Could not Start Login flow: DB not initialized");
        return;
    }

    if (!g_UserId.empty()) {
        LOGI("AUTH: Stored User ID found %s. Checking database directly...", g_UserId.c_str());
        g_LoginState   = CHECKING_USER_ID;
        g_LookupFuture = g_firebaseDB->GetReference("users").Child(g_UserId).GetValue();
    } else {
        LOGI("AUTH: No stored User ID. Searching by Device ID...");
        g_LoginState = CHECKING_DEVICE_ID;
        g_LookupFuture =
            g_firebaseDB->GetReference("users").OrderByChild("device_id").EqualTo(PlayerProfile.deviceId.value).GetValue();
    }
}

void UpdateUserLoginLoop() {
    // Only process if we are actively checking a network request
    if (g_LoginState != CHECKING_USER_ID && g_LoginState != CHECKING_DEVICE_ID) return;

    // Wait for the future to finish
    if (g_LookupFuture.status() != firebase::kFutureStatusComplete) return;

    if (g_LookupFuture.error() != firedb::kErrorNone) {
        LOGE("AUTH: Database search failed. Error: %s", g_LookupFuture.error_message());
        g_LoginState = FAILED;
        return;
    }

    firedb::DataSnapshot snapshot = *g_LookupFuture.result();

    // STEP 1: Process Direct ID Lookup
    if (g_LoginState == CHECKING_USER_ID) {
        if (snapshot.exists()) {
            LOGI("AUTH: Existing User ID validated. Loading profile...");
            // Because we queried a specific child, the snapshot IS the user node
            LoadProfileFromSnapshot(snapshot);
            g_LoginState = SUCCESS;
        } else {
            // User ID was stored locally, but deleted from DB. Fallback to Device ID.
            LOGI("AUTH: Stored User ID not found in DB. Falling back to Device ID query...");
            g_LoginState = CHECKING_DEVICE_ID;
            g_LookupFuture =
                g_firebaseDB->GetReference("users").OrderByChild("device_id").EqualTo(PlayerProfile.deviceId.value).GetValue();
        }
    }

    // STEP 2 & 3: Process Device ID Query or Create New User
    else if (g_LoginState == CHECKING_DEVICE_ID) {
        if (snapshot.exists() && snapshot.children_count() > 0) {
            LOGI("AUTH: User found via Device ID. Loading profile...");
            // Because this was a list query, we must get the first child
            auto children                 = snapshot.children();
            firedb::DataSnapshot userNode = children[0];

            LoadProfileFromSnapshot(userNode);
            g_LoginState = SUCCESS;
        } else {
            LOGI("AUTH: Device ID not found. Creating new user...");

            firedb::DatabaseReference newUserRef = g_firebaseDB->GetReference("users").PushChild();
            g_UserId                             = newUserRef.key();

            newUserRef.SetValue(GetFormattedPlayerProfile());

            LOGI("AUTH: New User created with userId: %s", g_UserId.c_str());
            WriteToAppStorage("user_id.dat", &g_UserId, g_UserId.size());
            g_LoginState = SUCCESS;
        }
    }
}

void RunGameBootAuthentication() {
    std::string user_hardware_id = GetAndroidDeviceID();
    LOGI("SYSTEM: Launching game check with Device ID: %s", user_hardware_id.c_str());
    StartUserLoginFlow(user_hardware_id);
}

void GetLocalUserId() {
    int size   = 0;
    void* data = ReadFromAppStorage("user_id.dat", &size);
    if (data && size > 0) {
        g_UserId.assign(static_cast<const char*>(data), size);
        LOGI("Retrieved user Id from storage: %s", g_UserId.c_str());
    } else {
        LOGI("AUTH: No user id found in storage");
    }
    free(data);
}
