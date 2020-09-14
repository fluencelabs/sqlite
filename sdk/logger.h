#ifndef FLUENCE_C_SDK_LOGGER_H
#define FLUENCE_C_SDK_LOGGER_H

#define __LOGGER_IMPORT(name) \
    __attribute__((__import_module__("host"), __import_name__(#name)))

/**
 * Writes provided utf8 string to Wasm VM logger.
 *
 * @param str a pointer to the string that should be logged.
 * @param len a size of the string that should be logged.
 */
void log_utf8_string(const char *str, int len) __LOGGER_IMPORT(log_utf8_string);

#endif // FLUENCE_C_SDK_LOGGER_H
