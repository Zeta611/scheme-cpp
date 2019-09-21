#include "tokenizer.h"
#include "utils.h"

tokenizer::tokenizer(string_stream stream)
  : stream{stream} {}


token tokenizer::get_token()
{
  strip_whitespace();

  if (stream.peek_char() == '\0') {
    return token::nil_token;
  }

  ++index;
  if (index < tl.get_size()) {
    tl.advance_current();
    return tl.get_current();
  }

  std::string raw_tok = get_from_stream();

  auto t = token::nil_token;
  if (raw_tok.empty()) {
    t = token::nil_token;
  } else if (raw_tok == "(") {
    t = parenthesis::left;
  } else if (raw_tok == ")") {
    t = parenthesis::right;
  } else if (raw_tok == "define") {
    t = keyword::define;
  } else if (raw_tok == "lambda") {
    t = keyword::lambda;
  } else if (raw_tok == "if") {
    t = keyword::_if;
  } else if (raw_tok == "cond") {
    t = keyword::cond;
  } else {
    t = variable(raw_tok);
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


std::string tokenizer::get_from_stream()
{
  std::string buffer = "";
  bool init_loop = true;

  while (true) {
    char c = stream.peek_char();
    if (c == '\0') { break; }

    switch (c) {
    case ' ': case '\t': case '\n': case '\r':
      assert(!init_loop);
      return buffer;
      break;

    case '(': case ')':
      // Return parenthesis if it is found in the first loop.
      if (init_loop) {
        buffer.push_back(stream.get_char());
      }
      return buffer;

    case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G':
    case 'H': case 'I': case 'J': case 'K': case 'L': case 'M': case 'N':
    case 'O': case 'P': case 'Q': case 'R': case 'S': case 'T': case 'U':
    case 'V': case 'W': case 'X': case 'Y': case 'Z':
      {
        int offset = 'A' - 'a';
        buffer.push_back(stream.get_char() - offset);
      }
      break;

    default:
      buffer.push_back(stream.get_char());
      break;
    }

    init_loop = false;
  }
  return buffer;
}


void tokenizer::strip_whitespace()
{
  while (utils::is_whitespace(stream.peek_char())) {
    stream.get_char();
  }
}
