FROM ubuntu:18.04

RUN apt-get update \
 && apt-get install -y ca-certificates \
                       curl \
                       git \
                       make \
                       libtinfo5

RUN curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-10/wasi-sdk-10.0-linux.tar.gz | tar xz --strip-components=1 -C /

VOLUME /code
WORKDIR /code
CMD make
