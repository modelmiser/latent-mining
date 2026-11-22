<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: R(4q + 1) >= 3q - 1 (experiment 18 follow-up)

Paths relative to `18-write-once-stack/`. I traced the proof by hand and wrote my own scheme and exhaustive checker from PROOF.md's text alone (not from quad.py), q = 1..4.

**Overall: no CRITICAL defect. The proof closes. Defects are in what the stored logs support and in stale README wording.**

## Verdicts

- **Q1 CONFIRMED.** `chain/3-question.txt:1-5`: cells only rise, Q/E see the configuration (and operation) only, POP on empty is illegal, "survives t" = all sequences of length <= t, nothing checked before the first operation. `quad/PROOF.md:56` states the same; Q(0^n) = EMPTY (`PROOF.md:9,136`) covers the strict reading. The scheme does not use t.
- **Q2 CONFIRMED.** Mode A is P3 (`solver-b/proofs.txt:47-55`). Before the freeze PUSH s = 2q + d; with no LL quad d >= q, so the freeze would be operation >= 3q + 1 (`PROOF.md:96-100`). At the freeze the killed and converted pairs are exactly the live pairs in quads < j**, a bottom prefix of S; afterwards the first LL quad is j**, N = (b), R = suffix + b (`PROOF.md:111-116`).
- **Q3 CONFIRMED.** Every row of the table re-derived (`PROOF.md:80-85`): each quad has 2 push charges; each pair killed by a POP or conversion adds 1; freeze cleaning and waste kills add 0 and only ever hit non-LL quads, which then hold exactly one live pair, so a wasted quad ends DD with charge 3. Converted quads lie in the left zone and are never wasted later. Sum: s = 4q - |R| - w; w <= q - 1. My checker asserted the equality and w <= q - 1 at every mode-B state for q <= 4: no violation.
- **Q4 CONFIRMED.** PUSH with j* = q + 1 needs s >= 3q beforehand (`PROOF.md:118-120`); PUSH and both POP cases preserve (I1); with N empty, |O| >= 3 puts the killed pair right of quad j* (`PROOF.md:125-130`). Q in mode B depends on the configuration only. q = 2, t = 5: mode B is reachable only by five pushes (any POP leaves at most four pushes), and the reading is right; no counterexample. My checker asserted (I1), (I2) and "POP never hits j*" over all sequences for q <= 4: none failed.
- **Q5 CONFIRMED.** R is monotone in n (ignore cells). 3·floor((n-1)/4) - 1 >= (3n - 16)/4. Recomputed against max(floor(n/2) + 2, 7): strictly better at n = 17, 21, 22, 23 and every n >= 25 (checked to 2000; 3n/4 - 4 > n/2 + 2 iff n > 24); ties at 13, 18, 19, 24. 17 against 14 at n = 25.
- **Q6 CONFIRMED** for the programs; **UNSUPPORTED** for part of what the logs are cited for (F1). `checker.py:26-52` keeps the exact set of (configuration, stack) after each step, skips POP on empty (`:31-32`), checks superset (`:40`) and Q (`:42-45`) after every operation, strict Q(0^n) (`:21`). `quad.py` matches PROOF.md clause by clause (`:47-59` Q, `:61-71` push, `:84-91` freeze, `:92-99` pop). My independent run: SURVIVES 9 / 209 / 5353 / 139625 at t = 3q - 1 and 25 / 617 / 15785 / 417257 at t = 3q for q = 1..4; FAILS at 3q + 1 for q = 1..4 with 3q pushes of 0 then a push of 1. All counts equal `organiser_rerun.txt:2-6` where present, and my own recount of legal sequences (also 3715049, 11080169, 99095529, 296855529 for t = 14, 15, 17, 18, matching `run_quad.log:5,10,16,17`). Mutants: `nowaste` fails at q = 3 and 4 with the logged sequences, survives q = 2; `nofreezeclean` survives q = 3, fails q = 4 with the logged sequence.
- **Q7 mostly CONFIRMED; three items UNSUPPORTED or stale** (F1, F2, F3, F5).

## Findings

**F1. MODERATE, confidence high. The cited log does not contain the runs claimed.** `quad/PROOF.md:141-144` ("see `run_quad.log`": q = 1..6 at 3q - 1, 3q, 3q + 1) and `README.md:53` (source `quad/run_quad.log`). `run_quad.log` holds unmutated runs for q = 5 and 6 only (`:5,10,15-18`). `organiser_rerun.txt` adds q = 1..4 at 3q - 1, q = 4 at 12 and 13, q = 3 failing at 10. No stored file shows q = 1, 2, 3 surviving 3q, or q = 1, 2 failing at 3q + 1. The statements are true (my run reproduces all of them), but not evidenced in the folder. Repair: store the q = 1..4 runs at all three t, or write "q = 5, 6 in `run_quad.log`; q <= 4 in `organiser_rerun.txt`" and cite both files in README rows 52-53.

**F2. MODERATE, confidence high. Stale sentences.**
- `README.md:89` "The gap between ⌊n/2⌋ + 2 and n − 1 is the open question." Replace: "The gap between the best lower bound (⌊n/2⌋ + 2 for small n, 3·⌊(n − 1)/4⌋ − 1 from n = 25, see below) and n − 1 is the open question."
- `README.md:9-10` "In general the best lower bound proved here is ⌊n/2⌋ + 2" is contradicted by the next sentence. Replace: "The chain's experiment proved ⌊n/2⌋ + 2 (for n ≥ 6), together with 7 for every n ≥ 8 …".
- Top-level `README.md` row 18, last column: says how "the general lower bound" was proved but nothing on the new one. Add: "the bound from n = 25 on is proved by hand by one instance, checked by program for n ≤ 25".
- `README.md:194` "The corrected wording has not been through a second round" will need the @@QREVIEW@@ account to qualify it.

**F3. LOW, confidence high.** `README.md:127` "ran out of memory at n = 29" and `PROOF.md:146-147` ("6 GB cap"): `run_quad.log:19-92` shows two MemoryError tracebacks with no command line, q or cap. Repair: log the command before each run, or say "the log shows the error but not the command".

**F4. LOW, confidence high.** `README.md:52` "296,855,529 sequences at q = 6" is the count for length <= 18 = 3q; the proved budget 17 has 99,095,529. Not wrong (the larger set contains the smaller). Suggested: "296,855,529 sequences of length ≤ 18 at q = 6".

**F5. LOW, confidence medium.** `README.md:120` "Its attempt to compute R(9) with solver B's program ran out of time": nothing in `quad/` records it. Either store the log or mark it "reported by the instance, no log kept".

**F6. LOW, confidence high.** `PROOF.md:35-56`: Q and E are specified only for flag-set configurations whose pairs are all L or D; E must be total. Unreachable, so harmless. Add after `:54`: "On a flag-set configuration with an untouched pair (never reached), Q = EMPTY and E stays."

**F7. LOW, confidence high.** `PROOF.md:89` "(in fact equality)" holds only while no "stay" operation (`:54`) has occurred. Add "as long as no operation was a stay, which the budget guarantees". Also `:99` "s >= 3q > t - 1" is correct but weaker than needed; "s + 1 >= 3q + 1 > t" reads more directly.

Not checked: q = 5, 6 runs (I did not rerun them beyond recounting the sequence totals); the survival of 3q for general q (correctly labelled unproved).
