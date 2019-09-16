#include <iostream>
#include "node.h"
#include "node_pool.h"

node_pool::node_pool(int capacity)
  : capacity{capacity}
{
  nodes = new node[capacity];
  for (int i = 0; i < capacity; ++i) {
    auto& node = nodes[i];
    node.left = 0;
    node.right = i + 2;
  }
  nodes[capacity - 1].right = 0;
}


node_pool::~node_pool()
{
  delete [] nodes;
}


bool node_pool::is_empty()
{
  return size == 0;
}


int node_pool::get_size()
{
  return size;
}


node& node_pool::get_node(int index)
{
  return nodes[index - 1];
}


int node_pool::get_free_head_index()
{
  return free_head_index;
}


int node_pool::allocate()
{
  if (size == capacity - 1) {
    reserve_capacity(capacity * 2);
  }

  ++size;
  int index = free_head_index;
  auto& node = get_node(index);

  free_head_index = node.right;
  node.right = 0;
  return index;
}


void node_pool::deallocate(int index)
{
  --size;
  auto& node = get_node(index);

  if (node.right > 0) {
    deallocate(node.right);
  }

  if (node.left > 0) {
    deallocate(node.left);
  }

  node.left = 0;
  node.right = free_head_index;
  free_head_index = index;
}


void node_pool::reserve_capacity(int minimum_capacity)
{
  auto new_nodes = new node[minimum_capacity];

  for (int i = 0; i < capacity; ++i) {
    new_nodes[i] = nodes[i];
  }
  for (int i = capacity; i < minimum_capacity; ++i) {
    auto& node = new_nodes[i];
    node.left = 0;
    node.right = i + 2;
  }
  new_nodes[minimum_capacity - 1].right = 0;

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
  auto center = [=](std::string input, int width) {
    auto left_pad = std::string((width - input.length()) / 2, ' ');
    auto right_pad = std::string(
      (width - input.length()) / 2 + (width - input.length()) % 2,
      ' ');
    return left_pad + input + right_pad;
  };

  stream << "Memory Table =\n"
         << "+-----+-------------+-------------+\n"
         << "|  i  |  Left Value | Right Value |\n"
         << "+-----+-------------+-------------+\n";

  for (int i = 0; i < pool.capacity; ++i) {
    auto node = pool.nodes[i];
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

    stream << "|"
           << center(i_str, 5)
           << "|"
           << center(left, 13)
           << "|"
           << center(right, 13)
           << "|\n";
  }
  stream << "+-----+-------------+-------------+\n";
  return stream;
}
