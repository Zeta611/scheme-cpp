#ifndef TOKENIZER_H
#define TOKENIZER_H
#include "token_list.h"
#include "string_stream.h"

class tokenizer {
public:
  tokenizer(string_stream stream);
  string_stream stream;

  token get_token();
  void put_back();

private:
  token_list tl;
  int index = -1;

  std::string get_from_stream(token_type type);
  std::string get_from_stream();
};

#endif /* ifndef TOKENIZER_H */
