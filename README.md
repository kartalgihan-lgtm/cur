# cur

## JSON string entries parsing

Use `ParseJsonStringArray` to parse a JSON array of strings into
`std::vector<std::string>` without external dependencies.

```cpp
#include "json_string_entries.h"

std::string json = R"(["alpha", "beta", "gamma"])";
std::vector<std::string> entries =
    json_entries::ParseJsonStringArray(json);
```