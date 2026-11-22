<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 1: experiment 23 (gossip-tug)

Paths are relative to `23-gossip-tug/`. I replayed both proofs by hand, read `solver-a/check.py`, `solver-b/check_cert.py` and the checker and cut-off code in `verifier/g6.cpp`, and recomputed A(2..6) = 1, 3, 6, 9, 12 and the variant 5, 8, 10 (n = 4..6) with a 25-line raw minimax of my own.

## Verdicts

- **Q1 CONFIRMED.** After {1,2}, agent 1's legal partners are 3, 4 only; 3 and 4 are interchangeable. After {1,3}: K1 = K3 = 123, K2 = 12, K4 = 4; agent 2's legal partners are exactly 3 (3 not in K2) and 4. Reply 3: secret 4 has one knower, at most one more per call, so 3 more calls. Reply 4: K2 = K4 = 124; {2,1}, {2,4} dead, {2,3} forced; then K2 = K3 = 1234, K1 = 123, K4 = 124; agent 3 is full, {3,1}, {3,2} dead, {3,4} legal (3 not in K4), forced; K1 still lacks 4. Symmetry is used soundly (first call arbitrary; 3/4 swap fixes the position). Naming a full agent is allowed: `chain/3-question.txt:3` says "any one active agent", and line 1 defines active by legal calls only. `solver-b/proofs.txt:25-37`, README:58-73.
- **Q2 CONFIRMED.** `solver-a/proofs.txt:58-69`. An earlier partner p_i has 1 in K_{p_i} and p_i in K_1, so the pair is dead; agent 1 stays active until K_1 is everything, which takes exactly n − 1 new partners. Chain, two experts, union = larger member, "new expert only if the other was one" all hold. Phase 2 is independent of the Delayer. n = 2 gives 1, n = 3 gives 3.
- **Q3 CONFIRMED.** `solver-b/proofs.txt:6-19`. Lemmas 1 and 2 hold for every play; Lemma 3's Hastener rule is always legal, adds a full agent every call and preserves the hypothesis. Implementations match the statements (`verifier/g6.cpp:92-102, 180-183`; `solver-b/ab2.cpp:63-69`). A wrong lower bound would bias up, a wrong upper bound down; none is wrong.
- **Q4 CONFIRMED for the checkers, REFUTED for what the folder contains** (F1). `solver-a/check.py:52-76` is sound for both sides: missing position, inactive named agent, missing or illegal reply all reject; every stored move is carried through the same relabelling that produced the key, so soundness does not depend on the canonical form being canonical. Relabelling is sound. n = 9, 10 provenance is as stated (logs `solver-b/n10d.log`, `n10e.log`, `verifier/thr10_g6*.log`, `thr9_nolem.log`).
- **Q5 CONFIRMED for the game; scope defect** (F3).
- **Q6 CONFIRMED.** Both quoted sentences are true in their context ("at most one fully informed agent" is said inside the chain argument, where it holds). No sentence goes beyond the proof files.
- **Q7 CONFIRMED with small defects** (F4–F6). 5, 8, 10, 14, 16; the differences 2, 3, 3, 3, 4, 3, 4, 4; 168M/203M-node referee runs; OEIS A053096; attributions all match `reports/referee.md` and `prior-art.md`. I did not recompute 14 and 16, and no log for them is in `verifier/`.
- **Q8 REFUTED in two places** (F1, F2).

## Findings

**F1. MODERATE, confidence high. Strategy files the README and top-level row rely on are not in the folder, and the "left out" list is incomplete.** `solver-a/check.log:8-9` accepts an n = 7 Hastener file and an n = 8 Delayer file, but `solver-a/` contains no `cert_n7_H.txt` and no `cert_n8_D.txt`. README:131-133 lists only solver B's two omitted files. With B's `table7.txt` also omitted, the folder holds no upper-side file at n = 7 and no Delayer file at n = 8. So README:97-99 ("Up to n = 7 there are strategy files for both players… At n = 8 only the Delayer's side has one"), README:46-47 and top-level `README.md:54` ("checkable strategy files for both players up to 7 people and for one player at 8") describe files a reader cannot check. Replacement for README:131: "Four files over 300 kB are left out: solver A's Hastener strategy for n = 7 and Delayer strategy for n = 8, solver B's value table for n = 7 and Delayer strategy for n = 8. In this folder the files cover both players up to n = 6 and the Delayer at n = 7; the rest must be regenerated with `solver-a/gen.cpp` and solver B's programs." Adjust README:97-99 and the top-level row to "included for both players up to 6 people and for one player at 7; larger ones were produced and checked but must be regenerated".

**F2. MODERATE, confidence high. Top-level `README.md:54`: "at least 2n − 3 in general, and more than that from four people on"** reads as a statement for every n ≥ 4. It is computed for n = 4..10 only; `solver-a/proofs.txt:82-83` lists it as not proved. Replacement: "…and more than that for 4 to 10 people".

**F3. LOW, confidence high. README:34** "the shortest possible run has 2n − 4 calls" is false for n = 2, 3 (1 and 3; `solver-a/sanity.log:1-2`), which the README's table covers. Replacement: "the shortest possible run has 2n − 4 calls for n ≥ 4 (the classical gossip number; 1 and 3 calls for n = 2, 3)".

**F4. LOW, confidence high. README:46** says both solvers produced strategy files for A(2), …, A(7); files exist only for n = 4..7. Replacement: "…strategy files for n = 4 to 7…".

**F5. LOW, confidence high. README:109** "Solver A's fit ⌈7n/2⌉ − 9 fails at n = 10" omits that it also misses n = 4 (5, not 6); the source says "fits n = 5..9" (`reports/solver-a.md` §3). Replacement: "fits n = 5 to 9 only (it gives 26 at n = 10)".

**F6. LOW, confidence high. README:124** "Three of solver B's test files": the third item is the strategy files, not a test file. Replacement: "Three things in solver B's files are weaker than they look: …".

**F7. LOW, confidence medium. The Delayer-file checks and the n = 10 "no" lean on more than README:99-102 says.** B's Delayer files end in leaves justified by Lemma 2 (`solver-b/check_cert.py:24-26`), and the referee's checker scores a position missing from a Delayer file by Lemma 2 instead of rejecting (`verifier/g6.cpp:235`); sound, but not "play continues inside the file". Both engines behind "no at 28" also skip "twin" agents by default (`verifier/g6.cpp:190-206`; `solver-b/ab2.cpp:13, 31`); I did not verify the twin test. Add to README:102: "The n = 10 searches also skip moves that differ by swapping two interchangeable agents, and solver B's Delayer files use the second bound (half the unfinished agents) at their leaves."

**F8. LOW, confidence medium. README:11** gives the nine values in bold with no qualifier. Replacement: "**Short answer (computed; proved only for n ≤ 4): 1, 3, …**".

Nothing CRITICAL: A(4) = 6 and A(n) ≥ 2n − 3 are correct as proved.
