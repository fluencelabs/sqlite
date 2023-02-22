# SQLite


## Overview
SQLite fork adapted to work with Fluence the protocol. The current version is based on SQLite version 3.40.1. The artifact is SQLite WASM module:


- that is compiled following [Marine runtime ABI conventions](https://fluence.dev/docs/marine-book/marine-rust-sdk/module-abi)
- to be used with [Marine runtime](https://github.com/fluencelabs/marine)


## How to build



Wasm module can be built with either docker-compose



```bash

docker-compose up

```



or using this [Makefile](./Makefile) with GNU make. There are prerequisites to be installed following this path, namely [wasi-sdk](https://github.com/WebAssembly/wasi-sdk) and marine crate. Take a look at the Dockerfile for the details.


```bash

make

```



## How to use



The SQLite Wasm module exports a set of SQLite C API functions. The easiest way to try this module is to run it with Marine REPL.
You can find MREPL output for a simple "CREATE-INSERT-SELECT" scenario below. Mind the second argument to sqlite3_open_v2() that is an OR-ed SQLite flag values. The value "6" in this context means `SQLITE_OPEN_READWRITE | SQLITE_OPEN_CREATE`.


```bash
$cargo install mrepl
...
$mrepl -q Config.toml

1> call sqlite3 sqlite3_open_v2 [":memory:",6,""]
result: {
  "db_handle": 198600,
  "ret_code": 0
}
 elapsed time: 357.556µs

2> call sqlite3 sqlite3_exec [198600, "CREATE TABLE tab1(i bigint);", 0, 0]
result: {
  "err_msg": "",
  "ret_code": 0
}
 elapsed time: 1.736661ms

3> call sqlite3 sqlite3_exec [198600, "INSERT INTO tab1 VALUES (42);", 0, 0]
result: {
  "err_msg": "",
  "ret_code": 0
}
 elapsed time: 330.098µs

4> call sqlite3 sqlite3_prepare_v2 [198600, "SELECT * FROM tab1;"]
result: {
  "ret_code": 0,
  "stmt_handle": 244584,
  "tail": 268147
}
 elapsed time: 280.668µs

5> call sqlite3 sqlite3_step [244584]
result: 100
 elapsed time: 124.122µs

6> call sqlite3 sqlite3_column_int64 [244584,0]
result: 42
 elapsed time: 72.483µs

```


You can also try the SQLite Wasm module using Rust together with [this Sqlite connector](https://github.com/fluencelabs/sqlite-wasm-connector). [Here](https://github.com/fluencelabs/examples/tree/main/marine-examples/sqlite) you can find a simple SQL REPL utility example built into a Wasm module.

## Support

Please, [file an issue](https://github.com/fluencelabs/sqlite/issues) if you find a bug. You can also contact us at [Discord](https://discord.com/invite/5qSnPZKh7u) or [Telegram](https://t.me/fluence_project).  We will do our best to resolve the issue ASAP.


## Contributing

Any interested person is welcome to contribute to the project. Please, make sure you read and follow some basic [rules](https://github.com/fluencelabs/rust-peer/blob/master/CONTRIBUTING.md).


## License

All software code is copyright (c) Fluence Labs, Inc. under the [Apache-2.0](https://github.com/fluencelabs/rust-peer/blob/master/LICENSE) license.
