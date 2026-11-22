<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — Attachment A:39-58; Attachment B:14-22 — none
Q2: CONFIRMED — Attachment A:60-84; Attachment B:36-40 — none
Q3: CONFIRMED — Attachment A:86-134; Attachment B:42-76; hand-computed 10 cases: (b) [α=0,o=0,ε_c=0,ε_n=1,ε'=0: gain=2, 3+π(0,F,F)−π(0,F,T)=2], [α=1,o=0,ε_c=0,ε_n=1,ε'=1: gain=3, 3+π(1,F,F)−π(0,F,F)=3], [α=2,o=0,ε_c=0,ε_n=1,ε'=0: gain=2, 3+π(2,T,F)−π(0,T,F)=2], [α=0,o=1,ε_c=0,ε_n=0,ε'=0: gain=3, 3+π(0,F,T)−π(1,T,T)=3], [α=1,o=1,ε_c=0,ε_n=1,ε'=0: gain=3, 3+π(1,F,F)−π(1,T,F)=3]; (a) [α=0,β=0,ε_y=1,ε_c=0,ε_n=1: gain=2, 2+π(0,F,F)=1], [α=1,β=0,ε_y=1,ε_c=0,ε_n=1: gain=1, 2+π(1,T,F)=1], [α=0,β=0,ε_y=0,ε_c=0,ε_n=0: gain=2, 2+π(0,F,T)=2]; (c) [ζ=0,ε_z=0,o=0,ε'=1: gain=2, 1−π(0,F,F)=2], [ζ=1,ε_z=0,o=0,ε'=0: gain=1, 1−π(0,T,F)=1] — none
Q4: CONFIRMED — Attachment A:136-150; 14-forest-orientation/NOTE.md:370-377,444-453 — none
Q5: CONFIRMED — Attachment B:14,31-79; real rounds with heights ≥ R are not covered by the script (covered by Attachment A's finite reduction); running `third_check.py 0` executes 0 checks and vacuously prints ALL OK with exit code 0 — none
Q6: CONFIRMED — 14-forest-orientation/NOTE.md:480-496,526-535; Attachment A:168-170; rounds: (1) k=1, h(z)=5, h(c_0)=0, h(y)=0: Φ rises from 1 to 4 (ΔΦ=3=3k); (2) k=1, h(c_0)=5, h(y)=5, h(w_1)=0: Φ rises from 2 to 15 (ΔΦ=13 ≥ 3k); (3) in round 2 {x,z} of Thm 4, x has slots (j−2, j−1) while z has (j−2, j−2), so rule L chooses z since (j−2, j−2) <_lex (j−2, j−1), flipping z→x and defeating the Adversary's construction — none

F: MODERATE — Attachment A:158-164 — Cited verification artefacts (`phiU_c3.py`, `pathcheck.py`, `gpoolC3.cpp`, `runC3.sh`, `cex1_c3.py`, `ruleL_emit.py`) are not present in the repository — confidence: high — Commit the supporting programs to the repository alongside `third_check.py`.
F: LOW — Attachment B:14 — `third_check.py` allows command-line argument R ≤ 0, executing 0 assertions and exiting 0 with `ALL OK` — confidence: high — Validate argument with `if R < 4: sys.exit("R must be >= 4")`.
F: LOW — Attachment A:141-143 — Text states slots of path vertices are non-decreasing without explicitly highlighting that w_k gaining a second out-neighbour raises its missing slot from −1 — confidence: high — Explicitly state that gaining an out-neighbour corresponds to increasing the missing slot from −1 to its non-negative height.

### Important matters not asked about

1. **Uncommitted verification programs:** Attachment A cites extensive empirical verification runs (e.g. `pathcheck.py 5 4` over 10.2M path profiles, `phiU_c3.py 7 12` over 634k tree rounds, `gpoolC3.cpp`, `cex1_c3.py`), but none exist in the repository. As with earlier review rounds, these artifacts should be committed before the draft is accepted.
2. **Lean formalization:** Theorem 1 is formalized in Lean 4 (`FO/Main.lean`), but Theorem 5 and rule L are unformalized. Because two previous candidate potentials broke on subtle higher in-degree and branch structures, formalizing Lemma 6–8 in Lean would provide definitive mechanical assurance.
3. **Choice of shortest path:** Rule L allows any shortest directed path from $x$ to be flipped. Lemma 8 and Theorem 5 hold regardless of which shortest path is picked, because only the path distance $k = h(x)$ and out-neighbour height bounds $a_i \ge k - i - 1$ are utilized.
