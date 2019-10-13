#include <cassert>
#include "tokenizer.h"
#include "utils.h"

tokenizer::tokenizer(string_stream stream)
  : stream{stream} {}


token tokenizer::get_token()
{
  strip_whitespace();

  if (stream.peek_char() == '\0') {
    return token::nil;
  }

  ++index;
  if (index < tl.get_size()) {
    tl.advance_current();
    return tl.get_current();
  }

  std::string raw_tok = get_from_stream();

  auto t = token::nil;
  if (raw_tok.empty()) {
    t = token::nil;
  } else if (raw_tok == "(") {
    t = token::left_paren;
  } else if (raw_tok == ")") {
    t = token::right_paren;
  } else if (raw_tok == "quote") {
    t = token::quote;
  } else if (raw_tok == "'") {
    t = token::quote_lit;
  } else if (raw_tok == "define") {
    t = token::define;
  } else if (raw_tok == "lambda") {
    t = token::lambda;
  } else if (raw_tok == "if") {
    t = token::if_t;
  } else if (raw_tok == "cond") {
    t = token::cond;
  } else if (raw_tok == "else") {
    t = token::else_t;
  } else if (raw_tok == "+") {
    t = token::plus;
  } else if (raw_tok == "-") {
    t = token::minus;
  } else if (raw_tok == "*") {
    t = token::times;
  } else if (raw_tok == "=") {
    t = token::eq_num;
  } else if (raw_tok == "eq?") {
    t = token::eq;
  } else if (raw_tok == "eqv?") {
    t = token::eqv;
  } else if (raw_tok == "equal?") {
    t = token::equal;
  } else if (raw_tok == "number?") {
    t = token::is_number;
  } else if (raw_tok == "symbol?") {
    t = token::is_symbol;
  } else if (raw_tok == "null?") {
    t = token::is_null;
  } else if (raw_tok == "#t") {
    t = token::true_t;
  } else if (raw_tok == "#f") {
    t = token::false_t;
  } else if (raw_tok == "cons") {
    t = token::cons;
  } else if (raw_tok == "car") {
    t = token::car;
  } else if (raw_tok == "cdr") {
    t = token::cdr;
  } else if (utils::is_number(raw_tok)) {
    t = token(raw_tok, token_type::number);
  } else {
    t = token(raw_tok, token_type::variable);
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

    case '(': case ')': case '\'':
      // Return parenthesis/quotation if it is found in the first loop.
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
