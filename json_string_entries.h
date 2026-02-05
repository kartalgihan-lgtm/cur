#pragma once

#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace json_entries {

class ParseError : public std::runtime_error {
 public:
  using std::runtime_error::runtime_error;
};

// Parses a JSON array of strings (e.g. ["a","b"]) into a vector.
// Throws ParseError on invalid JSON.
std::vector<std::string> ParseJsonStringArray(std::string_view json);

}  // namespace json_entries
