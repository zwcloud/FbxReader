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

            double[] contorlPoints;
            if(FbxRead.GetControlPoints(context, meshNames[0], out contorlPoints))
            {
                Debug.WriteLine("Mesh control points(first 5 ones):");
                Debug.Indent();
                for (int i = 0; i < 5; i++)
                {
                    Debug.WriteLine("{0},{1},{2}", contorlPoints[3*i], contorlPoints[3*i+1], contorlPoints[3*i+2]);
                }
                Debug.Unindent();
            }

            FbxRead.Unload(context);
        }
    }
}
