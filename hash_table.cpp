#include <iostream>
#include "hash_table.h"

int hash_table::count()
{
  return _count;
}

hash_table::hash_table(int size)
  : size{size}
{
  table = new bucket*[size]();
}


hash_table::~hash_table()
{
  delete [] table;
}


int hash_table::get(token key)
{
  int hash_value = hash(key);

  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket* bk = table[index];
    if (key == bk->key) { return bk->element; }
  }
  throw std::runtime_error("Key does not exists.");
}


token hash_table::get_key(int hash_value)
{
  return table[hash_value]->key;
}


int hash_table::insert(token key, int element)
{
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


int hash_table::hash(token t)
{
  int value = 0;
  for (auto c : t.value()) {
    value += (int)c;
  }
  return value;
}


std::ostream& operator<<(std::ostream& stream, const hash_table& table)
{
  auto center = [=](std::string input, int width) {
    auto left_pad = std::string((width - input.length()) / 2, ' ');
    auto right_pad = std::string(
      (width - input.length()) / 2 + (width - input.length()) % 2,
      ' ');
    return left_pad + input + right_pad;
  };

  stream << "Hash Table =\n"
         << "+------------+----------+--------+\n"
         << "| Hash Value |  Symbol  |  Link  |\n"
         << "+------------+----------+--------+\n";
  for (int i = 0; i < table.size; ++i) {
    auto bk = table.table[i];
    if (bk != nullptr) {
      auto i_str = std::to_string(-i);

      int link = bk->element;
      auto link_str = link == 0 ? "nil" : std::to_string(link);

      stream << "|"
             << center(i_str, 12)
             << "|"
             << center(bk->key.value(), 10)
             << "|"
             << center(link_str, 8)
             << "|\n";
    }
  }
  stream << "+------------+----------+--------+\n";
  return stream;
}
