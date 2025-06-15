FROM ubuntu:22.04

RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    g++-11 \
    && rm -rf /var/lib/apt/lists/*

ENV CC=gcc-11
ENV CXX=g++-11

WORKDIR /app

COPY . .

RUN mkdir build && cd build && \
    cmake .. && \
    make

CMD ["./build/metrics_collector"] 