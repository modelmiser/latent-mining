#!/bin/bash
# cross-checks: naive.py vs solve (plain) on every (G,M,H) with M<=6 (F: M<=5), plus restricted size sets
cd ./solver-b
export TMPDIR=$PWD
ulimit -v 8000000
bad=0; n=0
for G in N F O; do
  for M in 3 4 5 6; do
    [ $G = F ] && [ $M = 6 ] && continue
    for H in $(seq $M $((2*M-2))); do
      a=$(nice -n 15 timeout 1500 ./solve $G $M $H | awk '{print $NF}')
      b=$(nice -n 15 timeout 1500 python3 naive.py $G $M $H | awk '{print $NF}')
      n=$((n+1)); [ "$a" = "$b" ] || { bad=$((bad+1)); echo "MISMATCH $G $M $H solve=$a naive=$b"; }
      echo "$G M=$M H=$H solve=$a naive=$b"
    done
  done
done
for M in 2 3 4 5 6; do for H in $(seq $M $((2*M-1))); do
  a=$(nice -n 15 timeout 1500 ./solve N $M $H 3 | awk '{print $NF}')
  b=$(nice -n 15 timeout 1500 python3 naive.py N $M $H 12 | awk '{print $NF}')
  n=$((n+1)); [ "$a" = "$b" ] || { bad=$((bad+1)); echo "MISMATCH sizes12 $M $H"; }
  echo "sizes{1,2} M=$M H=$H solve=$a naive=$b"
done; done
echo "compared $n cases, mismatches $bad"
