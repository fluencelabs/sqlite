#include <stdlib.h>
#include "sqliteInt.h"

const char *RESULT_PTR;
int RESULT_SIZE;

void* OBJECTS_TO_RELEASE[];
unsigned int OBJECTS_TO_RELEASE_COUNT;

void* allocate(size_t size) {
  return malloc(size + 1);
}

void release_objects() {
  for(unsigned int i = 0; i < OBJECTS_TO_RELEASE_COUNT; ++i) {
    free(OBJECTS_TO_RELEASE[i]);
  }

  OBJECTS_TO_RELEASE_COUNT = 0;
}

void add_object_to_release(void *) {

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

int main() {
  // the main purpose of this empty main is to initialize WASi subsystem
  return 0;
}