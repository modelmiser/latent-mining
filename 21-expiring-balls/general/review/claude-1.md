<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: experiment 21, general bounds and N(5) <= 21

Paths are relative to `21-expiring-balls/`. I read the definition, `check_adv.py`, `PROOFS.md`, the README, the output files and the helper scripts. I did not run any repository program. I wrote my own replay (concrete bins 0..n-1, balls oldest-first as (u, v, receiver), Expire removes the first ball, union-find legality on real bins, positions memoised on a different invariant from the table's: renaming oldest-first, other end before receiver; a cycle on the current play fails).

## Verdicts

**Q1: CONFIRMED.** `general/check_adv.py`:
- Legality: equal or negative bins rejected (104); bin count m + unnamed bins <= n (93-95, 106-108); same-component offers rejected (110-112); an unnamed bin is an isolated vertex, so offers to it are always cross-component.
- Expire drops the last element of a newest-first list, which is the oldest ball (99); on the empty position it is rejected (97-98).
- Both answers are followed (115-123); a win is counted only when the receiver's load reaches L (116-117); every non-winning successor is pushed and looked up, and a missing one is rejected (90-91, 146-147).
- Acyclicity is needed because otherwise the Rule could steer play round a loop forever and load L would never appear. The grey/black depth-first search (128-147) is the standard back-edge test and the stored iterator resumes correctly.
- Renaming is sound: the rules never mention bin names, and a first-appearance renaming of an ordered ball list is a complete invariant of the list up to a bijection of touched bins; the number of untouched bins is n - m in both. The table's naming is enforced on every line (75) and the lookup key is the same text (79, 90).
- Malformed lines crash with a traceback and never print ACCEPT. I found no way to make it accept a table that does not win.

**Q2: CONFIRMED for the result, with two wording defects (findings 1, 2).** My replay of `adv_k5_n21.txt`: all 3,676 lines reached, 944 winning answers, 21 bins touched at most, no repeat, longest play 134 moves, always ending in load 5. Same counts as `general/out_check_k5_n21.txt:1`. It fails on 20 bins (needs a 21st bin) and with target 6 (position missing), at the same positions as `general/organiser_check.txt`. The tables for k = 2, 3, 4 give 3/2, 13/5, 101/28, as in `out_check_small.txt:1-3`. I repeated the k = 4 damage test with my replay: 101 deletions, 44 E-to-offer and 57 offer-to-E swaps all fail; of 57 altered offers, 36 fail and 21 pass, and all 21 rename an untouched bin. That matches `out_damage.txt`. `solve_small.py` read, not rerun; its fixpoint (lines 51-59 of the script) is the correct one.

**Q3: CONFIRMED.** `general/PROOFS.md:27-85`. Lemma C: after c_i receives, g and c_m both have load m at each step, so load k appears; legality holds as stated. Rounds 1 to 3: in each, the balls to be removed are all older than the kept ones (p, q, q' are counted before the kept tree and the fed balls are placed), so (W) applies. Counts: Round 1 b_(k-1) + 2^(k-2); Round 2 5·2^(k-3) + 1; Round 3 5·2^(k-3); Final k + 2^(k-1). At k = 5: 20, 21, 20, 21. At k = 6: 37, 41, 40, 38. Induction complete (k - 1 <= 2^(k-3) from k = 5; 9·2^(k-4) + 1 used only for k >= 6). 81 at k = 7; the recursion gives 28, 60, 124; the fit 38, 71.

**Q4: CONFIRMED.** `general/PROOFS.md:96-127`. (a), (b), (c) hold; the stamp comparison at time t gives v rich continuously from before t0, hence in S; (F) holds for the balls present at T under any removal. r = 1 and r = k give n <= k, trivially safe. Values 2, 3, 5, 7, 10, 13, 17, 21, 26 for k = 1..9: equal to 2k at k = 5, larger from k = 6. Consistent with the any-removal values 2, 4, 7, 11. "Between about k²/4 and k²/2" follows with README:52.

**Q5: CONFIRMED apart from findings 1 to 3.** 12 = N(4) and N is non-decreasing in k (a load reaches 5 only through 4). 21, 41, 81; 28, 60, 124; 41 against 38; "150-line checker" (154 lines); the fit is nowhere called proved (README:14, 57, 180). Top-level row accurate ("about" covers the +1 and k >= 5).

## Findings

1. **MODERATE, confidence high. README:54** says the organiser reran the checker with "reject on 20 and with a line removed". `general/organiser_check.txt` has five runs: accept 5/21, reject 5/20, reject target 6, accept 4/12, reject 4/11. No run with a line removed. Replacement: "the organiser reran the checker (accept on 21 bins; reject on 20 bins and with target 6)".

2. **MODERATE, confidence high. README:109-111** "every deleted line and every swapped move rejected" reads as covering the 21-bin table. `general/out_damage.txt:7-11` shows 150 lines sampled out of 3,676 for k = 5, and `out_damage2.txt:1` a sample of 400 offers; only the 12-bin table was damaged exhaustively. Also `out_damage.txt:12` ("k=4 table: accepted bin+1 ...") is printed by no script in the folder, and no command line is recorded. Replacement: "tested on damaged tables (for the 12-bin table every line, for the 21-bin table a sample of 150 lines and 400 offers: every deletion and swapped move rejected; altered offers accepted only when they rename an untouched bin)". I reproduced the k = 4 figures, including the unsourced line.

3. **LOW, confidence high. README:87-89** is stale: "Nobody has shown that a clean (k−1)-star can always be forced within that many bins". Theorem U Round 3 forces one within 5·2^(k-3) + 1 bins, which is within k + 2^(k-1) at k = 5. Replacement: "For k = 5 the follow-up forces a clean 4-star within 21 bins; for k ≥ 6 nobody has shown that a clean (k−1)-star can be forced within k + 2^(k−1) bins, nor that the adversary cannot do better."

4. **LOW, confidence high. `general/PROOFS.md:94, 131`** cite `REPORT.md`, which is not in `general/`. Replacement: cite `out_check_small.txt`, `out_check_k6_n41.txt` and `out_rh_test.txt`.

5. **LOW, confidence high. `general/out_check_k6_n41.txt:2`** and **`out_check_small.txt:5-10`** hold REJECT lines with no command line, so a reader cannot tell which wrong input produced each. Repair: prefix each with its command, as `organiser_check.txt` does.

Not checked: the 41-bin table (not in the folder), the 510 MB figure, `gen.py`, `rh_test.py`.
