FROM ubuntu:20.04

RUN apt update \
 && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata \
 && apt install -y ca-certificates \
    curl \
    git \
    make \
    pkg-config \
    libtinfo6 \
    cargo

RUN cargo install marine --version 0.12.7
RUN curl -L https://github.com/WebAssembly/wasi-sdk/releases/download/wasi-sdk-15/wasi-sdk-15.0-linux.tar.gz | tar xz -C /

VOLUME /code
WORKDIR /code
CMD make
