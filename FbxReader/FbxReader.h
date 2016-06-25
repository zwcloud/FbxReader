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

	/*
		Load fbx file into a context.
		@context [out] the context
		@filePathString [in] file path string(multibyte string)
		@byteSize the size of the path string
	*/
	FR_Result Load(FR_Context** context, void* filePathString, int byteSize);

	/*
		Unload a context.
		@context [in] the context
	*/
	FR_Result Unload(FR_Context* context);

	/*
		Free data.
		@data [in] the data to free

		Used to free memory that allocated by FbxReader
	*/
	FR_Result Free(void* data);

	/*
		Get mesh node count in the context.
		@context [in] the context
		@meshCount [out] mesh node count
	*/
	FR_Result GetMeshCount(FR_Context* context, int* meshCount);

	/*
		Get mesh node names in the context.
		@context [in] the context
		@names [out] array of mesh node name
		@names [out] array of mesh node names' byte size
		@count [out] count of mesh node names

		The @nameStrings and @nameStringByteSizes should be freed by calling the function @Free
	*/
	FR_Result GetMeshNames(FR_Context* context, void*** nameStrings, int** nameStringByteSizes, int* count);

	/*
		Get mesh control points
		@context [in] the context
		@meshNameString [in] mesh name string(multibyte string)
		@controlPoints [out] array of control points
		@count [out] count of control points
	*/
	FR_Result GetControlPoints(FR_Context* context, void* meshNameString, double** controlPoints, int* count);

	/*
		Get mesh control point indexes
		@context [in] the context
		@meshNameString [in] mesh name string(multibyte string)
		@indexes [out] array of control point indexes
		@count [out] count of control point indexes
	*/
	FR_Result GetIndexes(FR_Context* context, void* meshNameString, int** indexes, int* count);
}