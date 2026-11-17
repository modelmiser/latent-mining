# solver-a report: glimpse cop/robber on complete binary trees

## 1. Tables

| h | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 | 9 |
|---|---|---|---|---|---|---|---|---|---|---|
| e(h) | 0 | 0 | 0 | 1 | 2 | 2 | 3 | 3 | 4 | 4 |
| d(h) | 0 | 0 | 0 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |

Every entry is COMPUTED, exhaustively, and also PROVED (section 4).

Method: belief-set game. A state is (glimpses left, cop vertex, set S of possible robber vertices); an adversary picks glimpse answers. The cop wins iff he can force S empty. For each k, both "cop loses with value-1" and "cop wins with value" were run.

- `fast.cpp` (128-bit masks, h <= 6): memoised search over reachable states; within one glimpse level play is deterministic, so it is breadth-first reachability to a goal. No pruning beyond memoisation; "stay" moves are kept. For h <= 6 losses were tried from all 2^(h+1)-1 starts.
- `fast2.cpp` (same code on 1024-bit sets, h <= 9; one start per depth, the rest by symmetry). Agrees with `fast` on h = 3..6. Largest run: D, h=9, k=7, 4.9M states, 94 s, 0.95 GB.
- `slow.py`: independent all-states solver (every subset S, fixpoint sweeps, no reachability), h <= 3, k <= 2. Same values; 0 mismatches against every state cached by `fast` (up to 678 states per run).
- `lemma_check.py` part 4: a third, independent recursive solver, h <= 6 for E.

## 2. Witnesses (plain text, checked by `check.py`, written separately from the solver)

- Cop strategies `strat_{E,D}_h{1..6}_k{value}.txt`: table state -> move, or glimpse with a move per answer. The checker follows every answer and requires capture with no cycle. All ACCEPT.
- Impossibility `lose_{E,D}_h{3..6}_k{value-1}.txt`: a set L of states containing every start, closed under every blind move, with one listed glimpse answer after which every cop move stays in L, and never an empty S. All ACCEPT. Sizes: D h=3 k=1 is 250 states; E h=3 k=0 is 75; largest D h=6 k=4 is 56,127 states (2.0 MB; E h=6 k=2 is 0.63 MB).
- Watched failing on purpose: four mutants in `mutants/` (a D strategy with its glimpse budget cut, a strategy with one move changed, a certificate with a line removed, a false "e(3) >= 2" certificate) are all REJECTED.
- h = 7..9 have no dumped witnesses; those values rest on `fast2` and the proofs.

## 3. Headline

**h* = 3.** d(h) = e(h) = 0 for h = 0, 1, 2; d(3) = 2 > e(3) = 1. Confidence: very high (three independent programs, checked certificates both ways, plus proof).

General formula, proved: **e(h) = floor(h/2) and d(h) = h-1 for all h >= 3**; both 0 for h <= 2. So d(h) - e(h) = ceil(h/2) - 1, unbounded.

The statement's beliefs are all correct; its guess "h* is 4 or 5" is wrong; "unbounded" is right.

## 4. Proofs (full text in `solver-a/proofs.txt`; none is a sketch)

- Upper bounds, COMPLETE. With the robber strictly below the cop in a T_m: a direction glimpse and one step down leaves T_(m-1); an exact glimpse showing the robber two or more levels down lets the cop take two steps towards her and leaves T_(m-2); a parent of two leaves is cleared blind. Hence d <= h-1, e <= floor(h/2).
- Projection lemma, COMPLETE: a cop win on a tree gives a cop win on any subtree against a robber confined to it.
- Blind lemma, COMPLETE: on T_m, m >= 3, from a full start with no glimpses, every vertex of height >= 2 other than the cop's stays possible for ever; a seeded version covers the rounds after an exact glimpse. So a blind cop never wins for h >= 3.
- d(h) >= h-1, COMPLETE modulo one computation. Base "D(3,1) is lost" is `lose_D_h3_k1.txt`. Step: the robber commits to the root's child subtree X not holding the cop at his first glimpse; the answer "towards X" tells him nothing, so he would win T_(m-1) with one glimpse fewer.
- e(h) >= floor(h/2), COMPLETE modulo one tiny computation (8 blind states on T_3, `lemma_check.py` part 3). At each glimpse the blind lemma supplies a possible vertex u at distance >= 2 heading a subtree at most two levels shorter that the cop is outside; the adversary answers u, and induction applies.

`lemma_check.py` also tests the blind lemmas (h = 3..8 full start, h = 4..7 seeded) and the induction claim (h <= 6) numerically: all hold.

## 5. Interpretation

- One glimpse per round, before the move, allowed from round 1. Several per round would change nothing: the robber does not move between them.
- "Deterministic cop, robber knows the strategy" is taken as: the adversary may give any answer consistent with the history. A bounded capture time then follows from finiteness.
- The cop picks his start; losses were checked from every start.
- No ambiguity affects the answer. The statement is not degenerate.

## 6. Files (all in `solver-a/`)

`fast.cpp` (h <= 6 values, witnesses), `fast2.cpp` (h = 7..9), `slow.py` (h <= 3 cross-check), `check.py` (witness checker), `lemma_check.py`, `blind.py` (exploration only), `proofs.txt`, `run_all.sh` (reproduces everything).
