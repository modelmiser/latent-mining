#!/bin/bash
# sanity checks, witnesses and restricted (lower-bound) searches. Output: checks.txt
cd ./solver-a/ ; export TMPDIR=$PWD ; ulimit -v 7000000
g() { timeout 1500 nice -n 15 ./game "$@" | tail -1; }
{
echo "## online switch cost 0 => value must be 0"
g 3 6 0 0; g 3 6 1 0; g 4 7 0 0; g 4 6 1 0
echo "## free promise via frontier machinery (MODE 2) must equal MODE 0 (EDF): expect 1 2 2"
g 3 5 2; g 3 6 2; g 3 7 2
echo "## dominance reduction on (DOM=1), exhaustive: must equal tables.txt"
for T in 5 6 7 8; do DOM=1 g 3 $T 1; done; DOM=1 g 4 7 1; DOM=1 g 2 8 1
echo "## exhaustive S_8(4) (needs DOM=1 for memory)"
DOM=1 g 4 8 1
echo "## promise machinery vs brute-force feasibility (verify feas)"
for cfg in "3 6 1 0 5" "3 6 1 1 5" "3 6 0 0 5" "4 7 1 1 4"; do set -- $cfg
  DOM=$4 FEASK=$5 FEASDUMP=feas_tmp.txt timeout 600 nice -n 15 ./game $1 $2 $3; echo -n "mode=$3 DOM=$4: "; timeout 900 nice -n 15 ./verify feas_tmp.txt feas; done
echo "## a deliberately bad scheduler is caught: NP-EDF (POL 2) and never-switch (POL 3) where the optimum is 1"
g 3 5 1 1 -1 2; g 3 5 1 1 -1 3; g 3 5 0 1 -1 3
echo "## NP-EDF never exceeds P-1 (all adversaries)"
g 3 7 1 1 -1 2; g 3 8 0 1 -1 2; g 4 8 0 1 -1 2; g 2 8 0 1 -1 2
echo "## scheduler forbidden to idle with work pending (POL 1): same values as tables.txt"
for T in 3 4 5 6 7 8; do g 3 $T 0 1 -1 1; done; for T in 4 5 6 7; do g 3 $T 1 1 -1 1; done; DOM=1 g 3 8 1 1 -1 1
for T in 3 6 8 9; do g 4 $T 0 1 -1 1; done; for T in 4 6; do g 4 $T 1 1 -1 1; done; DOM=1 g 4 7 1 1 -1 1
echo "## 'simple rule' candidates against all adversaries, same-cost promise, P=3 (optimum is 1 for T<=8)"
echo "# POL 4 = take the first of [continue A, other jobs by deadline, switch, idle] after which the current jobs alone can still all finish by d+L"
echo "# POL 5 = take the move minimising the best achievable max lateness of the current jobs alone (same tie order)"
for T in 5 6 7; do g 3 $T 1 1 -1 4; done; for T in 5 6 7; do g 3 $T 1 1 -1 5; done
echo "## witnesses (restricted adversary => lower bounds), dumped and verified independently"
DUMP=strat_S_P2_T4.txt g 2 4 1 1 -1 0 0 2;  ./verify strat_S_P2_T4.txt
DUMP=strat_F_P2_T3.txt g 2 3 0 1 1 0 0 2;   ./verify strat_F_P2_T3.txt
DUMP=strat_S_P3_T9.txt g 3 9 1 1 1 0 1 2;   ./verify strat_S_P3_T9.txt
DUMP=strat_F_P3_T6.txt g 3 6 0 1 1 0 1 2;   ./verify strat_F_P3_T6.txt
DUMP=strat_F_P4_T10.txt g 4 10 0 1 1 0 2 2; ./verify strat_F_P4_T10.txt
PMASK=18 DOM=1 DUMP=strat_S_P4_T9.txt g 4 9 1 1 1 0 1 2; ./verify strat_S_P4_T9.txt
MAXLONG=3 UW=3 DOM=1 PMASK=18 DUMP=strat_S_P4_T16.txt g 4 16 1 1 1 0 2 2; ./verify strat_S_P4_T16.txt
echo "## the same restricted family does NOT reach 3 at T=15 for P=4 (inconclusive: restricted adversary)"
MAXLONG=3 UW=3 DOM=1 PMASK=18 timeout 600 nice -n 15 ./game 4 15 1 1 1 0 2 | tail -1
echo "## checker watched failing on corrupted inputs (each line must say VERIFY FAILED)"
sed 's/^node 0 0 1 2 9 /node 0 0 1 2 6 /' strat_S_P3_T9.txt > bad1.txt; ./verify bad1.txt | head -1
sed 's/^header P 3 T 9 L 1/header P 3 T 9 L 2/' strat_S_P3_T9.txt > bad3.txt; ./verify bad3.txt | head -1
sed 's/mode 0/mode 1/' strat_F_P3_T6.txt > bad4.txt; ./verify bad4.txt | head -1
sed 's/mode 0/mode 1/' strat_F_P4_T10.txt > bad5.txt; ./verify bad5.txt | head -1
} > checks.txt 2>&1
