@echo off
cd bin
ccom.exe -d ../media/test-data.json -l yaml -o output.yml ../media/test-file.yml
pause