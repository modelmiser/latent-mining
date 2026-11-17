#!/bin/bash
cd solver-b
ulimit -v 5000000
V="nice -n 15 timeout 900 python3 verify.py"
for a in "1 2" "2 3" "3 4" "4 5" "5 6" "6 7"; do $V clockwit $a clock_witnesses.log; $V sim $a clock clock_witnesses.log; done
for a in "1 1" "2 3" "3 3" "4 5" "5 5" "6 7" "7 7"; do set -- $a; $V histwit $1 $2 aut_d$1_B$2.txt; $V sim $1 $2 hist aut_d$1_B$2.txt; done
for d in 1 2 3 4 5 6; do B=$((3*d/2+1)); $V sim $d $B relay +0-; $V relay $d $B; $V relay $d $((B-1)); done
$V clocklb 4 4; $V clocklb 4 5
$V histlb 5 4 80; $V histlb 5 5 60; $V histlb 6 6 100
