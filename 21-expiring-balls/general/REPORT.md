<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the second follow-up instance (general bounds, N(5) <= 21), verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Report: general bounds for the expiring-balls game (experiment 21)

Folder: `(staging)/`

**REPORT.md was not written: the harness refused the file write, so this text is the report.** PROOFS.md and all programs and outputs are in the folder.

## Strongest statements

1. **N(5) ≤ 21 — PROVED and COMPUTED.** The previous bound was 28; 21 = 5 + 2^4 is what the fit k + 2^(k−1) predicts.
   - Proof: `PROOFS.md`, Theorem U.
   - Certificate: `adv_k5_n21.txt`, 3,676 positions with one Adversary move each, accepted by `check_adv.py`. The checker is written from the game definition and shares no code with the generator.
2. **N(k) ≤ 5·2^(k−3) + 1 for every k ≥ 5 (21, 41, 81, …) — PROVED** (Theorem U, by induction).
   - The old recursion N(k−1) + 2^(k−1) gives 28, 60, 124, … (about 2^k); this is about (5/8)·2^k.
   - It does not reach the fit for k ≥ 6 (38, 71, …).
   - COMPUTED for k = 6: the strategy written out on 41 bins (`adv_k6_n41.txt`, 2,689,236 positions, 510 MB) is accepted by the checker.
3. **N(k) ≥ ⌊(k+1)²/4⌋ + 1 for every k — PROVED** (Theorem L).
   - It beats 2k from k = 6 on (13, 17, 21, 26 against 12, 14, 16, 18).
   - It does not use the FIFO order: the same Rule is safe when the Adversary removes any ball. So the any-removal number lies between about k²/4 and k²/2.
   - It is the known "Robin Hood" argument adapted to forests, not a new idea.
4. **A lower bound that uses FIFO, or anything superpolynomial from below — FAILED.** Goal 1 as asked is not achieved.
   - N(5) is now between 12 and 21. The 12 comes from monotonicity and the repository's 11-bin certificate; it would be 14 if solver A's uncertified any-removal computation is trusted.

## Idea of the upper bound

For target k, a bin of load k−1 (a "forcer") makes the Rule give the ball to the other bin. Lemma C generalises this: if bins of loads j, j+1, …, k−1 stand in separate components, any bin f in a further component can be fed one ball from each as the newest balls, or load k appears. Everything older is then expired, and f keeps load k−1 on few bins.

The strategy runs three rounds of this, with feeders built by plain merging:

| Stage | Result | Bins occupied |
|---|---|---|
| Round 1 | forcer | 2^(k−2) + 1 |
| Round 2 | forcer | 2^(k−3) + 2 |
| Round 3 | clean (k−1)-star | k |
| Final | merge on fresh bins | 2^(k−1) more |

For k = 3 and 4 it reproduces the 7- and 12-bin strategies. Round 2 is the bottleneck for k ≥ 6.

## Computations and their failing runs

- **Generator and checker.** `gen.py K N out` writes the strategy as a table and is untrusted. `check_adv.py L n table` accepts only if, from the empty position, every Rule answer leads to load L or to a table position, every move is legal on n bins, and no position can repeat. It accepts the tables for (2,4), (3,7), (4,12), (5,21) and (6,41).
- **Wrong inputs rejected.** The same tables with one bin fewer or a higher target were rejected in all 7 runs, including (6,40). `gen.py` on 6, 11, 20 and 40 bins stops with "out of bins".
- **Damaged tables** (`damage.py`, `damage2.py`):
  - k = 4, all 101 lines: every deletion and every expire/offer swap is rejected. Of 57 altered offers, 36 are rejected; the 21 accepted ones only rename an untouched bin, which is the same move.
  - k = 5, 150 sampled lines: every deletion and swap is rejected. Of 400 sampled altered offers, 297 are rejected and 103 are accepted, all of them renamings; none is a different move.
- **Game model.** `solve_small.py` is an exhaustive solver using the checker's position conventions. It reproduces N(1) = 2, N(2) = 4, N(3) = 7, including no win on 6 bins, so the checker's model is not too kind to the Adversary at small sizes.
- **Theorem L.** `rh_test.py` plays the Rule against a random any-removal adversary and asserts the proof's steps. It passes on 4 settings and fails as it should with the tie-break reversed. Random play is a weak adversary; the proof is what counts.

## What failed and why

- **A real loop at k = 4.** The first generator run found that a play can return to an earlier position during a clean-up, so a one-move-per-position table needs the detour cut out. The generator now rewinds (6 times at k = 5, 38 at k = 6); the checker's no-repeat test is what guarantees the result.
- **Reaching k + 2^(k−1) for k ≥ 6.** Every variant I tried needs, at one moment, a forcer, a second bin of load k−2 and a bin of load k−3 in three components. With merge-built pieces that is 2^(k−2) + 2^(k−2) + 2^(k−3) bins. Two compact leaders at once would fix it, but at k = 4 that would give 11 bins, which the repository's computation excludes.
- **Lower bound from FIFO.** I have only a picture, marked SKETCH in `PROOFS.md`: each feeder component feeds a bin once, and only under a standing chain of higher loads. It proves nothing.
- **Beating 21 at k = 5.** Not attempted beyond the above.

## What a reader must still trust

- Theorems U and L are hand proofs that nobody but me has read.
- For k ≤ 6 the certificates replace Theorem U, provided `check_adv.py` (about 150 lines of Python) is right.
- The checker identifies positions up to renaming of bins and treats untouched bins as interchangeable. This is justified in its header, not machine-checked.
- The checker was not compared with the repository's checkers, which use a different file format.
- The upper bounds use nothing from the repository. The lower end of the N(5) range (12) rests on the repository's 11-bin certificate.
- For k ≥ 7 there is only the hand proof.

## Files

- `PROOFS.md`
- `gen.py`, `check_adv.py`, `damage.py`, `damage2.py`, `solve_small.py`, `rh_test.py`
- `adv_k2_n4.txt`, `adv_k3_n7.txt`, `adv_k4_n12.txt`
- `adv_k5_n21.txt` (sha256 2bb97e44…6119f3)
- `adv_k6_n41.txt` (510 MB, sha256 c432e3cc…7792aa; regenerate with `gen.py 6 41`, about 80 s)
- `out_check_small.txt`, `out_check_k5_n21.txt`, `out_check_k6_n41.txt`, `out_damage.txt`, `out_damage2.txt`, `out_solve_small.txt`, `out_rh_test.txt`

Nothing was written under `(workspace)`, and nothing was sent or published. Folder size is 488 MB.
