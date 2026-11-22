<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 21 (`21-expiring-balls/README.md`), sections touched by the `general/` follow-up.

**Verdict: no CRITICAL or MODERATE defect found. Three LOW wording points.**

**What I checked and recomputed**
- 5·2^(k−3)+1 = 21, 41, 81 for k = 5, 6, 7. Correct.
- Recursion from N(4) = 12: 28, 60, 124. Correct.
- ⌊(k+1)²/4⌋+1 = 10, 13, 17, 21 for k = 5..8, against 2k = 10, 12, 14, 16, so "better than 2k from k = 6" is right.
- k + 2^(k−1) = 21 at k = 5 and 38 at k = 6, so "41 against 38" is right.
- The round-by-round bin counts in `general/PROOFS.md` (k = 5: 20, 21, 20, 21; k = 6: 37, 41, 40, 38) match what the page says.
- I reran `check_adv.py` on copies in the scratchpad: ACCEPT at 5/21 with 3,676 positions; REJECT at 5/20; ACCEPT at 4/12 (101 positions) and 3/7 (13). All match `organiser_check.txt` and `out_check_small.txt`.
- Other figures agree with the files: checker 154 lines; `gen.py` and `check_adv.py` import nothing from each other; `out_check_k6_n41.txt` shows 2,689,236 positions on 41 bins; damage counts (101 deletions, 44+57 swaps, 21 accepted, all renames; 150-line and 400-offer samples); `solve_small.py` output gives N(1..3) = 2, 4, 7; `REPORT.md` is absent; exactly two output files contain REJECT lines.

**Sections**
- Short answer (line 14): clean.
- Results rows 54–57 and the recursion row 50: clean. Row 50 is not stale; row 55 names the right starting point for 28, 60, 124.
- "How the adversary uses expiry": clean. The 7-bin steps, the parenthetical case and the k = 5 / k ≥ 6 sentence agree with Theorem U and its "Not proved" list.
- "What to distrust", first bullet: clean.
- Files: clean (tables for k = 2..5 are present; the k = 6 table is absent, as stated).
- Top-level `README.md` row 21 (line 52): clean and consistent with the page.
- Review, last paragraph: consistent with the files where it can be checked. The reviewers' own replays (3,676 positions, failing on 20 bins) have no log in the folder; this matches my rerun of the checker, but is otherwise the organiser's account.

**LOW 1 — README.md:105**
- Sentence: "for the 21-bin table random samples of 150 lines and 400 offers, with the same outcome".
- Why: `out_damage.txt` classifies the accepted altered offers as pure renames only for the 12-bin table. For the 21-bin 150-line sample it records 15 accepted without classifying them; only the 400-offer sample is classified, in `out_damage2.txt`.
- The claim is true: I reproduced the sample (seed 1) and all 15 are renames of an untouched bin, none different.
- Replacement: "for the 21-bin table a random sample of 150 lines (every deletion and swap rejected) and a sample of 400 altered offers (297 rejected, 103 accepted, all of them renaming an untouched bin)".

**LOW 2 — README.md:109**
- Sentence: "`general/PROOFS.md` refers to a `REPORT.md` that is not in the folder, and two of its output files contain REJECT lines without the command that produced them."
- Why: "its" reads as belonging to PROOFS.md. Also, `out_rh_test.txt` ends with two AssertionError lines that have no command either.
- Replacement: "…and two output files in `general/` (`out_check_small.txt`, `out_check_k6_n41.txt`) contain REJECT lines, and `out_rh_test.txt` two assertion failures, without the commands that produced them."

**LOW 3 — README.md:201**
- Sentence: "All five confirmed the 7-bin strategy and the 12-bin strategy after following them by hand, the certificate checker, and the general bounds."
- Why: "the general bounds" here means the old N(k) ≥ 2k and N(k) ≤ 2^k. The page now uses the same phrase for the 21 November bounds (lines 102, 249), so it can be misread.
- Replacement: "…and the bounds 2k ≤ N(k) ≤ 2^k."

Nothing was written under `(workspace)`. Scratch files are in `(scratch)/r2-exp21-q7x/`.
