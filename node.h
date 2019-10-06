#ifndef NODE_H
#define NODE_H
#include <iosfwd>

class node_pool;

struct node {
  int left;
  int right;

  node& lchild(node_pool& pool);
  node& rchild(node_pool& pool);

  node();
  node(int left, int right);
};

#endif /* ifndef NODE_H */
