# cur

Minimal libcurl example with CMake. The build links against libcurl to avoid
unresolved external symbols such as `curl_easy_setopt` and `curl_easy_perform`.

## Build

Install libcurl development files first:

- Debian/Ubuntu: `sudo apt-get install libcurl4-openssl-dev`
- macOS (Homebrew): `brew install curl`
- Windows (vcpkg): `vcpkg install curl`

Configure and build:

```
cmake -S . -B build
cmake --build build
```

If you need a static link on Windows, enable:

```
cmake -S . -B build -DCUR_USE_STATIC_CURL=ON
```

## Visual Studio (no CMake)

If you want to build in Visual Studio without CMake, you still need to link
libcurl explicitly to avoid unresolved externals.

1. Install libcurl (recommended: vcpkg):
   - `vcpkg install curl:x64-windows`
   - or static: `vcpkg install curl:x64-windows-static`
2. In Visual Studio, open Project Properties:
   - C/C++ -> General -> Additional Include Directories:
     - `<vcpkg>\installed\x64-windows\include`
     - or `<vcpkg>\installed\x64-windows-static\include`
   - Linker -> General -> Additional Library Directories:
     - `<vcpkg>\installed\x64-windows\lib`
     - or `<vcpkg>\installed\x64-windows-static\lib`
   - Linker -> Input -> Additional Dependencies:
     - dynamic: `libcurl.lib`
     - static: `libcurl_a.lib`
3. For static linking, add `CURL_STATICLIB` to:
   - C/C++ -> Preprocessor -> Preprocessor Definitions
4. If the linker still complains, add common Windows dependencies:
   - `ws2_32.lib; wldap32.lib; crypt32.lib; normaliz.lib`

These steps ensure symbols like `curl_easy_setopt` and `curl_easy_perform`
resolve correctly in Visual Studio builds.