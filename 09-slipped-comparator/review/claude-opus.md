<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. At solver-a/proofs.txt:12 the new prefix sum after c_i is s_{i−1}+min(x_i,x_{i+1}), which equals max(s_{i−1}, s_{i+1}−1) in all three cases; it is nondecreasing in both arguments and never above the old s_i. Belief (a), L15-16: 1^k0^(n−k) has s_j = min(j,k), the largest possible, and it is the threshold image of n…1. Belief (d), L17-18, follows from monotonicity. — none

Q2: CONFIRMED. At L24-26, an active letter means y_i=1, y_{i+1}=0. Slipped to c_{i+1} it sees (0,·) and does nothing; slipped to c_{i−1} it sees (·,1) and does nothing. With n ≥ 3 the alphabet has at least 2 letters, so c_1→c_2 and c_{n−1}→c_{n−2} always exist. If the letter was idle, w− acts like w. The (<=) direction at L21 is (d) applied pointwise. — none

Q3: CONFIRMED, with one unstated step. At L32-34, on the reversed input inversions only decrease, so each of the N pairs is exchanged exactly once and stays in order afterwards. φ persists (L36-38) because no value lies strictly between v and v+1. q_v exists because otherwise w− ends at φ(sorted). q_v is idle in w (L39). Distinctness (L42) is asserted without a reason, though it holds: in the single run of w, a letter's two wires hold exactly one pair. — "The q_v are distinct: in the run of w, q_v's wires hold {v,v+1}, and a letter holds only one pair."

Q4: CONFIRMED for the theorem; the referee's gap is real. (i), L50-53, does not cover x = 1^m: the Z_m form requires a 0, so "c=0" misses the all-ones case. I traced (i) y=1 and y=0, (ii), (iii) and the case split, and they hold. The only other defect is wording: L58 says "before it the state is sorted on wires 1..k", but it should be "before D_k". — "(x = 1^m: the new 0, if any, climbs to wire 1; sorted.)" and "before D_k the state is sorted on wires 1..k".

Q5: CONFIRMED, with a provenance gap.
- L5 (solver-b/proofs.txt:101-104) is a correct proof that does not use T2. It reuses L4's decomposition by the path of the maximum, so it is independent of solver A's route but not of L4. It is also terse: it does not say that deleting a wasted letter leaves w* unchanged.
- L4 holds as stated. I traced the cases (A) and (B) and the (A) i+1 = k−1 deletion/reinsertion at L80-88, and the wasted-letter argument at L89-93. The arithmetic at L96 is right: 9+5+…+n = n(n+1)/2−1.
- The base case u(4)=9 is in the logs, but not from the run B cites: solver-b/runup.txt (fast, pruned) has L=8: 0 and L=9: 64, and verifier/out_brute.txt (definition-level) has n=4 L=8 up=0 and L=9 up=64. No output of B's own "slow.py 4 9 up" (L5) exists. — "base: fast (runup.txt) and referee brute force (verifier/out_brute.txt)"

Q6: CONFIRMED.
- For n ≥ 3, T2 + T1(=>) gives the lower bound and T3 + T1(<=) the upper bound (L63-64).
- At n = 2, c_1 has no slip, so S(2)=1 while A(2)=2 = (n−1)(n+2)/2. The equivalence and the formula both fail for S.
- The README restricts every claim to n ≥ 3 (L7, L38) and says nothing false. The A(n) row at L35 has no range given; it holds for n ≥ 2.

Q7: REFUTED (wording only).
- The arguments themselves are correct (L47-52, L61-66).
- L55 says "upward slips only … c_(n−1) cannot slip". Under B's definition (solver-b/proofs.txt:64) an up-slip is c_i→c_{i+1}. In the README's own geometry (L15, L47-48), that moves the comparator one wire down. Read with the README's geometry, the letter that cannot slip "upward" is c_1. The n = 3 value of 4 is mirror-symmetric, so the number stands (runup.txt: 2 1 1 2).
- L52, "can only help", is loose: the comparison is with the deleted word.
- Replacement: "if slips may only go from c_i to c_(i+1) (one wire down), c_(n−1) cannot slip at all…"

Q8: REFUTED, mildly.
- "Three independent programs" (L39): all three are by one model family, and for n = 6–7 all three use the same reversed-input layered search with dominance reduction (bfs.cpp:1-5, dp.cpp:13, fast.cpp:31). They are not independent methods. Only the referee's brute force (n ≤ 5) works directly from the definition.
- The n = 7 pruning remark (L82-84) is accurate: verifier/out_dp.txt compares dom=0 and dom=1 runs only up to n = 6.
- The word counts match the logs (out_n7_m0.txt; run7dom.txt; out_dp7.txt).
- The prior-art paragraph is faithful to prior-art.md:22-24 and :37. It omits that Yao–Yao already define the deletion ("passive fault") model (prior-art.md:8).
- Row L34's "tested word by word for n ≤ 5" covers only certain lengths (referee.md:24).

Replacement: "computed by three programs (same model family; the n = 6, 7 runs use one shared method, the reversed input plus dominance reduction)".

F: MODERATE — README.md:39-40 — "three independent programs" overstates independence: all three use the same reversed-input plus dominance-reduction method for n = 6, 7, and only one of them works from the definition (n ≤ 5) — medium — reword as in Q8.
F: MODERATE — solver-b/proofs.txt:5 — the cited base run "slow.py 4 9 up" has no output in the repository; the base is supported only by fast output (runup.txt) and the referee's brute force — high — cite those logs instead.
F: LOW — README.md:55-56 — "upward slips" means c_i→c_{i+1}, which is one wire down in the README's own geometry (L47-48) — high — define the slip direction explicitly.
F: LOW — solver-a/proofs.txt:50-53 — T3(i) misses x = 1^m — high — add the one-line case.
F: LOW — solver-a/proofs.txt:58 — "before it" should be "before D_k" — high — reword.
F: LOW — solver-a/proofs.txt:42 — the distinctness of the q_v is unjustified (it is true) — high — add the reason from Q3.
F: LOW — solver-b/proofs.txt:101-104 — L5 omits that deleting a wasted letter leaves w* unchanged, and it shares L4's decomposition — medium — add that sentence, and describe L5 as "independent of T2".
F: LOW — README.md:72-78 — the paragraph leaves out that the deletion fault model is Yao–Yao's passive fault (prior-art.md:8), so a reader may think A(n) is a new notion — medium — mention Yao–Yao.
F: LOW — reports/referee.md:51 — the referee says A has no size-26 run for n = 7, but out_n7_m0.txt shows level 26 with no GOAL, which already settles it — medium — correct the referee note or the README's reliance on it.
F: LOW — README.md:34 — "tested word by word for n ≤ 5" covers only certain lengths (referee.md:24) — high — state the ranges.

Not asked:
1. The headline itself holds: I found no gap in T1, T2 or L4 that breaks it.
2. Solver A's bfs.cpp gives identical state counts per level for slip and deletion modes (out_n7_m0.txt and out_n7_m1.txt). That fits T1 under the dominance reduction, but no one remarks on it or checks it. Solver B's per-layer counts differ between the two modes while its final totals agree.
3. The solvers' C++ was never audited (referee.md:66), yet the n = 6–7 rows rest on it.
