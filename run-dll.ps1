param(
  [Parameter(Mandatory = $true)]
  [string]$DllPath,
  [string]$Params = ""
)

Add-Type -TypeDefinition @"
using System;
using System.Runtime.InteropServices;

public static class CookiesRunner
{
    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void LogCallback([MarshalAs(UnmanagedType.LPStr)] string message);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate IntPtr RunDelegate([MarshalAs(UnmanagedType.LPStr)] string args,
                                       LogCallback cb);

    [UnmanagedFunctionPointer(CallingConvention.Cdecl)]
    public delegate void FreeStringDelegate(IntPtr ptr);

    [DllImport("kernel32", SetLastError = true, CharSet = CharSet.Unicode)]
    public static extern IntPtr LoadLibrary(string lpFileName);

    [DllImport("kernel32", SetLastError = true)]
    public static extern IntPtr GetProcAddress(IntPtr hModule, string lpProcName);

    [DllImport("kernel32", SetLastError = true)]
    public static extern bool FreeLibrary(IntPtr hModule);

    public static string Run(string dllPath, string args)
    {
        IntPtr h = LoadLibrary(dllPath);
        if (h == IntPtr.Zero)
        {
            throw new Exception("LoadLibrary failed: " + Marshal.GetLastWin32Error());
        }

        try
        {
            IntPtr pRun = GetProcAddress(h, "run_c");
            if (pRun == IntPtr.Zero)
            {
                throw new Exception("GetProcAddress failed: run_c not found");
            }

            var run = (RunDelegate)Marshal.GetDelegateForFunctionPointer(
                pRun, typeof(RunDelegate));

            LogCallback cb = (string s) =>
            {
                if (!string.IsNullOrEmpty(s))
                {
                    Console.WriteLine(s);
                }
            };

            IntPtr resultPtr = run(args ?? "", cb);
            string result = Marshal.PtrToStringAnsi(resultPtr) ?? "";

            IntPtr pFree = GetProcAddress(h, "free_string");
            if (pFree != IntPtr.Zero && resultPtr != IntPtr.Zero)
            {
                var freeString = (FreeStringDelegate)Marshal.GetDelegateForFunctionPointer(
                    pFree, typeof(FreeStringDelegate));
                freeString(resultPtr);
            }

            return result;
        }
        finally
        {
            FreeLibrary(h);
        }
    }
}
"@

$result = [CookiesRunner]::Run($DllPath, $Params)
Write-Output $result
