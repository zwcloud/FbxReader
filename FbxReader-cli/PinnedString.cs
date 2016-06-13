using System;
using System.Runtime.InteropServices;
using System.Text;

namespace FbxReader_cli
{
    public unsafe class PinnedString : IDisposable
    {
        private GCHandle handle;
        private void* pointer;
        private readonly int byteSize;

        public void* Pointer
        {
            get { return pointer; }
        }

        public int ByteSize
        {
            get { return byteSize; }
        }

        public PinnedString(string str)
        {
            byteSize = Encoding.ASCII.GetByteCount(str);
            byte[] data = new byte[byteSize];
            Encoding.ASCII.GetBytes(str, 0, str.Length, data, 0);
            handle = GCHandle.Alloc(data, GCHandleType.Pinned);
            pointer = handle.AddrOfPinnedObject().ToPointer();
        }

        public void Dispose()
        {
            if(handle.IsAllocated)
            {
                handle.Free();
            }
            pointer = (void*)0;
        }
    }
}