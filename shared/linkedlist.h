#ifndef LINKEDLIST_H
#define LINKEDLIST_H

// This has a linked list of void*

struct LinkedListNode {
  struct LinkedListNode* prev;
  struct LinkedListNode* next;
  void* value;
};

// a doubly linked list
struct LinkedList {
  struct LinkedListNode* head;
  struct LinkedListNode* tail;
  int size;
};

void  LL_init(struct LinkedList* list);
void  LL_clear(struct LinkedList* list);
void* LL_head(struct LinkedList* list);
void* LL_tail(struct LinkedList* list);
void* LL_pop_head(struct LinkedList* list);
void* LL_pop_tail(struct LinkedList* list);
void  LL_push_head(struct LinkedList* list, void* value);
void  LL_push_tail(struct LinkedList* list, void* value);

// Haven't yet had a need for these:
//void  LL_insert(struct LinkedList* list, int idx, void* value);
//void* LL_at(struct LinkedList* list, int idx);
//void* LL_pop_at(struct LinkedList* list, int idx);

#endif // LINKEDLIST_H
