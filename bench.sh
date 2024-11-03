#!/usr/bin/env bash

set -e
cd $(dirname "${BASH_SOURCE[0]}")

if [[ "$CC" == "" ]]; then
    CC=cc
fi

if [[ "$CXX" == "" ]]; then
    CXX=c++
fi

if [[ "$G" == "" ]]; then
    export G=50
fi

echo "tidwall/bgen"
$CC -O3 $CFLAGS ../bgen/tests/bench_b.c
./a.out

echo
echo "rust/btree"
cd rust-btree
cargo run --release --quiet
cd ..

echo
echo "frozenca/btree"
cd frozenca-btree
$CXX -Iinclude -std=c++20 -O3 $CXXFLAGS bench.cpp
./a.out
cd ..

echo
echo "tidwall/bgen (spatial)"
$CC -O3 $CFLAGS ../bgen/tests/bench_s.c
./a.out

echo
echo "tidwall/rtree (spatial)"
$CC -O3 $CFLAGS tidwall-rtree/main.c tidwall-rtree/rtree.c
./a.out
