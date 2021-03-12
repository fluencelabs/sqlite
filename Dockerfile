FROM ubuntu:18.04

RUN apt-get update \
 && apt-get install -y ca-certificates \
                       curl \
                       git \
                       make \
                       cargo

RUN curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-12/wasi-sdk-12.0-linux.tar.gz | tar xz --strip-components=1 -C /

RUN cargo install fcli --version 0.2.0

VOLUME /code
WORKDIR /code
CMD make
