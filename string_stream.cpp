#include "string_stream.h"
#include <iostream>

string_stream::string_stream(std::string str)
  : str{str} {}


bool string_stream::is_at_end()
{
  return str.size() == 0 ||
    (index >= 0 && (unsigned long)index == str.size() - 1);
}


bool string_stream::is_empty()
{
  return str.empty();
}


char string_stream::peek_char()
{
  if (is_empty() || is_at_end()) { return '\0'; }
  unsigned long peek_index = index >= 0 ? (unsigned long)index + 1 : 0;
  return str.at(peek_index);
}


char string_stream::get_char()
{
  if (is_empty() || is_at_end()) { return '\0'; }
  return str.at(++index);
}


void string_stream::flush()
{
  str.clear();
  index = -1;
}


void string_stream::insert_str(std::string str)
{
  this->str.append(str);
}
