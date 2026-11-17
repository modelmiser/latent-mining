#!/bin/bash
# exhaustive: every adversary vs rule (A,V) in game F, M=6..13
ulimit -v 8000000
for A in 0 1 2 3; do for V in 0 1 2 3 4 5; do
  echo -n "A=$A V=$V :"
  for m in 6 7 8 9 10 11 12 13; do timeout 600 nice -n 15 ./rt exh F $m $A $V | sed -E 's/.*H_needed=([0-9]+).*/ \1/;t;s/.*/ ?/' | tr -d '\n'; done; echo
done; done
