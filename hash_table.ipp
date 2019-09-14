template<typename T>
int hash_table<T>::get_count()
{
  return count;
}

template<typename T>
hash_table<T>::hash_table(int size)
  : size{size}
{
  this->table = new bucket*[size]();
}


template<typename T>
hash_table<T>::~hash_table()
{
  delete [] table;
}


template<typename T>
T hash_table<T>::get(std::string key)
{
  int hash_value = hash(key);

  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket* b = table[index];
    if (key == b->key) { return b->element; }
  }
  throw std::runtime_error("Key does not exists.");
}


template<typename T>
std::string hash_table<T>::get_key(int hash_value)
{
  return table[hash_value]->key;
}


template<typename T>
int hash_table<T>::insert(std::string key, T element)
{
  int hash_value = hash(key);

  // Use open addressing: increment a hash value if a collision occurs and
  // retry insertion.
  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket*& b = table[index];

    // `b` is empty, so add the pair to the table.
    if (b == nullptr) {
      b = new bucket;
      b->key = key;
      ++count;
    }

    // `b` with such `key` exists, so update `b` with the pair.
    if (key == b->key) {
      b->element = element;
      return index;
    }
  }
  return -1;
}


template<typename T>
int hash_table<T>::insert(std::string key)
{
  int hash_value = hash(key);

  // Use open addressing: increment a hash value if a collision occurs and
  // retry insertion.
  for (int i = 0; i < size; ++i) {
    int index = (hash_value + i) % size;
    bucket*& b = table[index];

    // `b` is empty, so add the pair to the table.
    if (b == nullptr) {
      b = new bucket;
      b->key = key;
      ++count;
    }

    // `b` with such `key` exists, so update `b` with the pair.
    if (key == b->key) {
      return index;
    }
  }
  return -1;
}


template<typename T>
int hash_table<T>::hash(std::string str)
{
  int value = 0;
  for (auto c : str) {
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
    auto b = table.table[i];
    if (b != nullptr) {
      auto i_str = std::to_string(-i);
      stream << "|"
             << center(i_str, 16)
             << "|"
             << center(b->key, 16)
             << "|\n";
    }
  }
  stream << "+----------------+----------------+\n";
  return stream;
}
