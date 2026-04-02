#ifndef COLIN_REGISTRY_H_
#define COLIN_REGISTRY_H_

struct registry {
  int length;
  int val_size;
  char** keys;
  void* values;
};

struct registry* registry_init(int val_size);
void registry_cleanup(struct registry* reg);

void registry_add(struct registry* reg, const char* key, const void* val);

// index to value. no bounds checking
void* registry_itov(struct registry* reg, int i);

// index to value. returns NULL on error
void* registry_itov_safe(struct registry* reg, int i);

// key to index. returns -1 if the key doesn't exist
int registry_ktoi(struct registry* reg, const char* key);

// key to value. returns NULL on error
void* registry_ktov(struct registry* reg, const char* key);

#endif
