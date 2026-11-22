<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of a Claude seat run on the plan login (one of the two on Claude Opus). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Claude seat 1: experiment 21 (expiring balls)

Method: read the files and traced the strategies and proofs by hand. Ran no repository program, so every "computed" number is taken from the logs and reports, not reproduced. No CRITICAL or MODERATE defect found; six LOW wording findings.

## Verdicts

**Q1. CONFIRMED** (`README.md:66-74`). Hand trace, bins a..g:
- Step 1: a|b, c|d, {receiver, receiver} gives x load 2; 3 balls, 4 bins in one component.
- Step 2: e|f is legal (both untouched), f receives; {x, f} is legal (components {a,b,c,d} and {e,f}); x would reach 3, so the Rule gives f the ball or loses at once.
- Step 3: three Expires remove exactly the step-1 balls; left are e|f→f and {x,f}→f on bins f, e, x. Untouched: the three other step-1 bins and g, which is 4. Total 7.
- The branch where x already holds the two youngest balls: the README still plays step 2 and the same count holds (7 bins). P5 (`solver-a/proofs.txt:36-37`) takes a shortcut there (expire once); the README drops it. That difference does not matter.
- Step 4: three offers inside the 4 untouched bins, then {f, z} across components; no Expire needed.

**Q2. CONFIRMED** (`verifier/wcheck.py:52-64`). Empty state required (54), load < L (56), closure under Expire by `canon(st[1:])` (57), every offer has an answer inside (58-63). `offers` (37-46) covers used–used pairs in different components, used–fresh, and fresh–fresh, bounded by n; untouched bins are interchangeable, so one representative each is enough. Relabelling by first appearance oldest-first preserves the age order and the game is invariant under bin permutations: sound. Adversary mode: strictly decreasing rank over every answer (98) gives termination.

**Q3. CONFIRMED** (`solver-a/proofs.txt:44-57`). No step fails.
- Ib: Y = second receiver gives a clean 3-star on r, s, p, x after one more Expire (4 bins, 8 untouched). Y = first receiver gives stage I': 5-bin component, 12 − 5 = 7 untouched.
- I': uses 4 + 2 of the 7 (11 bins in use, 1 spare). f's three balls (e|f, {a,f}, {f,Y}) are the three youngest because e|f is offered after a is built; the 7 older balls expire, leaving a star on f, e, a, Y.
- II: 3 + 7 + 1 = 11 balls = n − 1, no Expire.
- Every "must give" is a load-4 threat on a bin of load 3 across components.

**Q4. CONFIRMED as a method; the result stays computed only** (`verifier/vb.cpp:3-8, 37-49`).
- Seeds plus the offer rule plus the older-ball rule is the least fixed point of the Adversary's winning set. The case "one answer terminal, the other in A" fires when the non-terminal answer is visited (38).
- "Closed under adding an older ball" is not an extra monotonicity assumption: it is the Expire move itself (p + older ball → Expire → p). With newest-first labels, Expire is truncation, so core plus truncation test is exact.
- Pruning (45-48): a seed keeps two bins in different components, so at most n − 2 balls; `need` is a lower bound on the balls still to add. Justified.
- The README is right about what is shared and about the missing certificate (`reports/referee.md:39`).

**Q5. CONFIRMED.** 2k: `solver-a/proofs.txt:11-15`, `solver-b/proofs.txt:4-11`. A bin of load k − 1 has k − 1 distinct neighbours among present balls, so its component has at least k bins; loads and edges both count present balls only, so Expire does not break it. 2^k: complete. Any-removal: T(j) − j = T(j−1) checked; the second run touches only untouched bins, so x's star survives. No-expiry value (`solver-b/proofs.txt:21-25`): complete.

**Q6. CONFIRMED with LOW wording defects** (F3, F4). The relation to experiment 14 is described correctly (`14-forest-orientation/README.md:3-6`).

**Q7. CONFIRMED with LOW defects** (F1, F2, F5, F6). All counts match their sources: 974,272,116; 32 and 33; 3,843; 97 per cent; 64 MB and 48 MB.

**Q8. CONFIRMED.** Top-level `README.md:52` matches the folder. "The game" (`21-expiring-balls/README.md:21-35`) is the game of `chain/3-question.txt:1-8`.

## Findings

**F1. LOW, confidence 90%.** `README.md:46` "finds the same winning set"; `README.md:93-94` "agree to the last state". Only the sizes and seed counts were compared. The stored cores differ: 3,723,730 (`solver-b/out_11_4_back2.txt`) against 3,722,933 (`verifier/out_vb_11_4.txt`). That is expected, because the core depends on visiting order, but the sets were never compared.
Replacement: "which finds a winning set of the same size (974,272,116 states) from the same 3,183,950 minimal seeds" and "two programs whose counts agree exactly".

**F2. LOW, confidence 85%.** `README.md:103` "Ozdayi's semester project". `prior-art.md:233` does not say this, and no other file does.
Replacement: "Ozdayi (arXiv:2001.03015, Lemma 2.1)", or add the source of "semester project" to `prior-art.md`.

**F3. LOW, confidence 80%.** `README.md:78-79` "uses the same threat twice". In P6 the threat is used once (stage Ib, second-receiver branch) or twice (Ib, then I'), on top of its use inside the 7-bin stage.
Replacement: "uses the same threat once or twice more, depending on the Rule's answers, to obtain ...".

**F4. LOW, confidence 85%.** `README.md:86-87` "costs the adversary exactly one bin". This needs N(4) = 12, whose lower half has no certificate, and the 10-bin any-removal safe set, which is left out of the folder.
Replacement: "So for k = 4, if the computed g(11) = 3 is right, the order of expiry costs the adversary exactly one bin."

**F5. LOW, confidence 75%.** `README.md:52` states the tie-break without "(untouched bins last)", which is in `solver-a/greedy.cpp:4` and `solver-a/proofs.txt:65-66`. Without it the rule is undefined when a tied bin is untouched.
Replacement: add "(untouched bins last)".

**F6. LOW, confidence 70%.** `README.md:101-104` "read the relevant sections of three papers", then cites Bender et al. The three are items 1, 3 and 5; for Bender et al. only the introduction was read (`prior-art.md:236, 282`).
Replacement: "... three papers and the introduction of a fourth (Bender et al.)".

## Not checked

- No program was run; the checker, replay and attractor results are as logged.
- I read `vb.cpp` in full and did not read `back2.cpp`, `replay12.py` or `verify12.py`.
- The cited papers were not consulted.
