# LibSSL for C++ x64 Projects

This repo documents a simple, reliable way to link **OpenSSL (libssl + libcrypto)**
into a **C++ x64** project using **CMake**. The steps below cover Windows, Linux,
and macOS, and work for both local builds and CI.

## Recommended: CMake + system/vcpkg packages

### 1) Install OpenSSL

**Linux (Debian/Ubuntu):**
```bash
sudo apt-get update
sudo apt-get install -y libssl-dev
```

**macOS (Homebrew):**
```bash
brew install openssl
```

**Windows (vcpkg):**
```powershell
vcpkg install openssl:x64-windows
```

### 2) CMakeLists.txt

Add the following to your `CMakeLists.txt`:
```cmake
cmake_minimum_required(VERSION 3.20)
project(my_app LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(OpenSSL REQUIRED)

add_executable(my_app src/main.cpp)
target_link_libraries(my_app PRIVATE OpenSSL::SSL OpenSSL::Crypto)
```

### 3) Configure for x64

**Windows (Visual Studio generator):**
```powershell
cmake -S . -B build -A x64
cmake --build build --config Release
```

**Windows (vcpkg + x64 triplet):**
```powershell
cmake -S . -B build ^
  -A x64 ^
  -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake ^
  -DVCPKG_TARGET_TRIPLET=x64-windows
cmake --build build --config Release
```

**Linux/macOS:**
```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build -j
```

## Windows (Visual Studio, no CMake)

Use this if you are building directly in Visual Studio without CMake.

### 1) Install OpenSSL (x64)

**Recommended (vcpkg):**
```powershell
vcpkg install openssl:x64-windows
```

This installs headers and libraries under:
`C:\vcpkg\installed\x64-windows\`

### 2) Set the solution platform to x64

In Visual Studio: **Build > Configuration Manager** and set
**Active solution platform** to **x64**.

### 3) Add include path

Project Properties -> **C/C++** -> **General** ->
**Additional Include Directories**:
```
C:\vcpkg\installed\x64-windows\include
```

### 4) Add library path

Project Properties -> **Linker** -> **General** ->
**Additional Library Directories**:
```
C:\vcpkg\installed\x64-windows\lib
```

### 5) Link the libraries

Project Properties -> **Linker** -> **Input** ->
**Additional Dependencies**:
```
libssl.lib
libcrypto.lib
Ws2_32.lib
Crypt32.lib
```

If you see unresolved external symbols, also try adding `Advapi32.lib`.

### 6) Runtime DLLs (if using dynamic libs)

Copy the matching DLLs next to your `.exe`, or add their folder to `PATH`:
```
C:\vcpkg\installed\x64-windows\bin\libssl-3-x64.dll
C:\vcpkg\installed\x64-windows\bin\libcrypto-3-x64.dll
```

For Debug builds, use `debug\bin`. Ensure the DLLs match x64.

## Alternative: Manual include/lib paths (not preferred)

If you cannot use `find_package(OpenSSL)`, you can manually set include paths and
library files. You must link **both** `ssl` and `crypto`:
```cmake
target_include_directories(my_app PRIVATE /path/to/openssl/include)
target_link_libraries(my_app PRIVATE
  /path/to/openssl/lib/libssl.a
  /path/to/openssl/lib/libcrypto.a
)
```

On Windows with MSVC, link against the `.lib` files (and use matching x64
builds), e.g. `libssl.lib` and `libcrypto.lib`.

## Common pitfalls

- **Architecture mismatch**: x64 app must link against x64 OpenSSL binaries.
- **Missing OpenSSL**: `find_package(OpenSSL)` will fail if OpenSSL is not
  installed or not discoverable in `PATH`/CMake prefix paths.
- **Link order**: when linking manually, always include `ssl` before `crypto`.

## Quick test snippet

```cpp
#include <openssl/ssl.h>
#include <openssl/err.h>

int main() {
  SSL_library_init();
  return 0;
}
```