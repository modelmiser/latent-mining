<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — `solver-b/proofs.txt:8-18` vs `chain/3-question.txt:5-11`. Unlit = reachable with spread > s; empty-C states can't lie on cycles (Lemma 0), so a simple cycle with min spread > s ⇔ cycle inside unlit set; an infinite run avoiding F eventually walks a cycle in Q\F. README:24-29 ("reached by a long enough string" = non-empty C) matches. — none

**Q2: CONFIRMED** — `solver-b/proofs.txt:42-51`. (i) climb from last 0 needs ≥k ones; (ii) k ≤ h+a−(w−a), h≤K. L(k)=k for odd w (k≤(k+t)/2 iff k≤t; k=t+1 gives ⌈(2t+1)/2⌉=t+1), so C(k)⊆[k,k+t]. States K+1=⌊w/2⌋+2 ≤ w+1. My independent recomputation (/tmp script) gives max spread = ⌊w/2⌋ for w=1..11. Symmetry moves the start to K, but the lower bound never uses the start; this is only said explicitly in Theorem D(3), not A. — "add to Theorem A: 'the bound a ≥ L(k) uses only the window, hence holds from any start state'"

**Q3: CONFIRMED** — `solver-b/proofs.txt:95-125`. Identity: k−h = 2a−w+e0−eK ✓; parity ✓ (0→0,2; K→K,K−2 all even-preserving); automorphism checked on all three edge types; bounds use only h,e0,eK so transfer is legitimate. Four state-0 cases ✓ (eK≥1 ⇒ ≥K−1 zeros ⇒ a≤1≤t−1 needs t≥2; h=1 parity; h=0 forces e0≥2 using w≥2). Odd k: eK=0 ⇒ h≡k (w even) ⇒ h≤K−1 ✓; e0≥1 ⇒ a'=(k−2+m+eK')/2 ≥ (k−2)/2, integer ⇒ ≥(k−1)/2 ✓. Even inner → odd ✓. `witnesses.txt:4,6` are B_4, B_6; I recomputed g by brute-force simple cycles: all 12 lines correct; Theorem D ranges hold from every start for w=4,6,8,10. — none

**Q4: CONFIRMED** — `solver-b/proofs.txt:57-84`. a lies on the simple cycle of the 0^∞ run, so a = δ*(q0,0^n) for infinitely many n ≥ w ⇒ 0∈C(a). k<J ⇒ p_0..p_k distinct, k≥w−s ⇒ ≥w−s+1 states ✓. Theorem C: step 3's "k≥w then k=1" is valid as written (p_1=p_k with k<J forces k=1); the referee's "k=w" is a tightening, not an error. — README:96: "the referee notes 'k ≥ w' can be sharpened to k = w" (not "error")

**Q5: CONFIRMED** — `solver-b/proofs.txt:20-27`, `verifier/ref.cpp:62-95,128` (window sets propagated only along defined edges; cyc() detects cycle among sp>S). Pruning can only over-prune ⇒ reported f too large; too small would need a wrong witness, and I verified every witness independently. Provenance: `reports/solver-b.md:15-16` (unpruned to w=5, pruned w=6), `verifier/runs.log:10,15-19`, `solver-a.md:16` — README:37-40 matches. — none

**Q6: CONFIRMED** — `solver-a/proofs.txt:4-8`, `solver-b/proofs.txt:86-92`, `reports/referee.md:29`. I compared `family.split(w)` with B_w: identical for all w=3..12 (not only even w), so the statement is true but understated. — "the same machine as solver B's bouncy counter for every w ≥ 3; solver B uses it only for even w"

**Q7: CONFIRMED** — all numbers traced: 262,279,982 (`runs.log:10`), 6,000 (`referee.md:16`), 85/65 (`solver-a.md:48`, `solver-b.md:37`), w=4..12 (`referee.md:29`), "≥ 11" (`results.txt:38`), a3 (`referee.md:55`), 2-core (`solver-b.md:65`), prior-art wording (`prior-art.md:16-23,51`). One understatement: README:38 says the two solvers share one tree, but B also ran a second numbering (`final_lb.log:12,14`, 3,883,022 / 16,682,611 nodes ≠ A's). — "solver A's run coincides with solver B's BFS run; B's DFS run and the referee's two orders are separate"

**Q8: REFUTED (minor)** — top-level `README.md:49` says "values to w = 6 by exhaustive search three times" and "no general lower bound"; folder README:37 says solver B's w=6 is pruned, and f ≥ 1 is proved. Conjecture is correctly labelled open in both. — "values to w = 6 by unpruned search twice and pruned search once; no general lower bound beyond f ≥ 1"

F: LOW — `README.md:49` (top-level) — "exhaustive search three times" to w=6 contradicts folder README:37 (B unpruned only to w=5); "no general lower bound" omits f≥1 — high — wording above
F: LOW — `20-short-memory-count/README.md:38` — "two solvers visit the same tree" ignores B's DFS order (`final_lb.log:12,14`); undercounts checks — high — wording in Q7
F: LOW — `20-short-memory-count/README.md:96` — Theorem C's "k ≥ w" listed under "Small errors" though the proof is valid as written — medium — "can be sharpened to k = w"
F: LOW — `20-short-memory-count/README.md:65-66` — S_w = B_w for all w 3..12, not only even — high — wording in Q6
F: LOW — `solver-b/proofs.txt:46-47` — Theorem A's complement symmetry moves the start state to K without stating start-independence (Theorem D:100-103 does) — medium — add one sentence
F: LOW — `20-short-memory-count/README.md:30` — points to `chain/3-link.md` for the question; the formal statement is `chain/3-question.txt` — high — link both

**Not asked but worth noting (most severe first):**
- e(8)=3 and e(9)=4 upper halves rest on explicit machines, not on a listed `finde` run (`runs.log:22` has no W=8 S=3); README:44 "computed" is fine but the method differs from f.
- All three search programs share the same window-bitset design (`ref.cpp`, `fast3.cpp`); "independent" is by authorship, not by algorithm.
- My own brute force over all labelled machines confirms f(1..3)=0,1,1; w7n9/w9n11 machines verified (g=2, g=3).
