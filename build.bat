@echo off
mkdir bin 2> NUL
cd cli

go env -w GOOS=windows
go env -w GOARCH=amd64
go build -o ../bin/ccom.exe .

cd ../bin
cmake -DCMAKE_BUILD_TYPE=Release -G "MinGW Makefiles" ../compiler/c++
mingw32-make

move compiler.exe ccomc.exe

cd ..