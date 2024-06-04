#!/usr/bin/env bash

set -eux

bp_obj_filename="backpack.o"

if [ "$1" = "bt" ]; then
    g++ -c -std=c++20 -I../../backtrack -o "$bp_obj_filename" ../../backtrack/backpack.cxx
elif [ "$1" = "gr" ]; then
    gcc -c -std=c11 -o "$bp_obj_filename" ../backpack.c
else
    echo "Must specify which version of the solution to test" 1>&2
    exit 1
fi

g++ -std=c++20 -o backpack.out "$bp_obj_filename" bench.cxx
rm "$bp_obj_filename"