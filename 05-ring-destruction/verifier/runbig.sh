#!/bin/bash
# stream id $1; each line: n k target
cd verifier
ulimit -v 8000000
while read n k T; do
  /usr/bin/time -f "  [%es %MkB]" nice -n 15 timeout 1500 ./bfs $n $k 1 1 - 0 $T 2>&1 | grep -v "^n=.*layer [0-9]* states" ; echo "  exit ${PIPESTATUS[0]} for $n $k T=$T"
done
