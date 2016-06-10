using System;
using System.Diagnostics;

namespace FbxReader_cli
{
    class Program
    {
        static void Main(string[] args)
        {
            IntPtr context;
            FbxRead.Load("W:\\deer.fbx", out context);

            int meshCount;
            FbxRead.GetMeshCount(context, out meshCount);
            Debug.WriteLine("Mesh count: {0}", meshCount);

            FbxRead.Unload(context);
        }
    }
}
