<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: experiment 25, 5h - 28 <= G(h) <= 5h - 10

Paths are relative to `25-throttled-chips/`. I traced every step of Parts A and B by hand and found no gap in either proof. One MODERATE finding on the page; the rest are LOW.

## Verdicts

- **Q1 CONFIRMED.** `tight/PROOFS.md:20-39`. A surviving line must survive step (1) of round m, so P is the right position. Round 1 is a forced pass and f = m - 2 - p. Q rises by exactly 2 per firing and is unchanged by an add at 0. The identity holds in all four ranges of x, and (A) recomputes to 2p + 4d_0 + 3D + E = 6.
- **Q2 CONFIRMED.** `tight/PROOFS.md:41-79`. E = 6 is impossible, and the three solutions are exactly (I), (II), (III). A4 (all formulas) and A5 check. (I) fails on parity. In (II) the backward trace is right: v = +-1 needs H + 2 on v, v = 0 leaves H-1, H-1, H-1, and any v' then needs H + 1. h >= 4 is used at m - 2 >= 4 and also at f > 0 in A4 (see F5). Consistent with `tight/out_C.txt:1-11` (C = 5h - 10 for h = 4..12, C(3) = 7).
- **Q3 CONFIRMED.** `tight/PROOFS.md:102-128`. Every case redone, including both switches to a neighbour u (F = 1, k = 3 forces l_u = M = 6/5; F = 0, k = 1 forces l_u = M = 7/5; eta_u >= 2 in both). 6/5 is the threshold in B1, in Wset and in B2 case 1.
- **Q4 CONFIRMED.** `tight/PROOFS.md:132-154`. (i) holds because T is monotone; the fired j is in Wset for the same reason. Cases 1a and 1b and the algebra (k/(k+1))(B_{k+1} + 1) = B_k check. Cases 0 and 1 are exhaustive because 5M is an integer. B_1 = H_n.
- **Q5 CONFIRMED.** `tight/PROOFS.md:156-190`. Weights 1, 3, 1 give 6j <= 5e. Both refinements redone. All ten table rows recomputed by hand and agree with `tight/out_arith.txt:1-10`. Tail: g(10) = 4.479 and g is decreasing.
- **Q6 CONFIRMED, one wording defect (F2).** `tight/PROOFS.md:194-200`. The rule uses only k and F so far. Non-trivial from h = 6; beats 2h - 1 from h = 10; the two are equal (17) at h = 9.
- **Q7 CONFIRMED.** `tight/out_rule_main.txt:7`: 89,243 plays, largest 5 eta - t of 12 after a round and 16 after an add (t/5 + 2.4 and t/5 + 3.2), largest top lag with nothing ready 1, top-k slack 0. The four controls fail as intended (`tight/out_rule_controls.txt`). The page's description is accurate.
- **Q8 CONFIRMED for the new text; stale rows remain (F1, F2, F4).** Short answer, the two new rows, the superseded row and the top-level row are correct. 18 = (5h - 10) - (5h - 28), and it also holds for h <= 3 (gaps 0, 0, 2).

## My own simulator

Written from the rule, outside the repository. It asserts B1, B2 with Wset recomputed each round, B2 with Wset fixed at the final round and checked at every earlier round (the form the proof states, which `test_rule.py` does not test), the B3 inequalities, B4 for every y, identity (b) and both height bounds.

- Exhaustive over **all tie-breaks**: every list on windows (w, length) = (2, 11), (3, 8), (4, 7), (5, 6), (6, 5), (9, 4), plus 30 zeros: 72,113 plays.
- 12,137 random plays, and hill-climbing on the height excess and on 2 l_y - e/5.
- **No violation.** Largest top lag with nothing ready: 1. Top-k slack reaches 0.
- Nothing came near the bounds: largest 2 l_y - e/5 was 3.6 (bound 4.8); largest height t/5 + 2.6 after a round and t/5 + 3.4 after an add (bounds 4.8 and 5.6).

So my runs support B1 and B2 and, like the authors' tests, do not exercise B4 or the height bound.

## Findings

**F1. MODERATE, confidence 85%. `README.md:57`.** The row for G(h) >= 5h - 26 - 10·H_15h still reads as the working bound ("better than 5h − 176 ... better than 2h − 1 from h = 28"). 5h - 28 beats it and its exact form for every h (398 against 472 at h = 100). The 5h - 176 row at `README.md:55` is likewise unmarked.
Replacement: append to both first cells "(superseded for every h by the row G(h) ≥ 5h − 28 above)".

**F2. LOW, confidence 95%. `README.md:15`.** "with 2h − 1 the better lower bound for h ≤ 9": equal at h = 9.
Replacement: "(with 2h − 1 the better lower bound for h ≤ 8; the two agree at h = 9)".

**F3. LOW, confidence 95%. `tight/test_rule.py:2`.** The docstring says "lower bound G(h) >= 5h-32", a leftover from an earlier version.
Replacement: "G(h) >= 5h-28".

**F4. LOW, confidence 80%. Top-level `README.md` row 25, last cell.** "both bounds proved by hand, the lower bound by one instance building on another's firing rule" reads as if the upper bound had the older, refereed provenance; both now come from one instance on one day.
Replacement: "both bounds proved by hand by one follow-up instance (the lower bound on another instance's firing rule), not machine-checked".

**F5. LOW, confidence 90%. `tight/PROOFS.md:78`.** The note on h >= 4 omits that A4 also needs f >= 5H - 10 > 0.
Replacement: "case (II) needs m - 2 >= 4, and A4 needs 5H - 10 > 0."

**F6. LOW, confidence 75%, from reading only. `tight/run_all.sh:3,14`, `tight/test_rule.py:140`, `tight/out_search_h6.txt:4-22`.** `run_all.sh` runs from `tight/`, but `test_rule.py` opens `25-throttled-chips/lag/...` relative to the repository root; and the script produces only the first three lines of `out_search_h6.txt` (the controls and the h = 7 runs the page cites have no recorded command).
Repair: fix the paths and add the missing commands.

## Not checked

I ran none of the repository's programs. The "4 ≤ h ≤ 14" in the first new row rests on `reports/referee.md:11`, not on `tight/out_C.txt`, which stops at 12.
