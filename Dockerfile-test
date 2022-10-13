FROM ubuntu:latest

RUN apt-get update && apt-get install make

WORKDIR /test
COPY ./test/e2e/* ./

WORKDIR /tmp
COPY ./packages/*.deb ./
RUN apt-get install ./$(ls | grep *.deb)