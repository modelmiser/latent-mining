<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — `solver-b/proofs.txt:9-34`. W is an interval containing the position, so a fault is always adjacent to W (line 10); round 1 loads {0,1} and counts as pair fault 1; pair faults stop at |W|=2h for both parities (11-13, 25-26). Invariant (1) holds (pair fault at p=b+1 needs b visited, so only b+2 is new-unvisited on that side); visited ≥ 2f1−2 ⇒ opt ≥ f1−1. In (2): run-0 pages a0−1..a0−j0 are new, a0 visited ⇒ ≥2h−1+j0 distinct; after any run's last fault W=[x0,x0+k−1] (checked both parities); next run visits k+j distinct pages in (t0,t1], offline holds ≤k after t0 ⇒ ≥j entries; intervals disjoint; h+f2 ≤ 2opt+1−h. — none

**Q2: CONFIRMED** — `solver-b/proofs.txt:37-61`. Stage 0: pair schedule gives every prefix opt ≤ g ≤ m (2g ≤ k, no eviction); 2g−1 absent pages ⇒ ≥g faults; m<h stops at m+1. Free step: offline hits. Cycle: if |O|=k then C=O; if k−1 one of a±1 absent; (i)/(ii) traced for |O|∈{k−1,k}: x ≥ a+1 resp. a, new O ⊇ [x,b+1], |O|=k after. The deferred choice is one action at one fault of one explicit schedule, and legality of b+1 is witnessed by either branch (h+#cycles+1 ≤ m). Count h+1+2(m−h). — none

**Q3: CONFIRMED** — `solver-a/proofs.txt:8-47`. Lemma 1 trivial; Lemma 2's N ⊆ O and ⊆ requested pages, (ii)-events injective to prefetches ⇒ ≤2f. Lemma 3 is stated and used inside the NP (demand) model only; far end has latest next-request on a path. Theorem U: t* exists (line 42-43), N has ≥2j−1 faults by t*, cache ⊆ V ⊆ I; phase-2 FILL-FAR = Lemma-3 rule from I. — none

**Q4: CONFIRMED** — `solver-a/proofs.txt:52-90`. (i)-(iii) hold; zeros ⊆ {3..k−1} non-adjacent ⇒ ≤ j−1 for both parities (2j−3 or 2j−2 slots); stage-1-only case gives ≤ m−1 zeros in {3..2m}. Blocks: C=O (|O|=k) or x is the fixed uncached frontier (k odd, first block) ⇒ fault; single/pair each ≥2. Tiling legality and block schedules check; small cases k=2,m=1 and k=3,m=1 traced. — none

**Q5: CONFIRMED** — `solver-b/proofs.txt:68-75`, `solver-a/proofs.txt:96-103`. Invariant real−virtual ≤ |V₀\R₀|−|V\R| holds in all four cases (mutual domination impossible for C≠C'). Code: `solver-b/game.py:38-49` drops dominated offline entries and restricts Online to inclusion-maximal (`subsets_moves` maximal); `solver-a/game.py:28-44` same plus "prefetch the uncached neighbour", which is inclusion-maximality. — none

**Q6: CONFIRMED** — `chain/3-question.txt:7-9` vs `solver-b/game.py:54-61,97` (round-1 fault counted as 1+min), `wf_step` returns None when no offline entry with f≤m can serve the new request (legality including new request), V initialised 0 = stop, least fixpoint handles infinite play; proofs count round 1 (B line 10, A line 67) and bound every prefix (B line 34). — none

**Q7: CONFIRMED** — `verifier/table_mode2.txt` (k=2..14, m=1..5, 65 lines) + `verifier/table_m6.txt` (k=2..14) = 78 entries, all EQ; m=7,8 partial in `table_m7/m8.txt`; README table k≤12,m≤6 is a subset. Solvers: `solver-a/table_py.txt` k≤12,m≤5 minus (7,5); `solver-b/table_py.txt` k≤10,m≤5, m=6 k=2,3,4 + `table_py_extra.txt` k=10,12. — none

**Q8: REFUTED (one cell)** — README:42 "replayed each adversary against every online reply for k ≤ 13, m ≤ 6": `verifier/adva.txt`, `advb.txt`, `advb_m6.txt` are all `mode=1` (Online restricted to inclusion-maximal replies); all replies only in `adva_mode0.txt`/`advb_mode0.txt` (12 cases, k≤7, m≤4). — "replayed each adversary against every inclusion-maximal online reply for k ≤ 13, m ≤ 6, and against every reply on 12 cases up to (5,4)".

F: MODERATE — README.md:42 — Claims replay "against every online reply"; the logs restrict Online to inclusion-maximal replies except 12 small cases. Lemma (b) justifies that restriction for the game value, not for the forced-fault count of a fixed cache-reading adversary, so the computational check of the lower-bound adversaries is weaker than stated (proofs themselves cover all replies). — high — wording above.
F: LOW — README.md:112-113 — "did not find the two combined": `prior-art.md:15-16` item 5 (Gaysinsky–Itai–Shachnai) combines access-graph walks with prefetching, differing in objective. — medium — "did not find neighbour-only prefetch on a path with an exact bounded-opt value".
F: LOW — README.md:73 — "away from the side the online cache prefetched": if Online loads nothing extra in round 1, either side is used (`solver-b/proofs.txt:38`). — high — "toward a side whose first page is not in the online cache".
F: LOW — README.md:58-59 — "once m ≥ k/2": the saving is ⌊k/2⌋−1 already for m ≥ ⌊k/2⌋ (k=3, m=1: D=0). — high — "once m ≥ ⌊k/2⌋".
F: LOW — README.md:79 — "k − 1 once, for odd k": the (k−1)-window persists through all free steps before the first cycle completes. — medium — "k − 1 until the first cycle, for odd k".
F: LOW — solver-b/proofs.txt:75 — claims unpruned agreement on "all k=2..4, m=1..3"; `table_py_noprune.txt` has only k=3..8, m=2..4 (k=2 and m=1 absent). — high — delete the k=2..4, m=1..3 clause or add the log.
F: LOW — solver-b/proofs.txt:68-73, solver-a/proofs.txt:96-102 — pruning (b) is proved for a fixed future; the transfer to the perfect-information game (adversary sees the real, not the simulated, cache) needs the remark that legality depends only on the walk. — medium — add one sentence.
F: LOW — solver-a/proofs.txt:20-27 — Belady's theorem is invoked for "all NP schedules", which may evict more than one page per fault; the classical theorem is for lazy demand paging (the lazy conversion is standard but unstated). — low — add "non-lazy NP schedules can be made lazy without more faults".

Not asked: the referee's zero-fault-cycle detector (`wk.cpp:231-236`) only follows hit edges from nodes in the BFS, which is adequate; and `solver-a/game.py:89` caps values at 2m+3, so an unbounded strategy would print as a finite value — harmless here since all values are below cap, but worth a note.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
