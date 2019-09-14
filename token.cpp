#include "token.h"

token::token(std::string value, token_type type)
  : value{value}, type{type} {}


std::string token::get_value() const
{
  return value;
}


token_type token::get_type() const
{
  return type;
}


const token token::nil_token = token("", token_type::nil);

bool operator==(const token& lhs, const token& rhs)
{
  return lhs.get_value() == rhs.get_value()
    && lhs.get_type() == rhs.get_type();
}


bool operator!=(const token& lhs, const token& rhs)
{
  return lhs.get_value() != rhs.get_value()
    || lhs.get_type() != rhs.get_type();
}


number::number(std::string value)
  : token(value, token_type::number) {}


keyword::keyword(std::string value)
  : token(value, token_type::keyword) {}


const keyword keyword::define = keyword("define");
const keyword keyword::lambda = keyword("lambda");
const keyword keyword::_if = keyword("if");
const keyword keyword::cond = keyword("cond");


_operator::_operator(std::string value)
  : token(value, token_type::_operator) {}


const _operator _operator::addition = _operator("+");
const _operator _operator::subtraction = _operator("-");
const _operator _operator::multiplication = _operator("*");
const _operator _operator::division = _operator("/");
const _operator _operator::modulus = _operator("%");
const _operator _operator::equal = _operator("=");
const _operator _operator::greater = _operator(">");
const _operator _operator::less = _operator("<");


parenthesis::parenthesis(std::string value)
  : token(value, token_type::parenthesis) {}


const parenthesis parenthesis::left = parenthesis("(");
const parenthesis parenthesis::right = parenthesis(")");


string::string(std::string value)
  : token(value, token_type::string) {}


variable::variable(std::string value)
  : token(value, token_type::variable) {}


whitespace::whitespace(std::string value)
  : token(value, token_type::whitespace) {}
