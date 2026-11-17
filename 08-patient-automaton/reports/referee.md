# referee report

Own code in `verifier/`: `ref.cpp` (tables, census), `chk.py` (witness and lemma checks; output `chk_out.txt`). Nothing copied.

## Verdicts

1. **Tables n = 2, 3, 4, d = 0..6, W_inf** — CONFIRMED, recomputed (exhaustive, no symmetry, no pruning, to d = 8). 12; 405; 28960, 27952, 27808 (constant d = 2..8); W_inf = 12, 405, 27712. First agreement: d = 0 for n = 2, 3; none for n = 4.
2. **n = 5 column, d = 0..6, W_inf = 3199305** — CONFIRMED, recomputed over all 9765625 automata without symmetry, but using the pruning W_inf ⊆ W_(d+1) ⊆ W_d (verified unpruned at n ≤ 4).
3. **n = 5: 40800 survivors, 170 classes** — CONFIRMED to d = 13 (my canonical forms; all 170 winnable for d = 0..13, none blind-winnable). Their patience is UNSUPPORTED, as both solvers say.
4. **n = 6 column (B only)** — NOT CHECKED.
5. **A patient automaton with 4 states exists** — CONFIRMED (proofs read, witnesses checked).
   - A Theorem 1 (2213/4311 is d-winnable for every d): no gap found. Round tables recomputed; invariants hold on all plays, d ≤ 9.
   - A Theorem 2 (2213/4311 not blind-winnable; four pair families closed under either letter): no gap found. Certificate recomputed; the 9-family file is closed.
   - B Lemma A (1123/4134 is d-winnable, T = d+4): right in outline, **wrong in a stated detail**. "M_(r-1) = 13" is false when r = 2 (y_1 = a, d = 0 or 1), where M_1 = 123. The conclusion survives because 123.aa = {1}. All other claims hold on all plays, d ≤ 9.
   - B Lemma B (1123/4134 not blind-winnable; explicit Spoiler rule): no gap found. Facts F1–F3 rechecked; the rule beats every word of length ≤ 16.
   - Strategies, literal enumeration of all Spoiler plays, d = 0..12: A's σ_d on W1, W2 and the padded 5-state automaton; B's σ_d on C2, C1 and the padded 6-state automaton. All win at the claimed T. By image-family tracking: all win for d = 0..300. Legality is enforced: strategies receive only y_1..y_(t-1-d).
   - Minimal T for d = 0..10 is 4 4 6 7 7 9 10 10 12 13 13 for all four automata: CONFIRMED.
6. **Least n is 4** — CONFIRMED by computation only (W_0 = W_inf at n = 2, 3), as both say.
7. **s(n) = ∞ for n ≥ 4** — CONFIRMED; both padding proofs read, no gap.
8. **n = 4 census: 96 automata, two orbits of 48** — CONFIRMED, recomputed: W_8 \ W_inf is the orbits of 1123/3422 and 1123/4134.
9. **König and the reduction to the finite game** — no gap found. My literal-definition solver agrees with my rank solver on minimal T: all n = 2, 3 automata plus 154 at n = 4, d ≤ 2, T ≤ 6; 0 mismatches in 2697 cases.

## Disagreements between the solvers

- No numerical disagreement where both report.
- **Naming, noticed by neither:** A's Witness 1 lies in the orbit of B's C1, and A's Witness 2 in the orbit of B's C2. Each class therefore has a hand proof of blind-unwinnability from exactly one solver, while each calls the other class "computed only". Combined, both classes are fully proved, so |W_d(4)| = 27808 for every d ≥ 2 is proved given the d = 2 count.
- The two strategies are the same. B's T = d+4 is not tight when d ≡ 1 (mod 3); A's T(d) is exact.
- Blind closure sizes (A: 9, B: 26–27) differ by representation; not recounted. Padding differs; both valid.

## Errors found

- B Lemma A: "M_(r-1) = 13", above.
- B Lemma 1: "height + 1 is a uniform T" holds only if height counts levels; with edge-count height it is height + 2.
- A's report: Witness 2 has "no written proof"; B's Lemmas A and B supply one for that orbit, after relabelling.
- Mine: one intended negative control (B's σ on A's W1) was accepted, correctly, because the strategies coincide. A badly designed control.

## Negative controls on my checker (all rejected)

Swapped residues; one altered transition (1123/4133); a strategy reading y_1 one round early; T one too small; constant strategy; mutated automaton 2211/4311 claimed non-blind; the 4-family certificate with a family removed or shrunk; the Spoiler rule with residue 1 (1669 words lose).

## Problem as written

Both answered the stated problem (singleton after x_T, uniform T, first d+1 letters blind). I found no second reading that changes an answer.

## Not checked

- The n = 6 column and B's count of 25920 automata that are 7-winnable but not 8-winnable.
- n = 5 beyond d = 13, and n = 5 without inclusion pruning.
- The solvers' programs and checkers (not run).

## Follow-up tasks

1. Decide patience for each of the 170 n = 5 classes in `verifier/n5_classes.txt`: give a strategy valid for all d with an invariant proof and a closed blind certificate, or a d at which it loses.
2. Let D(n) be the largest d with W_d(n) ≠ W_(d+1)(n). D(4) = 1 is proved; D(5) ≥ 5, with equality if the 170 classes are patient; D(6) ≥ 7 per B, unchecked. Prove a bound D(n) ≤ f(n), which would make patience decidable, or determine D(6).
