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