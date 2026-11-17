#!/bin/bash
# usage: two.sh n t target depth   (two-level exact search; every stage niced, capped, timed)
cd verifier; export TMPDIR=$PWD; ulimit -v 8000000
n=$1; t=$2; T=$3; d=$4; L=two_${n}_${t}.log
nice -n 15 timeout 300 ./ref2 bb $n $t $T w_${n}_${t}.txt $d fr_${n}_${t}.txt > $L 2>&1
nice -n 15 timeout 1500 python3 sublp.py $n $t $T fr_${n}_${t}.txt sw_${n}_${t}.txt >> $L 2>&1
nice -n 15 timeout 1450 ./ref2 sub $n $t $T sw_${n}_${t}.txt >> $L 2>&1
echo "exit $?" >> $L
