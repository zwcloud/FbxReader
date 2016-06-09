#pragma once

extern "C"
{
	enum FR_Result
	{
		FR_Result_Success,
		FR_Result_Failed
	};

	FR_Result GetMeshCount();
	FR_Result GetMeshNames(void** names, int* count);
	FR_Result GetPositions(int meshIndex, float** positions, int* count);
	FR_Result GetUV(int meshIndex, int layer, float** texcoods, int* count);
}