#!/bin/bash
# Produces witness files from gen.py for the whole table, checks them and the search witnesses with check.py,
# and runs the "must fail" tests. Output: verify_log.txt
ulimit -v 8000000
cd "$(dirname "$0")"
mkdir -p witgen
{
for n in 3 4 5 6 7 8 9 10 11 12 13 30 60; do for k in 1 2 3 4 5; do
  m=$k; [ $k -gt $n ] && m=$n
  claim=$(( n - (n + m - 1)/m )); half=$(( n/2 )); 
  timeout 100 nice -n 15 python3 gen.py $n $k witgen/g_${n}_${k}.txt
  out=$(timeout 100 nice -n 15 python3 check.py witgen/g_${n}_${k}.txt)
  first=$(echo "$out" | sed 's/.*first_completion=//')
  st=OK; echo "$out" | grep -q ' LEGAL' || st=BAD; [ "$first" -lt "$claim" ] && st=BAD
  echo "gen n=$n k=$k claim>=$claim got=$first $st"
done; done
echo "--- search witnesses (wit/w_n_k_Tt.txt must be LEGAL with first completion > t)"
for f in wit/w_*_T*.txt; do
  t=${f##*_T}; t=${t%.txt}
  out=$(timeout 100 nice -n 15 python3 check.py $f); first=$(echo "$out" | sed 's/.*first_completion=//')
  st=OK; echo "$out" | grep -q ' LEGAL' || st=BAD; [ "$first" -gt "$t" ] || st=BAD
  echo "$f $out $st"
done | awk '{c[$NF]++} END{for(x in c) print "search-witness files", x, c[x]}'
echo "--- must-fail tests"
python3 check.py bad_repeat.txt
python3 check.py bad_budget.txt
python3 check.py witgen/g_9_3.txt 7; echo "exit=$? (expected 1: g_9_3 completes at 6, not 7)"
} > verify_log.txt 2>&1
