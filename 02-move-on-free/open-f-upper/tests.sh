#!/bin/bash
# single-core test battery (sequential).  Output: tests.log
ulimit -v 8000000; export TMPDIR=$PWD
echo "== exhaustive, game F, first fit + compaction (A=0 V=1); optimum h^F = 9 11 13 14 16 18 20 21 for M=6..13"
for m in 6 7 8 9 10 11 12 13 14; do timeout 1500 nice -n 15 ./rt2 exh F $m 0 1 0 28; done
echo "== NEGATIVE CONTROL 1: cap one cell too small must FAIL, exact cap must PASS (M=12, rule needs 22)"
timeout 600 nice -n 15 ./rt2 exh F 12 0 1 21 27; timeout 600 nice -n 15 ./rt2 exh F 12 0 1 22 27
echo "== NEGATIVE CONTROL 2: deliberately worse rules (last fit A=9; move lowest 1 into the freed hole V=8)"
for m in 6 8 10; do timeout 600 nice -n 15 ./rt2 exh F $m 9 1 0 27; timeout 600 nice -n 15 ./rt2 exh F $m 0 8 0 27; done
echo "== random structured adversary (lower bounds on what each rule needs), large M"
for m in 24 48 96; do for r in "0 0 0" "0 1 0" "0 5 0"; do set -- $r; COSTMODE=$3 timeout 900 nice -n 15 ./rtw rnd $m $1 $2 30000 7; done; done
for m in 24 48; do COSTMODE=3 timeout 1500 nice -n 15 ./rtw rnd $m 0 6 3000 7; done
echo "== done"
