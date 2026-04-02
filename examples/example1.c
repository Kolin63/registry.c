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
