# Referee report — write-once stack-top schemes, R(n)

Headline is **not settled**: R(n) = n − 1 holds for 2 ≤ n ≤ 8 and is open from n = 9. No table entry of either solver was refuted. I found two small errors, both in solver B's prose.

My code is in `verifier/`: `vcheck.py` (checker, both file formats), `vsolve.cpp` (solver written from the statement), and five driver scripts.

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| R(n) ≥ n − 1, n = 2..8, both readings | CONFIRMED | witnesses checked: A's `witness_n*` and `*_rootempty`, B's `s_n*_{L,S}` all pass `vcheck.py` |
| R(n) ≤ n − 1 for n ≥ 2 (literal), n ≥ 1 (root-empty) | CONFIRMED | proofs read, no gap (A §1, B P1) |
| R(1) = 1 literal, 0 root-empty | CONFIRMED | recomputed; proof read |
| Table n ≤ 5, all t ≤ n + 1, both readings | CONFIRMED | recomputed by `vsolve`; no entry differs from either solver |
| A §2 / B P0 (only top r + 1 entries matter) | CONFIRMED | proof read, no gap |
| B P2 (readings coincide for t ≥ 3) | CONFIRMED | proof read, no gap |
| Pair scheme, R(n) ≥ floor(n/2) (A §5, B P3) | CONFIRMED | proof read; A's files n = 2..8 and B's generator n = 2..10 pass, and fail one step later |
| B P4: R(n) ≥ floor(n/2) + 2, n ≥ 6 | CONFIRMED | proof read, no gap; all 246 `mwit/` files pass with correct headers; `hyb_p0..3` pass |
| Third UNVERIFIED claim false (A §4, B P5) | CONFIRMED | both cited stay events reproduced |
| Stay condition, A's version | CONFIRMED | 1900 brute-force cases, 0 mismatches; 2546 stay events in 74 witnesses, 0 violations |
| Stay condition, B's "iff" | REFUTED in a detail | Errors, item 1 |
| B's M table, r ≤ 3 | CONFIRMED | recomputed in full, 0 differences |
| B's M table, r = 4, 5 | CONFIRMED | impossibility on r cells recomputed for all 63 and 127 stacks; existence by `mwit/` |
| M table, r = 6, existence on 7 cells | CONFIRMED | B stored no witnesses; I regenerated all 255 with B's `sat` binary and all pass my checker (`verifier/m6/`) |
| M table, r = 6, impossibility on 6 cells | UNSUPPORTED | B's own SAT solver only, no certificate; my solver cannot reach it |
| R(9) ∈ {7, 8} (A) | CONFIRMED | upper-bound proof plus ignoring a cell |

Checker mutants, five base files: every Q flip on a reached configuration (866), every cleared bit in an E target (5610) and every redirect of a used E entry to a differently-labelled superset (1228) was rejected. So were t + 1, a wrong initial stack, and the solvers' six bad files.

## Disagreements between the solvers

- **R(9).** A says "7 or 8", B says "not reached". A is right; B's P4 gives only 6 there.
- **General lower bound.** A proves floor(n/2), B proves floor(n/2) + 2. B's bound stands.
- **Stay condition.** A restricts it to histories of length ≥ 1; B states an unrestricted "iff". A is right.
- No disagreement on any value or on the readings. Both took the literal reading as primary and reported root-empty alongside.

## Errors found

1. **B P5, wrong in a stated detail.** "A push of b from (S, r) may stay iff |S| ≥ r and the top r entries are all b" fails at the start under the literal reading: B's own R_L(1) = 1 witness has E(0, PUSH0) = 0 with S empty, r = 1.
2. **B report §3, off by one.** "No slack until the last two operations" is wrong. In both n = 8, t = 7 witnesses, operation 5 of 7 gains two cells in 12 plays (entering 000 or 111).
3. **B's "every entry is COMPUTED"** holds for the R table (impossibility comes from P1), not for the r = 6 lower half of the M table.
4. **Weak negative tests.** A's `mutant_t7_n7.txt` and B's `bad_t7.txt` only raise t to n, which the upper-bound theorem already forbids. The other mutants are real tests.
5. **The statement** is wrong at n = 1 under its literal wording, and its upper-bound argument omits the case where the first push leaves 0^n. Both solvers caught this.

## Problem as written

Both answered the stated problem. The only ambiguity is whether Q(0^n) must be EMPTY; only R(1) depends on it. Literally, "R(n) = n − 1 for every n ≥ 1" fails at n = 1 because R(1) = 1, but no n0 ≤ 8 has R(n0) ≤ n0 − 2.

## Not checked

- n = 6 at t = 5, 6 with my own solver (timed out at 200 s, both readings); n = 7, 8 not attempted. Existence there rests on checked witnesses, impossibility on the proof.
- The solvers' SAT encodings, CDCL code and conflict counts; A's §6 sketch.
- B's remark that the r = 6 table would give floor((n + 5)/2).
- The n = 9 timeouts.

## Follow-up tasks

1. **Decide R(9).** Produce a scheme on 9 cells surviving 8 operations that passes `vcheck.py`, or a checkable impossibility certificate.
2. **Beat n/2 in general.** Give an explicit scheme and proof that R(n) ≥ αn − O(1) for a constant α > 1/2, for example by replacing the pair phase of B's P4 with a denser first phase; machine-check it for n ≤ 14.
