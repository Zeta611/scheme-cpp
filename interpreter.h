#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <string>
#include "tokenizer.h"
#include "node_pool.h"
#include "hash_table.h"

class interpreter {
public:
  void run();

private:
  tokenizer tok = tokenizer(string_stream(""));
  node_pool pool = node_pool(30);
  hash_table<std::string> sym_table = hash_table<std::string>(997);

  void preprocess();

  /**
   * Recursively reads tokens from a tokenizer.
   *
   * - Returns: The index of a root node for tokens between the pair of
   * parentheses, or the negative hash value for a single token. The index is
   * one-based, in order to identify a terminator with a zero.
   */
  int read();

  /**
   * Recursively prints the expression starting from the root node.
   * - Parameters:
   *   - root_node_index: The one-based index of the root node.
   *   - start_list: A flag which indicates if the root node is at the start of
   *     the list.
   */
  void print(int root_node_index, bool start_list);

  void print_meta(int root_node_index);
};

#endif /* ifndef INTERPRETER_H */
