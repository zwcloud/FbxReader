#pragma once
#include <fbxsdk.h>

#pragma region debug functions
void _DebugAssert(bool expression, const char *Text, ...);
void _DebugAssert(bool expression, const wchar_t *Text, ...);
void DebugPrintf(const char *Text, ...);
void DebugPrintf(const wchar_t *Text, ...);
const void GetTestFileName(char* buffer);
const void GetTestFileNameW(wchar_t* buffer);

#define DebugAssert(condition,text,...) if (!(condition)){_DebugAssert((condition), (text), __VA_ARGS__); DebugBreak();}
#pragma endregion

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene);
void DestroySdkObjects(FbxManager* pManager, bool pExitStatus);
bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename);
