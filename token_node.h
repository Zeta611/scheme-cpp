#ifndef TOKEN_NODE_H
#define TOKEN_NODE_H
#include "token.h"

class token_node {
public:
  token_node(token t);
  token t;
  token_node* next = nullptr;
  token_node* prev = nullptr;
};

#endif /* ifndef TOKEN_NODE_H */
