#!/usr/bin/env bash

g++ -std=c++17 main.cpp algo/algo.cpp utility/utility.cpp UI/UI.cpp -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -o searching_alg
