#include <registry.h>

int main() {
  const char* x = "hello, world!";
  const char* y = "foobarbaz";

  int cmp;
  for (unsigned int i = 0; i < 0xFFFFFFFF; i++) {
    cmp = registry_strcmp(x, y);
  }

  return 0;
}
