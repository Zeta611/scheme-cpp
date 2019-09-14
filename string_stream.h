#ifndef STRING_STREAM_H
#define STRING_STREAM_H
#include <string>

class string_stream {
public:
  string_stream(std::string str);
  bool is_at_end();
  bool is_empty();
  char peek_char();
  char get_char();
  void flush();
  void insert_str(std::string str);

private:
  std::string str;
  long index = -1;
};

#endif /* ifndef STRING_STREAM_H */
