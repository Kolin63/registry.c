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

void* registry_get_by_index(struct registry* reg, int i);

#endif
