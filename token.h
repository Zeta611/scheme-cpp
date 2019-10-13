#ifndef TOEKN_H
#define TOEKN_H
#include <string>
#include "token_type.h"

class token {
public:
  token(std::string value, token_type type);
  std::string value() const;
  token_type type() const;

  static const token left_paren;
  static const token right_paren;

  static const token quote;
  static const token quote_lit;

  static const token define;
  static const token lambda;
  static const token if_t;
  static const token cond;
  static const token else_t;

  static const token plus;
  static const token minus;
  static const token times;
  static const token divide;

  static const token eq_num;
  static const token eq;
  static const token eqv;
  static const token equal;
  static const token is_number;
  static const token is_symbol;
  static const token is_null;

  static const token true_t;
  static const token false_t;

  static const token cons;
  static const token car;
  static const token cdr;

  static const token nil;

  friend bool operator==(const token& lhs, const token& rhs);
  friend bool operator!=(const token& lhs, const token& rhs);

private:
  std::string _value;
  token_type _type;
};

#endif /* ifndef TOEKN_H */
