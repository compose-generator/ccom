FROM alpine:3.13.5
WORKDIR /ccom/out

ENV TERM="xterm-256color"
ENV CCOM_DOCKERIZED=1

ARG COMPILER_PATH=linux-amd64

COPY ccom /usr/bin/ccom
COPY bin/ccomc-${COMPILER_PATH}/ /usr/lib/ccom/

ENTRYPOINT [ "ccom" ]