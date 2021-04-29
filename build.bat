@echo off
mkdir bin 2> NUL
cd cli

go env -w GOOS=windows
go env -w GOARCH=amd64
go build -o ../bin/ccom-amd64.exe main.go

cd ..