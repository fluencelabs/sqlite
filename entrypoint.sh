export PATH="${PATH}:/root/.cargo/bin:/root/.local/bin"
curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-15/wasi-sdk-15.0-linux.tar.gz | tar xz -C .
rm -rf wasi-sdk
mv -f wasi-sdk-* wasi-sdk
make
