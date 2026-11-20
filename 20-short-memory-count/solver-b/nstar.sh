#!/bin/bash
# nstar.sh WMIN WMAX PERCALL : table of N*(w,s) = least number of states admitting g <= s  (fast3, DFS numbering)
# starts each (w,s) at N = w-s+1 (Theorem B: fewer states impossible) ; lines go to stdout
for w in $(seq $1 $2); do for s in $(seq $((w-1)) -1 0); do
  for N in $(seq $((w-s+1)) 12); do
    out=$(ORDER=1 timeout $3 ./fast3 $w $N find $s 2>&1 >>nstar_wit.txt | tail -1)
    if echo "$out" | grep -q EXISTS; then echo "w=$w s=$s N*=$N   [$out]"; break; fi
    if ! echo "$out" | grep -q NONE; then echo "w=$w s=$s N=$N TIMEOUT/UNKNOWN (N* >= $N)"; break 2; fi
    if [ $N = 12 ]; then echo "w=$w s=$s N* > 12"; break 2; fi
  done
done; done
