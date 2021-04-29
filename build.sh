#!/bin/sh
mkdir -p bin
cd ./cli
env GOOS=linux GOARCH=arm GOARM=7 go build -x -o ../bin/ccom-armv7 main.go
env GOOS=linux GOARCH=amd64 go build -x -o ../bin/ccom-amd64 main.go
cd ..