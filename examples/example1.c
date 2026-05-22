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
  struct registry reg;
  registry_init(&reg, sizeof(struct city), city_cmp, NULL);

  // i make buf here so i can dereference it later. when you add a value to a
  // registry, it copies the bytes of the value, it does NOT copy the pointer.
  // therefore, i can change this variable and the value in the registry will
  // remain the same

  struct city buf;

  // nyc
  buf.popl = 8600000; /* 8.6 million */
  buf.name = "new_york_city";
  registry_add(&reg, &buf);

  // los angeles
  buf.popl = 3900000; /* 3.9 million */
  buf.name = "los_angeles";
  registry_add(&reg, &buf);

  // seattle
  buf.popl = 800000; /* 800 thousand */
  buf.name = "seattle";
  registry_add(&reg, &buf);

  // london
  buf.popl = 9000000; /* 9 million */
  buf.name = "london";
  registry_add(&reg, &buf);

  // tokyo
  buf.popl = 14000000; /* 14 million */
  buf.name = "tokyo";
  registry_add(&reg, &buf);

  // now we will get the population for london
  // this function returns NULL if the key is invalid, but for simplicity we
  // will not check that here
  printf("Population of London: %i\n",
         ((struct city*)registry_ktov(&reg, &(struct city){.name = "london"}))
             ->popl);

  // output: Population of London: 9000000

  // if you have a registry where the values are structs with fields on the
  // heap, remember to make a cleanup function and pass it in the init function
  // that cleanup function would be called here.
  registry_cleanup(&reg);

  return 0;
}
