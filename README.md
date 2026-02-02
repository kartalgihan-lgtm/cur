# Dynamic Library Client Example

This repository contains a minimal Windows C++ program that loads a DLL and
invokes an exported `run_main` entry point.

## Build (MSVC)

```
cl /std:c++17 main.cpp
```

## Notes

- Update the DLL path and arguments in `main.cpp` to match your environment.
- If the DLL export is not `extern "C"`, use the exact mangled name in
  `GetProcAddress`.