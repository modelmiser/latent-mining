#!/bin/bash
cd solver-a
ulimit -v 8000000; export TMPDIR=$PWD
for job in "78 3 76 4" "74 5 72 6" "76 7 74 8" "72 9 70 10"; do
  set -- $job
  nice -n 15 timeout 1500 ./anneal 6 $1 $2 240 > anneal_n6_L$1_s$2.txt 2>&1 &
  p1=$!
  nice -n 15 timeout 1500 ./anneal 6 $3 $4 240 > anneal_n6_L$3_s$4.txt 2>&1 &
  p2=$!
  wait $p1 $p2
done
echo done > anneal6.done
