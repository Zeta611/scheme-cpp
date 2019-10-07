#include "stack_node.h"

stack_node::stack_node(int symbol, int link)
  : symbol{symbol}, link{link} {}

stack_node::stack_node()
  : symbol{0}, link{0} {}
