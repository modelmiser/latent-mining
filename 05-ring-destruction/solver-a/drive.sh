#!/bin/bash
# usage: drive.sh n k prune [per-call timeout]  -> prints F(n,k) = 1 + max survivable T ; logs each call
n=$1; k=$2; pr=$3; to=${4:-600}
ulimit -v 8000000
T=1
while true; do
  out=$(timeout $to nice -n 15 ./fast $n $k $T $pr); rc=$?
  if [ $rc -ne 0 ]; then echo "n=$n k=$k prune=$pr T=$T TIMEOUT/ERR rc=$rc"; exit 1; fi
  echo "$out" | head -1 >> log_p$pr.txt
  if echo "$out" | head -1 | grep -q ": NO"; then echo "F($n,$k)=$T prune=$pr"; exit 0; fi
  echo "$n $k" > wit/w_${n}_${k}_T$T.txt; echo "$out" | grep '^W' | sed 's/^W //' >> wit/w_${n}_${k}_T$T.txt
  T=$((T+1))
done
