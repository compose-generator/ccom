FROM alpine:3.13.5
WORKDIR /ccom/out

ARG COMPILER_PATH=linux-amd64

COPY ccom_*.apk /tmp/
RUN apk add --allow-untrusted /tmp/ccom_*.apk

ENV TERM="xterm-256color"
ENV CCOM_DOCKERIZED=1

ENTRYPOINT [ "ccom" ]