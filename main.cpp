#include <windows.h>

#include <functional>
#include <string>

using RunFn = std::string(*)(std::string, std::function<void(std::string)>);

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;

    const wchar_t* dll_path = L"D:\\repos\\Cos.dll";
    HMODULE h = LoadLibraryW(dll_path);
    if (!h) {
        return 1;
    }

    auto run = reinterpret_cast<RunFn>(GetProcAddress(h, "run_main"));  // or mangled name
    if (!run) {
        FreeLibrary(h);
        return 2;
    }

    std::string result = run(
        "--out_path=\"d:/output\" --input=\"test inp\"",
        [](std::string s) {
            (void)s; // optional: print or handle output lines
        });
    (void)result;

    FreeLibrary(h);
    return 0;
}
