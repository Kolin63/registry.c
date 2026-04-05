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
  - [registry_cleanup()](#registry_cleanup)
  - [registry_add()](#registry_add)
  - [registry_itov()](#registry_itov)
  - [registry_itov_safe()](#registry_itov_safe)
  - [registry_itok()](#registry_itok)
  - [registry_itok_safe()](#registry_itok_safe)
  - [registry_ktoi()](#registry_ktoi)
  - [registry_ktov()](#registry_ktov)

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
   this example creates a registry where the key is a city and the value is the
   population
 */

#include <stdio.h>

#include "registry.h"

int main() {
  // the key is always a string. specify the type of the value here.
  // in a real scenario, a struct would be more useful. i am using an int here
  // for simplicity
  struct registry* reg = registry_init(sizeof(int));

  // i make an int variable here so i can dereference it later
  // when you add a value to a registry, it copies the bytes of the value, it
  // does NOT copy the pointer. therefore, i can change this variable and the
  // value in the registry will remain the same
  int buf;

  // nyc
  buf = 8600000; /* 8.6 million */
  registry_add(reg, "new_york_city", &buf);

  // los angeles
  buf = 3900000; /* 3.9 million */
  registry_add(reg, "los_angeles", &buf);

  // seattle
  buf = 800000; /* 800 thousand */
  registry_add(reg, "seattle", &buf);

  // london
  buf = 9000000; /* 9 million */
  registry_add(reg, "london", &buf);

  // tokyo
  buf = 14000000; /* 14 million */
  registry_add(reg, "tokyo", &buf);

  // now we will get the population for london
  // this function returns NULL if the key is invalid, but for simplicity we
  // will not check that here
  printf("Population of London: %i\n", *(int*)registry_ktov(reg, "london"));

  // output: Population of London: 9000000

  // if you have a registry where the values are structs with fields on the
  // heap, remember to free those fields before calling registry_cleanup()
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

  // array of keys
  char** keys;

  // value data. continguous in memory
  void* values;
};
```
an interface to a single registry. none of these fields should be manually
written to. to read from the keys or the values, either manually reading or
using the functions are ok

### registry_init()
```c
struct registry* registry_init(int val_size);
```
puts a new registry on the heap. `registry_cleanup()` must be called when it is
done being used

### registry_cleanup()
```c
void registry_cleanup(struct registry* reg);
```
frees allocated memory for a registry. if the registry contains structs with
data on the heap, those fields must be freed before calling this function

### registry_add()
```c
int registry_add(struct registry* reg, const char* key, const void* val);
```
adds a key and a value. returns -1 if the key already exists

### registry_itov()
```c
void* registry_itov(struct registry* reg, int i);
```
index to value. no bounds checking

### registry_itov_safe()
```c
void* registry_itov_safe(struct registry* reg, int i);
```
index to value. returns NULL on error

### registry_itok()
```c
const char* registry_itok(struct registry* reg, int i);
```
index to key. no bounds checking

### registry_itok_safe()
```c
const char* registry_itok_safe(struct registry* reg, int i);
```
index to key. returns NULL on error

### registry_ktoi()
```c
int registry_ktoi(struct registry* reg, const char* key);
```
key to index. returns -1 if the key doesn't exist

### registry_ktov()
```c
void* registry_ktov(struct registry* reg, const char* key);
```
key to value. returns NULL on error
