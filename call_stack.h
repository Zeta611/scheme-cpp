#ifndef CALL_STACK_H
#define CALL_STACK_H
#include "stack_node.h"

class call_stack {
public: 
  call_stack(int size);
  ~call_stack();

  void push(int symbol, int link);
  stack_node pop();

private:
  int size = 0;
  int pos = -1;
  stack_node* array;
};

#endif /* ifndef CALL_STACK_H */
