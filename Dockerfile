FROM ubuntu:19.04

RUN apt-get update \
 && apt-get install -y ca-certificates \
                       curl \
                       git \
                       make

RUN curl -L https://github.com/CraneStation/wasi-sdk/releases/download/wasi-sdk-5/wasi-sdk-5.0-linux.tar.gz | tar xz --strip-components=1 -C /

VOLUME /code
WORKDIR /code
CMD make
