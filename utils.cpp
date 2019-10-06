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
}
