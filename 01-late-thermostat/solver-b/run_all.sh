#!/bin/bash
# usage: run_all.sh dmin dmax ; finds least feasible B for each class, logs everything
cd solver-b
for d in $(seq $1 $2); do
  for m in R C H; do
    B=1
    while true; do
      out=$( (ulimit -v 6000000; /usr/bin/time -f "  time=%es mem=%MkB" nice -n 15 timeout 900 ./solve $d $B $m aut_d${d}_B${B}.txt 2>&1) )
      echo "$out"
      if echo "$out" | grep -q " FEASIBLE"; then break; fi
      if ! echo "$out" | grep -q "INFEASIBLE"; then echo "ABORT d=$d m=$m B=$B"; break; fi
      B=$((B+1))
    done
  done
done
