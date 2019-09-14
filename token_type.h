#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

enum class token_type {
  keyword,
  number,
  _operator,
  parenthesis,
  string,
  variable,
  whitespace,
  nil
};

#endif /* ifndef TOKEN_TYPE_H */
