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

FR_Result Free(void* data)
{
	if (!data) return FR_Result_Failed;
	free(data);
	return FR_Result_Success;
}

FR_Result GetMeshCount(FR_Context* context, int* meshCount)
{
	*meshCount = 0;

	FbxScene*& scene = (FbxScene*&)(context->Scene);
	int nodeCount = scene->GetNodeCount();
	for (int i = 0; i < nodeCount; i++)
	{
		FbxNode* node = scene->GetNode(i);
		if (!node) continue;
		FbxNodeAttribute* attribute = node->GetNodeAttribute();
		if (!attribute) continue;
		if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			(*meshCount)++;
		}
	}
	return FR_Result_Success;
}

FR_Result GetMeshNames(FR_Context* context, void*** names, int** sizes, int* count)
{
	FbxScene*& scene = (FbxScene*&)(context->Scene);
	int nodeCount = scene->GetNodeCount();
	FbxArray<FbxNode*> meshNodes;
	for (int i = 0; i < nodeCount; i++)
	{
		FbxNode* node = scene->GetNode(i);
		if (!node) continue;
		FbxNodeAttribute* attribute = node->GetNodeAttribute();
		if (!attribute) continue;
		if (attribute->GetAttributeType() == FbxNodeAttribute::EType::eMesh)
		{
			meshNodes.Add(node);
		}
	}

	int meshCount = meshNodes.Size();
	if (meshCount < 0) return FR_Result_Failed;

	*count = meshCount;
	*names = (void**)malloc(meshCount*sizeof(void*));//MALLOC
	*sizes = (int*)malloc(meshCount*sizeof(int));//MALLOC

	for (int i = 0; i < meshCount; ++i)
	{
		FbxNode* node = meshNodes[i];
		(*names)[i] = (char*)node->GetName();
		(*sizes)[i] = strlen(node->GetName());//http://stackoverflow.com/a/15000609
	}

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