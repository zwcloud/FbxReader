#pragma once

extern "C"
{
	enum FR_Result
	{
		FR_Result_Success,
		FR_Result_Failed
	};

	FR_Result Load(void* filePathString, int byteSize, void* context);
	FR_Result Unload(void* context);

	FR_Result GetMeshCount(void* context);
	FR_Result GetMeshNames(void* context, void** names, int* count);
	FR_Result GetPositions(void* context, int meshIndex, float** positions, int* count);
	FR_Result GetUV(void* context, int meshIndex, int layer, float** texcoods, int* count);
}