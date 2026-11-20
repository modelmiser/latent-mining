solver-a working folder. All numbers in the report come from these files.

slow.py      direct labelled implementation of the definition (n <= 5); modes fifo / any
fast.cpp     exhaustive solver, states up to bin renaming (n <= 8); mode 0 fifo, mode 1 any-removal;
             writes witnesses (Adversary strategy with ranks, or Rule safe set)
check.py     independent witness checker (FIFO)
anyiso.cpp   any-removal reading, oriented forests up to isomorphism (n <= 14)
capfifo.cpp  FIFO with Adversary forbidden to offer when CAP balls are present (upper-bound search only)
greedy.cpp   exhaustive test of the fixed Rule strategy "smaller load, tie -> earlier bin"
rules.py     search over simple feature-based Rule strategies
verify7.py   replays the hand-written 7-bin load-3 strategy against all Rule answers
verify12.py  replays the hand-written 12-bin load-4 strategy against all Rule answers
proofs.txt   proofs, each marked COMPLETE / COMPUTED / SKETCH / CONJECTURE

witnesses:   adv_n4_L2.txt adv_n7_L3.txt (Adversary), rule_n3_L2.txt rule_n6_L3.txt rule_n8_L4.txt (Rule)
             rule_n8_L4.txt is 64 MB (g(8) <= 3, beyond what was asked); regenerate with ./fast 8 4 0 file
bad_*.txt, verify12_bad11.py : deliberately wrong objects, each rejected by its checker
x            leftover test output of capfifo (strategy for n=4 L=2 CAP=3)

commands:    ./fast n L 0 [witnessfile]      python3 check.py adv|rule n L file
             ./anyiso n L                    ./capfifo n L CAP [file] [log2 table]
             ./greedy n L TIE [log2 table]   python3 verify12.py
