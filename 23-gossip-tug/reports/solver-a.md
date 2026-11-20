# solver-a report: Delayer/Hastener gossip game

**Headline: NO.** A(n) = 2n-3 fails already at n = 4: A(4) = 6, not 5. It fails for every n from 4 to 10. The exact function A(n), and whether A(n)/n is bounded, remain open.

## 1. Tables

| n | A(n) | 2n-3 | status |
|---|------|------|--------|
| 2 | 1 | 1 | COMPUTED, exhaustive (slow.py, raw states) |
| 3 | 3 | 3 | COMPUTED, exhaustive (slow.py) |
| 4 | 6 | 5 | COMPUTED, exhaustive (slow.py; fast.cpp modes 0/1/2; ab; certificates checked) |
| 5 | 9 | 7 | COMPUTED, same |
| 6 | 12 | 9 | COMPUTED, same (905,168 raw states) |
| 7 | 16 | 11 | COMPUTED, exhaustive (fast.cpp modes 1 and 2; ab; ab2; both certificates checked) |
| 8 | 19 | 13 | COMPUTED, exhaustive (fast.cpp mode 1, 2,923,259 states; ab, ab2 agree; Delayer certificate checked) |
| 9 | 23 | 15 | COMPUTED by the decision solver only (ab and ab2: ">=23 YES, >=24 NO"); no certificate, one code base |
| 10 | >= 27 | 17 | Lower bound COMPUTED (ab: ">=27 YES"); ">=28" timed out at 1500 s, undecided |

Methods:
- **slow.py:** the definition directly, memo on raw labelled states, no symmetry.
- **fast.cpp:** exact minimax with memo. Mode 0 (raw states) reproduces slow.py's state counts for n <= 6. Mode 1 memoises on a relabelled representative. Mode 2 takes the minimum over all n! relabellings; at n = 7 it gives the same value and 49,045 states as mode 1.
- **ab.cpp / ab2.cpp:** decide "A(n) >= t" with a table of bounds; they differ only in move ordering.

Prunings, all safe:
- Relabelling agents: every representative is an actual relabelling, so the value is unchanged.
- Upper bound = number of pairs not mutually known (a called pair is never legal again).
- Lower bound = ceil(non-experts / 2) (a call completes at most two agents).

Sanity (sanity.py, n <= 6): the shortest play over all plays is 2n-4 for n >= 4, the classical value; the longest is n(n-1)/2; for n = 3 every play has 3 calls.

## 2. Witnesses

- **Files:** `cert_n{4,5,6,7}_D.txt`, `cert_n{4,5,6,7}_H.txt`, `cert_n8_D.txt` (from gen.cpp).
- **Delayer file** (proves A(n) >= v): lines `K_0 .. K_{n-1} a`, hex masks plus the agent Delayer names.
- **Hastener file** (proves A(n) <= v): the masks plus `a:b` for every active agent a.
- **Checker:** `check.py n D|H file claim`, Python, no code shared with the solvers; it replays the strategy against every opposing choice.
- **Accepted (check.log):** D and H for n = 4, 5, 6, 7 at 6, 9, 12, 16; D for n = 8 at 19.
- **Not produced:** an n = 8 Hastener certificate (too large); anything for n = 9, 10.
- **Deliberate failures, all rejected:** n = 5 D with claim 10; n = 5 H with claim 8; a changed Delayer move (`bad_n4_D.txt`); an illegal Hastener reply (`bad_n4_H.txt`); a deleted line (`bad_n5_H.txt`).

## 3. Headline answer and confidence

- **A(4) = 6:** certain. Delayer forces all six pairs to be called.
- **A(5), A(6), A(7) = 9, 12, 16:** very high confidence. A(8) = 19: high. A(9) = 23: fairly high, but one code base with two move orderings.
- **Exact function:** unknown. Differences from n = 4 on are 3, 3, 3, 4, 3, 4, then at least 4.
- **A failed guess:** ceil(7n/2) - 9 fits n = 5..9 but predicts 26 at n = 10; computed A(10) >= 27.
- **lim A(n)/n:** not determined. The data fit a slope near 3.5 to 4 as well as slow superlinear growth; nothing proved either way.

## 4. Proofs (proofs.txt)

- **P1, COMPLETE: A(n) >= 2n-3 for all n >= 2.**
  - Delayer names agent 1 while it is active.
  - Those calls all contain 1, with distinct partners p_1, p_2, ..., and 1 stays active until it knows everything. So the phase lasts exactly n-1 calls.
  - It leaves K_{p_i} = {1, p_1, ..., p_i}: a chain with exactly two experts.
  - Unions of chain members stay in the chain, so a call creates a new expert only if one caller already was one. That needs at least n-2 more calls.
- **P0, COMPLETE:** the three "consequences" in the problem statement, and A(n) <= n(n-1)/2.
- **P2, COMPLETE by computation and certificate:** A(4) = 6.
- **Not proved:** any formula; A(n) > 2n-3 for all n >= 4; any linear upper bound.

The problem's unverified beliefs:
- A(2) = 1, A(3) = 3, A(n) >= 2n-4: true (P1 gives 2n-3).
- A(4) = 5 and the guess A(n) = 2n-3: false.
- "Fixed agent" line gives only 2n-3: consistent (seven simple Delayer rules give exactly 2n-3 for n = 4..8; rule.cpp, exploration only).

## 5. Interpretation

I found no ambiguity and implemented the text literally. No alternative reading was needed.

## 6. Code and outputs

Everything is in `solver-a/`.
- **slow.py:** n = 2..6.
- **fast.cpp:** n = 2..8 exact, modes 0/1/2.
- **ab.cpp:** n = 4..9 and A(10) >= 27 (ab10.log; ab10b.log is the >=28 timeout).
- **ab2.cpp:** n = 4..9 cross-check (ab2_9.log).
- **gen.cpp:** certificates. **check.py:** verification (check.log).
- **sanity.py:** shortest and longest plays (sanity.log).
- **rule.cpp, explore.py:** exploration only. **proofs.txt:** proofs.
