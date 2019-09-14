#ifndef TOEKN_H
#define TOEKN_H
#include <string>
#include "token_type.h"

class token {
public:
  token(std::string value, token_type type);
  std::string get_value() const;
  token_type get_type() const;

  static const token nil_token;

  friend bool operator==(const token& lhs, const token& rhs);
  friend bool operator!=(const token& lhs, const token& rhs);

private:
  std::string value;
  token_type type;
};


class number : public token {
public:
  number(std::string value);
};


class keyword : public token {
public:
  keyword(std::string value);

  static const keyword define;
  static const keyword lambda;
  static const keyword _if;
  static const keyword cond;
};


class _operator : public token {
public:
  _operator(std::string value);

  static const _operator addition;
  static const _operator subtraction;
  static const _operator multiplication;
  static const _operator division;
  static const _operator modulus;
  static const _operator equal;
  static const _operator greater;
  static const _operator less;
};


class parenthesis : public token {
public:
  parenthesis(std::string value);

  static const parenthesis left;
  static const parenthesis right;
};


class string : public token {
public:
  string(std::string value);
};


class variable : public token {
public:
  variable(std::string value);
};


class whitespace : public token {
public:
  whitespace(std::string value);
};

#endif /* ifndef TOEKN_H */
