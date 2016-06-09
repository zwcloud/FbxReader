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
        private static extern bool Load(void* filePathString, int byteSize, void** context);

        [DllImport(FbxReaderLibName, CallingConvention = CallingConvention.Cdecl)]
        private static extern bool Unload(void* context);

        #endregion

        #region Wrapper
        public static bool Load(string filePathString, out IntPtr context)
        {
            int filePathByteCount = Encoding.ASCII.GetByteCount(filePathString);
            byte[] filePathBytes = new byte[filePathByteCount];
            Encoding.ASCII.GetBytes(filePathString, 0, filePathString.Length, filePathBytes, 0);
            var handle = GCHandle.Alloc(filePathBytes, GCHandleType.Pinned);
            var ptr = handle.AddrOfPinnedObject();
            void* outContext;
            Load(ptr.ToPointer(), filePathByteCount, &outContext);
            context = new IntPtr(outContext);
            return true;
        }

        public static bool Unload(IntPtr context)
        {
            return Unload(context.ToPointer());
        }

        #endregion
    }
}