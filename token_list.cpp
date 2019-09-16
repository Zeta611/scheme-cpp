#include "token_node.h"
#include "token_list.h"

token_list::~token_list()
{

  auto current = head;
  while (current != nullptr) {
    auto temp = current;
    current = temp->next;
    delete temp;
  }
}


void token_list::append(token t)
{
  auto node = new token_node(t);
  ++size;

  if (head == nullptr) {
    head = node;
    tail = node;
  } else {
    node->prev = tail;
    tail->next = node;
    tail = node;
  }
}


int token_list::get_size()
{
  return size;
}


token token_list::get_current()
{
  return current->t;
}


void token_list::advance_current()
{
  if (pos == size) { return; }
  if (current == nullptr) {
    current = head;
    pos = 0;
    return;
  }
  current = current->next;
  ++pos;
}


void token_list::back_current()
{
  if (pos == size) {
    current = tail;
    pos = size - 1;
    return;
  }
  if (current == nullptr) { return; }
  current = current->prev;
  --pos;
}
