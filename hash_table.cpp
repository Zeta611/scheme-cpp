#include <iostream>
#include <cassert>
#include "hash_table.h"
#include "token.h"
#include "utils.h"

int hash_table::count() const
{
  return _count;
}

hash_table::hash_table(int size)
  : size{size}
{
  assert(size > 19);
  table = new bucket*[size]();

  // Insert predefined tokens in a specific order.
  table[0]  = new bucket(token::nil, 0);
  table[1]  = new bucket(token::left_paren, 0);
  table[2]  = new bucket(token::right_paren, 0);
  table[3]  = new bucket(token::true_t, 0);
  table[4]  = new bucket(token::false_t, 0);
  table[5]  = new bucket(token::plus, 0);
  table[6]  = new bucket(token::minus, 0);
  table[7]  = new bucket(token::times, 0);
  table[8]  = new bucket(token::is_number, 0);
  table[9]  = new bucket(token::is_symbol, 0);
  table[10] = new bucket(token::is_null, 0);
  table[11] = new bucket(token::cons, 0);
  table[12] = new bucket(token::cond, 0);
  table[13] = new bucket(token::else_t, 0);
  table[14] = new bucket(token::car, 0);
  table[15] = new bucket(token::cdr, 0);
  table[16] = new bucket(token::define, 0);
  table[17] = new bucket(token::quote, 0);
  table[18] = new bucket(token::lambda, 0);
}


hash_table::~hash_table()
{
  delete [] table;
}


int hash_table::get(token key) const
{
  int hash_value = hash(key);

  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket* bk = table[index];
    if (key == bk->key) { return bk->element; }
  }
  throw std::runtime_error("Key does not exists.");
}


token hash_table::get_key(int hash_value) const
{
  return table[hash_value]->key;
}


int hash_table::insert(token key, int element)
{
  if (key == token::left_paren) {
    table[1]->element = element;
    return 1;
  }
  if (key == token::right_paren) {
    table[2]->element = element;
    return 2;
  }
  if (key == token::true_t) {
    table[3]->element = element;
    return 3;
  }
  if (key == token::false_t) {
    table[4]->element = element;
    return 4;
  }
  if (key == token::plus) {
    table[5]->element = element;
    return 5;
  }
  if (key == token::minus) {
    table[6]->element = element;
    return 6;
  }
  if (key == token::times) {
    table[7]->element = element;
    return 7;
  }
  if (key == token::is_number) {
    table[8]->element = element;
    return 8;
  }
  if (key == token::is_symbol) {
    table[9]->element = element;
    return 9;
  }
  if (key == token::is_null) {
    table[10]->element = element;
    return 10;
  }
  if (key == token::cons) {
    table[11]->element = element;
    return 11;
  }
  if (key == token::cond) {
    table[12]->element = element;
    return 12;
  }
  if (key == token::else_t) {
    table[13]->element = element;
    return 13;
  }
  if (key == token::car) {
    table[14]->element = element;
    return 14;
  }
  if (key == token::cdr) {
    table[15]->element = element;
    return 15;
  }
  if (key == token::define) {
    table[16]->element = element;
    return 16;
  }
  if (key == token::quote) {
    table[17]->element = element;
    return 17;
  }
  if (key == token::lambda) {
    table[18]->element = element;
    return 18;
  }

  int hash_value = hash(key);

  // Use open addressing: increment a hash value if a collision occurs and
  // retry insertion.
  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket*& bk = table[index];

    // `bk` is empty, so add the pair to the table.
    if (bk == nullptr) {
      bk = new bucket;
      bk->key = key;
      ++_count;
    }

    // `bk` with such `key` exists, so update `bk` with the pair.
    if (key == bk->key) {
      bk->element = element;
      return index;
    }
  }
  return -1;
}


int hash_table::insert(token key)
{
  if (key == token::left_paren) { return 1; }
  if (key == token::right_paren) { return 2; }
  if (key == token::true_t) { return 3; }
  if (key == token::false_t) { return 4; }
  if (key == token::plus) { return 5; }
  if (key == token::minus) { return 6; }
  if (key == token::times) { return 7; }
  if (key == token::is_number) { return 8; }
  if (key == token::is_symbol) { return 9; }
  if (key == token::is_null) { return 10; }
  if (key == token::cons) { return 11; }
  if (key == token::cond) { return 12; }
  if (key == token::else_t) { return 13; }
  if (key == token::car) { return 14; }
  if (key == token::cdr) { return 15; }
  if (key == token::define) { return 16; }
  if (key == token::quote) { return 17; }
  if (key == token::lambda) { return 18; }

  int hash_value = hash(key);

  // Use open addressing: increment a hash value if a collision occurs and
  // retry insertion.
  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket*& bk = table[index];

    // `bk` is empty, so add the pair to the table.
    if (bk == nullptr) {
      bk = new bucket;
      bk->key = key;
      ++_count;
    }

    // `bk` with such `key` exists.
    if (key == bk->key) {
      return index;
    }
  }
  return -1;
}


hash_table::bucket::bucket()
  : key{token::nil}, element{0} {}


hash_table::bucket::bucket(token key, int element)
  : key{key}, element{element} {}


int hash_table::hash(token t) const
{
  int value = 0;
  for (auto c : t.value()) {
    value += (int)(c * c);
  }
  return value;
}


std::ostream& operator<<(std::ostream& stream, const hash_table& table)
{
  stream << "Hash Table =\n"
         << "┌────────────┬──────────┬────────┐\n"
         << "│ Hash Value │  Symbol  │  Link  │\n"
         << "╞════════════╪══════════╪════════╡\n";
  for (int i = 0; i < table.size; ++i) {
    auto bk = table.table[i];
    if (bk != nullptr) {
      auto i_str = std::to_string(-i);

      int link = bk->element;
      auto link_str = link == 0 ? "nil" : std::to_string(link);

      stream << "│"
             << utils::center_align(i_str, 12)
             << "│"
             << utils::center_align(bk->key.value(), 10)
             << "│"
             << utils::center_align(link_str, 8)
             << "│\n";
    }
  }
  stream << "└────────────┴──────────┴────────┘\n";
  return stream;
}
