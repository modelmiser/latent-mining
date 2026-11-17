#!/bin/bash
# exhaustive tables (MAXJ=-1, no PMASK/UW/LD/MAXLONG restriction, DOM off unless stated). Output: tables.txt
cd ./solver-a/ ; export TMPDIR=$PWD ; ulimit -v 8000000
run() { timeout 1500 nice -n 15 ./game7 "$@" | tail -1; }
{
for T in 1 2 3 4 5 6; do run 1 $T 0; run 1 $T 1; done
for T in 1 2 3 4 5 6 7 8 9; do run 2 $T 0; run 2 $T 1; done
for T in 1 2 3 4 5 6 7 8 9; do run 3 $T 0; done
for T in 1 2 3 4 5 6 7 8; do run 3 $T 1; done
for T in 1 2 3 4 5 6 7 8 9 10; do run 4 $T 0; done
for T in 1 2 3 4 5 6 7; do run 4 $T 1; done
} > tables.txt 2>&1
