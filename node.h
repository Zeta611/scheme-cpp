#ifndef NODE_H
#define NODE_H
#include <iosfwd>
#include "node_state.h"

class node_pool;

struct node {
  int left;
  int right;

  node_state state = node_state::empty;

  node& lchild(const node_pool& pool) const;
  node& rchild(const node_pool& pool) const;

  node();
  node(int left, int right);
};

#endif /* ifndef NODE_H */
