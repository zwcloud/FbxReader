#include "FbxReader.h"
#include <fbxsdk.h>
#include "FbxCommon.h"

FR_Result Load(FR_Context** context, void* filePathString, int byteSize)
{
	*context = (FR_Context*)malloc(sizeof(FR_Context));
	FbxManager*& manager = (FbxManager*&)(*context)->Manager;
	FbxScene*& scene = (FbxScene*&)((*context)->Scene);
	InitializeSdkObjects(manager, scene);
	bool bResult = LoadScene(manager, scene, (const char*)filePathString);
	if (!bResult)
	{
		return FR_Result_Failed;
	}
	return FR_Result_Success;
}

FR_Result Unload(FR_Context* context)
{
	FbxManager* manager = (FbxManager*)(context->Manager);
	DestroySdkObjects(manager);
	return FR_Result_Success;
}

FR_Result GetMeshCount(FR_Context* context)
{

	return FR_Result_Success;
}

FR_Result GetMeshNames(FR_Context* context, void** names, int* count)
{
	return FR_Result_Success;
}

FR_Result GetPositions(FR_Context* context, int meshIndex, float** positions, int* count)
{
	return FR_Result_Success;
}

FR_Result GetUV(FR_Context* context, int meshIndex, int layer, float** texcoods, int* count)
{
	return FR_Result_Success;
}
