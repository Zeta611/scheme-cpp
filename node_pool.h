#ifndef NODE_POOL_H
#define NODE_POOL_H
#include <iosfwd>

struct node;

class node_pool {
public:
  node_pool(int capacity);
  ~node_pool();

  bool is_empty() const;
  int size() const;
  node& get_node(int index) const;
  int get_free_head_index() const;
  int allocate();
  void deallocate(int index);

  friend std::ostream& operator<<(std::ostream&, const node_pool&);

private:
  node** nodes;
  int free_head_index = 1;
  int capacity;
  int _size = 0;

  void reserve_capacity(int minimum_capacity);
};

#endif /* ifndef NODE_POOL_H */
