#!/bin/bash
# usage: run_sa.sh outfile n iters T0 "L:seed L:seed ..."
cd solver-b; ulimit -v 8000000
out=$1; n=$2; it=$3; T0=$4
for ls in $5; do L=${ls%%:*}; s=${ls##*:}; nice -n 15 timeout 1500 ./sa2 $n $L $s $it $T0 2>/dev/null >> $out; done
echo done >> $out
