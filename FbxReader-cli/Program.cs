using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace FbxReader_cli
{
    class Program
    {
        static void Main(string[] args)
        {
            IntPtr context;
            FbxRead.Load("W:\\deer.fbx", out context);
            FbxRead.Unload(context);
        }
    }
}
