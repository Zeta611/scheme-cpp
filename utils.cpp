#include "utils.h"

namespace utils
{
  bool is_whitespace(char ch)
  {
    return ch == ' ' || ch == '\n' || ch == '\r' || ch == '\t';
  }
}
