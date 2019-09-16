#ifndef NODE_H
#define NODE_H
#include <iosfwd>

class node {
public:
  int left;
  int right;

  node();
  node(int left, int right);

  friend std::ostream& operator<<(std::ostream&, const node&);
};

#endif /* ifndef NODE_H */
