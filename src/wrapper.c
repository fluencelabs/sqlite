#include <stdlib.h>
#include "sqliteInt.h"

const char *RESULT_PTR;
int RESULT_SIZE;

const unsigned char __FCE_SDK_VERSION1[6] __attribute__((__section__(".custom_section.__fluence_sdk_version1"))) = "0.5.0";

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

int main() {
  // the main purpose of this empty main is to initialize WASi subsystem
  return 0;
}