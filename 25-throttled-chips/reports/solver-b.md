# solver-b report

All files are in `solver-b/`.

## 1. Tables

| h | G(h) | status | C(h) (COMPUTED) |
|---|------|--------|------|
| 1 | 1 | COMPUTED (exhaustive) | 1 |
| 2 | 2 | COMPUTED (exhaustive) | 2 |
| 3 | 5 | COMPUTED (exhaustive) and PROVED (P2 + witness) | 7 |
| 4 | 10 | COMPUTED (exhaustive, two independent prunings) | 10 |
| 5 | 15 | COMPUTED (exhaustive, zone rule only) | 15 |
| 6 | 16 <= G(6) <= 20 | lower bound COMPUTED (exhaustive, zone rule) | 20 |
| 7..12 | not computed | | 25, 30, 35, 40, 45, 50 |

- C(h): all Firer lines against the constant list are enumerated. h <= 6 by the slow Python reference, h <= 12 by C++. C(h) = 5h-10 for 4 <= h <= 12.
- G(h) upper bounds are explicit lists checked by the slow reference. Lower bounds are exhaustive searches over all lists, made finite by two proved reductions:
  - Lemma C (cluster rule, unrestricted Firer): h=3 (m<=4), h=4 (m<=9), h=5 (m<=12).
  - Lemma Z (zone rule, Firer restricted to a subclass of lines): h=3 (m<=4), h=4 (m<=9), h=5 (m<=14), h=6 (m<=15).
- G(5) >= 15 rests on Lemma Z alone for m = 13, 14. The two methods agree wherever both ran. The h=6, m=16 run hit the 1500 s limit.
- Evidence only (all lists inside a window around a_1, unrestricted Firer): no winning list for h=5, m=14, radius <= 4; h=6, m=19, radius 1; h=7, m=24, radius 1. The h=6, m=19, radius 2 run timed out.

**Secondary: "G(h) < C(h) for every h >= 3" is FALSE.** G(3) = 5 < 7, but G(4) = C(4) = 10 and G(5) = C(5) = 15.

Unverified beliefs in the statement:
- G(1), G(2), C(1), C(2), C(3), G(3) = 5 and h <= G <= C are confirmed.
- The sqrt(24) bound is not established by the argument given (P3 remark); done exactly, the idea proves C(h) <= 5h-6.
- The guess "L strictly below lim C(h)/h" is contradicted at h = 4, 5.

## 2. Witnesses

- `witnesses.txt`, checked by `verify.py` (output `verify_out.txt`):
  - G(3): `0 1 0 1 0`
  - G(4): ten zeros (also `0 -1 1 0 0 0 0 0 -1 0`)
  - G(5): fifteen zeros
  - Constant lists one shorter, each with an explicit surviving Firer line.
  - Two deliberately wrong objects, both rejected.
- `winners_h3_m5.txt`: all 8 winning 5-lists at h=3.
- "Next better value impossible" is the exhaustive logs `log_zone.txt` and `log_cluster.txt`; there is no per-list certificate. Reproduce with `run_final.sh`.
- `log_misc.txt`, `run_h5_m14_R4.log`: C(h) table, prune-off runs, window runs.

## 3. Headline

**Not settled.** I do not have the exact G(h) for all h.

- Proved: 2h-1 <= G(h) <= C(h) <= 5h-6 for h >= 3, so 2 <= liminf G(h)/h <= limsup G(h)/h <= 5. Existence of the limit is not proved.
- Conjecture (a guess, about 60%): G(h) = C(h) = 5h-10 for all h >= 4, hence L = 5. It rests on two values of h for G and nine for C. Mixing sites gains at h = 3 only; I cannot rule out a gain for large h.

## 4. Proofs (proofs.txt)

- P0 COMPLETE: a surviving line can be taken with <= m-2 firings.
- P1 COMPLETE: h <= G(h) <= C(h).
- P2 COMPLETE: G(h) >= 2h-1 for h >= 3. Firer fires only the unique vertex with >= h adds, each time it shows h-1.
- P3 COMPLETE: C(h) <= 5h-6 for h >= 2, by the potential Q with exact discrete filling.
- Lemma C COMPLETE: in a shortest winning list, consecutive add positions p < q with mA adds on the left and mB on the right satisfy q-p <= rho(mA-2)+rho(mB-2).
- Lemma Z COMPLETE: if no "zone-connected" list of length <= M beats the zone-restricted Firer, then G(h) > M.
- SKETCH only: the fluid heuristic for L = 5. C(h) = 5h-10 is not proved in either direction.

## 5. Interpretation

- Literal reading: the list is committed, Firer knows it and fires at most one ready vertex per round, and a list wins if height h occurs at any step up to round m. No ambiguity found; the only wrong item is the sqrt(24) derivation.
- Side note, different game: an adaptive Adder wins in 4 rounds at h=3 and 7 at h=4 (`adapt.cpp`).

## 6. Code

- `slow.py`: direct definition; C(h) for h <= 6, `wins`, `survivor`, window brute force.
- `g2.cpp` (binary `g2s`): Lemma C search, gives `log_cluster.txt`.
- `g4.cpp` (binaries `g4`, `g4w`, `g4m`): Lemma Z search (MODE 2, T1=2, T2=4) gives `log_zone.txt`; window search (MODE 0) and the C(h) table give `log_misc.txt`.
- `zslow.py`: slow reference for the zone game; `check_lemmaZ_h3.txt` is its check of Lemma Z at h=3.
- Superseded or unused: `gsearch.cpp`, `g3.cpp`, `g0.py`, `driver.sh`.

Cross-checks: C++ against `slow.py` and `zslow.py` on small windows (same first winning list or same "none"); hitting prune switched off (same answers).

Caveats:
- The fail-memo is keyed by a 122-bit hash. A collision, or an error in any prune, could only hide a winning list, making G look too large.
- Process limit: a few times a third process of mine overlapped briefly, when the harness backgrounded a command while an earlier one was still finishing.
