<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `NOTE.md:467-489` is a legal rule-R play (ties allowed at `NOTE.md:338-339`): I(1) holds, both rounds cost j−1, the second tie forces p=x so the new out-neighbours are y and z, and c_j=(3^j−1)/2−j; hand replay j=2 (cost 2, H=2) and j=3 (cost 10, H=3) matches — none

Q2: UNSUPPORTED — `NOTE.md:498-504` holds (deficit (1/2)Σd_i+Σ i d_i ≤ (J+1)^2; rule R sees only the named trees). `greedy_exact.cpp:213-217` with `-DGREEDY` and S≥n keeps minimum-cost paths and lets the Adversary max over ties, but no output file records G(n)=f(n) or G(14)=3 (f(14)=4 does follow from the recurrence) — attach that log or delete `NOTE.md:505-506`

Q3: CONFIRMED — `NOTE.md:358-441`; the k=0 split, the path induction, the four w_0 cases and E≤0 in all three H cases check out. Smaller-tree counterexample `NOTE.md:341-344`: 7-vertex root (cost 2) against the 8-vertex tree, Φ goes from 7/2 to 5, rise 3/2 < 2 — none

Q4: REFUTED — Theorems 3 and 4 at `NOTE.md:20-24` match their proofs, but `NOTE.md:32-35` is not justified: Theorem 4 needs adversarial ties, and `NOTE.md:503-504` plus `NOTE.md:696-699` leave a fixed tie-break open. (b) no: `NOTE.md:340-344`, `README.md:56` deny the smaller-tree rule; `NOTE.md:26`, `README.md:18,58`, top `README.md:45` give 1/3≤L≤1/2. (c) `compare_bounds.py:10-11` checks g≤f only through N (default 4000), not every n≥94 (`NOTE.md:463`) — Summary: “an upper bound below 1/2 needs another algorithm, or a tie-break of this one that is not analysed.” Comparison: “g(n)≤f(n) for 94≤n≤4000.”

Q5: CONFIRMED — `ternary_family.py:21-28` asserts h(p)≤h(q) and follows an h−1 child on every inductive insert; `pool_search.cpp:52-64` flips a shortest path only from a minimum-h endpoint and counts negative slack; `lemma4_check.py:67`, `lemma5_check.py:33`, `pool_search.cpp:87`, `compare_bounds.py:15` exit non-zero on a detected failure (asserts abort the ternary script) — none

Q6: CONFIRMED — j=2 pays 2 on 9 vertices and j=3 pays 10 on 27 vertices, both ≤ ⌊(n−3)/2⌋ (3 and 12) and ≤ the size recurrence (f(9)=2, and f(27)≥10). No clash with Theorem 1, B(n) for n≤15, or the certificates T(30)≥7, T(32)≥8, T(64)≥18 — none

F: MODERATE — `NOTE.md:32-35` and `14-forest-orientation/README.md:16-17` — “needs a different algorithm” does not follow from an adversarial-tie lower bound; a fixed tie-break of rule R is still open (`NOTE.md:503-504`) — high — use the wording already at `NOTE.md:696-699`
F: MODERATE — `NOTE.md:463` — “g(n)≤f(n) for every n≥94” is stronger than `compare_bounds.py:10-11`, which stops at N — high — limit the claim to 94≤n≤4000
F: LOW — `NOTE.md:491` — H=j is not proved, only h(root)=j via I(j); it is true for j=2 and j=3 by hand (degree <2 forces h=0) — high — add the one-line bound that no h exceeds j
F: LOW — `NOTE.md:370` — “for some” does not state the induction; the step from g_0 is valid if read as one — high — “for each”
F: LOW — `NOTE.md:20` vs `open-limit/proofs.txt:156` and `README.md:53` — two different Theorem 3s; round 5’s rename was not done — high — rename the pair-game result in citations
F: LOW — `NOTE.md:457` — “paths of length up to 6” is not in `pool_search.cpp`; only a run of `byk` could show it — high — say “whatever lengths that run produced”
F: LOW — `compare_bounds.py:10-11` — if N<126 the “from 126” check is vacuous and the script can still print OK — medium — refuse N<4000, or require the ranges to lie inside 1..N


<!-- stderr: Connection lost, reconnecting to https://agentn.global.api5.cursor.sh (attempt 1)...
Retry attempt 1...
 -->
