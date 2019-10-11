#include <string>
#include <cctype>
#include <cstdlib>
#include "utils.h"

namespace utils
{
  bool is_whitespace(char ch)
  {
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
  }


  bool is_integer(const std::string& s)
  {
    if (s.empty() || (!std::isdigit(s[0]) && (s[0] != '-') && (s[0] != '+'))) {
      return false;
    }

    char* ch;
    std::strtoll(s.c_str(), &ch, 10);
    return *ch == 0;
  }


  bool is_float(const std::string& s)
  {
    if (s.empty() || (!std::isdigit(s[0]) && (s[0] != '-') && (s[0] != '+'))) {
      return false;
    }

    char* ch;
    std::strtold(s.c_str(), &ch);
    return *ch == 0;
  }


  bool is_number(const std::string& s)
  {
    return is_integer(s) || is_float(s);
  }


  std::string center_align(const std::string& s, int width)
  {
    int len = s.length();
    auto left_pad = std::string((width - len) / 2, ' ');
    auto right_pad = std::string((width - len) / 2 + (width - len) % 2, ' ');
    return left_pad + s + right_pad;
  }
}
