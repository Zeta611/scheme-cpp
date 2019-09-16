#ifndef NODE_POOL_H
#define NODE_POOL_H
#include <iosfwd>

class node;
class node_pool {
public:
  node_pool(int capacity);
  ~node_pool();

  bool is_empty();
  int get_size();
  node& get_node(int index);
  int get_free_head_index();
  int allocate();
  void deallocate(int index);

  friend std::ostream& operator<<(std::ostream&, const node_pool&);

private:
  node* nodes;
  int free_head_index = 1;
  int capacity;
  int size = 0;
  void reserve_capacity(int minimum_capacity);
};

#endif /* ifndef NODE_POOL_H */
