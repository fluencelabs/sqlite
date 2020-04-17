# SQLite

Sqlite fork ported to WebAssembly and adapted for the Fluence network. Could be launched and played on the Fluence [dashboard](http://dash.fluence.network/deploy/sqlite).

Based on SQlite version 3.31.1.

# How to build

This app could be built either with docker 

```bash
docker-compose up
```

or by Makefile with [wasi-sdk](https://github.com/WebAssembly/wasi-sdk) installed
```bash
make
```

# How to use

At now, this fork exports five API functions:
```cpp

/**
 * Executes given SQL request and returns result in as a pointer to the following structure: | result size (4 bytes, le)| result (size bytes) |.
 *
 * @param sql a pointer to the supplied sql request
 * @param length a size of the supplied sql request
 * @return a pointer to the struct contains result_size and result
 */
char *invoke(const char *sql, size_t length);

/**
 * Allocates a memory region of a given size. Could be used by Wasm execution environments for byte array passing.
 *
 * @param size a size of allocated memory region
 * @return a pointer to the allocated memory region
 */
void *allocate(size_t size);

/**
 * Frees a memory region. Could be used by Wasm execution environments for freeing previous memory allocated by `allocate` function.
 *
 * @param ptr a pointer to the previously allocated memory region
 * @param size a size of the previously allocated memory region
 */
void deallocate(void *ptr, size_t size);

/**
 * Stores one byte by a given address in the module memory.
 *
 * @param ptr a address where byte should be stored
 * @param value a byte to be stored
 */
void store(void *ptr, char value);

/**
 * Returns one byte by a given address in the module memory.
 *
 * @param ptr a address at which the needed byte is located
 * @return the byte at the given address
 */
char load(void *ptr);
```

Given char string `sql` as the request, the general scheme to use it is following:
 1. `void *ptr = allocate(strlen(sql))` that returns a pointer to the memory region enough for the string
 2. `void *res = invoke(ptr, strlen(sql))` to execute the request
 3. read a result from the `res` by reading 4 bytes as little-endian `result_size` and the read `result_size` bytes as the final result.
 4. `deallocate(res, strlen(sql))` to clean memory.
 
Depends on your Wasm execution environment, `load`/`store` could be used for reading and writing a module memory.

## More insights

At Fluence, we use WebAssembly (Wasm) to run applications in a trustless network of independent nodes. This trustless environment mandates that every piece of the code executed by any network node must be verified by another node to check whether the execution was performed correctly. In order to be verifiable, every computation must be made deterministic, which means that to run WebAssembly code in a trustless network, we need to make its execution deterministic.

There are three primary sources of nondeterminism in WebAssembly: external functions invocation, NaN payloads, and VM resource exhaustion. The first one is the most problematic; currently, we deal with it by simply prohibiting any interactions with the host environment from Wasm programs. This means that submitted WebAssembly code should not contain any external imports.

The main purpose of this fork is to compile to Wasm without any imports. SQlite is famous for it embeddability and allows to configure itself by variety of macros. E.g. multithreading that produces a lot of imports of host functions could be completely disabled by setting SQLITE_THREADSAFE to 0. Also this fork has been patched to use only memory database without any interaction with a hard disk. So, now it is Wasm in-memory database.

More information about ways of porting C/C++ code to Wasm could be found in our [article](https://medium.com/fluence-network/porting-redis-to-webassembly-with-clang-wasi-af99b264ca8) about porting of Redis.

## Future plans

We are working on supporting of subset of WASI syscalls and interface types support for more convinient parameter passing scheme.
