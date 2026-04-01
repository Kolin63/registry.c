#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/registry.h"

#define KEY_CHECK(reg, i, key) \
  if (strcmp(reg->keys[i], key) != 0) assert(key && false);

struct animal {
  int age;
  char* name;
  char* noise;
};

void animal_test() {
  struct registry* reg = registry_init(sizeof(struct animal));

  {
    char* key = malloc(6);
    strcpy(key, "willy");
    char* name = malloc(6);
    strcpy(name, "Willy");
    char* noise = malloc(6);
    strcpy(noise, "Woof!");

    struct animal x = {.age = 6, .name = name, .noise = noise};
    registry_add(reg, key, &x);
  }

  {
    char* key = malloc(9);
    strcpy(key, "twilight");
    char* name = malloc(9);
    strcpy(name, "Twilight");
    char* noise = malloc(6);
    strcpy(noise, "Meow!");

    struct animal x = {.age = 8, .name = name, .noise = noise};
    registry_add(reg, key, &x);
  }

  {
    char* key = malloc(11);
    strcpy(key, "apple_jack");
    char* name = malloc(11);
    strcpy(name, "Apple Jack");
    char* noise = malloc(6);
    strcpy(noise, "Neigh");

    struct animal x = {.age = 28, .name = name, .noise = noise};
    registry_add(reg, key, &x);
    registry_add(reg, key, &x);
  }

  {
    char* key = malloc(5);
    strcpy(key, "puff");
    char* name = malloc(5);
    strcpy(name, "Puff");
    char* noise = malloc(5);
    strcpy(noise, "Roar");

    struct animal x = {.age = 20, .name = name, .noise = noise};
    registry_add(reg, key, &x);
  }

  {
    char* key = malloc(5);
    strcpy(key, "babe");
    char* name = malloc(5);
    strcpy(name, "Babe");
    char* noise = malloc(5);
    strcpy(noise, "Oink");

    struct animal x = {.age = 10, .name = name, .noise = noise};
    registry_add(reg, key, &x);
  }

  for (size_t i = 0; i < reg->length; i++) {
    const struct animal* elem =
        (struct animal*)((char*)reg->values + i * reg->val_size);
    printf(
        "animal test #%zi:'%s':\n"
        "  age: %i\n"
        "  name: %s\n"
        "  noise: %s\n",
        i, reg->keys[i], elem->age, elem->name, elem->noise);
  }

  KEY_CHECK(reg, 0, "apple_jack");
  KEY_CHECK(reg, 1, "babe");
  KEY_CHECK(reg, 2, "puff");
  KEY_CHECK(reg, 3, "twilight");
  KEY_CHECK(reg, 4, "willy");
}

int main() { animal_test(); }
