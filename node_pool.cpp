#include <iostream>
#include "utils.h"
#include "node_state.h"
#include "node.h"
#include "node_pool.h"
#include "hash_table.h"

node_pool::node_pool(int capacity)
  : capacity{capacity}
{
  nodes = new node*[capacity];
  for (int i = 0; i < capacity; ++i) {
    nodes[i] = new node();
    nodes[i]->left = 0;
    nodes[i]->right = i + 2;
  }
  nodes[capacity - 1]->right = 0;
}


node_pool::~node_pool()
{
  for (int i = 0; i < capacity; ++i) {
    delete nodes[i];
  }
  delete [] nodes;
}


bool node_pool::is_empty() const
{
  return _size == 0;
}


int node_pool::size() const
{
  return _size;
}


node& node_pool::get_node(int index) const
{
  return *nodes[index - 1];
}


int node_pool::get_free_head_index() const
{
  return free_head_index;
}


int node_pool::allocate(const hash_table& sym_table)
{
  if (_size == capacity - 1) {
    collect_garbage(sym_table);
  }

  if (_size == capacity - 1) {
    reserve_capacity(capacity * 2);
  }

  ++_size;
  int index = free_head_index;
  auto& node = get_node(index);

  free_head_index = node.right;
  node.right = 0;
  node.state = node_state::pending;
  return index;
}


void node_pool::deallocate(int index)
{
  --_size;
  auto& node = get_node(index);

  if (node.right > 0) {
    deallocate(node.right);
  }

  if (node.left > 0) {
    deallocate(node.left);
  }

  node.left = 0;
  node.right = free_head_index;
  node.state = node_state::empty;
  free_head_index = index;
}


void node_pool::collect_garbage(const hash_table& sym_table)
{
#ifndef NDEBUG
  std::cout << "========== Garbage Collecting... ==========\n";
#endif
  // Set the state of the nodes referenced from `sym_table` to `occupied`.
  for (int i = 0; i < sym_table.size; ++i) {
    auto* bucket = sym_table.table[i];
    if (bucket == nullptr) { continue; }
    int link = bucket->element;
    if (link > 0) { set_state(link); }
  }

  // Set the state of the rest of the non-empty nodes to `purgeable`.
  for (int i = 0; i < capacity; ++i) {
    auto& node = *nodes[i];
    if (node.state == node_state::pending) {
      node.state = node_state::purgeable;
    }
  }
}


void node_pool::set_state(int index)
{
  auto& node = get_node(index);
  node.state = node_state::occupied;

  int left = node.left;
  if (left > 0) { set_state(left); }

  int right = node.right;
  if (right > 0) { set_state(right); }
}


void node_pool::reserve_capacity(int minimum_capacity)
{
  auto new_nodes = new node*[minimum_capacity];

  for (int i = 0; i < capacity; ++i) {
    new_nodes[i] = nodes[i];
  }
  for (int i = capacity; i < minimum_capacity; ++i) {
    new_nodes[i] = new node();
    new_nodes[i]->left = 0;
    new_nodes[i]->right = i + 2;
  }
  new_nodes[minimum_capacity - 1]->right = 0;

  delete [] nodes;
  nodes = new_nodes;

  auto& node = get_node(free_head_index);
  while (node.right != 0) {
    node = get_node(node.right);
  }
  node.right = capacity + 1;
  capacity = minimum_capacity;
}


std::ostream& operator<<(std::ostream& stream, const node_pool& pool)
{
  stream << "Memory Table =\n"
         << "+-----+-------------+-------------+-----------+\n"
         << "|  i  |  Left Value | Right Value |   State   |\n"
         << "+-----+-------------+-------------+-----------+\n";

  for (int i = 0; i < pool.capacity; ++i) {
    auto node = *pool.nodes[i];
    auto i_str = std::to_string(i + 1);

    std::string left;
    if (node.left == 0) {
      left = "nil";
    } else {
      left = std::to_string(node.left);
    }

    std::string right;
    if (node.right == 0) {
      right = "nil";
    } else {
      right = std::to_string(node.right);
    }

    std::string state;
    switch (node.state) {
    case node_state::empty:
      state = "empty";
      break;
    case node_state::pending:
      state = "pending";
      break;
    case node_state::purgeable:
      state = "purgeable";
      break;
    case node_state::occupied:
      state = "occupied";
      break;
    }

    stream << "|"
           << utils::center_align(i_str, 5)
           << "|"
           << utils::center_align(left, 13)
           << "|"
           << utils::center_align(right, 13)
           << "|"
           << utils::center_align(state, 11)
           << "|\n";
  }
  stream << "+-----+-------------+-------------+-----------+\n";
  return stream;
}
