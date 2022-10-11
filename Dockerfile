FROM ubuntu:22.04

RUN apt-get update

# Install build environment
RUN apt-get install -y cmake make gcc g++ git valgrind lcov