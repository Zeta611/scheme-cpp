template<typename T>
int hash_table<T>::count()
{
  return _count;
}

template<typename T>
hash_table<T>::hash_table(int size)
  : size{size}
{
  table = new bucket*[size]();
}


template<typename T>
hash_table<T>::~hash_table()
{
  delete [] table;
}


template<typename T>
T hash_table<T>::get(token key)
{
  int hash_value = hash(key);

  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket* bk = table[index];
    if (key == bk->key) { return bk->element; }
  }
  throw std::runtime_error("Key does not exists.");
}


template<typename T>
token hash_table<T>::get_key(int hash_value)
{
  return table[hash_value]->key;
}


template<typename T>
int hash_table<T>::insert(token key, T element)
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


template<typename T>
int hash_table<T>::insert(token key)
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
      return index;
    }
  }
  return -1;
}


template<typename T>
int hash_table<T>::hash(token t)
{
  int value = 0;
  for (auto c : t.value()) {
    value += (int)c;
  }
  return value;
}


template<typename T>
std::ostream& operator<<(std::ostream& stream, const hash_table<T>& table)
{
  auto center = [=](std::string input, int width) {
    auto left_pad = std::string((width - input.length()) / 2, ' ');
    auto right_pad = std::string(
      (width - input.length()) / 2 + (width - input.length()) % 2,
      ' ');
    return left_pad + input + right_pad;
  };

  stream << "Hash Table =\n"
         << "+----------------+----------------+\n"
         << "|   Hash Value   |     Symbol     |\n"
         << "+----------------+----------------+\n";
  for (int i = 0; i < table.size; ++i) {
    auto bk = table.table[i];
    if (bk != nullptr) {
      auto i_str = std::to_string(-i);
      stream << "|"
             << center(i_str, 16)
             << "|"
             << center(bk->key.value(), 16)
             << "|\n";
    }
  }
  stream << "+----------------+----------------+\n";
  return stream;
}
