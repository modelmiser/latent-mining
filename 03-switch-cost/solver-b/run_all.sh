#!/bin/bash
# Reproduces every number in the report. One core, niced, memory-capped. Usage: bash run_all.sh > results.txt
cd "$(dirname "$0")"; export TMPDIR=$PWD
run(){ echo "\$ $*"; (ulimit -v 8000000; nice -n 15 timeout 1500 env "$@"); }
# sanity: P=1 ; online switch cost 0
run ./game 1 7 0 S; run ./game 1 7 0 F
run ./game 3 6 0 S 0; run ./game 3 6 0 F 0
# P=2
run ./game 2 9 0 S; run ./game 2 9 1 S; run ./game 2 11 0 F; run ./game 2 11 1 F
# P=3
run ./game 3 8 0 S; run ./game 3 8 1 S; run ./game 3 9 1 S 1 1; run ./game 3 8 2 S
run ./game 3 9 0 F; run ./game 3 9 1 F; run ./game 3 9 2 F
# P=4
run ./game 4 8 0 S; run ./game 4 8 1 S; run ./game 4 9 1 S 1 1
run SLACKMAX=4 LENS=14 ./game 4 15 2 S 1 1
run ./game 4 10 0 F; run ./game 4 10 1 F; run ./game 4 10 2 F; run ./game 4 9 3 F
# restricted schedulers (full adversary): 1 = never an empty tick while free with work pending; 2 = never switch/idle; 3 = non-preemptive EDF
run SCHED=1 ./game 3 8 1 S; run SCHED=1 ./game 2 8 0 S; run SCHED=2 ./game 3 6 1 S; run SCHED=3 ./game 3 8 2 F; run SCHED=3 ./game 4 8 3 F
# hindsight-set dominance pruning off (cross-check of Lemma D)
run NODOM=1 ./game 3 7 1 S
