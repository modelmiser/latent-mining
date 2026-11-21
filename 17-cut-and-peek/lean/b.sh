#!/bin/bash
# usage: ./b.sh [targets...]   build under the compute rules (2 threads on cores 4,5, niced, memory-capped)
cd "$(dirname "$0")"
export TMPDIR=$PWD/tmp
export LEAN_NUM_THREADS=2
ulimit -v 40000000
timeout 1500 taskset -c 4,5 nice -n 15 lake build "$@" 2>&1 | grep -v '^$' | grep -v 'Hint:\|\[apply\]\|^Note:'
exit ${PIPESTATUS[0]}
