#!/bin/bash
# driver.sh H M LOG A2LIST...   : runs g2 in chunks (each chunk: one process, timeout 1500, nice 15, ulimit -v 8e6)
# claims each a2 via mkdir lock (so two drivers can share the work). No chunk is started between 04:20 and 05:45.
cd solver-b || exit 1
H=$1; M=$2; LOG=$3; shift 3
for a2 in "$@"; do
  mkdir "locks/h${H}m${M}a${a2}" 2>/dev/null || continue
  a3=-999
  while true; do
    while true; do now=$(date +%H%M); if [ "$now" -ge 0420 ] && [ "$now" -lt 0545 ]; then sleep 60; else break; fi; done
    out=$( (ulimit -v 8000000; ulimit -c 0; A2=$a2 A3START=$a3 TLIM=800 MEMLOG=${MEMLOG:-27} nice -n 15 timeout 1500 ./g2 $H $M 1) 2>&1 ); rc=$?
    echo "$out" | grep -v "^done" >> "$LOG"
    echo "$out" | grep "^done" | tail -1 >> "$LOG"
    if echo "$out" | grep -q "WIN"; then echo "WIN FOUND a2=$a2" >> "$LOG"; exit 0; fi
    if echo "$out" | grep -q "^NEXT"; then a3=$(echo "$out" | grep "^NEXT" | sed 's/NEXT a3=//'); continue; fi
    if echo "$out" | grep -q "^A2DONE"; then break; fi
    if echo "$out" | grep -q "no winning list"; then echo "A2TRIVIAL $a2" >> "$LOG"; break; fi
    echo "FAIL a2=$a2 a3start=$a3 rc=$rc" >> "$LOG"; exit 1
  done
done
echo "DRIVER FINISHED $H $M : $*" >> "$LOG"
