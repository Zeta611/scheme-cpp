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

    try {
      int root_node_index = read();
      int result = eval(root_node_index);
      std::cout << "] ";
      print(result, true);

#ifndef NDEBUG
      std::cout << "\n";
      print_meta(root_node_index);
#endif
    } catch (const std::exception& e) {
      std::cout << e.what() << "\n";
      continue;
    }

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
  // root_node_index is a token hash value.
  if (root_node_index <= 0) {
    auto t = sym_table.get_key(-root_node_index);
    if (t.type() == token_type::number ||
        t.type() == token_type::boolean ||
        t.type() == token_type::nil ||
        t.type() == token_type::keyword) {
      return root_node_index;
    } else {
      return sym_table.get(t);
    }
  }

  auto& root = pool.get_node(root_node_index);
  int lchild = eval(root.left);
  // lchild represents a list or a user-defined function.
  if (lchild > 0) {
    auto& link_node = pool.get_node(lchild);
    // lchild must be a function, not a list.
    if (sym_table.get_key(-link_node.left) != token::lambda) {
      throw std::runtime_error("Syntax error: function required.");
    }

    auto* param_node = &link_node.rchild(pool).lchild(pool);
    auto* arg_node = &root.rchild(pool);
    int param_cnt = 0;
    int arg_cnt = 0;

    if (arg_node->left != 0) {
      while (true) {
        ++arg_cnt;
        if (arg_node->right == 0) { break; }
        arg_node = &arg_node->rchild(pool);
      }
    }
    // Store evaluated argument values.
    auto* tmp_eval = new int[arg_cnt];
    arg_node = &root.rchild(pool);
    for (int i = 0; i < arg_cnt; ++i) {
      tmp_eval[i] = eval(arg_node->left);
      arg_node = &arg_node->rchild(pool);
    }

    arg_node = &root.rchild(pool);
    if (param_node->left != 0) {
      while (true) {
        // Keep original parameter values in `stack`.
        int param_hash = param_node->left;
        auto param_tok = sym_table.get_key(-param_hash);
        int param_link = sym_table.get(param_tok);

        stack.push(param_hash, param_link);
        sym_table.insert(param_tok, tmp_eval[param_cnt++]);

        if (param_node->right != 0) {
          param_node = &param_node->rchild(pool);
          if (arg_node->right == 0) {
            throw std::runtime_error("Syntax error: more arguments required.");
          }
          arg_node = &arg_node->rchild(pool);
        } else {
          if (arg_node->right != 0) {
            throw std::runtime_error("Syntax error: more arguments provided.");
          }
          break;
        }
      }
    } else if (arg_node->left != 0) {
      throw std::runtime_error("Syntax error: more arguments provided.");
    }

    delete [] tmp_eval;

    int result = eval(link_node.rchild(pool).rchild(pool).left);

    // Restore parameter values.
    for (int i = 0; i < param_cnt; ++i) {
      auto pair = stack.pop();
      auto t = sym_table.get_key(-pair.symbol);
      sym_table.insert(t, pair.link);
    }

    return result;
  }

  // lchild represents a predefined token.
  auto t = sym_table.get_key(-lchild);
  if (t == token::plus || t == token::minus || t == token::times) {
    // Throw if more than two operands are given.
    if (root.rchild(pool).rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

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
    // Throw if more than one operand are given.
    if (root.rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

    int op_hash = eval(root.rchild(pool).left);
    if (op_hash > 0) {
      return -sym_table.insert(token::false_t);
    }
    auto op_val = sym_table.get_key(-op_hash).value();
    auto bool_t = utils::is_number(op_val) ? token::true_t : token::false_t;
    return -sym_table.insert(bool_t);

  } else if (t == token::is_symbol) {
    // Throw if more than one operand are given.
    if (root.rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

    int op_hash = eval(root.rchild(pool).left);

    if (op_hash <= 0) {
      auto op_t = sym_table.get_key(-op_hash);
      if (op_t.type() != token_type::keyword &&
          op_t.type() != token_type::number &&
          op_t.type() != token_type::boolean &&
          op_t.type() != token_type::nil) {
        return -sym_table.insert(token::true_t);
      }
    }
    return -sym_table.insert(token::false_t);

  } else if (t == token::is_null) {
    // Throw if more than one operand are given.
    if (root.rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

    int op_hash = root.rchild(pool).left;
    auto bool_t = token::true_t;
    if (eval(op_hash) == 0) {
      bool_t = token::true_t;
    } else {
      bool_t = token::false_t;
    }
    return -sym_table.insert(bool_t);

  } else if (t == token::cons) {
    // Throw if more than two operands are given.
    if (root.rchild(pool).rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

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
      if (cond_clause.rchild(pool).right != 0) {
        throw std::runtime_error("Syntax error: too much operands.");
      }

      int check = eval(cond_clause.left);
      if (check == -sym_table.insert(token::true_t)) {
        return eval(cond_clause.rchild(pool).left);
      } else if (check == -sym_table.insert(token::false_t)) {
        continue;
      } else {
        throw std::runtime_error("Type error: Boolean expected.");
      }
    }
    new_root = &pool.get_node(root_node_index);
    int last_hash = new_root->rchild(pool).lchild(pool).left;
    if (last_hash >= 0 || sym_table.get_key(-last_hash) != token::else_t) {
      throw std::runtime_error("Syntax error: else required.");
    }

    if (new_root->rchild(pool).lchild(pool).rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }
    return eval(new_root->rchild(pool).lchild(pool).rchild(pool).left);

  } else if (t == token::car) {
    if (root.rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

    int operand = eval(root.rchild(pool).left);
    if (operand == 0) {
      throw std::runtime_error("Syntax error: list is empty.");
    }
    return pool.get_node(operand).left;

  } else if (t == token::cdr) {
    if (root.rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

    int operand = eval(root.rchild(pool).left);
    if (operand == 0) {
      throw std::runtime_error("Syntax error: list is empty.");
    }
    return pool.get_node(operand).right;

  } else if (t == token::define) {
    if (root.rchild(pool).rchild(pool).right != 0) {
      throw std::runtime_error("Syntax error: too much operands.");
    }

    int op_hash = root.rchild(pool).left;
    auto op_tok = sym_table.get_key(-op_hash);
    int def = eval(root.rchild(pool).rchild(pool).left);
    sym_table.insert(op_tok, def);
    return 0;

  } else if (t == token::quote) {
    return root.rchild(pool).left;

  } else if (t == token::lambda) {
    return root_node_index;

  } else {
    throw std::runtime_error("Syntax error: undefined behavior.");
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
  std::cout << "Free List Root = "
            << pool.get_free_head_index()
            << "\nNode List Root = "
            << root_node_index
            << "\n\n"
            << pool
            << "\n"
            << sym_table
            << "\n";
}
