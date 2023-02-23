#include "sqliteInt.h"
#include "../cvector/cvector.h"

#include <stdlib.h>

void *RESULT_PTR;
int RESULT_SIZE;

cvector_vector_type(void *) OBJECTS_TO_RELEASE;

void* allocate(size_t size, size_t _type_tag) {
  if (size == 0 || size + 1 == 0) {
    return 0;
  }

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

char *handle_input_string(char *str, int len) {
  if (len == 0) {
    free(str);
    return NULL;
  }

  // this strings are supposed to be allocated by the allocate function, which allocates 1 byte more for null character
  str[len] = '\x00';
  return str;
}

void write_le_int(unsigned char *array, unsigned int offset, unsigned int value) {
  array[offset] = value & 0xff;
  array[offset + 1] = (value >> 8) & 0xff;
  array[offset + 2] = (value >> 16) & 0xff;
  array[offset + 3] = (value >> 24) & 0xff;
}

extern void __wasm_call_ctors();

int main() {
  __wasm_call_ctors(); // for more details see https://github.com/WebAssembly/WASI/issues/471

  return 0;
}
