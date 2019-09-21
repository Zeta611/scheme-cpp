#include <iostream>
#include "node.h"
#include "interpreter.h"

void interpreter::run()
{
  while (true) {
    std::cout << "> ";
    if (tok.stream.is_empty()) {
      std::string tmp;
      // return if EOF.
      if (!std::getline(std::cin, tmp)) { return; }
      tok.stream.insert_str(tmp);
    }

    preprocess();
    int root_node_index = read();
    print_meta(root_node_index);
    print(root_node_index, true);

    if (root_node_index > 0) {
      pool.deallocate(root_node_index);
    }

    assert(pool.is_empty());

    if (tok.stream.is_at_end()) {
      tok.stream.flush();
    }
    std::cout << "\n";
  }
}


void interpreter::preprocess() {}


int interpreter::read()
{
  auto start_token = tok.get_token();
  if (start_token == token::nil) { return 0; }

  int token_hash_value = sym_table.insert(start_token);
  if (token_hash_value == -1) {
    throw std::runtime_error("Symbol table is full.");
  }

  // Immediately return the hash value of a non-parenthesis starting token.
  if (start_token != token::left_paren) {
    return -token_hash_value;
  }

  bool is_root_node = true;

  // One-based index for the root node of a current `read` call.
  int root_node_index = 0;

  // One-based index that tracks current node.
  int current_node_index = 0;

  // Repeat adding a node until the token reaches a right parenthesis.
  while (true) {
    auto t = tok.get_token();

    if (t == token::nil || t == token::right_paren) { break; }

    int token_hash_value = sym_table.insert(t);
    if (token_hash_value == -1) {
      throw std::runtime_error("Symbol table is full.");
    }

    // Allocate a new node, and set `currentNodeIndex` as the index of it.
    int index = pool.allocate();
    if (is_root_node) {
      root_node_index = index;
      is_root_node = false;
    } else {
      pool.get_node(current_node_index).right = index;
    }
    current_node_index = index;

    // If the token is a left parenthesis, recursively set `left` of the node
    // and put it back, since `read` begins parsing from a left parenthesis;
    // Otherwise set `left` as a negative hash value.
    if (t == token::left_paren) {
      tok.put_back();
      int index = read();

      // `current_node` is assigned here, as `pool` can create a new
      // array while performing `reserve_capacity`.
      auto& current_node = pool.get_node(current_node_index);
      current_node.left = index;
    } else {
      auto& current_node = pool.get_node(current_node_index);
      current_node.left = -token_hash_value;
    }
  }
  return root_node_index;
}


void interpreter::print(int root_node_index, bool start_list)
{
  if (root_node_index == 0) {
    std::cout << "() ";
  } else if (root_node_index < 0) {
    std::cout << sym_table.get_key(-root_node_index).value() << " ";
  } else if (root_node_index > 0) {
    if (start_list) {
      std::cout << "( ";
    }

    auto& current_node = pool.get_node(root_node_index);
    print(current_node.left, true);

    if (current_node.right != 0) {
      print(current_node.right, false);
    } else {
      std::cout << ") ";
    }
  }
}


void interpreter::print_meta(int root_node_index)
{
  std::cout << "] Free List Root = "
            << pool.get_free_head_index()
            << "\n  Node List Root = "
            << root_node_index
            << "\n\n"
            << pool
            << "\n"
            << sym_table
            << "\n";
}
