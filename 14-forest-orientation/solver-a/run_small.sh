#!/bin/bash
# Reproduces the small tables and cross-checks. Run from this directory:
#   (ulimit -v 8000000; timeout 1500 nice -n 15 bash run_small.sh)
export TMPDIR=$PWD
echo "== reference (whole-forest re-orientation, no pruning) vs lazy branch-and-bound vs budget solver =="
for cap in 1 2; do for n in 1 2 3 4 5 6 7 8 9; do
  ./fast full $n $cap; ./fast fast $n $cap; OPTLIM=0 ./fast3d $n $cap | tail -1
done; done
echo "== exact T(n), cap 2, n = 10..14 (fast = exact value B&B ; fast3d = budget search) =="
for n in 10 11 12 13 14; do ./fast fast $n 2; OPTLIM=0 ./fast3d $n 2 | tail -1; done
echo "== cap 1 sanity table (not asked) =="
for n in 10 11 12; do ./fast fast $n 1; done
echo "== W_j(n) (terminal payoff psi_j), exact, n = 1..12 =="
for j in 1 2 3; do for n in 1 2 3 4 5 6 7 8 9 10 11 12; do OPTLIM=0 TERMJ=$j ./fast3d $n 2 | tail -1 | sed "s/^/J=$j /"; done; done
echo "== restricted Adversary (blocks of 4 only), n=16 =="
OPTLIM=0 ./fast3d 16 2 5 | tail -1
