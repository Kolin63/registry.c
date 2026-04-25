# registry.c
A 2-file C-23 library that implements a simple registry (a.k.a. hashmap). It is
designed for moddable or data-driven games where the entries could be items,
monsters, etc.

**Contents**
- [Usage](#usage)
- [Examples](#examples)
- [Documentation](#documentation)
  - [struct registry](#struct-registry)
  - [registry_init()](#registry_init)
  - [registry_clear()](#registry_clear)
  - [registry_safe_cmp()](#registry_safe_cmp)
  - [registry_add()](#registry_add)
  - [registry_cleanup()](#registry_cleanup)
  - [registry_itov()](#registry_itov)
  - [registry_itov_safe()](#registry_itov_safe)
  - [registry_ktoi()](#registry_ktoi)
  - [registry_ktov()](#registry_ktov)
  - [registry_strcmp()](#registry_strcmp)

## Usage
You can add `src/registry.c` and `src/registry.h` to your project (this code is
under the MIT License).

Alternatively, you can build the library from source using CMake. Then, put
src/registry.h in an include directory and `libcolinregistry.a` in a lib
directory.

## Examples
This can also be found in the examples folder.
```c
/*
   this example creates a registry of cities. it stores the city name and its
   population
 */

#include <stdio.h>

#include "registry.h"

struct city {
  int popl;
  const char* name;
};

// we need to provide a comparison function so the registry knows how to
// interpret the data. its passed in registry_init
int city_cmp(const void* a, const void* b) {
  const struct city* ca = a;
  const struct city* cb = b;
  // we could use the regular strcmp here, but the custom implementation is
  // faster for our use
  return registry_strcmp(ca->name, cb->name);
}

int main() {
  // initialize registry
  // the last argument passed (the NULL) is a pointer to a cleanup function.
  // we do not need a cleanup function because this example is simple
  struct registry* reg = registry_init(sizeof(struct city), city_cmp, NULL);

  // i make buf here so i can dereference it later. when you add a value to a
  // registry, it copies the bytes of the value, it does NOT copy the pointer.
  // therefore, i can change this variable and the value in the registry will
  // remain the same

  struct city buf;

  // nyc
  buf.popl = 8600000; /* 8.6 million */
  buf.name = "new_york_city";
  registry_add(reg, &buf);

  // los angeles
  buf.popl = 3900000; /* 3.9 million */
  buf.name = "los_angeles";
  registry_add(reg, &buf);

  // seattle
  buf.popl = 800000; /* 800 thousand */
  buf.name = "seattle";
  registry_add(reg, &buf);

  // london
  buf.popl = 9000000; /* 9 million */
  buf.name = "london";
  registry_add(reg, &buf);

  // tokyo
  buf.popl = 14000000; /* 14 million */
  buf.name = "tokyo";
  registry_add(reg, &buf);

  // now we will get the population for london
  // this function returns NULL if the key is invalid, but for simplicity we
  // will not check that here
  printf("Population of London: %i\n",
         ((struct city*)registry_ktov(reg, &(struct city){.name = "london"}))
             ->popl);

  // output: Population of London: 9000000

  // if you have a registry where the values are structs with fields on the
  // heap, remember to make a cleanup function and pass it in the init function
  // that cleanup function would be called here.
  registry_cleanup(reg);

  return 0;
}
```

## Documentation
### struct registry
```c
struct registry {
  // amount of keys / values
  int length;

  // size of value type in bytes
  int val_size;

  // comparison function for data type. does not need to check if values are
  // null. should return:
  // * 0 if a == b
  // * a negative value if a < b
  // * a positive value if a > b
  int (*cmp)(const void* a, const void* b);

  // cleanup function that will be called whenever a value is removed from the
  // registry. the argument elem is a pointer to the element that will be freed
  // this should be used if, for example, there is a struct that has a pointer
  // to something on the heap. if this is NULL, it will not be called
  void (*cleanup)(void* elem);

  // value data. continguous in memory
  void* vals;
};
```
an interface to a single registry. none of these fields should be manually
written to.

### registry_init()
```c
struct registry* registry_init(int val_size, int (*cmp)(const void*, const void*));
```
puts a new registry on the heap. `registry_cleanup()` must be called when it is
done being used

### registry_cleanup()
```c
void registry_cleanup(struct registry* reg);
```
frees allocated memory for a registry. if the registry contains structs with
data on the heap, those fields must be freed before calling this function

### registry_safe_cmp()
```c
int registry_safe_cmp(const struct registry* reg, const void* a, const void* b);
```
calls the registry's cmp function. elides function call if either a or b are
NULL. will segfault if the cmp function is unset.

### registry_add()
```c
void* registry_add(struct registry* reg, const void* val);
```
adds a value. returns pointer to value in registry, or NULL if key already
exists

### registry_clear()
```c
void registry_clear(struct registry* reg);
```
removes all entries from registry. does not call registry_cleanup(). does not
need to be called before calling registry_cleanup()

### registry_itov()
```c
void* registry_itov(const struct registry* reg, int i);
```
index to value. no bounds checking

### registry_itov_safe()
```c
void* registry_itov_safe(const struct registry* reg, int i);
```
index to value. returns NULL on error

### registry_ktoi()
```c
int registry_ktoi(const struct registry* reg, const void* key);
```
key to index. returns -1 if the key doesn't exist

### registry_ktov()
```c
void* registry_ktov(const struct registry* reg, const void* key);
```
key to value. returns NULL on error

### registry_strcmp()
```c
int registry_strcmp(const char* a, const char* b);
```
fast implementation of strcmp. only return values are 1, 0, or -1
