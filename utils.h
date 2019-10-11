#ifndef UTILS_H
#define UTILS_H
#include <string>

namespace utils
{
  bool is_whitespace(char ch);
  bool is_integer(const std::string& s);
  bool is_float(const std::string& s);
  bool is_number(const std::string& s);
  std::string center_align(const std::string& s, int width);
}

#endif /* ifndef UTILS_H */
