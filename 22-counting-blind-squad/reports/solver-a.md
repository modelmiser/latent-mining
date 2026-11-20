# solver-a report: counting-blind firing squad

All files are in `solver-a/`.

## 1. Tables

| s | L(s) | rules attaining it (of s^(3s-4)) | status |
|---|------|------|--------|
| 3 | 2 | 36 of 243 | COMPUTED, exhaustive, 3 programs agree |
| 4 | 3 | 232 of 65,536 | COMPUTED, exhaustive, 3 programs agree |
| 5 | **5** | 1,244 of 48,828,125 | COMPUTED, exhaustive, 3 programs agree |
| 6 | 8 | 336 of 78,364,164,096 | COMPUTED, exhaustive, 2 programs agree (one beyond what was asked) |
| 7 | 11 | 4 classes up to relabelling of unnamed states | COMPUTED, one program only (C++ lazy search, 162,484,625 leaves, 70 s) |

Methods:
- `fsq brute s`: every rule enumerated, each simulated on n = 2, 3, ... until the first failure (s = 3, 4, 5).
- `fsq dfs s`: lazy search. A table entry is assigned only when a simulation first needs it, branching over all values (s = 3..7).
- `check.py lazy s`: independent Python implementation written from the definition, with no symmetry pruning (s = 3..6). Each leaf is weighted by s^(unassigned entries); the weights sum to exactly s^(3s-4) in every case.
- `check.py brute s`: plain enumeration in Python (s = 3, 4).

"Never fires" is decided exactly by cycle detection (Brent in C++, visited set in Python); there is no step cap. The C++ length cap of n = 200 was never reached.

Full histograms (number of rules whose longest synchronised prefix is 2..L; L = 1 means the rule fails n = 2) agree between the C++ brute force and the Python lazy search for s = 3, 4, 5:
- s = 5: L1 43,920,625; L2 4,663,756; L3 239,212; L4 3,288; L5 1,244.
- s = 6 (Python only): L1 71,265,267,072; L2 6,666,570,006; L3 418,310,088; L4 10,046,130; L5 3,939,300; L6 28,998; L7 2,166; L8 336.

Pruning:
- Lazy assignment is safe: a run that never reads an entry is the same for every value of that entry.
- The C++ dfs additionally allows a new unnamed state only in index order. This is safe because relabelling unnamed states is an isomorphism; an error here could only lower the answer. It is cross-checked by the unpruned programs for s <= 6, but not for s = 7.

Minimum firing time over all rules synchronising a single n (`mintime.py`, exhaustive):
- s = 4: n = 2..6 give 1, 3, 5, 6, 7; no 4-state rule synchronises n = 7 at all.
- s = 5: n = 2..7 give 1, 3, 4, 6, 7, 9.

## 2. Witnesses (`witnesses.txt`)

Numbering: 0 = Q, 1 = G, 2 = F, 3.. = U1, U2, ... A rule line lists d(x,k) for x = Q, G, U1, ... and k = 0, 1, 2.

s = 5, L = 5, rule `0 3 3 3 1 0 2 4 1 2 1 0`:

| state | k=0 | k=1 | k=2 |
|---|---|---|---|
| Q | Q | U1 | U1 |
| G | U1 | G | Q |
| U1 | F | U2 | G |
| U2 | F | G | Q |

Firing times for n = 2, 3, 4, 5 are 5, 8, 7, 10; it fails at n = 6.

Other witnesses:
- s = 3: `0 2 1 1 2 2`, time 1.
- s = 4: `0 3 3 0 0 1 2 3 0`, times 3, 4.
- s = 6: `0 4 3 2 3 0 5 3 0 1 5 0 4 1 3`, times 8, 10, 10, 19, 21, 20, 20.
- s = 7: `0 3 1 6 1 0 5 4 1 2 5 3 4 1 0 3 5 1`, times 9, 10, 17, 19, 14, 23, 43, 39, 24, 38.

All five were accepted by `python3 check.py witness s L "<line>"`, which is independent of the C++ program that found them. A deliberately wrong rule (`0 1 3 3 1 0 2 4 1 2 1 0`) was REJECTED: it synchronises only n = 2..3.

The reason the next value is impossible is the exhaustive enumeration itself: no rule on 5 states synchronises all of 2..6. There is no hand proof; rerun `fsq brute 5` (3 s) or `check.py lazy 5`.

## 3. Headline

**L(5) = 5.** Confidence is very high: three implementations give identical histograms. Calibration: L(3) = 2, L(4) = 3. Beyond: L(6) = 8, L(7) = 11 (the latter from a single program).

The poser's guess "finite and at most 6" is correct.

General form: the sequence 2, 3, 5, 8, 11 for s = 3..7. I have no formula I trust and no evidence either way on whether L(s) is infinite for some s; this is OPEN. As a pure guess, the growth looks roughly linear, about 3 per state at the end.

## 4. Proofs (`proofs.txt`)

- **Fact 0, COMPLETE.** Cell j is Q at all times before j-1, so every firing time is at least n-1.
- **Lemma 1, COMPLETE.** A rule synchronising n = 3 has d(Q,1) not in {Q, F}. This confirms the poser's belief; it was also checked by enumeration for s = 3, 4.
- **Lemma 2, COMPLETE.** If d synchronises n and some m > n, then t(n) >= 2n-1, or t(n) = t(m) with m-1 <= t(n) <= 2n-2. The proof is the usual light-cone argument, with one extra tick because an end cell in Q cannot distinguish "no neighbour" from "Q neighbour". There were zero violations over all rules for s = 3, 4.
- **The poser's belief "a rule synchronising n fires at time >= 2n-2" is FALSE as stated.** n = 2 fires at time 1; a 4-state rule fires n = 4 at time 5; a 5-state rule fires n = 4 at time 4 and n = 7 at time 9. Only Fact 0 and Lemma 2 hold.
- **The poser's L(3) argument is confirmed by enumeration.** Exactly 3 three-state rules synchronise n = 3, all with d(Q,1) = G and d(G,1) = Q, and none of them synchronises n = 2.
- **"Is L(4) >= 3?"** Yes; L(4) = 3 exactly.
- There is no proof and no sketch for the values of L(s) or for the general question.

## 5. Interpretation

- I read the text literally. k counts existing neighbours only. The rule may output G or Q anywhere except that d(Q,0) = Q. A run stops at the first F.
- "Synchronises" requires all cells in F at the same first time. A run that enters a repeated F-free configuration fails.
- I found no ambiguity affecting the answer. The only defect is the false 2n-2 belief above.
- I did not compute any alternative reading, such as a missing neighbour counting as "different".

## 6. Code

- `fsq.cpp` (binary `fsq`): `brute 3|4|5`, `dfs 3..7`. Outputs are in `brute5.txt`, `dfs5.txt`, `dfs6.txt`, `dfs7.txt`.
- `check.py`: `brute 3|4`, `lazy 3..6`, `witness`. This is the independent implementation.
- `mintime.py`: the single-n minimum firing times.
- `witnesses.txt`, `proofs.txt`.
