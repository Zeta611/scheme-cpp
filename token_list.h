#ifndef TOKEN_LIST_H
#define TOKEN_LIST_H
#include "token.h"

struct token_node;

class token_list {
public:
  ~token_list();

  void append(token t);
  int get_size() const;

  token get_current() const;
  void advance_current();
  void back_current();

private:
  int size = 0;
  int pos = -1;

  token_node* head = nullptr;
  token_node* tail = nullptr;
  token_node* current = nullptr;
};

#endif /* ifndef TOKEN_LIST_H */
