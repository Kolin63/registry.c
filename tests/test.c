#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../src/registry.h"

static int tests_passed = 0;
static int tests_total = 0;

#define KEY_CHECK(reg, i, key)                                               \
  tests_total++;                                                             \
  if (strcmp(reg->keys[i], key) == 0) {                                      \
    printf("\e[0;92m%s:%i: Correct key: %s\e[0m\n", __FILE_NAME__, __LINE__, \
           key);                                                             \
    tests_passed++;                                                          \
  } else                                                                     \
    printf("\e[0;91m%s:%i: Incorrect key. Got: %s. Expected: %s\e[0m\n",     \
           __FILE_NAME__, __LINE__, reg->keys[i], key);

#define VALUE_CHECK(reg, i, val_type, val)                                  \
  tests_total++;                                                            \
  {                                                                         \
    const val_type val_buf = val;                                           \
    if (memcmp(&val_buf, reg->values + i * reg->val_size, reg->val_size) == \
        0) {                                                                \
      printf("\e[0;92m%s:%i: Correct value: %s\e[0m\n", __FILE_NAME__,      \
             __LINE__, #val);                                               \
      tests_passed++;                                                       \
    } else {                                                                \
      printf("\e[0;91m%s:%i: Incorrect value. Expected: %s\e[0m\n",         \
             __FILE_NAME__, __LINE__, #val);                                \
    }                                                                       \
  }

#define KEY_VALUE_CHECK(reg, i, key, val_type, val) \
  KEY_CHECK(reg, i, key);                           \
  VALUE_CHECK(reg, i, val_type, val);

void animal_test() {
  struct registry* reg = registry_init(sizeof(int));

  {
    char* key = "willy";
    int age = 6;
    registry_add(reg, key, &age);
  }

  KEY_VALUE_CHECK(reg, 0, "willy", int, 6);

  {
    char* key = "twilight";
    int age = 8;
    registry_add(reg, key, &age);
  }

  KEY_VALUE_CHECK(reg, 0, "twilight", int, 8);
  KEY_VALUE_CHECK(reg, 1, "willy", int, 6);

  {
    char* key = "apple_jack";
    int age = 28;
    registry_add(reg, key, &age);
    registry_add(reg, key, &age);
  }

  KEY_VALUE_CHECK(reg, 0, "apple_jack", int, 28);
  KEY_VALUE_CHECK(reg, 1, "twilight", int, 8);
  KEY_VALUE_CHECK(reg, 2, "willy", int, 6);

  {
    char* key = "puff";
    int age = 23;
    registry_add(reg, key, &age);
  }

  KEY_VALUE_CHECK(reg, 0, "apple_jack", int, 28);
  KEY_VALUE_CHECK(reg, 1, "puff", int, 23);
  KEY_VALUE_CHECK(reg, 2, "twilight", int, 8);
  KEY_VALUE_CHECK(reg, 3, "willy", int, 6);

  {
    char* key = "babe";
    int age = 10;
    registry_add(reg, key, &age);
  }

  for (size_t i = 0; i < reg->length; i++) {
    printf("animal_test #%zi, %s, age %i\n", i, reg->keys[i],
           *(int*)(registry_itov(reg, i)));
  }

  KEY_VALUE_CHECK(reg, 0, "apple_jack", int, 28);
  KEY_VALUE_CHECK(reg, 1, "babe", int, 10);
  KEY_VALUE_CHECK(reg, 2, "puff", int, 23);
  KEY_VALUE_CHECK(reg, 3, "twilight", int, 8);
  KEY_VALUE_CHECK(reg, 4, "willy", int, 6);

  registry_cleanup(reg);
}

int main() {
  animal_test();

  if (tests_passed == tests_total) {
    printf("\n\e[0;102m\e[1;30m\e[4;30mAll Tests Passed (%i/%i)\e[0m\n",
           tests_passed, tests_total);
    return 0;
  } else {
    printf("\n\e[0;101m\e[1;30m\e[4;30mFailed Tests (%i/%i)\e[0m\n",
           tests_passed, tests_total);
    return tests_total - tests_passed;
  }
}
