<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `16-erasing-counter/solver-a/proofs.txt:4-31`, `16-erasing-counter/solver-b/proofs.txt:3-19`, `16-erasing-counter/solver-a/sylvester_r3.txt:1-20` — none
Q2: CONFIRMED — `16-erasing-counter/solver-a/proofs.txt:34-39`, `16-erasing-counter/solver-b/proofs.txt:20` — none
Q3: CONFIRMED — `16-erasing-counter/solver-a/proofs.txt:41-71`, `16-erasing-counter/reports/referee.md:16-18,39` — none
Q4: CONFIRMED — `16-erasing-counter/verifier/lazy.cpp:1-50`, `16-erasing-counter/solver-b/lazy.cpp:1-53`, `16-erasing-counter/README.md:48,93-99` — none
Q5: CONFIRMED — `16-erasing-counter/README.md:60-88`, `16-erasing-counter/solver-a/proofs.txt:4-39`, `16-erasing-counter/solver-b/proofs.txt:3-19` — none
Q6: REFUTED — `16-erasing-counter/README.md:44,109,124-125`, `16-erasing-counter/reports/solver-a.md:8-11`, `16-erasing-counter/prior-art.md:9` — In table row 1: "computed by all three instances (solver B and referee over all 1201 trees; solver A via lazy trajectory search)"; in What to distrust: cite "(Jansens, Bose, Carmi, Maheshwari, Morin, Smid, 2010)", and delete the unsupported sentence claiming solver A reported a 100-second wait loop.
Q7: CONFIRMED — `README.md:47-48`, `16-erasing-counter/README.md:9-16,40-58` — none
Q8: CONFIRMED — `16-erasing-counter/README.md:21-36`, `16-erasing-counter/chain/3-question.txt:1-16` — none

ROUNDS MODE. After the verdict lines and before anything else, list every problem you found, one per line:
F: MODERATE — `16-erasing-counter/README.md:124-125` — States solver A reported a wait loop ran ~100s outside run rules, but solver A's report and repository files contain no such statement — confidence: high — Delete sentence or document source
F: MODERATE — `16-erasing-counter/README.md:44` — Results table asserts D(1), D(2) computed by "all three instances over every tree", but solver A ran trajectory search lazy.cpp (482 nodes), not all 1201 trees — confidence: high — Replace with "computed by all three instances (solver B and referee over every tree, solver A via lazy trajectory search)"
F: MODERATE — `16-erasing-counter/solver-b/bad_idx.txt:1` — Described in solver-b report as testing repeated index rejection, but index 14 is in separate branches; rejected by checker only because leaf [15] is off-path — confidence: high — Place an index twice on one path so it tests repeated index detection
F: LOW — `16-erasing-counter/solver-a/proofs.txt:51` — Lemma 2(c) states every step on a zero-creation cycle reads exactly one 1; fails for fixed point 0^d -> 0^d with empty leaf (reads 0 ones) — confidence: high — Qualify with "for cycles of length > 1"
F: LOW — `16-erasing-counter/README.md:109` — Author list puts Bose first; prior-art.md:9 lists Jansens first — confidence: high — Change citation to "(Jansens, Bose, Carmi, Maheshwari, Morin, Smid, 2010)"
F: LOW — `16-erasing-counter/README.md:86` — "uses every one of the 2^r - 1 positions once" is ambiguous between node count and step usage — confidence: medium — Clarify to "uses each of the 2^r - 1 positions at exactly one internal node"

### Important matters not covered by the questions

1. **`solver-b/bad_idx.txt` negative test is invalid (MODERATE)**: `solver-b/bad_idx.txt:1` splits index 14 across 0- and 1-branches of node 13. Because no index repeats on any single root-to-leaf path, the tree's index structure is legal under the specification. It is rejected solely because leaf `[15]` writes an off-path bit. Solver B never validated that its checker catches a true repeated index on a path.
2. **32-bit state representation limit in search programs (MODERATE)**: Both `solver-b/lazy.cpp:9` and `verifier/lazy.cpp:9` use `unsigned` 32-bit bitmasks for state vectors (`1u << i`). While correct for \(r \le 5\) (\(d \le 31\)), running either program on \(r = 6\) (\(d = 63\)) causes integer overflow and incorrect transitions without triggering a compiler error.
3. **Ghost discrepancy in reported solver infractions (LOW)**: `16-erasing-counter/README.md:124-125` attributes a run-rule infraction to Solver A ("one of its wait loops ran for about 100 seconds outside the run rules"). `reports/solver-a.md` and `solver-a/results.txt` make no mention of this; it appears to be editorial commentary that has no backing source in the submitted artefacts.
