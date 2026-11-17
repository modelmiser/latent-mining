#!/bin/bash
# for each G, M: find least H with WIN, starting from H=M
for M in $(seq $1 $2); do for G in N F O; do
  H=$M
  while true; do r=$(./v3 $G $M $H); case "$r" in *WIN*) break;; esac; H=$((H+1)); done
  echo "h^$G($M)=$H   [$r ; H-1 LOSE verified]"
done; done
