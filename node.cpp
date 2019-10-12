#include <iostream>
#include "node.h"
#include "node_pool.h"

node::node()
  : left{0}, right{0} {}


node::node(int left, int right)
  : left{left}, right{right} {}


node& node::lchild(const node_pool& pool) const
{
  return pool.get_node(left);
}


node& node::rchild(const node_pool& pool) const
{
  return pool.get_node(right);
}


std::ostream& operator<<(std::ostream& stream, const node& node)
{
  return stream
    << "(left: " << node.left << ", right: " << node.right << ")\n";
}
