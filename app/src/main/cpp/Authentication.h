#pragma once
#include "Android.h"
#include "Log.h"
#include "PlayerProfile.h"
#include "RemoteConfig.h"
#include "firebase.h"
#include "raymob.h"
#include <firebase/database/common.h>
#include <firebase/database/data_snapshot.h>
#include <firebase/database/database_reference.h>
#include <firebase/database/query.h>
#include <firebase/future.h>
#include <string>

namespace Auth {
#define firedb firebase::database

enum LoginState {
    LOGIN_IDLE,
    CHECKING_USER_ID,   // State 1: Trying direct ID lookup
    CHECKING_DEVICE_ID, // State 2: Trying fallback device ID query
    LOGIN_SUCCESS,
    LOGIN_FAILED
};

extern LoginState g_LoginState;

void GetLocalUserId();
void RunGameBootAuthentication();
void UpdateUserLoginLoop();

} // namespace Auth
