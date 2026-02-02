# Dynamic Library Client Example

This repository contains a minimal Windows C++ program that loads a DLL and
invokes an exported `run_main` entry point.

## Build (MSVC)

```
cl /std:c++17 main.cpp
```

## OpenSSL (libssl) x64

If you need to link against `libssl`/`libcrypto`, install OpenSSL for x64 and
make sure your include and lib paths point at the 64-bit build.

### Option A: vcpkg + CMake (recommended)

```
vcpkg install openssl:x64-windows
cmake -S . -B build -DCMAKE_TOOLCHAIN_FILE=[vcpkg root]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### Option B: MSVC command line (manual)

```
cl /std:c++17 main.cpp /I C:\OpenSSL-Win64\include ^
  /link /LIBPATH:C:\OpenSSL-Win64\lib libssl.lib libcrypto.lib
```

## Notes

- Update the DLL path and arguments in `main.cpp` to match your environment.
- If the DLL export is not `extern "C"`, use the exact mangled name in
  `GetProcAddress`.