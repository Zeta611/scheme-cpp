#include "tokenizer.h"

tokenizer::tokenizer(string_stream stream)
  : stream{stream} {}


token tokenizer::get_token()
{
  char c = stream.peek_char();
  if (c == '\0') {
    return token::nil_token;
  }

  ++index;
  if (index < tokens.get_size()) {
    tokens.advance_current();
    return tokens.get_current();
  }

  auto t = token::nil_token;
  switch (c) {
  case ' ': case '\t': case '\n':
    t = whitespace(get_from_stream(token_type::whitespace));
    break;

  case '(': case ')':
    t = parenthesis(get_from_stream());
    break;

  // case '0': case '1': case '2': case '3': case '4': case '5': case '6':
  // case '7': case '8': case '9':
  //   t = number(get_from_stream(token_type::number));
  //   break;
  //
  // case '+': case '-': case '*': case '/': case '%': case '=': case '>':
  // case '<':
  //   t = _operator(get_from_stream());
  //   break;

  default:
    std::string value = get_from_stream(token_type::variable);
    if (value == "define" ||
        value == "lambda" ||
        value == "if" ||
        value == "cond")
    {
      t = keyword(value);
    } else {
      t = variable(value);
    }
  }

  tokens.append(t);
  tokens.advance_current();

  return t;
}


void tokenizer::put_back()
{
  --index;
  tokens.back_current();
}


std::string tokenizer::get_from_stream(token_type type)
{
  std::string s = "";

  while (true) {
    char c = stream.peek_char();
    if (c == '\0') { break; }

    switch (c) {
    case ' ': case '\t': case '\n':
      if (type != token_type::whitespace) {
        goto end_while;
      }
      s.push_back(stream.get_char());
      break;

    case '(': case ')':
      goto end_while;

    default:
      if (type == token_type::whitespace) {
        goto end_while;
      }
      if (c >= 'A' && c <= 'Z') {
        stream.get_char();
        s.push_back(c - ('Z' - 'z'));
      } else {
        s.push_back(stream.get_char());
      }
      break;
    // case '0': case '1': case '2': case '3': case '4': case '5': case '6':
    // case '7': case '8': case '9':
    //   if (type != token_type::number && type != token_type::variable) {
    //     goto end_while;
    //   }
    //   s.push_back(stream.get_char());
    //   break;
    //
    // default:
    //   if (type != token_type::variable && type != token_type::keyword) {
    //     goto end_while;
    //   }
    //   if ((c >= 'a' && c <= 'z') ||
    //       (c >= 'A' && c <= 'Z') ||
    //       c == '_' || c == '-')
    //   {
    //     s.push_back(stream.get_char());
    //     break;
    //   } else {
    //     goto end_while;
    //   }
    }
  }
  end_while:
  return s;
}


std::string tokenizer::get_from_stream()
{
  char c = stream.get_char();
  if (c == '\0') { return ""; }
  return std::string(1, c);
}
