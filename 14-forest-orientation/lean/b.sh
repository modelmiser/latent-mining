#!/bin/bash
# usage: ./b.sh [targets...]   build with the compute rules of the brief
cd "$(dirname "$0")"
export TMPDIR=$PWD/tmp
export LEAN_NUM_THREADS=2
ulimit -v 16000000
timeout 1500 taskset -c 4,5 nice -n 15 lake build "$@" 2>&1 | grep -v '^$' | grep -v 'linter\|Hint:\|\[apply\]\|^Note:\|unusedVariables' 
