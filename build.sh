#!/usr/bin/env bash
clang++ -std=c++17 -Wall -Wextra `sdl2-config --libs --cflags` main.cpp library.cpp GUI.cpp -o fpc
