# Solver B report: counting-blind firing squad

All files are in `solver-b/`.

## 1. Tables

| s | rules | L(s) | rules attaining L(s) | status |
|---|---|---|---|---|
| 3 | 243 | **2** | 36 | COMPUTED three ways (slow.py, brute, lazy); also PROVED by hand (proofs.txt P3) |
| 4 | 65 536 | **3** | 232 | COMPUTED three ways |
| 5 | 48 828 125 | **5** | 1244 | COMPUTED two ways (brute: every rule, no pruning; lazy backtracking) |
| 6 | 6^14 | **8** | 336 | COMPUTED by lazy backtracking only, with and without symmetry breaking |
| 7 | 7^17 | **11** | 4 classes up to relabelling | COMPUTED by lazy backtracking with symmetry breaking only (one run, 164 s) |

s = 8 was not attempted (estimated hours).

Methods.
- `slow.py brute`: direct definition, every rule, "never fires" decided by exact repeat of a configuration. Reaches s = 3, 4.
- `brute.cpp`: every rule, no pruning, Brent cycle detection (exact). Reaches s = 5 in 3 s. Its histogram for s = 5 of the longest synchronised prefix: fails n=2: 43 920 625; N=2: 4 663 756; N=3: 239 212; N=4: 3288; N=5: 1244; nothing beyond.
- `lazy.cpp`: rule entries assigned only when a run first needs them; n = 2, 3, ... run in order; a branch is cut only when the entries already fixed force a failure. Symmetry breaking: a new unnamed state must be the lowest unused label. Unsafe pruning here would bias L downward.

Cross-checks: slow = brute = lazy on L and on the count of optimal rules for s = 3, 4 (36, 232); brute = lazy for s = 5 (1244, lazy without symmetry breaking); for s = 6 the count without symmetry breaking (336) is 6 times the count with it (56), as it should be when the best rules use all three unnamed states. All 1244 dumped s = 5 rules were re-run by slow.py: each synchronises 2..5 and fails 6.

Single-length statistics over all rules (brute, COMPUTED), s = 5: number of rules synchronising that one n and the minimum firing time: n=2: 4 907 500, t=1; n=3: 1 011 783, t=3; n=4: 260 488, t=4; n=5: 29 524, t=6; n=6: 8854, t=7; n=7: 2176, t=9; n=8: 2384, t=10. For s = 4 single lengths up to n = 6 are synchronisable (one rule for n = 6), none for n = 7, 8; for s = 3 only n = 2 and n = 3.

## 2. Witnesses

Format: lines `state k next`, then `TIMES`, then `n time` or `n FAIL`. Checker: `python3 slow.py check FILE`.

`witness_s5.txt` (attains L(5) = 5):

    Q1→G  Q2→Q  G0→F  G1→A  G2→B  A0→B  A1→A  A2→Q  B0→G  B1→A  B2→A
    firing times: n=2: 5, n=3: 6, n=4: 10, n=5: 8, n=6: FAIL

- `witness_s3.txt` (n=2 at t=2), `witness_s4.txt` (times 3, 7), `witness_s6.txt` (times 7, 9, 9, 15, 17, 13, 13 for n = 2..8, n = 9 fails), `witness_s7.txt` (times 9, 10, 17, 19, 14, 23, 43, 39, 24, 38 for n = 2..11, n = 12 fails).
- `all_L5_rules_s5.txt`: all 1244 five-state rules synchronising 2..5, one per line, entries in the order Q1 Q2 G0 G1 G2 A0 A1 A2 B0 B1 B2. The 23 distinct firing-time vectors among them range from (3,5,6,6) to (6,9,8,11).
- Upper bounds ("next value impossible") are exhaustive searches, not compact certificates: to check L(5) < 6, re-enumerate the 48.8 M rules, or confirm that the 1244 listed rules are exactly those passing 2..5 and that each fails 6.
- Negative tests: `BAD_witness_s5.txt` (one rule entry changed) and `BAD2_witness_s5.txt` (claims n = 6 fires) are both rejected by the checker. My first attempt at a bad witness changed A2, which that rule never consults, and was accepted; I replaced it.

## 3. Headline

**L(5) = 5.** Confidence high (about 99%): two independent programs with different search structure agree on the value and on the count of 1244 optimal rules, and the slow reference re-verified those rules. The problem's guess "finite and at most 6" is right, but not tight.

Sequence found: L(3..7) = 2, 3, 5, 8, 11. No formula conjectured; it is not Fibonacci (11, not 13). Whether L(s) is infinite for some s is not settled; I have no proof and no sketch in either direction.

## 4. Proofs (`proofs.txt`)

- P1 COMPLETE: a rule synchronising any n ≥ 3 has d(Q,1) ∉ {Q, F}. Confirms the first belief.
- P2 COMPLETE: any rule synchronising n fires at t ≥ n − 1 (cell j is Q at all times ≤ j − 2).
- P3 COMPLETE: L(3) = 2. Synchronising n = 3 forces d(Q,1) = G and d(G,1) = Q, and then n = 2 oscillates. Confirms the second belief; the enumeration agrees (exactly 3 three-state rules synchronise n = 3, all with those entries, all failing n = 2).
- L(4), L(5), L(6), L(7): computation only.

## 5. Problem statement

- No ambiguity found that changes the answer. Reading used: F is never an input to d; a run fails at the first tick where some but not all cells are F; "never fires" is decided exactly by a repeated configuration. The rule count s^(3(s−1)−1) matches my enumeration.
- **The belief "a rule synchronising n fires at a time ≥ 2n − 2" is false in this model.** Counterexamples (COMPUTED): the trivial n = 2 rule fires at t = 1 < 2; four-state rules synchronise n = 3 at t = 3 and n = 6 at t = 7; five-state rules that attain L(5) fire n = 5 at t = 6 < 8. The classical bound needs the rule to also work on longer lines, which a prefix-optimal rule does not. The true bound I can prove is n − 1 (P2).
- "I do not know whether L(4) ≥ 3": yes, L(4) = 3 exactly.

## 6. Code and outputs

- `slow.py`: reference and witness checker. Output `slow_out.txt`.
- `brute.cpp`: `./brute S NMAX NSINGLE [DUMP]`. Outputs `brute_out.txt` (s = 3, 4, 5), `brute5_single_out.txt` (single-length statistics), `all_L5_rules_s5.txt`.
- `lazy.cpp`: `./lazy S NMAX [nosym]`. Outputs `lazy_out.txt` (first run, s = 3..6), `lazy_out2.txt` (s = 7 and optimal-rule counts), `lazy6_nosym_out.txt`.
- `proofs.txt`: P1 to P3.
