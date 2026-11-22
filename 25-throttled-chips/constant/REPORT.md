<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the first follow-up instance (the constant bound 5h - 176), verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Report: constant bound for the lag strategy (goal A reached)

Goal (A) is proved for solver-a's Theorem 2 strategy, unchanged: height <= t/5 + 35.2 at every step, so 5h - 176 <= G(h) <= 5h - 6. The harness refused to write REPORT.md, so this text is the report; the full proofs are in `(staging)/proofs.txt`.

## What is proved (all COMPLETE)

Notation: e_y = number of adds so far outside [y-2, y+2], so eta*_y = (t - e_y)/5. Any tie rule.

**Theorem A.** Against every list, every vertex y has height <= t/5 + 35.2 at every step (<= t/5 + 34 after step 2 of a round).

**Corollary.** Every list of length <= 5h - 177 loses, so 5h - 176 <= G(h) <= C(h) <= 5h - 6 (upper bound from solver-b P3).

The proof uses only facts (a), (b), (d) of Theorem 2:

1. **Lemma 1 (level sets).** For a >= 5 let Phi_a = sum_z max(l_z - a, 0).
   - An add raises Phi_a by at most 1.
   - If the after-add maximum lag M is >= a+1 (so >= 6), fact (d) fires a vertex of lag M, and Phi_a drops by exactly 1. So Phi_a does not increase over that round.
   - If M < a+1, all lags are < a+1 after the round.
2. **Lemma 2 (local lag bound).** After every round, for every y and every a >= 5: l_y <= a + 7 + e_y/a.
   - Suppose L = l_y > a+7. Let tau be the last round with M < a+1 (or time 0).
   - Then L - a <= Phi_a(now) <= Phi_a(tau) < |Z|, where Z is the set of vertices with lag > a at time tau; each contributes less than 1.
   - Remove the 7 vertices of [y-3, y+3]. More than L - a - 7 vertices z remain, each with T_z > a, and T_z for these counts only chips outside [y-2, y+2].
   - One chip contributes at most 5 to sum_z 5T_z, so e_y > a(L - a - 7).
3. **Theorem A.** Take a = 10: l_y <= 17 + e_y/10. By the height identity, eta_y <= eta*_y + 2 l_y <= (t - e_y)/5 + 34 + e_y/5 = t/5 + 34. The next add raises l_y by at most 3/5, giving 35.2.

The idea the sqrt(t) proof lacked: a large lag at y has to be paid for by chips added away from y, and those chips are exactly the slack between eta*_y and t/5. The same proof covers the lazy variants (fire only at lag >= theta, 6 <= theta <= 11). The constant is not optimised.

## What I computed

- **Lemma tests** (`test_lemmas.py`, output `test_out.txt` and `test_out2.txt`): fact (d), Lemma 1, each inequality in the proof of Lemma 2, a static counting check and Theorem A all pass. The largest run was 126 lists (length 1200, three tie rules) for each of theta = 1, 6, 11.
- **Where the tests bite:** for theta = 1 the lag stays <= 2.6, so (d), Lemma 1 and the chain are never exercised on the real strategy, only on the lazy variants.

  | theta | max lag | max height - t/5 | Lemma 1 instances | chain instances |
  |---|---|---|---|---|
  | 1 | 2.6 | 3.0 | 0 | 0 |
  | 6 | 7.4 | 8.0 | 344,260 | 3,054,414 |
  | 11 | 12.4 | 15.2 | 322,048 | 2,448,680 |

- **Deliberately wrong statements, each rejected by the test:**
  - `--wrong 1`: Lemma 1 with a below theta - 1.
  - `--wrong 2`: the counting step without removing [y-3, y+3].
  - `--wrong 3`: e_y counted outside [y-4, y+4].
  - `--wrong 4`: height <= t/5 + 1.
- **One stale line:** my first `--wrong 3` used a smaller window, which weakens the statement, so it passed. That pass is still recorded in `test_out.txt`; the corrected rejection is in `test_out2.txt`.

## Unexpected finding: the lag itself is probably unbounded (EVIDENCE, not proof)

The referee's follow-up 1 asks for a proof that the lag is bounded by a constant (observed <= 2.4). That is probably false for this strategy. A staged greedy adversary (`lagadv2.cpp`, leftmost tie rule) works on n consecutive vertices: 8 sweeps, then adds chosen to maximise the number of vertices at a rising target lag. List length is about 9.2n.

| n | 16 | 64 | 128 | 512 | 1024 | 4096 | 8192 | 16384 | 32768 |
|---|---|---|---|---|---|---|---|---|---|
| max lag | 2.4 | 2.6 | 3.4 | 3.8 | 4.4 | 5.4 | 5.6 | 6.4 | 6.6 |

- **Growth:** about +0.3 per doubling of n (seed 1; seeds 2 and 3 give 4.8 at n = 4096).
- **Independent replay:** the lists for n = 512 (length 4656, lag 3.8) and n = 16384 (length 150161, lag 6.4) were replayed by a separate exact-rational program written from the theorem text (`replay.py`), with the same result.
- **Tie rule dependence:** the lists are tuned to leftmost ties. The n = 512 list gives 2.2 under rightmost and 3.4 under highest-first.
- **Heights:** height - t/5 <= 0.8 in all these runs, consistent with Theorem A; the lag is spread over many vertices.
- **Mechanism:** a top-lag vertex is fired at once unless another ready vertex ties it. One add lifts both neighbours to a new level, one is fired and one survives, so each +1/5 of lag costs a constant factor in vertices.
- **Not proved:** that the growth continues for ever; there is no explicit infinite construction.

## What failed or was not done

- One-step greedy on sum c^lag (`lagadv.cpp`) stalls at lag 2.0: it parks one high vertex and shuttles around it. The staged target fixed that.
- Not attempted: a log t bound for the lag, optimising c, proving C(h) = 5h - 10.
- Fact (d) was not asserted on the real strategy at lag >= 6. Only the n >= 16384 lists reach it, and `replay.py` does not check it.

## Files

All in `(staging)/`:
- `proofs.txt`
- `test_lemmas.py`, `test_out.txt`, `test_out2.txt`
- `lagadv2.cpp`, `lagadv2_out.txt`, `lagadv2_big.txt`
- `replay.py`, `replay_n16384.txt`, `list_n512.txt`, `list_n16384.txt`
- `lagadv.cpp`, `lagadv_out1.txt`, `lagadv_trace.cpp`, `dbg.py`
