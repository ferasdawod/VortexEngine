using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Runtime.InteropServices;

namespace EditorApplication.Native
{
    public static class NativeMethods
    {
#if DEBUG
        private const string kDllName = "EditorDLL_Debug.dll";
#else
        private const string kDllName = "EditorDLL_Release.dll";
#endif

        [DllImport(kDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern bool Initialize(IntPtr hInstance, IntPtr hWnd);

        [DllImport(kDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Tick();

        [DllImport(kDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void Resize();

        [DllImport(kDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void ShutDown();

        [DllImport(kDllName, CallingConvention = CallingConvention.Cdecl)]
        public static extern void LoadLevel([MarshalAs(UnmanagedType.BStr)] string levelPath);
    }
}
