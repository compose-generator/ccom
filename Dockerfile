FROM alpine:3.21.0
WORKDIR /ccom/out

# Set env variables
ENV TERM="xterm-256color"
ENV CCOM_DOCKERIZED=1

# Specify volumes
VOLUME /ccom/out

# Set default arg value
ARG COMPILER_PATH=linux-amd64

# Prepare container
RUN apk update && apk add --no-cache libc6-compat libstdc++ && rm -rf /var/cache/apk/*

# Copy sources
COPY ccom /usr/bin/ccom
COPY bin/ccomc-${COMPILER_PATH}/ /usr/lib/ccom/

# Set entrypoint
ENTRYPOINT [ "ccom" ]