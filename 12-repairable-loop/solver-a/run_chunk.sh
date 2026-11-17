#!/bin/bash
# usage: run_chunk.sh n L k MOD R
cd solver-a
ulimit -v 8000000; export TMPDIR=$PWD
( /usr/bin/time -f "%es" nice -n 15 timeout 1500 ./dfs2c $1 $2 $3 20 $4 $5 ; echo "exit=$?" ) > n$1_L$2_k$3_chunk$5of$4.txt 2>&1
