#!/bin/bash
# blocker-family runs (FILTER=2, alpha-beta) on doubled super-periods; prediction = 2/3 + (1/4)(1/3 - B/M)
cd ./open-f-lower
export TMPDIR=$PWD FILTER=2 PRUNE=1
ulimit -v 8000000
for h in "6,6" "9,9" "4,5,4,5" "7,7,4" "5,7,5,7" "4,8,4,8" "5,5" "4,4,7,4,4,7" "4,4,4,5,5,5"; do
  echo "### home=$h  $(date)"
  timeout 2400 nice -n 15 ./pergame2 "$h" 4 8 3 "" 3
  echo "exit $?"
done
