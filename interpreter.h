#ifndef INTERPRETER_H
#define INTERPRETER_H
#include <string>
#include "token.h"
#include "tokenizer.h"
#include "node_pool.h"
#include "hash_table.h"
#include "call_stack.h"

class interpreter {
public:
  void run();

private:
  tokenizer tok = tokenizer(string_stream(""));
  node_pool pool = node_pool(30);
  hash_table sym_table = hash_table(1031);
  call_stack stack = call_stack(100);

  /**
   * Recursively replaces functions and literals to process.
   * - Returns: Processed string.
   */
  std::string preprocess();

  /**
   * Recursively reads tokens from a tokenizer.
   * - Returns: The index of a root node for tokens between the pair of
   * parentheses, or the negative hash value for a single token. The index is
   * one-based, in order to identify a terminator with a zero.
   */
  int read();

  /**
   * Recursively evaluates tokens stored in node_pool.
   * - Parameter root_node_index: The one-based index of the root node.
   */
  int eval(int root_node_index);

  int eval_token(int hash_value);

  int eval_list_or_func(int root_node_index, int link_node_index);

  int eval_predefined(int root_node_index, token t);

  void post_process();

  /**
   * Recursively prints the expression starting from the root node.
   * - Parameters:
   *   - root_node_index: The one-based index of the root node.
   *   - start_list: A flag which indicates if the root node is at the start of
   *     the list.
   */
  void print(int root_node_index, bool start_list) const;

  void print_meta(int root_node_index) const;
};

#endif /* ifndef INTERPRETER_H */
