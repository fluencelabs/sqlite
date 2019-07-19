#include "logger.h"

#define __LOGGER_IMPORT(name) \
    __attribute__((__import_module__("logger"), __import_name__(#name)))

void __write(char ch) __LOGGER_IMPORT(write);
void __flush() __LOGGER_IMPORT(flush);

void wasm_log(const char *str, int len) {
    for(int byteId = 0; byteId < len; ++byteId) {
        __write(str[byteId]);
    }

    __flush();
}
