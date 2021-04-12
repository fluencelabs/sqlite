#include "sqliteInt.h"
#include "../cvector/cvector.h"

#include <stdlib.h>

void *RESULT_PTR;
int RESULT_SIZE;

cvector_vector_type(void *) OBJECTS_TO_RELEASE;

void* allocate(size_t size) {
  // this +1 is needed to append then zero byte to strings passing to this module.
  return malloc(size + 1);
}

void release_objects() {
  const unsigned int objects_count = cvector_size(OBJECTS_TO_RELEASE);
  for (unsigned int obj_id = objects_count; obj_id > 0; --obj_id) {
    void *object = OBJECTS_TO_RELEASE[obj_id-1];
    free(object);

    cvector_pop_back(OBJECTS_TO_RELEASE);
  }
}

void add_object_to_release(void *object) {
  cvector_push_back(OBJECTS_TO_RELEASE, object);
}

void set_result_ptr(void *ptr) {
  RESULT_PTR = ptr;
}

void set_result_size(int size) {
  RESULT_SIZE = size;
}

int get_result_size(void) {
  return RESULT_SIZE;
}

void *get_result_ptr() {
  return RESULT_PTR;
}

int main() {
  // the main purpose of this empty main is to initialize WASI subsystem
  return 0;
}
