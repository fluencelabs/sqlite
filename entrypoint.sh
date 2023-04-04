export PATH="${PATH}:/root/.cargo/bin"
curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-15/wasi-sdk-15.0-linux.tar.gz | tar xz -C .
mv wasi-sdk-* wasi-sdk
make
