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
  return registry_strcmp(aa->name, ab->name);
}

void key_check(const struct registry* reg, int i, const struct animal* key,
               const char* file, int line) {
  tests_total++;
  if (registry_safe_cmp(reg, reg->vals + i * reg->val_size, key) == 0) {
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

void equals_check_not_null(const void* x, const char* file, int line) {
  {
    tests_total++;
    if (x != NULL) {
      printf("\e[0;92m%s:%i: Success: %p != NULL\e[0m\n", file, line, x);
      tests_passed++;
    } else {
      printf("\e[0;91m%s:%i: Failure: %p == NULL\e[0m\n", file, line, x);
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
  equals_check_int(registry_vtoi(reg, registry_itov(reg, i)), i, file, line);
}

void total_check(const struct registry* reg, int i, const struct animal* key,
                 const struct animal* val, const char* file, int line) {
  key_value_check(reg, i, key, val, file, line);
  getter_check(reg, i, key, val, file, line);
}

void animal_test() {
  struct registry reg;
  registry_init(&reg, sizeof(struct animal), animal_cmp, NULL);

  struct animal willy = {.name = "willy", .age = 6};
  struct animal twilight = {.name = "twilight", .age = 8};
  struct animal apple_jack = {.name = "apple_jack", .age = 28};
  struct animal puff = {.name = "puff", .age = 23};
  struct animal babe = {.name = "babe", .age = 10};

  equals_check_not_null(registry_add(&reg, &willy), __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_add(&reg, &willy), NULL, __FILE_NAME__,
                        __LINE__);

  total_check(&reg, 0, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);
  total_check(&reg, 0, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_not_null(registry_add(&reg, &twilight), __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_add(&reg, &twilight), NULL, __FILE_NAME__,
                        __LINE__);

  total_check(&reg, 0, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 1, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_not_null(registry_add(&reg, &apple_jack), __FILE_NAME__,
                        __LINE__);
  equals_check_void_ptr(registry_add(&reg, &apple_jack), NULL, __FILE_NAME__,
                        __LINE__);

  total_check(&reg, 0, &(struct animal){.name = "apple_jack"}, &apple_jack,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 1, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 2, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_not_null(registry_add(&reg, &puff), __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_add(&reg, &puff), NULL, __FILE_NAME__,
                        __LINE__);

  total_check(&reg, 0, &(struct animal){.name = "apple_jack"}, &apple_jack,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 1, &(struct animal){.name = "puff"}, &puff, __FILE_NAME__,
              __LINE__);
  total_check(&reg, 2, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 3, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_not_null(registry_add(&reg, &babe), __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_add(&reg, &babe), NULL, __FILE_NAME__,
                        __LINE__);

  for (size_t i = 0; i < reg.length; i++) {
    struct animal* animal = registry_itov(&reg, i);
    printf("animal_test #%zi, %s, age %i\n", i, animal->name, animal->age);
  }

  total_check(&reg, 0, &(struct animal){.name = "apple_jack"}, &apple_jack,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 1, &(struct animal){.name = "babe"}, &babe, __FILE_NAME__,
              __LINE__);
  total_check(&reg, 2, &(struct animal){.name = "puff"}, &puff, __FILE_NAME__,
              __LINE__);
  total_check(&reg, 3, &(struct animal){.name = "twilight"}, &twilight,
              __FILE_NAME__, __LINE__);
  total_check(&reg, 4, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  equals_check_void_ptr(registry_itov_safe(&reg, -1), NULL, __FILE_NAME__,
                        __LINE__);
  equals_check_int(registry_ktoi(&reg, &(struct animal){.name = "foobar"}), -1,
                   __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_ktov(&reg, &(struct animal){.name = "foobar"}),
                        NULL, __FILE_NAME__, __LINE__);

  registry_clear(&reg);
  equals_check_void_ptr(registry_itov_safe(&reg, 0), NULL, __FILE_NAME__,
                        __LINE__);

  equals_check_not_null(registry_add(&reg, &willy), __FILE_NAME__, __LINE__);
  equals_check_void_ptr(registry_add(&reg, &willy), NULL, __FILE_NAME__,
                        __LINE__);

  total_check(&reg, 0, &(struct animal){.name = "willy"}, &willy, __FILE_NAME__,
              __LINE__);

  for (size_t i = 0; i < reg.length; i++) {
    struct animal* animal = registry_itov(&reg, i);
    printf("animal_test #%zi, %s, age %i\n", i, animal->name, animal->age);
  }

  registry_cleanup(&reg);
}

struct thing {
  char* foo;
};

int thing_cmp(const void* a, const void* b) {
  return registry_strcmp(((struct thing*)a)->foo, ((struct thing*)b)->foo);
}

void thing_cleanup(void* elem) {
  struct thing* thing = elem;
  free(thing->foo);
}

void thing_test() {
  struct registry reg;
  registry_init(&reg, sizeof(struct thing), thing_cmp, thing_cleanup);

  const char* thing1_const_text = "skibidi toilet";
  char* thing1_text = malloc(strlen(thing1_const_text) + 1);
  strcpy(thing1_text, thing1_const_text);
  struct thing thing1 = {.foo = thing1_text};
  struct thing* thing1_reg = registry_add(&reg, &thing1);
  equals_check_int(thing_cmp(&thing1, thing1_reg), 0, __FILE_NAME__, __LINE__);

  const char* thing2_const_text = "bop bop";
  char* thing2_text = malloc(strlen(thing2_const_text) + 1);
  strcpy(thing2_text, thing2_const_text);
  struct thing thing2 = {.foo = thing2_text};
  struct thing* thing2_reg = registry_add(&reg, &thing2);
  equals_check_int(thing_cmp(&thing2, thing2_reg), 0, __FILE_NAME__, __LINE__);

  const char* thing3_const_text = "yes yes";
  char* thing3_text = malloc(strlen(thing3_const_text) + 1);
  strcpy(thing3_text, thing3_const_text);
  struct thing thing3 = {.foo = thing3_text};
  struct thing* thing3_reg = registry_add(&reg, &thing3);
  equals_check_int(thing_cmp(&thing3, thing3_reg), 0, __FILE_NAME__, __LINE__);

  equals_check_void_ptr(thing1.foo, thing1_text, __FILE_NAME__, __LINE__);
  equals_check_void_ptr(thing2.foo, thing2_text, __FILE_NAME__, __LINE__);
  equals_check_void_ptr(thing3.foo, thing3_text, __FILE_NAME__, __LINE__);

  registry_cleanup(&reg);
}

int main() {
  animal_test();
  thing_test();

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
