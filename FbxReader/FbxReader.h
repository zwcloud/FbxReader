#pragma once

extern "C"
{
	enum FR_Result
	{
		FR_Result_Failed = 0,
		FR_Result_Success = 1
	};

	struct FR_Context
	{
		void* Manager;
		void* Scene;
	};

	FR_Result Load(FR_Context** context, void* filePathString, int byteSize);
	FR_Result Unload(FR_Context* context);

	FR_Result GetMeshCount(FR_Context* context, int* meshCount);
	FR_Result GetMeshNames(FR_Context* context, void** names, int* count);
	FR_Result GetPositions(FR_Context* context, int meshIndex, float** positions, int* count);
	FR_Result GetUV(FR_Context* context, int meshIndex, int layer, float** texcoods, int* count);
}