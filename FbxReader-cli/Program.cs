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
            if(FbxRead.GetMeshCount(context, out meshCount))
            {
                Debug.WriteLine("Mesh count: {0}", meshCount);
            }

            string[] meshNames;
            if(FbxRead.GetMeshNames(context, out meshNames))
            {
                Debug.WriteLine("Mesh names:");
                Debug.Indent();
                foreach (var meshName in meshNames)
                {
                    Debug.WriteLine(meshName);
                }
                Debug.Unindent();
            }

            FbxRead.Unload(context);
        }
    }
}
