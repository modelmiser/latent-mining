# Solver A report — write-once stack-top schemes, R(n)

**Headline: not settled in general.** R(n) = n − 1 is established for 2 ≤ n ≤ 8; the upper bound R(n) ≤ n − 1 is proved for all n ≥ 2; the lower bound for n ≥ 9 is open. Read literally, the statement fails at n = 1, where R(1) = 1.

## 1. Tables

Two readings (§5): **literal** = Q(0^n) is never checked; **root-empty** = Q(0^n) = EMPTY is also required.

| n | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|
| R(n), literal | **1** | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 7 or 8 |
| R(n), root-empty | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 7 or 8 |

- **n = 2..8, both readings:** lower bound by an explicit scheme checked from the definition; upper bound by the proof in §4. No search "impossible" verdict is needed.
- **n = 1:** COMPUTED by enumerating all 72 schemes (`slow.py`), plus a short hand argument.
- **n = 9:** not computed; two 1500 s runs at t = 8 timed out. R(9) ≤ 8 is proved; R(9) ≥ 7 follows from R(8) by ignoring a cell.
- **Search verdicts (COMPUTED, exhaustive):** for n ≤ 7, every t ≤ n + 1, both readings, the SAT search says SURVIVES exactly for t ≤ R(n) (`grid_sym1.log`); n = 8 was run at t = 7, 8 only.
- **Agreement:** SAT with symmetry breaking on/off agree for n ≤ 6; with/without weight-bound unit clauses for n ≤ 7; the backtracking search agrees for n ≤ 5 except n = 5, t = 4, where it hit its node limit; `slow.py` (brute force n ≤ 2, unpruned search n ≤ 4) agrees.

## 2. Witnesses

- `witness_n{n}_t{n-1}.txt` for n = 2..8 and `witness_n1_t1.txt`; `witness_n{n}_t{n-1}_rootempty.txt` for n = 2..8. Format is in each file's header.
- `check.py file [--root-empty]` enumerates every legal sequence of length ≤ t (1801 for n = 8) and checks monotonicity. All files pass.
- Checker seen failing on purpose: `mutant_swap_n7.txt`, `mutant_t7_n7.txt`, `pairs_n6_t4_SHOULD_FAIL.txt` are rejected; `--root-empty` rejects a scheme with Q(0^n) ≠ EMPTY.
- The statement's n = 3 example passes (`statement_example_n3_t2.txt`). Its pair scheme passes at t = floor(n/2) for n ≤ 8 (`pairs_n*.txt`) and fails one step later.

## 3. Headline answer

- With §1 and §4: there is no n0 ≤ 8 with R(n0) ≤ n0 − 2.
- **Guess:** R(n) = n − 1 for all n ≥ 2, about 65% confidence. Doubts:
  - SAT effort at t = n − 1 grows fast: roughly 450, 740, 50 000, 400 000 conflicts for n = 5..8; n = 9 unresolved after 3 million.
  - The natural continuation of the pair scheme provably cannot reach n − 1 (sketch, `proofs.txt` §6).
  - I found no general construction.

## 4. Proofs (full text in `proofs.txt`)

**Upper bound — COMPLETE.** Fact F: if two consecutive positions, both after at least one operation, have different tops, the configuration gained a cell. Suppose a scheme survives t = n ≥ 2.
- Case A, both first pushes leave 0^n: the two alternating push sequences gain a cell every step, so both sit at 1^n after n steps with different tops.
- Case B, E(0^n, PUSHb) = 0^n, hence Q(0^n) = b:
  - n ≥ 3: PUSHb,POP and PUSHb,PUSHb,POP end in the same configuration but need EMPTY and b.
  - n = 2: with x = E(00, PUSH(1−b)) ≠ 00, the sequences PUSH(1−b),PUSHb and PUSH(1−b),POP need two distinct strict supersets of x.
- Under root-empty only Case A arises.

**Third UNVERIFIED claim is false — COMPLETE (by witness).** "Every non-last operation must change the configuration" fails in surviving schemes with t = n − 1: in `witness_n5_t4.txt` the third PUSH1 of PUSH1,PUSH1,PUSH1 stays (`stays.py`). Correct necessary condition: with r operations left before it, PUSHb can stay only if the top r elements exist and all equal b; POP only if the top r+1 exist and are all equal.

**Pair scheme cannot be extended to n − 1 — SKETCH** (`proofs.txt` §6).

**General lower bound — not proved.**

## 5. Interpretation and defects in the statement

- **Q(0^n) unchecked.** "Nothing is checked before the first operation" lets Q(0^n) be 0 or 1, so a first push may leave the memory at 0^n. I took this literal reading as primary and report root-empty alongside; only R(1) changes.
- **The statement's upper-bound argument is wrong as written.** It assumes the first step sets a cell. Its conclusion is false for n = 1 (literal) and needs Case B for n ≥ 2.
- **Claimed values.** R(2) = 1, R(3) = 2, R(4) = 3 confirmed; R(1) = 0 holds only under root-empty.

## 6. Code (all in `solver-a/`)

- `sat.cpp` — SAT encoding plus my own small CDCL solver; produced all witnesses and `grid_sym1.log`, `grid_sym0.log`, `grid_prune.log`, `run_n8_*.log`. A clause-minimisation bug gave one wrong "impossible" (n = 5, t = 4) during development; it was caught by a literal/root-empty inconsistency and fixed. No table entry depends on a solver "impossible" verdict.
- `search.cpp` — independent backtracking search (`grid_dfs.log`).
- `slow.py` — brute force and unpruned search (`slow_0.log`, `slow_1.log`).
- `check.py`, `stays.py`, `gen_pairs.py` — checker, stay-finder, pair-scheme generator.
