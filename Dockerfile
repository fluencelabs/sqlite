FROM ubuntu:20.04

RUN apt update \
 && DEBIAN_FRONTEND=noninteractive TZ=Etc/UTC apt-get -y install tzdata \
 && apt install -y ca-certificates \
    curl \
    git \
    make \
    pkg-config \
    libtinfo6

RUN mkdir -p ~/.local/bin && curl -L https://github.com/fluencelabs/marine/releases/download/marine-v0.14.1/marine-linux-x86_64 -o ~/.local/bin/marine && chmod +x ~/.local/bin/marine

VOLUME /code
WORKDIR /code
ENTRYPOINT /code/entrypoint.sh
