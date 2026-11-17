#!/bin/bash
# usage: run_chunks.sh outdir n m mode altonly(0/1) worker(0/1) prefixbits   -- runs chunks with index%2==worker, sequentially
out=$1; n=$2; m=$3; mode=$4; alt=$5; wk=$6; pb=$7
cd solver-b
ulimit -v 8000000
zeros=$(printf '0%.0s' $(seq 1 $n))
i=0
for suf in $(python3 -c "
import itertools
for t in itertools.product('01',repeat=$pb): print(''.join(t))"); do
  if [ $((i % 2)) -eq $wk ]; then
    f=$out/chunk_$zeros$suf.txt
    if ! grep -q '^exit 0' $f 2>/dev/null; then
      if [ "$alt" = 1 ]; then ALTONLY=1 nice -n 15 timeout 1500 ./uni128 $n $m $mode $zeros$suf > $f 2>&1; else nice -n 15 timeout 1500 ./uni128 $n $m $mode $zeros$suf > $f 2>&1; fi
      echo "exit $?" >> $f
    fi
  fi
  i=$((i+1))
done
echo done > $out/worker$wk.done
