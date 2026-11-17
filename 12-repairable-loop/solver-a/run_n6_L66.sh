#!/bin/bash
# exhaustive n=6 L=66, k=1..5, in chunks (prefix index mod 4), each chunk <= 1500 s, 2 threads
cd solver-a
ulimit -v 8000000; export TMPDIR=$PWD
for k in 1 5 2 3 4; do for R in 0 1 2 3; do
  ( /usr/bin/time -f "%es" nice -n 15 timeout 1500 ./dfs3 6 66 $k 20 4 $R ; echo "exit=$?" ) > n6_L66_k${k}_chunk${R}of4.txt 2>&1
done; done
echo done > n6_L66.done
