#!/bin/bash
# Compare solver A's pruned search (their binary dfs3, run read-only from here) with the referee's unpruned results, per k.
# usage: cmp_dfs3.sh n L mode   (mode = brute | route)
cd verifier
ulimit -v 8000000; export TMPDIR=$PWD
n=$1; L=$2
if [ "$3" = brute ]; then DUMP=1 nice -n 15 timeout 1500 ./ver brute $n $L 2> mine_${n}_${L}.tmp >/dev/null
else DUMP=1 FAST=1 nice -n 15 timeout 1500 ./ver route $n $L 2> mine_${n}_${L}.tmp >/dev/null; fi
for k in $(seq 0 $((n-1))); do
  OMP_NUM_THREADS=2 nice -n 15 timeout 1500 ../solver-a/dfs3 $n $L $k 100000000 | python3 -c "
import sys
k=$k; S=set(); leaves='?'
for ln in sys.stdin:
    t=ln.split()
    if t and t[0]=='REPAIRABLE':
        s=t[1]; S.add(min(s[i:]+s[:i] for i in range(len(s))))
    elif 'leaves=' in ln: leaves=[x for x in t if x.startswith('leaves=')][0]
mine=set(l.split()[0] for l in open('mine_${n}_${L}.tmp') if int(l.split()[1])==k)
def miss(s,n=$n):
    return 2**n-len(set((s+s)[i:i+n] for i in range(len(s))))
wrongk=sum(1 for s in S if miss(s)!=k)
print(f'n=$n L=$L k={k}: dfs3(pruned) necklaces={len(S)} [with missing!=k: {wrongk}] {leaves}; referee unpruned with exactly k missing={len(mine)}; pruned set contains all of mine: {mine<=S}')
"
done
rm -f mine_${n}_${L}.tmp
