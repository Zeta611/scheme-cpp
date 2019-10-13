#ifndef NODE_POOL_H
#define NODE_POOL_H
#include <iosfwd>

class hash_table;
struct node;

class node_pool {
public:
  node_pool(int capacity);
  ~node_pool();

  bool is_empty() const;
  int size() const;

  node& get_node(int index) const;
  int get_free_head_index() const;

  int allocate(const hash_table& sym_table);
  void deallocate(int index);

  bool cmp_struct(const node& lnode, const node& rnode) const;

  void collect_garbage(const hash_table& sym_table);
  void set_state(int index);
  void release_busy_nodes();

  friend std::ostream& operator<<(std::ostream&, const node_pool&);

private:
  node** nodes;
  int free_head_index = 1;
  int capacity;
  int _size = 0;

  void reserve_capacity(int minimum_capacity);
};

#endif /* ifndef NODE_POOL_H */
