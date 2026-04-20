#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "registry.h"

static int tests_passed = 0;
static int tests_total = 0;

struct animal {
  char* name;
  int age;
};

int animal_cmp(const void* a, const void* b) {
  const struct animal* aa = a;
  const struct animal* ab = b;
  return strcmp(aa->name, ab->name);
}

void key_check(const struct registry* reg, int i, const struct animal* key,
               const char* file, int line) {
  tests_total++;
  if (registry_safe_cmp(reg, reg->values + i * reg->val_size, key) == 0) {
    printf("\e[0;92m%s:%i: Correct key\e[0m\n", file, line);
    tests_passed++;
  } else
    printf("\e[0;91m%s:%i: Incorrect key.\e[0m\n", file, line);
}

void value_check(const struct registry* reg, const struct animal* a,
                 const struct animal* b, const char* file, int line) {
  tests_total++;
  if (registry_safe_cmp(reg, a, b) == 0 && a->age == b->age) {
    printf("\e[0;92m%s:%i: Correct value\e[0m\n", file, line);
    tests_passed++;
  } else {
    printf("\e[0;91m%s:%i: Incorrect value\e[0m\n", file, line);
  }
}

void value_check_i(const struct registry* reg, int i, const struct animal* val,
                   const char* file, int line) {
  value_check(reg, val, registry_itov_safe(reg, i), file, line);
}

void key_value_check(const struct registry* reg, int i,
                     const struct animal* key, const struct animal* val,
                     const char* file, int line) {
  key_check(reg, i, key, file, line);
  value_check_i(reg, i, val, file, line);
}

void equals_check_int(int x, int y, const char* file, int line) {
  tests_total++;
  if (x == y) {
    printf("\e[0;92m%s:%i: Equality: %i == %i\e[0m\n", file, line, x, y);
    tests_passed++;
  } else {
    printf("\e[0;91m%s:%i: Inequality: %i != %i\e[0m\n", file, line, x, y);
  }
}

void equals_check_void_ptr(const void* x, const void* y, const char* file,
                           int line) {
  {
    tests_total++;
    if (x == y) {
      printf("\e[0;92m%s:%i: Equality: %p == %p\e[0m\n", file, line, x, y);
      tests_passed++;
    } else {
      printf("\e[0;91m%s:%i: Inequality: %p != %p\e[0m\n", file, line, x, y);
    }
  }
}

void equals_check_string(const char* x, const char* y, const char* file,
                         int line) {
  tests_total++;
  if (strcmp(x, y) == 0) {
    printf("\e[0;92m%s:%i: Equality: %s == %s\e[0m\n", file, line, x, y);
    tests_passed++;
  } else {
    printf("\e[0;91m%s:%i: Inequality: %s != %s\e[0m\n", file, line, x, y);
  }
}

void getter_check(const struct registry* reg, int i, const struct animal* key,
                  const struct animal* val, const char* file, int line) {
  value_check(reg, (const struct animal*)registry_itov(reg, i), val, file,
              line);
  value_check(reg, (const struct animal*)registry_itov_safe(reg, i), val, file,
              line);
  equals_check_int(registry_ktoi(reg, key), i, file, line);
  value_check(reg, (const struct animal*)registry_ktov(reg, key), val, file,
              line);
}

void total_check(const struct registry* reg, int i, const struct animal* key,
                 const struct animal* val, const char* file, int line) {
  key_value_check(reg, i, key, val, file, line);
  getter_check(reg, i, key, val, file, line);
}

void animal_test() {
  struct registry* reg = registry_init(sizeof(struct animal), animal_cmp);

  struct animal willy = {.name = "willy", .age = 6};
  struct animal twilight = {.name = "twilight", .age = 8};
  struct animal apple_jack = {.name = "apple_jack", .age = 28};
  struct animal puff = {.name = "puff", .age = 23};
  struct animal babe = {.name = "babe", .age = 10};

  equals_check_int(registry_add(reg, &willy), 0, __FILE_NAME__, __LINE__);
  equals_check_int(registry_add(reg, &willy), -1, __FILE_NAME__, __LINE__);

  total_check(reg, 0, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);
  total_check(reg, 0, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_int(registry_add(reg, &twilight), 0, __FILE_NAME__, __LINE__);
  equals_check_int(registry_add(reg, &twilight), -1, __FILE_NAME__, __LINE__);

  total_check(reg, 0, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(reg, 1, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_int(registry_add(reg, &apple_jack), 0, __FILE_NAME__, __LINE__);
  equals_check_int(registry_add(reg, &apple_jack), -1, __FILE_NAME__, __LINE__);

  total_check(reg, 0, &(struct animal){.name = "apple_jack"}, &apple_jack,
              __FILE_NAME__, __LINE__);
  total_check(reg, 1, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(reg, 2, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_int(registry_add(reg, &puff), 0, __FILE_NAME__, __LINE__);
  equals_check_int(registry_add(reg, &puff), -1, __FILE_NAME__, __LINE__);

  total_check(reg, 0, &(struct animal){.name = "apple_jack"}, &apple_jack,
              __FILE_NAME__, __LINE__);
  total_check(reg, 1, &(struct animal){.name = "puff"}, &puff, __FILE_NAME__,
              __LINE__);
  total_check(reg, 2, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(reg, 3, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_int(registry_add(reg, &babe), 0, __FILE_NAME__, __LINE__);
  equals_check_int(registry_add(reg, &babe), -1, __FILE_NAME__, __LINE__);

  for (size_t i = 0; i < reg->length; i++) {
    struct animal* animal = registry_itov(reg, i);
    printf("animal_test #%zi, %s, age %i\n", i, animal->name, animal->age);
  }

  total_check(reg, 0, &(struct animal){.name = "apple_jack"}, &apple_jack,
              __FILE_NAME__, __LINE__);
  total_check(reg, 1, &(struct animal){.name = "babe"}, &babe, __FILE_NAME__,
              __LINE__);
  total_check(reg, 2, &(struct animal){.name = "puff"}, &puff, __FILE_NAME__,
              __LINE__);
  total_check(reg, 3, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(reg, 4, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_void_ptr(registry_itov_safe(reg, -1), NULL, __FILE_NAME__,
                        __LINE__);
  equals_check_int(registry_ktoi(reg, &(struct animal){.name = "foobar"}), -1,
                   __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_ktov(reg, &(struct animal){.name = "foobar"}),
                        NULL, __FILE_NAME__, __LINE__);

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
