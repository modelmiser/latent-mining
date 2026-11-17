#!/bin/bash
cd solver-a
ulimit -v 8000000; export TMPDIR=$PWD
for pair in "67 68" "69 70" "72 74" "76 80"; do
  set -- $pair
  nice -n 15 timeout 1500 ./randsearch 6 $1 7 2 200 > rand6_L$1.txt 2>&1 &
  p1=$!
  nice -n 15 timeout 1500 ./randsearch 6 $2 7 2 200 > rand6_L$2.txt 2>&1 &
  p2=$!
  wait $p1 $p2
done
