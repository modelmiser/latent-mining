<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — `open-growth/proofs.txt:12-26`. C1: each n-word with prefix f sits at a distinct cell; surplus form is the sum of mult(c)−1 over c with c[j..j+k)=f. C2: I re-derived loss (k lost, k−1 junction) and doubling (windows containing old c and c+1 not starting at c: start ∈ [c−k+2, c−1] = k−2 lost; k−1 gained; old window at c survives at the copy); needs len ≥ k+2, which holds. C3: ρ/2 = N_01 = N_10, surplus 01- and 10-windows disjoint, so a ≤ e/2. — none

Q2: CONFIRMED — `proofs.txt:35-57`; Fact C (`solver-b/proofs.txt:20-29`, n ≥ 6) gives one altered run and preserved adjacency. (E1): overlapping alternating n-words are shifted by an odd amount, union has ≥ n−1 ≥ 6 interior singletons; disjoint case: 5 each, one alteration hits one group. (E3): N_00100 ≥ 4 in u0, an altered run de-isolates ≤ 2. Step 1 uses Fact B (≥ 4 runs, from E2) and Fact A at lengths 2^n, 2^n+1, 2^n+2 exactly as stated; Step 2 uses C3 with a ≤ 0 or ≤ 1. I re-derived both. — none

Q3: CONFIRMED — `proofs.txt:59-86`. Step 3 algebra from (*) checks (incl. x(111) = −z via x(00)+x(11)=0). My own enumeration (pass + 32 loss contexts + 8 doubling contexts on the 3-word excess vectors, all (t,z) ∈ [−4,4]²): only (0,0) survives both isolated deletions; after deleting an isolated 1 the only reply is doubling in context 101; after deleting the middle of 10101 the only replies are losses in contexts 01001, 10010, 10011, 11001 (a 0 of a 00 run between 1s). The surplus-window argument (both surplus n-words are 0^n, one 0-run of length n+2) follows from C1 at every offset. — none

Q4: CONFIRMED — `proofs.txt:86-96`. The two operations D→w touch only 0-runs of length 1 or 2 and no 1-run, so s* (from 0001000 ⊂ some n-word of D, n ≥ 7) and A, B (≥ 3) are untouched; Z (length n) is unique with length ≥ n−1. Cases: Z ∉ {A,B} ⇒ Z survives with length n ≠ n+2; Z = A or B ⇒ merged length ≥ n+3. Only the pass move and C1 on w'' are used, so universality of w is never assumed. — none

Q5: CONFIRMED — `proofs.txt:117-132`. G2 uses only the pass move (mechanic's ≤ 1 edit, C1/C2). G3: I re-derived Σ_{F_s} N_f(w') = Σ N_f(w) − k + J; repeated words are harmless because each of the k windows contributes 1 to N of its own word while the sum runs over the set F_s. My test on 2423 (loop,k) cases incl. 56 non-universal repairable loops: 0 violations. — none

Q6: CONFIRMED — `proofs.txt:134-157`. Equations for word(J_p)=word(W_u) re-derived; d=0/1 excluded by singleton-ness; reversal maps d ↦ 1−d; the k cells [p+d, p+k+d−1] are copies of earlier cells; cells −1, 0 lie in the period-d range and in different classes mod d, so exactly half of 2^d is excluded (referee's sentence, `reports/open-growth-referee.md:30`, is right). Repeat cells give s ∈ [t+d, t+k−1], k−2 ≤ k−1. |P| ≤ Σ_P x(f) justifies (b+1)·(e+k−1). My G4 and (**) tests: 0 violations. — none

Q7: CONFIRMED — `proofs.txt:158`, `bound.out`. I get 2Σ = 2^(n−3)+16k−48 (author's 48(k−2) is looser, valid), and e(19k−2) ≥ 2^(n−3)−19n²+117n−64 exactly; dividing by 19k−2 < 19n gives the closed bound whenever positive, trivially otherwise. Recomputed from (**): e ≥ 9 (n=15, k=11; sum = 4224, num 1816, den 207), 418 (n=20), 272775 (n=30). Unbounded follows. Closed bound is −0.63 at n=15, +10.9 at n=16. — "for all n ≥ 16 the closed bound is positive; (**) first beats 2^n+3 at n = 15"

Q8: REFUTED (one sentence) — P5 (`solver-a/proofs.txt:42-60`) and B's Theorem 1 re-read, no gap; witness `solver-b/witnesses.txt:12` is present and my own checker from the definition says REPAIRABLE (153 adversary moves), `verifier/witness_checks.txt:11` agrees; all 216+20 loops are D+pair and every isolated singleton has neighbours {n,2} (my check). But `README.md:11` "growing at least like 2^n/(19n)" contradicts `proofs.txt:113` "(1−o(1)) 2^n/(152n)" and `bound.out` (~2^n/(130n)). Prior-art paragraph matches `prior-art.md:7,37`. — "growing at least like 2^n/(152n)"

**Findings**

F: MODERATE — `README.md:11` — "at least like 2^n/(19n)" overstates the proved rate by a factor 8; proved is (2^(n−3)−19n²)/(19n) ≈ 2^n/(152n), and even the optimised (**) gives ≈ 2^n/(130n) — high — "growing at least like 2^n/(152n)"

F: LOW — `README.md:99-101`, `open-growth/proofs.txt:162` — "fewer than 2^(n−3) coincidences … leaves at least 2^(n−3) lone bits": the proved bad-singleton bound with k=n−4 is 2^(n−3)+16k−48+2(k−1)(e+n−1) > 2^(n−3) — high — "about 2^(n−3) (plus lower-order terms), leaving nearly 2^(n−3)"

F: LOW — `README.md:46,112-113` — "says nothing below n = 15 / vacuous below n = 15": the closed bound is negative at n = 15 (first positive at 16); (**) is what gives 9 at n = 15 and a trivial e ≥ 1 at n = 14 — high — "the closed form is vacuous up to n = 15; the inequality (**) gives e ≥ 9 at n = 15 and nothing new below"

F: LOW — `open-growth/proofs.txt:108` — "for all n" is true only because the bound is negative for n ≤ 15 (and the proof needs n ≥ 9 for the sum formula) — high — "for all n (non-trivial from n = 16)"

F: LOW — `README.md:64` — "stands alone between two different bits" misreads: the neighbours equal each other and differ from the lone bit — medium — "a bit whose two neighbours both differ from it"

F: LOW — `open-growth/proofs.txt:149` — "at most k−1 singletons" per repeat cell; proved count is k−2 (referee defect 2); harmless — high — "k−2"

F: LOW — `README.md:43,120` — solver B's "third" n=6 computation covers universal w only (`solver-b/witnesses.txt:28`); the README says so at :43 but :120 "three computations" could be read as three complete ones — medium — "two complete computations plus B's universal-only run"

**Not asked but worth noting:** Theorem 1 Step 6's "complementing w if necessary" silently reuses Steps 1–5 for the complement; fine since all steps are complement-symmetric, but a sentence should say so. Theorem 2 is untestable end-to-end; the independent evidence is the two readings plus lemma tests (mine included), nothing more.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
