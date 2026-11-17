#!/bin/bash
# usage: run_pol.sh outfile n kicks "seed:loop seed:loop ..."
cd solver-b; ulimit -v 8000000
out=$1; n=$2; kicks=$3
for sl in $4; do s=${sl%%:*}; l=${sl##*:}; nice -n 15 timeout 700 ./polish2 $n $l $s $kicks >> $out 2>&1; echo "exit $?" >> $out; done
echo done >> $out
