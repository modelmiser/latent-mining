#!/bin/bash
# Reproduces every number in the report. Run from this folder.  ~20 min, 1 core, <1.2 GB.
export TMPDIR=$PWD; ulimit -v 8000000
g++ -O2 -o fast fast.cpp && g++ -O2 -o fast2 fast2.cpp
R="nice -n 15 timeout 1500"
for g in E D; do for h in 0 1 2 3 4 5 6; do for k in 0 1 2 3 4 5; do $R ./fast $g $h $k | grep RESULT; done; done; done   # 128-bit solver
for a in "E 7 2" "E 7 3" "E 8 3" "E 8 4" "E 9 3" "E 9 4" "D 7 5" "D 7 6" "D 8 6" "D 8 7" "D 9 7" "D 9 8"; do $R ./fast2 $a | grep RESULT; done  # bitset solver
E=(0 0 0 1 2 2 3); D=(0 0 0 2 3 4 5)
for h in 1 2 3 4 5 6; do $R ./fast E $h ${E[$h]} 1 1 | grep wrote; $R ./fast D $h ${D[$h]} 1 1 | grep wrote
  if [ ${E[$h]} -gt 0 ]; then $R ./fast E $h $((${E[$h]}-1)) 1 1 | grep wrote; $R ./fast D $h $((${D[$h]}-1)) 1 1 | grep wrote; fi; done
for f in strat_*.txt lose_*.txt; do $R python3 check.py $f; done
for f in mutants/*.txt; do $R python3 check.py $f; done      # all four must print REJECT
for g in E D; do for h in 1 2 3; do for k in 0 1 2; do $R ./fast $g $h $k 0 1 cache_${g}_h${h}_k${k}.dump >/dev/null; done; $R python3 slow.py $g $h 2 cache_${g}_h${h}_k*.dump; done; done
$R python3 lemma_check.py
