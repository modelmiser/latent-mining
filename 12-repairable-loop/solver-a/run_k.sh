#!/bin/bash
# usage: run_k.sh n L k
cd solver-a
ulimit -v 8000000; export TMPDIR=$PWD
( /usr/bin/time -f "%es" nice -n 15 timeout 1500 ./dfs2 $1 $2 $3 20 ; echo "exit=$?" ) > n$1_L$2_k$3.txt 2>&1
