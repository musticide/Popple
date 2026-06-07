#pragma once
#include <firebase/app.h> 
#include <firebase/database.h>

extern firebase::App* g_firebaseApp;
extern firebase::database::Database* g_firebaseDB;
void InitializeFirebaseCore();
void CleanupFirebase();
