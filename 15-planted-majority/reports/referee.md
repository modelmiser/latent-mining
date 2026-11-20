# Referee report

Code and logs: `verifier/` (`chk.cpp`, `ref.py`, `srch.cpp`, `closed.py`, `gen.py`).

## Verdicts

| Claim | Verdict | How |
|---|---|---|
| G(2) = 1 | CONFIRMED | recomputed, reduced and literal ordered model |
| G(3) = 3 | CONFIRMED | recomputed in both models, all 8 output maps; 6 reduced-model winners at N=3 (matches A) |
| G(4) = 3 | CONFIRMED, reduced model only | recomputed: none at N=4 (6 non-constant output-map classes under one branching rule, all 7 under a second). The ordered model rests on the reduction lemma |
| G(5) >= 3, G(6) >= 5 (B) | CONFIRMED | G(5) by monotonicity; B's 6-state witness checked: wins N=5, fails at (2,4,x=qA) |
| G(8) >= 5, G(10) >= 7, G(11) >= 9, G(15) >= 15 (A) | CONFIRMED | witnesses checked, each exact; the 15-state one fails at (7,9,x=qA) |
| Headline "no", quadratic growth (both) | CONFIRMED | proofs read, no gap |
| G(8M-5) >= M^3 - 1 (A, Cor. 1b) | CONFIRMED | proof read; closure machine-checked, M = 2..12 |
| "Quadratic is the limit of this construction" (B) | REFUTED | A's cubic sets satisfy B's own conditions (C) and (S) |
| G(s) finite for every s (A, Thm 2) | CONFIRMED | proof read line by line, no gap found |
| Budget protocol (both; B's T2) | CONFIRMED | proof read; K = 1..5 wins exactly 3, 3, 5, 5, 7 |
| K(m), m = 1..13 (A) | CONFIRMED for m <= 10 | recomputed 1, 2, 4, 6, 10, 14, 18, 24, 32, 42; m = 11..13 only checked as closed sets |

Lemmas R/L1, M/L2 and B's T3: no gap found.

## Points of care

**Cubic bound.** A's set {1..M} ∪ {cM} ∪ {M²-j} ∪ {cM²} has 4M-3 elements, maximum M³, and every difference is an element or a sum of two. Cubic first beats quadratic at M = 3: S = {1,2,3,6,7,8,9,18,27}, 19 states, N = 26, against 24 for the quadratic set of that size. I generated this protocol myself. Its rule table passes a mechanical check of every local property the proof uses (weight conservation, descent, legality of the required pairs); so do M = 4..8. Enumeration reached only N = 13 in 10 minutes, so N = 14..26 rests on the proof plus the rule-table check. The M = 2 case (11 states) wins exactly N = 7.

**Theorem 2.** No step fails. What I tried to break:
- Step 1: padding with qB agents flips the majority while the cycle survives. Sound.
- Step 3: a zero-sum combination of moves becomes a closed walk on a union of private populations, so Gordan gives a linear potential. Swaps are covered.
- Step 4: margin 2 with the plant in the minority state is a pure population with margin 1, allowed because the plant ranges over all of Q. The staged play is legal because unactivated agents are untouched.

Nit: each interval forces k+1 moves, not k.

## Disagreements

1. Growth: A is right. B's proofs.txt marks its remark SKETCH; B's report states it flatly.
2. Finiteness: A's proof stands; B left it open.
3. State count: A uses one weightless state (2|S|+1), B two. A's variant is correct and checked.
4. Largest closed 10-set: A says 42; B lists 36 (proofs.txt) and 40 (report). All three sets are closed; 42 is the maximum. B's two files disagree with each other.
5. No disagreement on exact values or on the reading of the problem (a or b may be 0, the plant may be qA or qB, cycles are multiset-level).

## Errors and weaknesses

- **The exhaustive G(4) searches are not independent.** A's and B's node counts agree to the unit on every output map (e.g. 133,003,801), and my first search reproduced them: three codings of one lazy search. My second run uses a different branching rule (15,637,141 nodes against 107,860,241 on one map) and also finds none. A third variant (reversed instance order) timed out: inconclusive.
- A's third net option (partial netting) is dead generality: K(m) is the same without it for m <= 10, and no claimed set needs it.
- Broken witnesses rejected: B's 6-state witness minus one rule (5 drops to 3); the cubic protocol with a non-conserving rule (fails at N=6); the 3-state protocol minus a rule (fails at N=2); 343 of 400 random or mutated protocols, checker and reference agreeing on all 400. Two other mutants of mine were not actually broken. A missing split rule is caught by the rule-table check but not by enumeration at N <= 7.
- For about a second I ran three processes instead of two.

## Not checked

- G(4) in the literal ordered model; G(5) (one output map did not finish in 160 s).
- The cubic protocol by enumeration beyond N = 13; M >= 4 by enumeration.
- K(11..13) as maxima; A's claim that the fourth recursion level is not closed for M >= 4.
- The solvers' programs: not run or audited.

## Follow-up tasks

1. **G(5).** Is there a 5-state protocol winning every planted instance with 2 <= a+b <= 4? Give the witness, or an exhaustive "none" over all output-map classes from two searches with different branching rules.
2. **Growth of closed sets.** K(m) = the largest maximum of an m-set in which every element >= 2 is a sum of two elements and every difference is an element or a sum of two. Known: the ten values above and K(4M-3) >= M³. Either give a family with K(m) >= m⁴/C, or prove K(m) <= C·m^d.
