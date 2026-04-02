/*
   registry.h - part of the registry.c library by Colin Melican (Kolin63)

   https://github.com/kolin63/registry.c

   Under the MIT License

   Copyright (c) 2026 Colin Melican

   Permission is hereby granted, free of charge, to any person obtaining a copy
   of this software and associated documentation files (the "Software"), to deal
   in the Software without restriction, including without limitation the rights
   to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
   copies of the Software, and to permit persons to whom the Software is
   furnished to do so, subject to the following conditions:

   The above copyright notice and this permission notice shall be included in
   all copies or substantial portions of the Software.

   THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
   IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
   FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
   AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
   LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
   OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
   SOFTWARE.
 */

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

// index to value. no bounds checking
void* registry_itov(struct registry* reg, int i);

// index to value. returns NULL on error
void* registry_itov_safe(struct registry* reg, int i);

// index to key. no bounds checking
char* registry_itok(struct registry* reg, int i);

// index to key. returns NULL on error
char* registry_itok_safe(struct registry* reg, int i);

// key to index. returns -1 if the key doesn't exist
int registry_ktoi(struct registry* reg, const char* key);

// key to value. returns NULL on error
void* registry_ktov(struct registry* reg, const char* key);

#endif
