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

void store(char *ptr, unsigned char byte) {
  *ptr = byte;
}

void sqlite_store(char *ptr, unsigned char byte) {
  store(ptr, byte);
}

unsigned char load(const unsigned char *ptr) {
  return *ptr;
}

unsigned char sqlite_load(const unsigned char *ptr) {
  return load(ptr);
}

void* allocate(size_t size) {
  return malloc(size + 1);
}

void* sqlite_allocate(size_t size) {
  return allocate(size);
}

void deallocate(void *ptr, int size) {
  free(ptr);
}

void sqlite_deallocate(void *ptr, int size) {
  deallocate(ptr, size);
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

const char *invoke(char *request, int request_size) {
  if(g_isInited == 0) {
    // TODO: check the return code
    init();
    const char successInitMessage[] = "Sqlite has been initialized";
    wasm_log(successInitMessage, sizeof(successInitMessage));
    g_isInited = 1;
  }

  request[request_size] = 0;

  wasm_log(request, request_size);

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

const char *sqlite_invoke(char *request, int request_size) {
  return invoke(request, request_size);
}
