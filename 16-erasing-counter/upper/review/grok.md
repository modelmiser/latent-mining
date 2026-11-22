<!-- Archived by the organiser on 2026-11-21. Report of the Grok seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `chain/3-question.txt:9`, `upper/PROOFS.md:16-22` — every bit read is erased and at most one of them is set, so a step that reads c ones changes the weight by 1−c or −c and raises it only at a writing all-zero leaf; the first 1 on Z is erased and at most one bit is written, so the Z-count does not rise — none

Q2: CONFIRMED — `upper/PROOFS.md:32-45` — Case A never ends at λ0, so the weight cannot rise and an empty step would drop it. In Case B an empty λ0 would fix that state, so λ0 writes some z_j0; R lies on the all-zero path, so it cannot write a data index. F2 keeps Z≤1, homes are the data bits, and each excursion is the λ0 step, then (α) steps, then one (β) or (γ); only (γ) is empty — none

Q3: CONFIRMED — `upper/PROOFS.md:3-4,48-59` — a graft either grows K or extends a chain of distinct modes, so F is finite; a kept index is not in K, and a data index removed because it is already in K (including one read in an earlier mode) is already on the F-path, so leaves stay legal. idx(F)⊆idx(T)\Z has size at most m−k<m, and the induction is on that count at any depth, so a deeper F is allowed — none

Q4: CONFIRMED — `upper/PROOFS.md:40,61-73` — an (α) step erases exactly the data indices just added to K, matching the invariant, and F reads each of them once; a later constant-K return to the same mode repeats the state and never reaches a home, so no dummy is hit. The terminal leaf is f_F, empty exactly for (γ). The x_t are pairwise distinct, so the period is H and the induction hypothesis applies to F. The depth-3 orbit below has H=4 and one empty step — none

Q5: CONFIRMED — `upper/PROOFS.md:91-115` — a leaf writing the root erases its subtree and writes none of those bits, like the empty leaf. Case I is a cycle of g0; case II is an all-empty cycle of g1, hence length 1 by Theorem E; case III sends every a=1 state through the single exit, giving |C|≤|C1|+|C0||C1|. T0° has depth ≤r−1, and disjointness above level j passes to the children above level j−1. With j=r−4 this forces an LCA at depth ≤r−5 — none

Q6: CONFIRMED — `upper/PROOFS.md:125-135`, `solver-a/proofs.txt:14-30` — T0° and T1° stay read-once with the same L, so RD gives length ≤L(T). Changing T0’s empty all-ones leaf to write a does not change the map on T0’s bits; the orbit is q blocks of p steps at a=0 plus one T1 step, and T’s all-ones leaf is T1’s, used once. Read-once trees therefore reach exactly W_r, and D(r)=W_r iff reuse never beats that — none

Q7: CONFIRMED — `upper/PROOFS.md:142-150` — Z≤1 gives at most (k+1)2^(m−k) states. If k<r then m≤2^r−2^(r−k), and (k+1)2^(j+1−2^j)≤r+1 for j≥1. The values are 3·2^1=6, 4·2^4=64, 5·2^11=10240, and 6·2^26=402653184≈4.03×10^8 — none

Q8: CONFIRMED — `upper/PROOFS.md:155-164` — hand replay gives 01→00→10→01 (length 3; both modified subtrees have only fixed points; product 2; L=6) and 0000,1000,0001,1001,0100,0010,1010,0011,1011,0110 (length 10; four states with bit 1 set; L(T1)=3, L(T0)=4, L=15, product 6). P2 failures are 0 at depth ≤2 and at three indices (`out_brute_r2.txt:7`, `out_brute3_r3_m3.txt:35-40`) — none

Q9: REFUTED — `16-erasing-counter/README.md:62` — not every new sentence follows: “depth r−5 or higher” is the reverse of `PROOFS.md:115` (“depth ≤ r−5”). The RD row (`README.md:60`), the excursion and product account (`README.md:107-129`), the counts 5478539 and 383161 (`out_brute3_r3.txt:67,74`) and 40000 (`out_checkF.txt:1-2`), and the conjecture rows (`README.md:64-65`) do match, and the top-level row (`README.md:47`) does not present D(r)=W_r as proved. Replace the depth clause with “lowest common ancestor at depth at most r−5”, and “four” (`README.md:134`) with “five”.

F: MODERATE — `16-erasing-counter/README.md:62` — “separate at depth r−5 or higher” reads as depth ≥ r−5, but the argument gives LCA depth ≤ r−5; the r=5 clause (“across the root”) only matches the ≤ reading — high — write “lowest common ancestor at depth at most r−5 (at depth 5, the root)”
F: LOW — `16-erasing-counter/README.md:134` — “four natural strengthenings”, while `upper/PROOFS.md:168-172` lists (i)–(v) — high — “five”
F: LOW — `16-erasing-counter/README.md:143-144` — “The organiser read the proofs and traced the two small examples by hand” is not in `PROOFS.md` or the cited outputs — high — delete it, or cite a source that says so
F: LOW — `upper/PROOFS.md:57-58` — leaf legality is spelled only for a leaf of the current S_i; a leaf reached after a later graft is the same argument with K′ and is not written out — medium — add that sentence for the grafted call
F: LOW — `16-erasing-counter/README.md:16-17` — the upper bound is smaller than the state space by 2^r/(r+1), so “close to the number of memory states” is only fair of the exponent — medium — “within a factor 2^r/(r+1) of the number of states”

The depth-5 and “LCA ≤ r−5” corollaries take D(4)=1806 as a computed input (`PROOFS.md:107`); they are not proved from the model alone. The page already says that search is one design.
