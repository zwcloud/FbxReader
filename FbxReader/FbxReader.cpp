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

FR_Result GetControlPoints(FR_Context* context, void* meshNameString, double** controlPoints, int* count)
{
	FbxScene*& scene = (FbxScene*&)(context->Scene);
	FbxString meshName((const char*)meshNameString);
	FbxNode* node = scene->FindNodeByName(meshName);
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (!attribute)
	{
		return FR_Result_Failed;
	}
	if (attribute->GetAttributeType() != FbxNodeAttribute::EType::eMesh)
	{
		return FR_Result_Failed;
	}
	FbxMesh* mesh = (FbxMesh*)attribute;
	FbxVector4* lControlPoints = mesh->GetControlPoints();
	int controlPointCount = mesh->GetControlPointsCount();
	*controlPoints = (double*)lControlPoints;
	*count = controlPointCount;

	return FR_Result_Success;
}

FR_Result GetIndexes(FR_Context* context, void* meshNameString, int** indexes, int* count)
{
	FbxScene*& scene = (FbxScene*&)(context->Scene);
	FbxString meshName((const char*)meshNameString);
	FbxNode* node = scene->FindNodeByName(meshName);
	FbxNodeAttribute* attribute = node->GetNodeAttribute();
	if (!attribute)
	{
		return FR_Result_Failed;
	}
	if (attribute->GetAttributeType() != FbxNodeAttribute::EType::eMesh)
	{
		return FR_Result_Failed;
	}
	FbxMesh* mesh = (FbxMesh*)attribute;

	//TODO Check if any non-triangle existed?
	int polygonCount = mesh->GetPolygonCount();
	for (int i = 0; i < polygonCount; i++)
	{
		if (3 != mesh->GetPolygonSize(i))
		{
			return FR_Result_Failed;//polygon i is not a triangle
		}
	}
	*indexes = mesh->GetPolygonVertices();
	*count = mesh->GetPolygonVertexCount();
	return FR_Result_Success;
}