# Referee report on open-growth (Theorems 1 and 2)

Own code and outputs: `open-growth-referee/` (`gen.cpp` literal brute force; `lib.py`; `chk1.py`, `chk2.py`, `chk3.py` and their `.out` files). The author's tests were not run or reused.

## Verdict

Both theorems are **CONFIRMED**. I followed every step and found no gap that affects a conclusion. **"M(n) - 2^n is unbounded" is established**, with M(n) >= 2^n + (2^(n-3) - 19n^2)/(19n). Three small defects are listed below; one needed a sentence of repair.

## Per item

- **C1** (each k-word occurs >= 2^(n-k) times; exact surplus form): CONFIRMED. Applied only to w'' or u0, which are universal.
- **C2** (edit formulas, k >= 2): CONFIRMED, wrapping windows included.
- **C3** (run count): CONFIRMED.
- **Theorem 1**, step 0 (E1, E2, E3), steps 1, 2 (S = sigma+2, rho = T+2), steps 3, 4 (excess vector), step 5 (forced reply, 0-run of length n+2), step 6 (de Bruijn cycle plus alternating pair), step 7: each CONFIRMED. **Theorem 1: CONFIRMED.**
- **G1, G2, G3**: CONFIRMED. G3 holds for any cell, not only singletons.
- **G4**: CONFIRMED after a one-sentence repair (defect 1).
- **(\*\*) and the closed bound**: CONFIRMED. **Theorem 2: CONFIRMED.**

## Points I was asked to check

- **Non-universal w.** Neither proof uses universality of w. In Theorem 2 the pass move gives G2, the singleton bound 2^(n-2) - 2 (G2 at k = 3) and "at most n-1 missing words". The case is covered, not merely asserted.
- **Edit formulas.** Correct as written. For k = 1 the inequality fails; k = 1 is never used.
- **Isolated 1 in step 7.** It comes from D = w'', the de Bruijn cycle forced in step 6, not from w or u0. D contains 0001000 for n >= 7, and the two operations turning D into w touch only 0-runs of length 1 or 2. So it survives whether or not w is universal.
- **Forced replies.** My own enumeration of pass, 32 loss contexts and 8 doubling contexts on 3-window excesses: only (t, z) = (0, 0) survives both moves. After deleting an isolated 1 the only option is doubling the 0 of 101. After deleting the middle of 10101 the only options are four loss contexts, each removing a 0 of 1001.
- **17** is 2^m + 1 with m = 4.
- **Arithmetic.** With k = n-4 the sum is exactly 2^(n-3) + 16k - 48 (k >= 5); the author's 48(k-2) + 2^(n-3) is looser but valid. The author's rearrangement is correct and implies the closed bound whenever that is non-negative (checked n = 6..399). Recomputed from (**): e >= 9 at n = 15, 418 at n = 20, 272775 at n = 30. All match.

## Defects found

1. **G4, small gap.** "w[-1] != w[0] is a non-trivial condition on the first d cells" is asserted, not shown. Repair: the first family of equations makes w periodic with period d on [p, d-1]. Cells -1 and 0 lie in that range and are in different classes mod d because d >= 2. So they copy two different cells among the d free ones, and the condition excludes exactly half of the 2^d assignments. Machine count: exactly 2^(d-1) in all 56 cases with k <= 8.
2. **G4, constant.** A repeat cell t accounts for s in [t+2, t+k-1], which is k-2 singletons, not k-1. The stated bound is weaker than what is proved, so it stands.
3. **Wording.** "for all n" in Theorem 2: the closed bound is negative up to n = 15. The first new information from (**) is e >= 9 at n = 15.

## Mechanical tests (all could fail; false variants did fail)

- Repairable loops by my own brute force: n = 3, L = 10..16 (2130 necklaces, 332 not universal); n = 4, L = 18..22 (1578, 62 not universal). Also 20 loops for n = 5 and 31 for n = 6 from solver files, re-verified from the definition.
- C1, C3: 748 universal loops (exhaustive n = 3, 4; random n = 5..8), 0 violations. Both false variants fail.
- C2: exhaustive, all loops of length k+2..11, k = 2..6, every cell: 202240 cases, 0 mismatches. "Doubling destroys k-1" fails in every case.
- Step 0: every inverse edit of random universal loops, n = 7, 8, 9: 18439 loops, 0 failures. E1 and E3 do fail at n = 5, 6.
- Steps 5 to 7 literally on "de Bruijn + pair" loops, n = 7, 8: 24 isolated 1s between 0-runs >= 3, none answerable. At n = 4 the same checker finds 24 repairable loops.
- G1 to G3: 56777 singleton cases, 0 violations, 4960 of them on non-universal loops. Variants "right side 2" (628) and "ignore junction" (394) fail. "G2 with k-2" fails only on non-universal loops (8), so that case is really exercised. G3 fails on non-repairable universal loops.
- G4: exhaustive on 27806 necklaces (n = 3..6, L up to 16) and 621 random, periodic and edited loops (n up to 10): 0 violations, bound attained. Three false variants fail (353, 272, 62).
- Each inequality in the proof of (**), all 3759 loops: 0 violations.

`chk2.out` shows 28 "violations" in the G4 factor count. The fault was mine: at d = 0 I imposed w[-1] != w[0] where the proof uses w[0] != w[1]. `chk3.py` corrects it (0 violations).

## Not checked

- Theorem 2 end to end: impossible below n = 14. It rests on reading plus lemma tests at n <= 10.
- Theorem 1 at n >= 7 on arbitrary loops: only structured candidates, small samples (14 and 10 cases).
- The author's test files, the n = 6 computation, and Sketches A and B.
