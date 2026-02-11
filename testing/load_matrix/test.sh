#!/bin/sh

g++ load_matrix.cpp -o ./bin/load_matrix;

./bin/load_matrix < input.in;
