#include "registry.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct registry* registry_init(int val_size) {
  struct registry* reg = malloc(sizeof(struct registry));
  reg->length = 0;
  reg->val_size = val_size;
  reg->keys = NULL;
  reg->values = NULL;
  return reg;
}

void registry_cleanup(struct registry* reg) {
  for (size_t i = 0; i < reg->length; i++) {
    free(reg->keys[i]);
  }
  free(reg->keys);
  free(reg->values);
  free(reg);
}

void registry_add(struct registry* reg, const char* key, const void* val) {
  // do a binary search to find insertion index
  // 0 2 4 6 8
  //       ^
  //       5 (insert at index 3)
  int insert_index = 0;
  {
    int left = 0;
    int right = reg->length - 1;
    while (left <= right) {
      int mid = left + (right - left) / 2;
      if (strcmp(reg->keys[mid], key) < 0) {
        left = mid + 1;
      } else if (strcmp(reg->keys[mid], key) > 0) {
        right = mid - 1;
      } else {
        fprintf(stderr, "Key already exists in registry: %s\n", key);
        return;
      }
    }
    insert_index = left + (right - left) / 2;
  }

  // move keys and values to make room
  // 0 2 4 _ 6 8
  //       ^
  //       5

  // first keys
  {
    reg->keys = realloc(reg->keys, (reg->length + 1) * sizeof(char*));
    char** src = reg->keys + insert_index;
    char** dest = src + 1;
    const size_t n = (reg->length - insert_index) * sizeof(char*);
    memmove(dest, src, n);
  }

  // then values
  {
    reg->values = realloc(reg->values, (reg->length + 1) * reg->val_size);
    void* src = reg->values + insert_index * reg->val_size;
    void* dest = src + reg->val_size;
    const size_t n = (reg->length - insert_index) * reg->val_size;
    memmove(dest, src, n);
  }

  // move in new key
  {
    char* new_key = malloc(strlen(key) + 1);
    strcpy(new_key, key);
    *(reg->keys + insert_index) = new_key;
  }

  // move in new value
  {
    const void* src = val;
    void* dest = reg->values + insert_index * reg->val_size;
    const size_t n = reg->val_size;
    memcpy(dest, src, n);
  }

  reg->length++;
}

// get value from index
void* registry_itov(struct registry* reg, int i) {
  return reg->values + i * reg->val_size;
}
