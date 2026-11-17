#!/bin/bash
# usage: run.sh G M Hstart [mask]  -- increases H until manager wins
G=$1; M=$2; H=$3; MASK=${4:-7}
export TMPDIR=./solver-a
ulimit -v 8000000
while true; do
  out=$( { /usr/bin/time -f "%es %MKB" nice -n 15 timeout 1500 ./solve $G $M $H $MASK; } 2>&1 )
  echo "$out" | tr '\n' ' '; echo
  echo "$out" | grep -q MANAGER_WINS && break
  echo "$out" | grep -q ADVERSARY_WINS || break
  H=$((H+1))
done
