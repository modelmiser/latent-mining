#!/bin/bash
# final recorded runs (sequential, one process at a time)
cd solver-b || exit 1
ulimit -v 8000000; ulimit -c 0
export TMPDIR=$PWD
run() { nice -n 15 timeout 1500 "$@" 2>&1 | grep -v "^done"; }
{
echo "### MODE 2 (zone rule, Lemma Z, T1=2 T2=4): 'no winning list' = no Z-winning list of that length => every list of that length loses"
for hm in "3 5" "4 10" "5 14" "6 15"; do set -- $hm; for m in $(seq 1 $2); do T1=2 T2=4 MEMLOG=26 run ./g4 $1 $m 2; done; done
} > log_zone.txt
{
echo "### MODE 1 (cluster rule, Lemma C, unrestricted Firer): exhaustive over all lists"
for hm in "1 1" "2 2" "3 5" "4 10" "5 12"; do set -- $hm; for m in $(seq 1 $2); do MEMLOG=26 run ./g2s $1 $m 1; done; done
} > log_cluster.txt
{
echo "### cross-checks: hitting prune off (NOHB=1)"
T1=2 T2=4 NOHB=1 MEMLOG=26 run ./g4 5 12 2
T1=2 T2=4 NOHB=1 MEMLOG=26 run ./g4 4 9 2
NOHB=1 MEMLOG=26 run ./g2s 5 10 1
NOHB=1 MEMLOG=26 run ./g2s 4 9 1
echo "### C(h): constant list = window R=0, MODE 0, unrestricted Firer; least m with WIN"
for h in $(seq 1 12); do for m in $(seq 1 60); do out=$(run ./g4w $h $m 0 0); if echo "$out" | grep -q WIN; then echo "C($h) = $m"; break; fi; done; done
echo "### window evidence, unrestricted Firer, MODE 0: all lists with a_1=0, a_i in [-R,R]"
MEMLOG=26 run ./g4w 5 14 0 3
MEMLOG=26 run ./g4w 6 19 0 1
MEMLOG=26 run ./g4w 6 19 0 2
MEMLOG=26 run ./g4w 7 24 0 1
} > log_misc.txt
echo ALLDONE >> log_misc.txt
