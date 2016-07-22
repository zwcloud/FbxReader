using System;
using System.Runtime.InteropServices;
using System.Text;

namespace FbxReader_cli
{
    public unsafe static class FbxRead
    {
        #region Native

        private const string FbxReaderLibName = "FbxReader.dll";

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Load(void** context, void* filePathString, int byteSize);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Unload(void* context);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Free(IntPtr data);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool GetMeshCount(void* context, int* meshCount);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool GetMeshNames(void* context, void*** nameStrings, int** nameStringByteSizes, int* count);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool GetControlPoints(void* context, void* meshNameString, double** controlPoints, int* count);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool GetIndexes(void* context, void* meshNameString, int** indexes, int* count);

        #endregion

        #region Wrapper

        public static bool Load(string filePathString, out IntPtr context)
        {
            void* outContext;
            using (PinnedString path = new PinnedString(filePathString))
            {
                if(!Load(&outContext, path.Pointer, path.ByteSize))
                {
                    context = IntPtr.Zero;
                    return false;
                }
            }
            context = new IntPtr(outContext);
            return true;
        }

        public static bool Unload(IntPtr context)
        {
            return Unload(context.ToPointer());
        }

        public static bool GetMeshCount(IntPtr context, out int meshCount)
        {
            meshCount = 0;
            int count = 0;
            bool result = GetMeshCount(context.ToPointer(), &count);
            if(result)
            {
                meshCount = count;
            }
            return result;
        }

        public static bool GetMeshNames(IntPtr context, out string[] names)
        {
            void** nameStrings;
            int* nameStringByteSizes;
            int count;
            bool result = GetMeshNames(context.ToPointer(), &nameStrings, &nameStringByteSizes, &count);
            if(!result)
            {
                names = null;
                return false;
            }

            if(count == 0)
            {
                names = null;
                return true;
            }

            names = new string[count];

            IntPtr namesPtr = new IntPtr(nameStrings);
            IntPtr namesSizePtr = new IntPtr(nameStringByteSizes);
            Decoder decoder = Encoding.ASCII.GetDecoder();
            for (int i = 0; i < count; i++)
            {
                IntPtr ptr = Marshal.ReadIntPtr(namesPtr, i);
                IntPtr byteSizePtr = namesSizePtr;
                int byteSize = Marshal.ReadInt32(byteSizePtr, i);
            
                byte[] nameData = new byte[byteSize];
                Marshal.Copy(ptr, nameData, 0, byteSize);
                int maxCharCount = Encoding.ASCII.GetMaxCharCount(byteSize);
                char[] nameChars = new char[maxCharCount];
                int dummy;
                bool dummyBool;
                decoder.Convert(nameData, 0, byteSize, nameChars, 0, maxCharCount, true, out dummy, out dummy, out dummyBool);
                names[i] = new string(nameChars);
            }

            Free(namesPtr);
            Free(namesSizePtr);

            return true;
        }

        public static bool GetControlPoints(IntPtr context, string meshNameString, out double[] controlPoints)
        {
            using (PinnedString meshName = new PinnedString(meshNameString))
            {
                double* ptr_native_controlPointsData;
                int count;
                bool result = GetControlPoints(context.ToPointer(), meshName.Pointer, &ptr_native_controlPointsData, &count);
                if(result)
                {
                    controlPoints = new double[count * 3];
                    fixed (double* ptr_managed_controlPointsData = controlPoints)
                    {
                        ulong byteSize = (ulong) (Marshal.SizeOf(typeof (double))*count*3);
                        Buffer.MemoryCopy(ptr_native_controlPointsData, ptr_managed_controlPointsData, byteSize, byteSize);
                    }
                    return true;
                }
            }
            controlPoints = null;
            return false;
        }

        public static bool GetIndexes(IntPtr context, string meshNameString, out int[] indexes)
        {
            using (PinnedString meshName = new PinnedString(meshNameString))
            {
                int* ptr_native_indexesData;
                int count;
                bool result = GetIndexes(context.ToPointer(), meshName.Pointer, &ptr_native_indexesData, &count);
                if (result)
                {
                    indexes = new int[count];
                    fixed (int* ptr_managed_Data = indexes)
                    {
                        ulong byteSize = (ulong)(Marshal.SizeOf(typeof(int)) * count);
                        Buffer.MemoryCopy(ptr_native_indexesData, ptr_managed_Data, byteSize, byteSize);
                    }
                    return true;
                }
            }
            indexes = null;
            return false;
        }

        #endregion
    }
}