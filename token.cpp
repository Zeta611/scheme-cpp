#include "token.h"

token::token(std::string value, token_type type)
  : _value{value}, _type{type} {}


std::string token::value() const
{
  return _value;
}


token_type token::type() const
{
  return _type;
}

const token token::left_paren = token("(", token_type::paren);
const token token::right_paren = token(")", token_type::paren);

const token token::quote = token("'", token_type::literal);

const token token::define = token("define", token_type::keyword);
const token token::lambda = token("lambda", token_type::keyword);
const token token::cond = token("cond", token_type::keyword);
const token token::if_t = token("if", token_type::keyword);

const token token::plus = token("+", token_type::keyword);
const token token::minus = token("-", token_type::keyword);
const token token::times = token("*", token_type::keyword);
const token token::divide = token("/", token_type::keyword);

const token token::eq = token("eq?", token_type::keyword);
const token token::equal = token("equal?", token_type::keyword);

const token token::nil = token("", token_type::nil);

bool operator==(const token& lhs, const token& rhs)
{
  return lhs.value() == rhs.value() && lhs.type() == rhs.type();
}


bool operator!=(const token& lhs, const token& rhs)
{
  return lhs.value() != rhs.value() || lhs.type() != rhs.type();
}
