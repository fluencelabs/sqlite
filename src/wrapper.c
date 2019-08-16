#include <stdlib.h>
#include "../sdk/logger.h"
#include "sqliteInt.h"

sqlite3 *state;

int init() {
  const int rc = sqlite3_initialize();
  if(rc != 0) {
    return rc;
  }

  return sqlite3_open(":memory:", &state);
}

int g_isInited = 0;

/**
 * Stores one byte by a given address in the module memory.
 *
 * @param ptr a address where byte should be stored
 * @param value a byte to be stored
 */
void store(char *ptr, unsigned char byte) {
  *ptr = byte;
}

/**
 * Returns one byte by a given address in the module memory.
 *
 * @param ptr a address at which the needed byte is located
 * @return the byte at the given address
 */
 unsigned char load(const unsigned char *ptr) {
  return *ptr;
}

/**
 * Allocates a memory region of a given size.
 * Could be used by Wasm execution environments for byte array passing.
 *
 * @param size a size of allocated memory region
 * @return a pointer to the allocated memory region
 */
 void* allocate(size_t size) {
  return malloc(size + 1);
}

/**
 * Frees a memory region.
 * Could be used by Wasm execution environments for freeing previous memory allocated by `allocate` function.
 *
 * @param ptr a pointer to the previously allocated memory region
 * @param size a size of the previously allocated memory region
 */
 void deallocate(void *ptr, int size) {
  free(ptr);
}

char *write_response(char *response, int response_size) {
  char *result_response = allocate(response_size + 4);

  for(int i = 0; i < 4; ++i) {
    result_response[i] = (response_size >> 8*i) & 0xFF;
  }

  memcpy(result_response + 4, response, response_size);
  return result_response;
}

typedef struct ShellText ShellText;
struct ShellText {
    char *z;
    int n;
    int nAlloc;
};

static void initText(ShellText *p){
  memset(p, 0, sizeof(*p));
}

static void freeText(ShellText *p){
  free(p->z);
  initText(p);
}

static int strlen30(const char *z){
  const char *z2 = z;
  while( *z2 ){ z2++; }
  return 0x3fffffff & (int)(z2 - z);
}

static void appendText(ShellText *p, char const *zAppend, char quote){
  int len;
  int i;
  int nAppend = strlen30(zAppend);

  len = nAppend+p->n+1;
  if( quote ){
    len += 2;
    for(i=0; i<nAppend; i++){
      if( zAppend[i]==quote ) len++;
    }
  }

  if( p->n+len>=p->nAlloc ){
    p->nAlloc = p->nAlloc*2 + len + 20;
    p->z = realloc(p->z, p->nAlloc);
    // TODO: more solid work with OOM
    if( p->z==0 ) __builtin_unreachable();
  }

  if( quote ){
    char *zCsr = p->z+p->n;
    *zCsr++ = quote;
    for(i=0; i<nAppend; i++){
      *zCsr++ = zAppend[i];
      if( zAppend[i]==quote ) *zCsr++ = quote;
    }
    *zCsr++ = quote;
    p->n = (int)(zCsr - p->z);
    *zCsr = '\0';
  }else{
    memcpy(p->z+p->n, zAppend, nAppend);
    p->n += nAppend;
    p->z[p->n] = '\0';
  }
}

static int captureOutputCallback(void *pArg, int nArg, char **azArg, char **az){
  ShellText *p = (ShellText*)pArg;
  int i;
  UNUSED_PARAMETER(az);
  if( azArg==0 ) return 0;
  if( p->n ) appendText(p, "|", 0);
  for(i=0; i<nArg; i++){
    if( i ) appendText(p, ",", 0);
    if( azArg[i] ) appendText(p, azArg[i], 0);
  }
  return 0;
}

/**
 * Executes given SQL request and returns result in as a pointer to the following structure:
 * | result size (4 bytes, le)| result (size bytes) |.
 *
 * @param sql a pointer to the supplied sql request
 * @param length a size of the supplied sql request
 * @return a pointer to the struct contains result_size and result
 */
 const char *invoke(char *request, int request_size) {
  if(g_isInited == 0) {
    // TODO: check the return code
    init();

#ifdef ENABLE_LOG
    const char successInitMessage[] = "Sqlite has been initialized\n";
    wasm_log(successInitMessage, sizeof(successInitMessage));
#endif

    g_isInited = 1;
  }

  request[request_size] = '\n';

#ifdef ENABLE_LOG
  wasm_log(request, request_size);
#endif
  request[request_size] = 0;

  ShellText str;
  initText(&str);
  char *errorMessage = 0;

  int rc = sqlite3_exec(state, request, captureOutputCallback, &str, &errorMessage);

  char *response = 0;
  if(rc || errorMessage) {
    response = write_response(errorMessage, strlen(errorMessage));
  }
  else {
    if(str.n != 0) {
      response = write_response(str.z, str.n);
    } else {
      // if a request was successfull, sqlite doesn't return anything as the result string
      const char success_result[] = "OK";
      response = write_response((char *)success_result, sizeof(success_result));
    }
  }

  deallocate(request, request_size + 1);
  freeText(&str);

  return response;
}

// these functions are needed to support Rust backends
// (https://github.com/rust-lang/rust/issues/63562)
const char *sqlite_invoke(char *request, int request_size) {
  return invoke(request, request_size);
}

void* sqlite_allocate(size_t size) {
  return allocate(size);
}

void sqlite_deallocate(void *ptr, int size) {
  deallocate(ptr, size);
}

void sqlite_store(char *ptr, unsigned char byte) {
  store(ptr, byte);
}

unsigned char sqlite_load(const unsigned char *ptr) {
  return load(ptr);
}
