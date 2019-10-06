#include <iostream>
#include <cassert>
#include "node.h"
#include "utils.h"
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

    auto new_cmd = preprocess();
    tok.stream.flush();
    tok.stream.insert_str(new_cmd);

    int root_node_index = read();
    int result = eval(root_node_index);
    print(result, true);
    std::cout << "\n";
    print_meta(root_node_index);
    // print(root_node_index, true);

    // if (root_node_index > 0) {
    //   pool.deallocate(root_node_index);
    // }
    //
    // assert(pool.is_empty());

    if (tok.stream.is_at_end()) {
      tok.stream.flush();
    }
    std::cout << "\n";
  }
}


std::string interpreter::preprocess()
{
  std::string new_cmd;

  while (true) {
    auto t = tok.get_token();
    if (t == token::nil) { break; }

    if (t == token::define) {
      new_cmd += " define";

      t = tok.get_token();
      if (t == token::left_paren) {
        t = tok.get_token();
        new_cmd += " " + t.value() + "(lambda(" + preprocess() + ")";
      } else {
        new_cmd += " " + t.value();
      }
    } else if (t == token::quote_lit) {
      new_cmd += "(quote ";

      int cnt_left_parens = 0;
      do {
        t = tok.get_token();
        new_cmd += " " + t.value();
        if (t == token::left_paren) {
          ++cnt_left_parens;
        } else if (t == token::right_paren) {
          --cnt_left_parens;
        }
      } while (cnt_left_parens > 0);

      new_cmd += ")";
    } else {
      new_cmd += " " + t.value();
    }
  }
  return new_cmd;
}


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


int interpreter::eval(int root_node_index)
{
  if (root_node_index < 0) {
    int link = sym_table.get(sym_table.get_key(-root_node_index));
    if (link != 0) {
      return eval(link);
    } else {
      return root_node_index;
    }
  } else if (root_node_index == 0) {
    return 0;
  }
  auto& root = pool.get_node(root_node_index);
  if (root.left >= 0) { return root_node_index; }

  auto t = sym_table.get_key(-root.left);
  if (t == token::plus || t == token::minus || t == token::times) {
    int lop_hash = eval(root.rchild(pool).left);
    int rop_hash = eval(root.rchild(pool).rchild(pool).left);

    auto lop_val = sym_table.get_key(-lop_hash).value();
    auto rop_val = sym_table.get_key(-rop_hash).value();

    // Both lop_val and rop_val are integer literals.
    if (utils::is_integer(lop_val) && utils::is_integer(rop_val)) {
      long long loperand = std::stoll(lop_val);
      long long roperand = std::stoll(rop_val);

      long long value = 0LL;
      if (t == token::plus) {
        value = loperand + roperand;
      } else if (t == token::minus) {
        value = loperand - roperand;
      } else if (t == token::times) {
        value = loperand * roperand;
      }

      auto result = token(std::to_string(value), token_type::number);
      return -sym_table.insert(result);
    // Both lop_val and rop_val are number literals, and at least one of the
    // two is a float literal.
    } else if (utils::is_number(lop_val) && utils::is_number(rop_val)) {
      long double loperand = std::stold(lop_val);
      long double roperand = std::stold(rop_val);

      long double value = 0L;
      if (t == token::plus) {
        value = loperand + roperand;
      } else if (t == token::minus) {
        value = loperand - roperand;
      } else if (t == token::times) {
        value = loperand * roperand;
      }

      auto result = token(std::to_string(value), token_type::number);
      return -sym_table.insert(result);
    // At least one of lop_val and rop_val is not a number.
    } else {
      throw std::runtime_error("Type error: not a number.");
    }
  } else if (t == token::is_number) {
    int op_hash = eval(root.rchild(pool).left);
    auto op_val = sym_table.get_key(-op_hash).value();
    auto bool_t = utils::is_number(op_val) ? token::true_t : token::false_t;
    return -sym_table.insert(bool_t);
  } else if (t == token::is_symbol) {
    int op_hash = eval(root.rchild(pool).left);
    auto bool_t = token::false_t;

    if (op_hash < 0) {
      auto op_val = sym_table.get_key(-op_hash).value();
      bool_t = utils::is_number(op_val) ? token::false_t : token::true_t;
    }
    return -sym_table.insert(bool_t);
  } else if (t == token::is_null) {
    int op_hash = root.right;
    auto bool_t = token::true_t;
    if (op_hash == 0 || eval(op_hash) == 0) {
      bool_t = token::true_t;
    } else {
      bool_t = token::false_t;
    }
    return -sym_table.insert(bool_t);
  } else if (t == token::cons) {
    int index = pool.allocate();
    auto& new_node = pool.get_node(index);
    new_node.left = eval(root.rchild(pool).left);
    new_node.right = eval(root.rchild(pool).rchild(pool).left);
    return index;
  } else if (t == token::cond) {
    auto* new_root = &pool.get_node(root_node_index);
    while (new_root->rchild(pool).right != 0) {
      root_node_index = new_root->right;
      new_root = &pool.get_node(root_node_index);

      auto& cond_clause = new_root->lchild(pool);
      if (eval(cond_clause.left) == -sym_table.insert(token::true_t)) {
        return eval(cond_clause.rchild(pool).left);
      }
    }
    new_root = &pool.get_node(root_node_index);
    int last_hash = new_root->rchild(pool).lchild(pool).left;
    if (last_hash >= 0 || sym_table.get_key(-last_hash) != token::else_t) {
      throw std::runtime_error("Syntax error: else required.");
    }
    return eval(new_root->rchild(pool).lchild(pool).rchild(pool).left);
  } else if (t == token::car) {
    int operand = root.rchild(pool).left;
    return pool.get_node(eval(operand)).left;
  } else if (t == token::cdr) {
    int operand = root.rchild(pool).left;
    return pool.get_node(eval(operand)).right;
  } else if (t == token::define) {
    int op_hash = root.rchild(pool).left;
    auto op_tok = sym_table.get_key(-op_hash);
    int def = root.rchild(pool).rchild(pool).left;
    // A function is defined.
    if (def > 0 && sym_table.get_key(pool.get_node(def).left) == token::lambda) {
      sym_table.insert(op_tok, def);
    } else {
      sym_table.insert(op_tok, eval(def));
    }
    return 0;
  } else if (t == token::quote) {
    return root.rchild(pool).left;
  } else {
    return root_node_index;
  }
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
