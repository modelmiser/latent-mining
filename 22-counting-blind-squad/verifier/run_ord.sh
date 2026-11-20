#!/bin/bash
# usage: run_ord.sh C c1 c2 ...  (s=7, permuted order, rtl scan, no symmetry breaking), sequential
cd verifier
export TMPDIR=$PWD
ulimit -v 8000000
C=$1; shift; first=$1
for c in "$@"; do
  h=$(date +%H%M)
  if [ "$h" -ge 0420 ] && [ "$h" -lt 0545 ]; then echo "skipped (time window)" > o7_${C}_$c.txt; continue; fi
  /usr/bin/time -f "elapsed %e s" nice -n 15 timeout 1500 ./ref3 ord 7 3,2,5,4,7,6,9,8,11,10,12 rtl $C $c > o7_${C}_$c.txt 2>&1
  echo "exit $?" >> o7_${C}_$c.txt
done
echo done > ord_$first.done
