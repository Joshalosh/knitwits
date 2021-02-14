@echo off

IF NOT EXIST build mkdir build

pushd build

gcc ..\main.cpp -o generator.exe -g -Wall -std=c++11 -Wno-missing-braces -I ..\include/ -L ..\lib/ -lraylib -lopengl32 -lgdi32 -lwinmm

popd
