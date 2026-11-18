solver-b files
 slow.py            direct definition (explicit orientations, Algorithm may re-orient the whole forest); T(1..10)
 fast.cpp           exact solver on charges; mode 0 normal-form replies, mode 1 all orientations of merged tree (check, n<=9)
 exactv.cpp         exact value of EVERY reachable position, compared with formula F=P-1 of Lemma 2 (n<=11: always equal)
 fast2.cpp          boolean solver + F cutoff + restricted adversaries (ADV 1..7); memory heavy
 fast3.cpp          lean version of fast2 (exact runs n<=15; ADV 3,7,8,9) ; fast3b.cpp = same with 16-bit fields (n>=128)
 fast3bonus.cpp     heuristic gadget search (terminal bonus for a 'bad' vertex)
 certgen.cpp        writes lower-bound certificates (adversary strategy)  -> cert_n*_k*.txt
 check_cert.py      independent checker for those (python, explicit orientations)
 upgen.cpp          writes upper-bound certificates (closed set of bounded positions) -> upper_n*.txt
 check_upper.py     independent checker for those (needs no lemma)
 gdp.py             numeric greedy upper bound
 proofs.txt         proofs (COMPLETE / SKETCH)
 *_MUTANT.txt       deliberately corrupted certificates; the checkers reject them
Certificate text: a tree is '(' digit children ')' with digit = 2 - outdegree of the vertex; a position is a list of trees.
 cert line : k | position | treeA rootedAtU | treeB rootedAtV   = "from this position, to force >= k more, name u,v"
 upper line: b | position                                        = "from this position Adversary cannot force > b"
