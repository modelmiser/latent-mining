#!/bin/bash
# exhaustive: every adversary vs rule in game F.  columns M=6..12.  args: A V COSTMODE
ulimit -v 8000000
echo -n "A=$1 V=$2 cm=$3 :"
for m in 6 7 8 9 10 11 12; do COSTMODE=$3 timeout 900 nice -n 15 ./rt2 exh F $m $1 $2 0 27 | sed -E 's/.*H_needed=([0-9]+).*/ \1/;t;s/.*/ ?/' | tr -d '\n'; done; echo
