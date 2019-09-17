#ifndef NODE_H
#define NODE_H
#include <iosfwd>

struct node {
  int left;
  int right;

  node();
  node(int left, int right);
};

#endif /* ifndef NODE_H */
