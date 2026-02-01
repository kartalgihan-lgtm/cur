# DLL PowerShell Runner

This repo shows how to call a native DLL export from PowerShell. The C++
`run` function that uses `std::string` and `std::function` cannot be called
directly from PowerShell. Export a C ABI wrapper instead, then call that
wrapper from PowerShell.

## 1) Export a C ABI wrapper from the DLL

```cpp
// in your DLL project
#include <cstdlib>
#include <cstring>
#include <string>

extern "C" __declspec(dllexport)
const char* __cdecl run_c(const char* params,
                          void(__cdecl *log)(const char*))
{
    try {
        std::string p = params ? params : "";
        auto cb = [log](const std::string& s) {
            if (log) {
                log(s.c_str());
            }
        };

        // call your existing C++ implementation
        std::string out = run(p, cb);

        // allocate with malloc so PowerShell can free
        char* result = (char*)std::malloc(out.size() + 1);
        if (!result) {
            return nullptr;
        }
        std::memcpy(result, out.c_str(), out.size() + 1);
        return result;
    } catch (...) {
        return nullptr;
    }
}

extern "C" __declspec(dllexport)
void __cdecl free_string(const char* p)
{
    std::free((void*)p);
}
```

Notes:
- Use `extern "C"` and `__cdecl` so the export name is exactly `run_c`.
- If you already return a static string, do not free it. Otherwise provide
  `free_string` as above.

## 2) Run the DLL from PowerShell

Use the provided script:

```powershell
pwsh .\run-dll.ps1 `
  -DllPath "D:\repos\IT\isya\modules\x64\Release\Coockies.dll" `
  -Params "your args"
```

If `run_c` is found, it is called with your params. Any messages from the
callback are printed. The return string is written to stdout.

### Troubleshooting

- PowerShell (and the DLL) must be the same bitness (both x64 or both x86).
- If `LoadLibrary` fails, confirm the DLL path and its dependencies.