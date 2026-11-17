#!/bin/bash
# n=6, L=66, Lemma-5 route with rho(w)=34 filter and complement symmetry, 64 chunks, worker $1 of 2
cd verifier
ulimit -v 8000000; export TMPDIR=$PWD
for R in $(seq $1 2 63); do
  FAST=1 nice -n 15 timeout 1500 ./ver_run6 route 6 66 34 1 64 $R; echo "chunk $R exit=$?"
done > n6_L66_route_w$1.txt 2>&1
echo done > n6_L66_route_w$1.done
