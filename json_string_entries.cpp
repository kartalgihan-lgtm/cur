#include "json_string_entries.h"

#include <cstdint>
#include <string>

namespace json_entries {
namespace {

class Parser {
 public:
  explicit Parser(std::string_view input) : input_(input) {}

  std::vector<std::string> ParseStringArray() {
    SkipWhitespace();
    Expect('[');
    SkipWhitespace();
    std::vector<std::string> result;
    if (ConsumeIf(']')) {
      return result;
    }
    while (true) {
      result.push_back(ParseString());
      SkipWhitespace();
      if (ConsumeIf(',')) {
        SkipWhitespace();
        continue;
      }
      if (ConsumeIf(']')) {
        break;
      }
      throw ParseError("Expected ',' or ']'");
    }
    SkipWhitespace();
    if (!AtEnd()) {
      throw ParseError("Unexpected trailing characters");
    }
    return result;
  }

 private:
  bool AtEnd() const { return pos_ >= input_.size(); }

  char Peek() const {
    if (AtEnd()) {
      return '\0';
    }
    return input_[pos_];
  }

  char Get() {
    if (AtEnd()) {
      throw ParseError("Unexpected end of input");
    }
    return input_[pos_++];
  }

  void SkipWhitespace() {
    while (!AtEnd()) {
      char c = Peek();
      if (c == ' ' || c == '\n' || c == '\r' || c == '\t') {
        ++pos_;
      } else {
        break;
      }
    }
  }

  void Expect(char expected) {
    if (Get() != expected) {
      throw ParseError("Unexpected character");
    }
  }

  bool ConsumeIf(char expected) {
    if (Peek() == expected) {
      ++pos_;
      return true;
    }
    return false;
  }

  static int HexDigit(char c) {
    if (c >= '0' && c <= '9') {
      return c - '0';
    }
    if (c >= 'a' && c <= 'f') {
      return 10 + (c - 'a');
    }
    if (c >= 'A' && c <= 'F') {
      return 10 + (c - 'A');
    }
    return -1;
  }

  uint32_t ParseHexCodepoint() {
    uint32_t value = 0;
    for (int i = 0; i < 4; ++i) {
      if (AtEnd()) {
        throw ParseError("Unexpected end of input in unicode escape");
      }
      int digit = HexDigit(Get());
      if (digit < 0) {
        throw ParseError("Invalid hex digit in unicode escape");
      }
      value = (value << 4) | static_cast<uint32_t>(digit);
    }
    return value;
  }

  static void AppendUtf8(std::string& out, uint32_t codepoint) {
    if (codepoint <= 0x7F) {
      out.push_back(static_cast<char>(codepoint));
      return;
    }
    if (codepoint <= 0x7FF) {
      out.push_back(static_cast<char>(0xC0 | (codepoint >> 6)));
      out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
      return;
    }
    if (codepoint <= 0xFFFF) {
      out.push_back(static_cast<char>(0xE0 | (codepoint >> 12)));
      out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
      out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
      return;
    }
    if (codepoint <= 0x10FFFF) {
      out.push_back(static_cast<char>(0xF0 | (codepoint >> 18)));
      out.push_back(static_cast<char>(0x80 | ((codepoint >> 12) & 0x3F)));
      out.push_back(static_cast<char>(0x80 | ((codepoint >> 6) & 0x3F)));
      out.push_back(static_cast<char>(0x80 | (codepoint & 0x3F)));
      return;
    }
    throw ParseError("Invalid unicode codepoint");
  }

  std::string ParseString() {
    if (!ConsumeIf('"')) {
      throw ParseError("Expected string");
    }
    std::string result;
    while (!AtEnd()) {
      char c = Get();
      if (c == '"') {
        return result;
      }
      if (static_cast<unsigned char>(c) < 0x20) {
        throw ParseError("Unescaped control character in string");
      }
      if (c != '\\') {
        result.push_back(c);
        continue;
      }
      if (AtEnd()) {
        throw ParseError("Unexpected end of input in escape");
      }
      char esc = Get();
      switch (esc) {
        case '"':
        case '\\':
        case '/':
          result.push_back(esc);
          break;
        case 'b':
          result.push_back('\b');
          break;
        case 'f':
          result.push_back('\f');
          break;
        case 'n':
          result.push_back('\n');
          break;
        case 'r':
          result.push_back('\r');
          break;
        case 't':
          result.push_back('\t');
          break;
        case 'u': {
          uint32_t codepoint = ParseHexCodepoint();
          if (codepoint >= 0xD800 && codepoint <= 0xDBFF) {
            if (!(ConsumeIf('\\') && ConsumeIf('u'))) {
              throw ParseError("Missing low surrogate");
            }
            uint32_t low = ParseHexCodepoint();
            if (low < 0xDC00 || low > 0xDFFF) {
              throw ParseError("Invalid low surrogate");
            }
            codepoint = 0x10000 +
                        (((codepoint - 0xD800) << 10) | (low - 0xDC00));
          } else if (codepoint >= 0xDC00 && codepoint <= 0xDFFF) {
            throw ParseError("Unexpected low surrogate");
          }
          AppendUtf8(result, codepoint);
          break;
        }
        default:
          throw ParseError("Invalid escape sequence");
      }
    }
    throw ParseError("Unterminated string");
  }

  std::string_view input_;
  size_t pos_ = 0;
};

}  // namespace

std::vector<std::string> ParseJsonStringArray(std::string_view json) {
  Parser parser(json);
  return parser.ParseStringArray();
}

}  // namespace json_entries
