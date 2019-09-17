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
  if (index < tl.get_size()) {
    tl.advance_current();
    return tl.get_current();
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

  tl.append(t);
  tl.advance_current();

  return t;
}


void tokenizer::put_back()
{
  --index;
  tl.back_current();
}


std::string tokenizer::get_from_stream(token_type type)
{
  std::string buffer = "";

  while (true) {
    char c = stream.peek_char();
    if (c == '\0') { break; }

    switch (c) {
    case ' ': case '\t': case '\n':
      if (type != token_type::whitespace) {
        return buffer;
      }
      buffer.push_back(stream.get_char());
      break;

    case '(': case ')':
      return buffer;

    default:
      if (type == token_type::whitespace) {
        return buffer;
      }
      if (c >= 'A' && c <= 'Z') {
        stream.get_char();
        buffer.push_back(c - ('Z' - 'z'));
      } else {
        buffer.push_back(stream.get_char());
      }
      break;
    // case '0': case '1': case '2': case '3': case '4': case '5': case '6':
    // case '7': case '8': case '9':
    //   if (type != token_type::number && type != token_type::variable) {
    //     return buffer;
    //   }
    //   buffer.push_back(stream.get_char());
    //   break;
    //
    // default:
    //   if (type != token_type::variable && type != token_type::keyword) {
    //     return buffer;
    //   }
    //   if ((c >= 'a' && c <= 'z') ||
    //       (c >= 'A' && c <= 'Z') ||
    //       c == '_' || c == '-')
    //   {
    //     buffer.push_back(stream.get_char());
    //     break;
    //   } else {
    //     return buffer;
    //   }
    }
  }
  return buffer;
}


std::string tokenizer::get_from_stream()
{
  char c = stream.get_char();
  if (c == '\0') { return ""; }
  return std::string(1, c);
}
