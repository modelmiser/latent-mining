#!/bin/bash
# usage: iter.sh n t target swfile budget depth   -- iterate: solve subproblems within node budget; split+re-LP the hard ones
cd verifier; export TMPDIR=$PWD; ulimit -v 8000000
n=$1; t=$2; T=$3; cur=$4; B=$5; d=$6; L=iter_${n}_${t}${7}.log; : > $L; r=0
while true; do r=$((r+1))
  nice -n 15 timeout 1400 ./ref3 sub $n $t $T $cur -1 x $B hard_${n}_${t}_$r.txt >> $L 2>&1 || { echo "FAILED/timeout round $r" >> $L; exit 1; }
  if grep -q "result=FOUND" $L; then echo "DONE FOUND" >> $L; exit 0; fi
  if [ ! -s hard_${n}_${t}_$r.txt ]; then echo "DONE: all subproblems closed, result NONE (rounds=$r)" >> $L; exit 0; fi
  nice -n 15 timeout 1400 ./ref3 sub $n $t $T hard_${n}_${t}_$r.txt $d fr_${n}_${t}_$r.txt >> $L 2>&1 || { echo "FAILED split round $r" >> $L; exit 1; }
  nice -n 15 timeout 1400 python3 sublp.py $n $t $T fr_${n}_${t}_$r.txt sw_${n}_${t}_$r.txt >> $L 2>&1 || { echo "FAILED lp round $r" >> $L; exit 1; }
  cur=sw_${n}_${t}_$r.txt
done
