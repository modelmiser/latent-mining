#!/bin/bash
cd solver-b
ulimit -v 6000000
for a in "5 6" "6 6" "6 7"; do
  /usr/bin/time -f "  time=%es mem=%MkB" nice -n 15 timeout 900 ./clock $a 0 28 2>&1 | grep -v "^nodes="
  echo "exit=${PIPESTATUS[0]} for $a"
done
