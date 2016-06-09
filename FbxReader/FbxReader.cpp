#include "FbxReader.h"
#include <fbxsdk.h>
#include "FbxCommon.h"

FR_Result Load(void* filePathString, int byteSize, void* context)
{
	FbxManager* lSdkManager = NULL;
	FbxScene* fbxScene;
	InitializeSdkObjects(lSdkManager, fbxScene);
	bool bResult = LoadScene(lSdkManager, fbxScene, (const char*)filePathString);

	return FR_Result_Success;
}

FR_Result Unload(void* context)
{
	return FR_Result_Success;
}


FR_Result GetMeshCount()
{
	return FR_Result_Success;
}

FR_Result GetMeshNames(void** names, int* count)
{
	return FR_Result_Success;
}

FR_Result GetPositions(int meshIndex, float** positions, int* count)
{
	return FR_Result_Success;
}

FR_Result GetUV(int meshIndex, int layer, float** texcoods, int* count)
{
	return FR_Result_Success;
}
