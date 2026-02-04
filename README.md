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