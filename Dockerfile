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

VOLUME /code
WORKDIR /code
ENTRYPOINT /code/entrypoint.sh
