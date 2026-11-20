# solver-a report

**Headline: r(3) is finite. 16 ≤ r(3) ≤ 36; the exact value is not determined.** The problem's guess r(3) = ∞ is false.

## 1. Tables

Cards: 0 = ♣, 1 = ♥. All files are in `solver-a/`.

| quantity | value | status |
|---|---|---|
| E(2,m), m ≤ 3 | 0 | PROVED (L2) |
| E(2,4) | 0 | COMPUTED, brute force from the definition (`slow.cpp`) |
| E(2,m), m = 5..14 | 1 | COMPUTED (`fast.cpp`); all m ≥ 5 PROVED (L4) |
| r(2) = f(2) | 5 | PROVED given the m = 4 computation |
| E(3,m), m ≤ 5 | 0 | PROVED (L2) |
| E(3,m), m = 6..15 | 0 | COMPUTED, exhaustive (`fast.cpp`; 2.1e9 configurations at m = 14, 1.6e10 at m = 15, every admissible R) |
| f(3) | ≥ 16 | follows from the row above |
| E(3,36), E(3,132) | 1 | explicit witnesses, checked by `check.py`; E(3,36k) = 1 by L5' |
| E(4,m), m ≤ 13 | 0 | COMPUTED, exhaustive (`fast.cpp`) |

Strand family (m = a·b, R inside the subgroup of order a; see L5), n = 3:

- a ≤ 5, any b, any R: none. COMPUTED (LP, `zscanR.py`; floating-point solver).
- a = 6: only R = the whole subgroup works, and the least b is exactly 6 (m = 36). b ≤ 5 is excluded twice: exact hash join (`strand2.cpp`, `strandsearch.cpp`) and ILP over all 173 class-set orbits (`zilp.py`).
- a = 7, 8 with b ≤ 4, and a = 9, 10 with b ≤ 3, any R inside the subgroup: none (`strand3.cpp`).
- R = the whole subgroup: a = 11 with b ≤ 3 and a = 12 with b ≤ 2: none.
- Not searched: a = 7 with b = 5 (m = 35), and a ≥ 11 with partial R.

For n = 4 with R a whole subgroup, a = 5 and 6 are LP-infeasible for every b; a = 7 timed out (`strands2.py`).

## 2. Witnesses

- `w_n3_m36.txt`: n = 3, m = 36, R = {0,6,12,18,24,30}, six strands of length 6.
  - owner `111111012301331022102230222123003012`
  - u0 `000011000011011001000000110111111011`
  - u1 `111100011110100010101110001000110000`
- `w_n3_m132.txt`: first witness found, 22 strands.
- `w_n2_m5.txt`: five-card trick; owner `10212`, u0 `00011`, R = {0,1,2,3}.
- `bad_n3_m36.txt`, `bad_n2_m5.txt`: one card altered; `check.py` rejects both.

`python3 check.py w_*.txt` recomputes every M(x) from the definition and shares no code with the searches.

The lower bounds have no compact certificate. They rest on the exhaustive logs (`fast_n3_*.txt`, `fast_3_14_p*.txt`, `fast_3_15_p*.txt`) and on the pruning argument at the end of `proofs.txt`.

## 3. Headline answer and confidence

- **r(3) ≤ 36**: certain (direct check).
- **r(3) ≥ 16**: computed. `fast.cpp` matches the definition-level brute force exactly on the counts of valid and of private (protocol, R) pairs for n = 2, m ≤ 6 and n = 3, m = 6 (`crosscheck1.txt`, `slowred.cpp`). Above that it stands on the pruning lemmas, where an error could only produce false zeros. Confidence about 95%.
- m = 16..35 is open. Inside the strand family only the unsearched cases above could beat 36. General R at these sizes was not searched exhaustively; simulated annealing (`anneal.cpp`) found nothing and carries no weight. I have no conjectured formula.
- General form: {n : r(n) < ∞} contains 2 and 3. n ≥ 4 is open; r(4) ≥ 14.
- Conjecture, unproved: f(3) = ∞.

## 4. Proofs (`proofs.txt`)

- L1 COMPLETE: WLOG u_i^1 is the complement of u_i^0 on P_i.
- L2 COMPLETE: every |P_i| is even and balanced, so r(n) ≥ 2n (belief (a)).
- L3 COMPLETE, new: in a valid protocol every translate of R meets every P_i; otherwise a window of w(1^n) equals a window of an AND-0 row. Hence |R|·|P_i| ≥ m and |R| ≥ n. This is stronger than belief (b).
- L4 COMPLETE: E(2,m) = 1 iff m ≥ 5, by an explicit family with R = Z_m (belief (c)).
- L5, L5' COMPLETE: strand decomposition, and E(n,a) = 1 ⇒ E(n,ab) = 1.
- L6 COMPLETE but partial: for n = 3 and R = Z_m, w(111) is always homometric to w(000), and is a rotation unless an "antipodal" frequency type occurs. This contains belief (f) for that case and does not settle f(3).
- Belief (e): the deduction is fine, but the reconstruction theorem it needs (I recall Pebody, not Grünbaum–Moore) is from memory and unverified. "f(n) = ∞ for n ≥ 7" is NOT proved here.

## 5. Interpretation

- Shift: the card at p moves to p+j, so t_q = w_{q−j}; M(x) has m words with multiplicity.
- Correctness read literally: the supports of M(1^n) and M(0^n) are disjoint. Under the weaker reading M(1^n) ≠ M(0^n), E(3,m) = 0 for m ≤ 11 and E(3,12) = 1 (`fastw.cpp`).
- Positions where u_i^0 = u_i^1 are allowed and behave as H; a player whose two words are equal makes the protocol invalid.

## 6. Code per number

- E tables: `fast.cpp` (mode 0; m = 14 in 2 parts, m = 15 in 8 parts), `slow.cpp`, `slowred.cpp`.
- Witnesses: `strands2.py` (LP that first showed feasibility at a = 6), `zscan.py`, `zilp.py`, `strands_ilp.py`, `mkwit.py`, `check.py`.
- Family exclusions: `strandsearch.cpp`, `strand2.cpp`, `strand3.cpp`, `zscanR.py`.
- `README.txt` lists every file and log.

Process note: one `fast 3 14` run (about 10 minutes) was started by mistake without `nice`/`timeout`. I could not kill it under the no-`ps` rule; it ended on its own.
