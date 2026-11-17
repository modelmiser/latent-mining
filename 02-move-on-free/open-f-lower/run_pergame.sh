#!/bin/bash
# periodic density game for several stage-1 layouts (3 threads, niced, memory-capped)
cd ./open-f-lower
export TMPDIR=$PWD
ulimit -v 8000000
for spec in "4,5:" "5,7:" "4,8:" "4:" "5:9,11,13" "4,4,7:9,11,13" "7:17,19" "5,5,8:14,16"; do
  h=${spec%%:*}; sz=${spec##*:}
  echo "### home=$h sizes=$sz  $(date)"
  timeout 3000 nice -n 15 ./pergame "$h" 4 8 3 "$sz" 6
  echo "exit $?"
done
