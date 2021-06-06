FROM alpine:3.13.5
WORKDIR /ccom/out

COPY ccom /ccom/ccom
COPY bin/ccomc-${COMPILER_PATH}/* /ccom/

ENV PATH="/ccom:${PATH}"
ENV TERM="xterm-256color"

RUN chmod +x /ccom/ccom
RUN mkdir /lib64 && ln -s /lib/libc.musl-x86_64.so.1 /lib64/ld-linux-x86-64.so.2
ENTRYPOINT [ "ccom" ]