solver-a files
 slow.cpp / slowred.cpp   definition-level brute force (slowred: complement-form configs only, also counts private pairs)
 fast.cpp                 exhaustive search, usage: fast n m mode sym [K k]   (mode 0 any R, mode 1 R=Z_m; K k = part k of K)
 fastw.cpp, fasth.cpp     variants: WEAK=1 (correctness weakened to M(1^n) != M(0^n)), histogram of private R sizes
 check.py                 independent witness checker (definition), witness files w_*.txt, rejected files bad_*.txt
 strands.py strands2.py strands_ilp.py zscan.py zilp.py zscanR.py   LP/ILP for the strand family (proofs.txt L5)
 strandsearch.cpp strand2.cpp strand3.cpp   exact hash-join searches for b strands of length a
 mkwit.py                 strand list -> witness file
 anneal.cpp               heuristic search (found nothing for n=3; no evidential value)
 logs: crosscheck1.txt fast_n2*.txt fast_n3_*.txt fast_3_14_p*.txt fast_3_15_p*.txt fast_n4*.txt weak_n3.txt hist_n3.txt
       strands_out*.txt strands_n4*.txt zscan_n3_a6.txt zilp_n3_a6_p*.log strand2_*.txt strand3_*.txt zscanR_*.txt
 proofs.txt               lemmas L1-L6 and the safety argument for the pruning
