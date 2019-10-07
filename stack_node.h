#ifndef STACK_NODE_H
#define STACK_NODE_H

struct stack_node {
  stack_node(int symbol, int link);
  stack_node();
  int symbol;
  int link;
};

#endif /* ifndef STACK_NODE_H */
