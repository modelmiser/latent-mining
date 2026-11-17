# solver-b report

All files are in `solver-b/`.

## 1. Tables

| h | 0 | 1 | 2 | 3 | 4 | 5 | 6 | 7 | 8 |
|---|---|---|---|---|---|---|---|---|---|
| d(h) | 0 | 0 | 0 | 2 | 3 | 4 | 5 | 6 | 7 |
| e(h) | 0 | 0 | 0 | 1 | 2 | 2 | 3 | 3 | 4 |

Every entry is COMPUTED exhaustively: the cop wins with that many glimpses and loses with one fewer.

- **h <= 5:** three routes agree: `slow.cpp` (unpruned enumeration of all reachable belief states, least fixpoint), `fast.cpp` (same game, pruned), and `check.py` (independent Python checker on witness files).
- **h = 6:** `fast.cpp` and `fastw.cpp` only.
- **h = 7, 8:** `fastw.cpp` only (512-bit version of the same algorithm).
- d(h) for h >= 3 is also PROVED for all h (section 4), given one certificate.

Model: the cop's knowledge is the set S of possible robber vertices. A glimpse replaces S by one part chosen by an adversary; a cop step to c' removes c'; a robber move replaces S by N[S] minus c'. The cop wins iff he can force S empty.

Prunings in `fast.cpp`, both exact (proofs.txt, section P): tree automorphisms; and, within one glimpse-free search, dropping a state whose S contains the S of an already-seen state at the same cop vertex. A wrong domination pruning would bias towards "cop loses"; the unpruned program agrees on h <= 5.

Sanity checks: h <= 2 give 0; monotone in k where both were run; at the threshold every start vertex wins and below it none does; e <= d; d(h) <= h-1.

## 2. Witnesses

Directory `wit/`, plain text, 0-based heap labels (root 0, children 2v+1, 2v+2). Formats are described at the top of `check.py`.

- **Cop strategies** (decision DAGs; the checker re-derives S itself): `D_h1_k0`, `D_h2_k0`, `E_h2_k0`, `D_h3_k2`, `E_h3_k1`, `D_h4_k3`, `E_h4_k2`, `D_h5_k4`, `E_h5_k2`. All start at the root; capture within 3 to 13 rounds.
- **Impossibility certificates** (a set of states closed under every cop action, covering every opening, with no forced capture): `E_h3_k0` (75 states), `D_h3_k1` (334), `E_h4_k1` (3262), `D_h4_k2` (2817), `E_h5_k1` (14774), `D_h5_k3` (17452; 336 kB).

`check.py` accepts all 15 files (`run_check.log`).

Checks watched failing on purpose (`bad/`): a certificate with one line dropped; the D(3,1) certificate relabelled as D(3,2); the E(3,1) strategy replayed under D rules; a strategy with one move altered. All four are rejected.

## 3. Headline

**h\* = 3**: d = e = 0 for h <= 2, d(3) = 2, e(3) = 1. Confidence is very high: three implementations, checked witnesses, and hand proofs of e(3) <= 1 and d(3) <= 2. The problem's guess of 4 or 5 is wrong.

General form:
- **d(h) = h-1 for all h >= 3**: proved, given the D(3,1) certificate.
- **e(h) <= floor(h/2) for all h >= 1**: proved.
- **e(h) = floor(h/2) for h >= 3**: computed for 3 <= h <= 8, CONJECTURED beyond.
- **d(h) - e(h) >= ceil(h/2) - 1, so unbounded**: proved. Equality holds if the conjecture does.

## 4. Proofs (full text in `proofs.txt`)

- **U1, COMPLETE: d(h) <= h-1.** With the cop at x of height t and the robber below x, a direction glimpse plus one step puts the robber strictly below a child. At height 1 he goes leaf, back, other leaf.
- **U2, COMPLETE: e(h) <= floor(h/2).** After an exact glimpse at distance m >= 2, two steps along the path leave the robber caught or strictly below the cop's new vertex, because N[r] minus c1 lies in the subtree of c2. That is two levels per glimpse; heights 1 and 2 need 0 and 1.
- **L1, COMPLETE: if the cop wins D(h,k), he wins D(h-1, max(k-1,0)).** The cop's play before his first glimpse is robber-independent, so that glimpse happens at a known vertex. Take a root-child subtree Q not containing it and project the cop onto Q, sending everything outside to Q's root. Against a robber who stays in Q, glimpses taken outside Q have a forced answer, so the projected strategy saves at least the first one; robber trajectories in Q lift verbatim. This fails for E, where outside glimpses are informative.
- **L2, COMPLETE-GIVEN-CERTIFICATE: d(h) = h-1 for h >= 3.** Base d(3) >= 2 is `wit/D_h3_k1.txt`; induction by L1; upper bound U1.
- **L5, CONJECTURE, not a proof:** e(h) >= floor(h/2) for h >= 9. Only e(h) >= e(h-1) is proved in general; I found no proof of e(h) >= e(h-2) + 1.

## 5. Interpretation

- A glimpse may be taken in round 1, before any move, as the round order says.
- A robber stepping onto the cop is never advantageous, so it is treated as forbidden.
- "Wins" means bounded-time capture, computed as a least fixpoint; no cycle is credited.
- h = 0: the cop wins vacuously, as stated.
- No alternative reading was needed or computed.

## 6. Code

- `fast.cpp`: table for h <= 6 (`run_fast_56.log` holds h = 5, 6; h <= 4 was printed, not logged).
- `fastw.cpp`: h = 7, 8 (`run_fastw_78.log`, `run_fastw_D8.log`); its recheck of 3..6 was printed, not logged.
- `slow.cpp`: unpruned check for h <= 5 (`run_slow.log`, `run_slow45.log`).
- `gen.cpp`: `slow.cpp` plus witness output; writes `wit/*`.
- `check.py`: witness checker (`run_check.log`).
- `proofs.txt`: the proofs.
