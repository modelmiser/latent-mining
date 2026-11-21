# Report: experiment 25, lag of the firing rule (21 November 2026)

Open end (1) is settled with complete proofs. Open end (2) gets a side result only; 176 is not improved.

## Strongest statement

**The lag of the firing rule is unbounded and grows like the logarithm of the list length.** Over lists of length t, the largest lag after a round lies between 1.6 + 0.2·⌊log₃₄((t − 84)/24)⌋ and 2 + H₃ₜ (harmonic number), that is between about 0.056·ln t and ln t + 4.1. Observation B is true; no bound independent of e exists.

| Part | Status | Where |
|---|---|---|
| **Theorem U**: after every round the k largest lags sum to at most k(3 + H_n − H_k), n = number of points within distance 1 of the list; so every lag ≤ 2 + H_n ≤ 2 + H₃ₜ, for any tie-breaking. Replaces 6 + √t. | PROVED (short induction) | `PROOFS.md` |
| **Theorem L**: for every tie rule that depends only on the play so far, and every j, a list of length ≤ 24·34^j + 84 after which some lag is (8 + j)/5 | PROVED (construction: base, one stage lemma, counting) | `PROOFS.md` |
| The construction played against the real game with every claim of the proof asserted: N = 34 and 1156 gadgets give lag 1.8 and 2.0 under leftmost, rightmost and a fixed pseudo-random tie order; N = 39304 gives 2.2 (leftmost) | COMPUTED | `gen.py`, `gen_out_1156.txt`, `gen_out_39304.txt` |
| Greedy variant (same asserted claims): lag 2.6, 3.6, 4.0 after a round for N = 1156, 39304, 300000 (lengths 35 374, 1 221 687, 9 331 937), leftmost; 3.4 for N = 39304, pseudo-random order | COMPUTED; every list replayed by a separate C++ program | `replay.cpp`, `replay_out_*.txt`, `gen_out_300000.txt` |
| Theorem U tested for every k and round on the repository's n = 512 list (lag 3.8), one generated list, 8 random lists | COMPUTED; not binding | `check_U.py`, `check_U_out.txt` |
| **Corollary U2**: every list with m/5 + 5.2 + 2H₃ₘ < h loses, so G(h) ≥ 5h − 26 − 10·H₁₅ₕ: G(59) ≥ 200, G(100) ≥ 398, G(1000) ≥ 4873 (5h − 176 gives 119, 324, 4824). Beats 2h − 1 from h = 28. | PROVED from Theorem U; values COMPUTED | `PROOFS.md`, `corollary.py`, `corollary_out.txt` |
| Constant below 176; upper bound below 5h − 6; exact G(6), G(7) | NOT ATTEMPTED | |

## How Theorem L works

First one chip on every point: no firing, every lag 1, and every later top-lag point is ready. A gadget is flank–centre–flank with spare points outside. With the flanks one level (1/5) below the top and the centre two below, an add at the centre makes the centre the single largest lag, so it is fired and both flanks reach the top at no cost; a second add lifts both flanks above everything, and the firer can take only one. Fired flanks are refilled by adds next to them, paid for by letting the firer shoot other top flanks. Each level costs a factor 34 in the proof, 2 to 4 in the greedy runs.

## Failed

- A construction with no ties (every firing forced): forced moves alone raised a plateau by two levels only.
- Merging two mirrored gadgets at a shared point: the survivor sits in the middle and cannot be paired again. Fresh gadgets per level work but give only log n / log log n; refilling fixed that.
- Constant in G(h): nothing beyond Corollary U2, whose loss grows like 10·ln h; 5h − 176 is stronger again from h of order 10⁵ (estimate; the exact script ran out of memory there).

## Checks watched failing

`gen.py --no-reservoir` and `gen.py --overdraw` (assertions fail, `neg_out.txt`); `replay` with the expected level one too high (exit 1); `check_U.py` with constant 0, and with a wrong firing rule.
Two checks did not fail where I expected: a leftmost-rule list replayed under the rightmost rule, and a pseudo-random-order list under leftmost, still reached the level, so the replays do not show that a list depends on its tie rule. `check_U.py` also passes with constant 1 for 3.

## What a reader must still trust

- Both proofs are by hand, one instance, unreviewed. The claims of Theorem L are asserted in the runs, which reach 12 stages.
- Theorem L does not cover a firer whose tie-breaking looks at the rest of the list.
- Facts (a), (b), (c) of the earlier proof files; fact (d) is re-proved with threshold 3.
- No literature check (no network); Theorem U's argument is the standard one for the greedy "cup game".

## Files

All in `.`: `PROOFS.md`, `REPORT.md`, `gen.py`, `replay.cpp`, `check_U.py`, `corollary.py`; outputs `gen_out_*.txt` (`gen_out_1M.txt` is a run that hit the 6 GB cap, no result), `replay_out_*.txt`, `check_U_out.txt`, `corollary_out.txt`, `neg_out.txt`; lists `list_<N>_<tie>[g].txt` (g = greedy). Nothing was written under `~/Claude`; nothing was sent.
