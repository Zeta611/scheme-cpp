#include <cassert>
#include <string>
#include "test.h"
#include "hash_table.h"
#include "node.h"
#include "node_pool.h"
#include "string_stream.h"
#include "tokenizer.h"

void test()
{
  test_hash_table();
  test_node_pool();
  test_tokenizer();
}


void test_hash_table()
{
  test_hash_table_can_insert();
  test_hash_table_can_get();
  test_hash_table_can_count();
  test_hash_table_can_be_full();
  test_hash_table_can_overflow();
  test_hash_table_can_update();
  test_hash_table_can_be_accessed_with_hash_value();
}


void test_hash_table_can_insert()
{
  int size = 13;
  auto table = hash_table<std::string>(size);
  int index = table.insert("key");
  assert(index >= 0);
  index = table.insert("key", "element");
  assert(index >= 0);
}


void test_hash_table_can_get()
{
  int size = 13;
  auto table = hash_table<std::string>(size);
  std::string key = "key";
  std::string element = "element";
  assert(table.insert(key, element) >= 0);
  try {
    assert(table.get(key) == element);
  } catch (std::runtime_error& e) {
    std::cerr << e.what() << "\n";
  }
}

void test_hash_table_can_count()
{
  int size = 13;
  auto table = hash_table<std::string>(size);

  for (int i = 1; i <= 5; ++i) {
    table.insert(std::string(1, char(i)));
    assert(i == table.get_count());
  }
}


void test_hash_table_can_be_full()
{
  int size = 13;
  auto table = hash_table<std::string>(size);

  for (int i = 1; i <= size; ++i) {
    table.insert(std::string(1, char(i)));
    assert(i == table.get_count());
  }
}


void test_hash_table_can_overflow()
{
  int size = 13;
  auto table = hash_table<std::string>(size);

  for (int i = 1; i <= size; ++i) {
    table.insert(std::string(1, char(i)));
  }
  assert(table.insert("test") == -1);
}


void test_hash_table_can_update()
{
  int size = 13;
  auto table = hash_table<std::string>(size);

  for (int i = 1; i <= size; ++i) {
    auto key = std::string(1, char(i));
    std::string element = "initial";
    table.insert(key, element);
    assert(table.get(key) == element);
  }

  for (int i = 1; i <= size; ++i) {
    auto key = std::string(1, char(i));
    std::string element = "new";
    table.insert(key, element);
    assert(table.get(key) == element);
  }
}


void test_hash_table_can_be_accessed_with_hash_value()
{
  int size = 13;
  auto table = hash_table<std::string>(size);

  int hash_value = table.insert("key");
  assert(table.get_key(hash_value) == "key");
}


void test_node_pool()
{
  test_node_pool_can_be_empty();
  test_node_pool_can_get();
  test_node_pool_can_allocate();
  test_node_pool_can_deallocate();
  test_node_pool_can_reserve_capacity();
}


void test_node_pool_can_be_empty()
{
  int capacity = 5;
  auto pool = node_pool(capacity);
  assert(pool.is_empty());
}


void test_node_pool_can_get()
{
  int capacity = 5;
  auto pool = node_pool(capacity);
  auto node = pool.get_node(1);
  assert(node.left == 0);
  assert(node.right == 2);

  node = pool.get_node(5);
  assert(node.left == 0);
  assert(node.right == 0);
}


void test_node_pool_can_allocate()
{
  int capacity = 5;
  auto pool = node_pool(capacity);

  auto index = pool.allocate();
  assert(pool.get_size() == 1);
  assert(pool.get_node(index).right == 0);
}


void test_node_pool_can_deallocate()
{
  int capacity = 5;
  auto pool = node_pool(capacity);

  auto index = pool.allocate();
  assert(pool.get_size() == 1);
  assert(pool.get_node(index).right == 0);
  pool.deallocate(index);
  assert(pool.get_size() == 0);
  assert(pool.get_node(index).right == 2);
}


void test_node_pool_can_reserve_capacity()
{
  int capacity = 5;
  int size = 9;
  auto pool = node_pool(capacity);

  for (int i = 0; i < size; ++i) {
    pool.allocate();
  }
  assert(pool.get_size() == size);

  for (int i = 0; i < size; ++i) {
    pool.deallocate(i + 1);
  }
  assert(pool.get_size() == 0);
}


void test_tokenizer()
{
  test_tokenizer_can_tokenize_parenthesis();
  test_tokenizer_can_tokenize_whitespaces();
  test_tokenizer_can_tokenize_variables();
  // test_tokenizer_can_tokenize_numbers();
  // test_tokenizer_can_tokenize_mixed_tokens();
  // test_tokenizer_can_put_back_tokens();
}


void test_tokenizer_can_tokenize_parenthesis()
{
  auto stream = string_stream("()");
  auto t = tokenizer(stream);

  auto left_paren = t.get_token();
  auto right_paren = t.get_token();

  assert(left_paren == parenthesis::left);
  assert(right_paren == parenthesis::right);
  assert(t.get_token() == token::nil_token);
}


void test_tokenizer_can_tokenize_whitespaces()
{
  std::string whitespace_str = " \n \t ";
  auto stream = string_stream(whitespace_str);
  auto t = tokenizer(stream);

  auto whitespace_token = t.get_token();

  assert(whitespace_token == whitespace(whitespace_str));
  assert(t.get_token() == token::nil_token);
}


void test_tokenizer_can_tokenize_variables()
{
  std::string variable_str = "a-new_variable_with-some-numbers_123_456_";
  auto stream = string_stream(variable_str);
  auto t = tokenizer(stream);

  auto variable_token = t.get_token();

  assert(variable_token == variable(variable_str));
  assert(t.get_token() == token::nil_token);
}


void test_tokenizer_can_tokenize_numbers()
{
  std::string number_str = "201811974";
  auto stream = string_stream(number_str);
  auto t = tokenizer(stream);

  auto number_token = t.get_token();

  assert(number_token == number(number_str));
  assert(t.get_token() == token::nil_token);
}

void test_tokenizer_can_tokenize_mixed_tokens()
{
  std::string str = "\t (define x-4 \n(+ 3 4))";
  token expected_tokens[] = {
    whitespace("\t "),
    parenthesis("("),
    keyword("define"),
    whitespace(" "),
    variable("x-4"),
    whitespace(" \n"),
    parenthesis("("),
    _operator("+"),
    whitespace(" "),
    number("3"),
    whitespace(" "),
    number("4"),
    parenthesis(")"),
    parenthesis(")"),
  };

  auto stream = string_stream(str);
  auto t = tokenizer(stream);

  int i = 0;
  while (true) {
    auto tok = t.get_token();
    if (tok == token::nil_token) { break; }
    assert(tok == expected_tokens[i++]);
  }

  assert(t.get_token() == token::nil_token);
}

void test_tokenizer_can_put_back_tokens()
{
  std::string str = "\t (define x-4 \n(+ 3 4))";
  token expected_tokens[] = {
    whitespace("\t "),
    parenthesis("("),
    keyword("define"),
    whitespace(" "),
    variable("x-4"),
    whitespace(" \n"),
    parenthesis("("),
    _operator("+"),
    whitespace(" "),
    number("3"),
    whitespace(" "),
    number("4"),
    parenthesis(")"),
    parenthesis(")"),
  };

  auto stream = string_stream(str);
  auto t = tokenizer(stream);

  for (int i = 0; i < 3; ++i) {
    t.get_token();
  }
  for (int i = 0; i < 3; ++i) {
    t.put_back();
  }
  for (int i = 0; i < 3; ++i) {
    assert(t.get_token() == expected_tokens[i]);
  }
  for (int i = 0; i < 3; ++i) {
    t.put_back();
  }

  int i = 0;
  while (true) {
    auto tok = t.get_token();
    if (tok == token::nil_token) { break; }
    assert(tok == expected_tokens[i++]);
  }

  t.put_back();
  t.put_back();

  assert(t.get_token() == expected_tokens[13]);
  assert(t.get_token() == token::nil_token);
}
