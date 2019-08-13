FROM ubuntu:19.04

RUN apt-get update \
 && apt-get install -y ca-certificates \
                       curl \
                       git \
                       make \
                       libtinfo5

RUN curl -L https://github.com/CraneStation/wasi-sdk/releases/download/wasi-sdk-6/wasi-sdk-6.0-linux.tar.gz | tar xz --strip-components=1 -C /

VOLUME /code
WORKDIR /code
CMD make
