#include <iostream>
#include "node.h"
#include "interpreter.h"

void interpreter::run()
{
  while (true) {
    if (_tokenizer.stream.is_empty()) {
      std::cout << "> ";
      std::string tmp;
      std::getline(std::cin, tmp);
      _tokenizer.stream.insert_str(tmp);
    }

    _preprocess();
    int root_node_index = _read();
    _print_meta(root_node_index);
    _print(root_node_index, true);

    if (root_node_index > 0) {
      _node_pool.deallocate(root_node_index);
    }

    assert(_node_pool.is_empty());

    if (_tokenizer.stream.is_at_end()) {
      _tokenizer.stream.flush();
      std::cout << "\n";
    }
  }
}


void interpreter::_preprocess() {}


int interpreter::_read()
{
  token starting_token = _tokenizer.get_token();
  if (starting_token == token::nil_token) { return 0; }

  // Skip whitespaces
  while (starting_token.get_type() == token_type::whitespace) {
    token t = _tokenizer.get_token();
    if (t == token::nil_token) { return 0; }
    starting_token = t;
  }

  int token_hash_value = _sym_table.insert(starting_token.get_value());
  if (token_hash_value == -1) {
    throw std::runtime_error("Symbol table is full.");
  }

  // Immediately return the hash value of a non-parenthesis starting token.
  if (starting_token != parenthesis::left) {
    return -token_hash_value;
  }

  bool is_root_node = true;

  // One-based index for the root node of a current `_read` call.
  int root_node_index = 0;

  // One-based index that tracks current node.
  int current_node_index = 0;

  // Repeat adding a node until the token reaches a right parenthesis.
  while (true) {
    token tok = _tokenizer.get_token();

    if (tok == token::nil_token || tok == parenthesis::right) { break; }
    if (tok.get_type() == token_type::whitespace) { continue; }

    int token_hash_value = _sym_table.insert(tok.get_value());
    if (token_hash_value == -1) {
      throw std::runtime_error("Symbol table is full.");
    }

    // Allocate a new node, and set `currentNodeIndex` as the index of it.
    int index = _node_pool.allocate();
    if (is_root_node) {
      root_node_index = index;
      is_root_node = false;
    } else {
      _node_pool.get_node(current_node_index).right = index;
    }
    current_node_index = index;

    // If the token is a left parenthesis, recursively set `left` of the node
    // and put it back, since `_read` begins parsing from a left parenthesis;
    // Otherwise set `left` as a negative hash value.
    if (tok == parenthesis::left) {
      _tokenizer.put_back();
      int index = _read();

      // `current_node` is assigned here, as `_node_pool` can create a new
      // array while performing `reserve_capacity`.
      node& current_node = _node_pool.get_node(current_node_index);
      current_node.left = index;
    } else {
      node& current_node = _node_pool.get_node(current_node_index);
      current_node.left = -token_hash_value;
    }
  }
  return root_node_index;
}


void interpreter::_print(int root_node_index, bool start_list)
{
  if (root_node_index == 0) {
    std::cout << "() ";
  } else if (root_node_index < 0) {
    std::cout << _sym_table.get_key(-root_node_index) << " ";
  } else if (root_node_index > 0) {
    if (start_list) {
      std::cout << "( ";
    }

    node& current_node = _node_pool.get_node(root_node_index);
    _print(current_node.left, true);

    if (current_node.right != 0) {
      _print(current_node.right, false);
    } else {
      std::cout << ") ";
    }
  }
}


void interpreter::_print_meta(int root_node_index)
{
  std::cout << "] Free List Root = "
            << _node_pool.get_free_head_index()
            << "\n  Node List Root = "
            << root_node_index
            << "\n\n"
            << _node_pool
            << "\n"
            << _sym_table
            << "\n";
}
