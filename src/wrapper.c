#include <stdlib.h>
#include "sqliteInt.h"

sqlite3 *state;
const char *RESULT_PTR;
int RESULT_SIZE;

void* allocate(size_t size) {
  return malloc(size + 1);
}

void deallocate(void *ptr, int size) {
  free(ptr);
}

void set_result_ptr(const char *ptr) {
  RESULT_PTR = ptr;
}

void set_result_size(int size) {
  RESULT_SIZE = size;
}

int get_result_size(void) {
  return RESULT_SIZE;
}

const char *get_result_ptr() {
  return RESULT_PTR;
}

