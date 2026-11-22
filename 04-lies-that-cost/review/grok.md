<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `solver-a/proofs.txt:9-18`, `solver-b/proofs.txt:8-21`, rule in `chain/3-question.txt:5-7`. ell uses C_{s-1} only, so dead members of S do not count; ell=0 gives an empty window; (ii) is per candidate and only forbids later lies. r = max(0, D−t) is the number of upcoming answers that must be honest, so with j left, r≥j means no lie remains. No off-by-one. — none

Q2: CONFIRMED — `open-upper/proofs.txt:16-27`. Re-ask only while both sides of the current alive set meet S, so ell≥1 each time; k·ell≥2 blocks the next two asks, hence at most one lie in the triple, majority is true for every survivor, and an early stop already lies inside one side of size ≤⌈m/2⌉. h(m)≤3+h(⌈m/2⌉) is 3⌈log2 m⌉. — none

Q3: CONFIRMED — `open-upper/proofs.txt:30-51`. Even a: difference 0 or 1, ell=⌊m/2⌋. Odd a: niceness and m≥2j−2 give b≥j−1≥d∈{j−1,j−2}, b−d even, difference (j−1)−d∈{0,1}, and 2·ell≥j−1 (j=1 odd a excluded by w≤2). A lie then leaves obligation ≥j−1 with j−1 questions left, which is blocked; blocked liars die; both outcomes are nice and weight ≤2^{j−1}. — none

Q4: CONFIRMED — `open-upper/proofs.txt:68-76`. For m≥2, m≥2j−2 and m≤2j−3 partition the integers. j=15,16,17: 2j−3≤31 so 3⌈log2⌉=15≤j; j≥18: 2j−3≤2^{⌊j/3⌋} (33,37,40≤64, and +6 versus a doubling) so 3⌈log2(2j−3)⌉≤j. An emptying answer is illegal for the responder; Lemma S splits are in any case nonempty on both answers. — none

Q5: CONFIRMED — T1/T2 are the quantifiers at `open-upper/proofs.txt:54-59`. For j≤14, after a lie at most 13 questions remain, so k·ell is 0 or ≥13 for every k≥13 and matches k=13. `table_out.txt:1-15` is the superset j≤16, m≤2j, k=2..16, 1884 states, lost=0, and lines 24-29 are T2. `run1.txt:1-13` is the exact 960-state T1 for k≥4; `run2.txt:2-4` is the literal T2 checks for n=3. — none

Q6: CONFIRMED — `open-upper/proofs.txt:87-99`. Even: obligation ≥n≥q−1 because n(n+2)≤2^{n+1} (8=8, step iff n²≥3), state (n/2,n/2;q−1), weight n(q+1)/2. Odd n≥7: obligation ≥n−1≥q−1 because n²+2n−1≤2^n (62≤128, step iff n²≥4); “yes” weight is exactly the odd-n half of P, “no” is smaller by q−1. Only n=3 (P=5) and n=5 (P=6) miss that block, and only for k=2,3 and k=2. — none

Q7: CONFIRMED — `solver-a/proofs.txt:21-38`, `solver-b/proofs.txt:23-39`. |L|≤1 meets (ii), so U1≤Q_k; the one-lie weight halves, and for odd n the heavier first answer has weight ≥(n(q+1)+(q−1))/2. — none

Q8: REFUTED — Status cells, the repeated-lies paragraph (`README.md:53-71`), the k=1 claims (`README.md:49-50` vs `reports/referee.md:15-24`, `solver-a/out_k1_fix.txt:1-8`), and the prior-art paragraph (`README.md:88-92` vs `prior-art.md:3-31`) match. The T1 cell does not: `README.md:46` cites `run1.txt` for k=2…13, but `run1.txt:1` starts at `t1 k=4`. — “k=2,3 T1 wins are in `table_out.txt:1-2` (superset, lost=0) and asserted in `reports/open-upper-referee.md:41`; `run1.txt` records the 960-state T1 only for k≥4.”

F: LOW — `README.md:46` — cites `run1.txt` for T1 at k=2 and k=3; that file has no `t1` line for either k, so the second program’s saved log does not show the headline table — high — cite `table_out.txt` and the referee report for k=2,3, and `run1.txt` for k≥4
F: LOW — `open-upper/proofs.txt:57-58` — “k·ell≥13≥number of questions left” is false if that number is the state’s j=14 (13≱14); it is true of the questions left after the lie — high — “after any lie at most 13 questions remain, and k·ell is 0 or ≥13”
F: LOW — `solver-a/proofs.txt:24` — “|L|≤1 satisfies (ii) vacuously” is the wrong word: the ∀ over s∈L is not vacuous when |L|=1; the window simply cannot contain another lie — high — “a single lie is never inside its own later window”
F: none beyond those

The headline argument in `open-upper/proofs.txt` closes: Lemma C, Lemma S, the j≥15 bound, Theorem 2’s first-question weights, and the odd-n term in P all match the game in `chain/3-question.txt`. T1 for k=2,3 is one program’s log plus a report, not two saved `t1` logs.
