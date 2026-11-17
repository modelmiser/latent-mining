REFEREE REPORT

Solver B is right on the disputed point: P6 is a complete, manager-independent proof of h^N(M) >= 7M/4 - 17/4. Solver A's "only works against packing managers" objection applies to A's own S1 attack (keep every second 1, free every second 2), which is a different argument; A never considered B's residue-class argument.

**L2 (B's P6): CONFIRMED.** Line by line:
- Stages 1-2: X >= 2M/3 follows (the fewest class holds <= M/3). No placement assumption; classes are residues of absolute cell index.
- Stage 3: legality holds (live <= X+2y-2 <= M-2). A 2 on two consecutive cells covers exactly two of the three classes, so at least one of {a, a+1}; hence y_a + y_{a+1} >= y. This is true for any placement, including spread-out or gapped ones. Averaging gives an r with 2x_r + y_r >= X + y/2. Follows.
- Stage 4: frees are always legal and only lower live. The survivors each cover exactly one class-r cell, all distinct, and live = x_r + 2y_r. Follows.
- Stage 5: t = floor((M - x_r - 2y_r)/3), live <= M-3 before each 3. Every 3-window contains exactly one class-r cell, which must be free, so each 3 takes a fresh class-r cell. Count x_r + y_r + t <= ceil(H/3). Follows.
- Arithmetic rechecked: 3t >= M - x_r - 2y_r - 2 and y >= (M-X-1)/2 give H >= M + (3X+M-1)/4 - 4 >= 7M/4 - 17/4. No off-by-one found.
- Against the table: ceil((7M-17)/4) = 1,3,5,7,8,10,12,14,15,17,19 for M = 3..13, never above h^N (4..22). No contradiction.

**L1 (A's P1 read; B's P1 is the same argument): CONFIRMED.** No gaps. 3(K+t)-2 = 1,4,7,7,10,10,13,16,16,16,19 <= h^O for M = 3..13.

**U1 (A's P2 read in full; B's P5 not checked line by line): CONFIRMED.** Facts (a)-(c), the gap count f <= (s-1)(b+1) and f + b <= c+1 <= M all check; a block cannot straddle the extent because cell E-1 is occupied. My exhaustive first-fit-vs-every-adversary run in game O, M = 3..11, gives max extent 4,6,8,10,12,14,15,17,19, always <= ceil(5M/3) and equal to it from M = 6. Reachable-state counts match both solvers' logs. ceil(5M/3) >= h^O(M) for all M = 3..13.

**U2 (A's P3, 43M/18 + 12): CONFIRMED by reading, weakly tested.** I rederived the three inequalities (with M' = M+5, from the hexad about to be opened), the LP vertex (coefficients of n_a, n_b are non-negative, so all-tight is the maximum), and 86/216. It is a proof, with no unchecked step that I could find. The machine evidence is only a random stress test of the hexad manager (M = 6..120, worst extents far below the bound), not an exhaustive check. B's 3M-6 (P2) is also correct and is tight at M = 4.

**Task 3: independent table (v3.cpp, table_3_8.log, table_9_11.log).** My own greatest-fixpoint solver, written from the game definition. M = 3 was worked by hand first (H=3 loses to 111 -> .1. then allocate 2; H=4 wins by case check). Result for M = 3..11: h^N = h^F = h^O = 4,6,7,9,11,13,14,16,18, each with H-1 verified LOSE. This matches both solvers. M = 12 and 13 were not recomputed, so the N/F separation at M = 13 rests on the solvers' runs only.

**Task 2: P6 as an algorithm (p6.py, p6_opt.py).**
- Exhaustive best response (p6_A.log, p6_A2.log): the least H at which any manager survives the P6 adversary, with every choice the proof leaves open resolved in the manager's favour. M = 3..16: 3,4,7,7,10,13,13,16,16,19,19,22,22,25 against a bound of 1,3,5,7,8,10,12,14,15,17,19,21,22,24. Never below the bound and never above h^N. Equality at M = 6, 13, 15 means a manager survives at exactly the bound, so asking for bound+1 fails; the test could have failed and the constant is tight for this adversary.
- Optimal managers (p6_opt.log): a random safe-set manager at H = h^N(M), M = 7..10, 500 runs each: extents 10-11, 13, 13-14, 16, all >= bound.
- Large M (p6_B.log): 321 managers per M (first fit, best fit, 12 gap-leaving "spread" managers, 6 zoned/stride managers, one manager designed against P6, 300 random). Minimum forced extent 40, 61, 82, 103 for M = 24, 36, 48, 60 against bounds 38, 59, 80, 101. Zero violations; the class-r count is asserted in every run.
- Negative controls (p6_C.log, p6_opt.log): dropping the 2s stage (the L1 adversary) falls short of the bound from M = 48 (77 vs 80, 98 vs 101, 197 vs 206, 398 vs 416). Picking the worse r against a lopsided manager falls short from M = 48 (78 vs 80, up to 399 vs 416). One designed control was uninformative: the worse-r adversary against my "smart" manager, which balances both classes, did not fall short.
- Not finished: the exhaustive best-response run for M = 17 was still running when I stopped (it carries its own 1500 s timeout). No exhaustive check exists above M = 16.

**Task 6 verdicts.**
- (i) mu^O = 5/3: PROVED (L1 and U1, both read and found complete).
- (ii) mu^N >= 7/4: PROVED (B's P6).
- (iii) mu^O < mu^N: PROVED, as a consequence of (i) and (ii). The data alone would not show it: h^N = h^O through M = 12, and the two bounds only separate for M above about 59.
- (iv) mu^F: nothing beyond 5/3 <= mu^F <= mu^N <= 43/18 is proved. h^F = h^O for M <= 13 is data only (M <= 11 reproduced by me). B's remark that the P6 plan gains nothing in F, and A's S1, are heuristics about one plan, not theorems. Whether mu^F = 5/3, equals mu^N, or lies strictly between is OPEN.

**Not checked:** B's P5 line by line; hand.txt for M = 4; either solver's code; A's and B's S1 sketches (including the 11M/6 first-fit claim); the table at M = 12, 13.

Everything is in ./verifier/ ; nothing in the solver folders was modified.
