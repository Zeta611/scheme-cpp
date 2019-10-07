#include <stdexcept>
#include "call_stack.h"
#include "stack_node.h"

call_stack::call_stack(int size) : size{size}
{
  array = new stack_node[size];
}


call_stack::~call_stack()
{
  delete [] array;
}


void call_stack::push(int symbol, int link)
{
  if (pos == size - 1) { throw std::runtime_error("Stack overflow."); }
  array[++pos] = stack_node(symbol, link);
}


stack_node call_stack::pop()
{
  if (pos == -1) { throw std::runtime_error("Stack empty."); }
  return array[pos--];
}
