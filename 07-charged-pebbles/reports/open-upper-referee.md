# open-upper referee report

**Verdict: no gap found. The Main Theorem (a), (b) and the bound C(p,k) <= (pk+2)·2^(p-2) are CONFIRMED.** I followed every step of `open-upper/proofs.txt` by hand for general q and tested every lemma with my own program. No constant is wrong; the only defects are cosmetic (listed below).

## Verdict per lemma

| Item | Verdict |
|---|---|
| W(i), W(ii) (imported from solver B: lowest <= a+2^(c-1), all <= a+2^c−1) | CONFIRMED (I read B's proof; induction order and base c=0 are sound) |
| W(iii) (l-th lowest <= a+2^c−2^(c-l)) | CONFIRMED |
| L1 (a reduction step is a P/R path) | CONFIRMED |
| L2 (confluence, red unique) | CONFIRMED |
| L3 (S ~ S' iff red equal) | CONFIRMED |
| L4 (structure of comp(A)) | CONFIRMED |
| L5 (truncation) | CONFIRMED |
| L6 (lifting from the (q−1)-game) | CONFIRMED |
| Lemma N (normal form of a deletion) | CONFIRMED, for every A |
| Lemma Step (one-shift shape survives reduction, A empty or {a}) | CONFIRMED, all five branches |
| Shape Lemma | CONFIRMED |
| Theorem I (J(f) to J(f+1) under any deletion) | CONFIRMED (m=0, m=1, m>=2) |
| Main Theorem (a), (b) | CONFIRMED, including f=0 and non-reduced boards |
| Reduction to the real game | CONFIRMED |

## Points I pressed on

- **Small q hidden in "for all q"**: none. The exponents q−k−1 (k <= q−1) and r'−1 (r' >= 1 since A' is nonempty), and the (q−1)-game at q=2, are all legitimate.
- **Lemma Step (3b)**: s−u' <= 2^phi < s−u gives u' > u; V≠A holds because V={a} has no successor; e≠s because gap_V(s) > 2^phi. With e=u the new shift s−u' <= 2^phi is exactly the bound allowed at size k−1.
- **Theorem I, m>=2**: never uses a_1 <= (f+2)U, only the a_m bound and L5. The sub-case "a_m = z, b_1 = min T_m" needs a_m+2^(r-1) <= (f+3)U, which is J's second bound exactly.
- **Final deduction**: for n > 2U the board {n} is reduced, so J(f) forces f>=1 and n <= (f+2)U; for n <= 2U the bound is trivial. Part (b) on a non-reduced board follows from L4 (A nonempty) or W(ii) (A empty).
- **Real game**: charged P, R, F are legal relaxed moves on the same board; each F lowers the total charge pk by 1, so f <= pk; both bounds increase with f. This gives the upper bound only; the lower bound is the solvers' and I did not referee it.

## Small defects (none affects the result)

1. J(f) uses 2^(r−1), which is 1/2 when m=q. That happens only on unreachable boards, which Theorem I's "reachable or not" includes. The inequalities still hold (my test uses exact rationals), but the text should say so.
2. "Always m <= q−1" is true only for reachable boards, yet sits beside a theorem stated for unreachable ones.
3. W(iii)'s proof cites "as in B's proof of W(ii)" for the b-play having at most c−1 elements. The reason is W(i): a nonempty set always has an element <= b. One sentence would make it self-contained.
4. The real-game reduction is one line and does not justify f <= pk.

## Mechanical tests (my own code, not the author's)

`ref.py q N` takes every set of at most q nodes in [1..N], computes P/R components by plain unbounded BFS (not via red), and checks each statement as written:
- W(i)–(iii), L1, L2, L3 (both directions), L4 (play membership and all bounds), L5, L6(1),(2);
- Lemma N with the proof's own Q0, for all anchor sets;
- Shape Lemma for A empty or {a};
- Lemma Step: the statement, plus each branch of the written case analysis for every (V,u,u') and every reducible y;
- Theorem I for every S and z, including m=q.

Windows: q=2 N=14, q=3 N=20, q=4 N=22, q=5 N=36 (443,704 sets), q=6 N=34 (1,676,116 sets; 9.7M Theorem I and Lemma N cases, 7.5M Lemma Step branch cases). **0 failures.**

The tests can fail: seven deliberately false variants at q=4 are each reported FAIL (`mutants.log`): W(iii) bound −1; reducibility threshold +1; L5 bound −1; shift bound −1 in Lemma N, Lemma Step (one counterexample, branch 3b e=u) and the Shape Lemma; J's a_m bound −1 in Theorem I.

`game.py`:
- Relaxed game by 0-1 BFS (q=2 f<=6, q=3 f<=5, q=4 f<=4), and for q=3, 4 also with "an F-move may delete any pebble": (a) and (b) hold with equality at every f; lowering (a) by 1 gives 6 violations.
- Charged game by direct BFS: C(p,k) equals the formula for (2,0), (2,1), (2,3), (3,0), (3,1), (3,2), (4,1).

## Not checked

- The lower bound, beyond the seven BFS values.
- The author's programs and logs (not run, not audited).
- q >= 7 mechanically; at q=6 the only single anchors in the window are {33}, {34}.
- No proof assistant; the general-q argument rests on my reading.
- I am the same model as the author, so shared blind spots are possible; a reader from another model family would be an independent check.

Files are in `open-upper-referee/`:
- ref.py
- game.py
- mutants.log
- game.log
- q5.log
- q6.log
