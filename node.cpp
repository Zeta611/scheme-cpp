#include <iostream>
#include "node.h"

node::node()
  : left{0}, right{0} {}


node::node(int left, int right)
  : left{left}, right{right} {}


std::ostream& operator<<(std::ostream& stream, const node& node)
{
  return stream
    << "(left: " << node.left << ", right: " << node.right << ")\n";
}
