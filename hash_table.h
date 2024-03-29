#ifndef HASH_TABLE_H
#define HASH_TABLE_H
#include <iostream>
#include "token.h"
#include "node_pool.h"

class hash_table {
public:
  /**
   * Intializes `hash_table` with a given size.
   */
  hash_table(int size);
  ~hash_table();

  int count() const;

  /**
   * Gets the element for the given `key`.
   *
   * - Parameter key: The key to find in the hash table.
   * - Returns: The element associated with `key` if `key` is in the hash
   *   table.
   * - Complexity: O(1) expected, worst case O(*n*), where *n* is the size of
   *   the hash table.
   */
  int get(token key) const;

  /**
   * Gets the key for the given `hash_value`.
   *
   * - Parameter hash_value: The hash value to find in the hash table.
   * - Returns: The key in the hash table with hash value equal to `hash_value`
   *   if it exists.
   * - Complexity: O(1)
   */
  token get_key(int hash_value) const;


  /**
   * Inserts `key` to the hash table, and returns the hash value for the
   * inserted `key` in the hash table.
   *
   * This method leaves the element associated with `key` uninitialized.
   *
   * The return value is -1 only if the hash table if full.
   *
   * - Parameter key: The key to add to the hash table.
   * - Returns: The hash value for the inserted key if the insertion was
   *   successful; -1 otherwise.
   * - Complexity: O(1) expected, worst case O(*n*), where *n* is the size of
   *   the hash table.
   */
  int insert(token key);

  /**
   * Inserts the pair `(key, element)` to the hash table, and returns the hash
   * value for the inserted pair in the hash table.
   *
   * If the `key` already exists in the hash table, update the previous element
   * associated with `key` with `element`.
   *
   * The return value is -1 only if the hash table if full.
   *
   * - Parameters:
   *   - key: The key associated with `element` to add to the hash table.
   *   - element: The element to add to the hash table.
   * - Returns: The hash value for the inserted pair if the insertion was
   *   successful; -1 otherwise.
   * - Complexity: O(1) expected, worst case O(*n*), where *n* is the size of
   *   the hash table.
   */
  int insert(token key, int element);

  struct bucket {
    bucket();
    bucket(token key, int element);
    token key;
    int element;
  };

  friend std::ostream& operator<<(std::ostream&, const hash_table&);
  friend void node_pool::collect_garbage(const hash_table&);

private:
  const int size;
  int _count = 0;

  bucket** table;

  int hash(token tok) const;
};

#endif /* ifndef HASH_TABLE_H */
