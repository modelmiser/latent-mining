#!/bin/bash
# usage: run_chunks.sh C c1 c2 ...   runs chunks sequentially (one process at a time)
cd verifier
export TMPDIR=$PWD
ulimit -v 8000000
C=$1; shift
for c in "$@"; do
  h=$(date +%H%M)
  if [ "$h" -ge 0420 ] && [ "$h" -lt 0545 ]; then echo "skipped (time window)" > l7_${C}_$c.txt; continue; fi
  /usr/bin/time -f "elapsed %e s" nice -n 15 timeout 1500 ./ref lazy 7 asc $C $c > l7_${C}_$c.txt 2>&1
  echo "exit $?" >> l7_${C}_$c.txt
done
echo done > chunks_$1.done
