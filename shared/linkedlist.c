#include "linkedlist.h"

#include <stdlib.h>  // for free() and malloc()

void  LL_init(struct LinkedList* list) {
  list->head = NULL;
  list->tail = NULL;
  list->size = 0;
}

void  LL_clear(struct LinkedList* list) {
  while (list->head != NULL) {
    LL_pop_head(list);
  }
}

void* LL_head(struct LinkedList* list) {
  void* value = NULL;
  if (list->head != NULL) {
    value = list->head->value;
  }
  return value;
}

void* LL_tail(struct LinkedList* list) {
  void* value = NULL;
  if (list->tail != NULL) {
    value = list->tail->value;
  }
  return value;
}

void* LL_pop_head(struct LinkedList* list) {
  void* value = NULL;
  if (list->head != NULL) {
    value = list->head->value;
    struct LinkedListNode* head = list->head;
    list->head = list->head->next;
    if (list->head != NULL) {
      list->head->prev = NULL;
    }
    list->size--;
    free(head);
    if (list->size == 0) {
      LL_init(list);
    }
  }
  return value;
}

void* LL_pop_tail(struct LinkedList* list) {
  void* value = NULL;
  if (list->tail != NULL) {
    value = list->tail->value;
    struct LinkedListNode* tail = list->tail;
    list->tail = list->tail->prev;
    if (list->tail != NULL) {
      list->tail->next = NULL;
    }
    list->size--;
    free(tail);
    if (list->size == 0) {
      LL_init(list);
    }
  }
  return value;
}

void  LL_push_head(struct LinkedList* list, void* value) {
  struct LinkedListNode* node =
      (struct LinkedListNode*) malloc(sizeof(struct LinkedListNode));
  node->prev = NULL;
  node->next = list->head;
  node->value = value;
  if (list->head != NULL) {
    list->head->prev = node;
  } else {
    list->tail = node;
  }
  list->head = node;
  list->size++;
}

void  LL_push_tail(struct LinkedList* list, void* value) {
  struct LinkedListNode* node =
      (struct LinkedListNode*) malloc(sizeof(struct LinkedListNode));
  node->prev = list->tail;
  node->next = NULL;
  node->value = value;
  if (list->tail != NULL) {
    list->tail->next = node;
  } else {
    list->head = node;
  }
  list->tail = node;
  list->size++;
}

