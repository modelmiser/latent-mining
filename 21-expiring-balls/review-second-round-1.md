<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 21 (`21-expiring-balls/README.md`). No CRITICAL defects. Two MODERATE, five LOW. Nothing written under ~/Claude.

**Recomputed and found correct**
- All figures against the logs: 974,272,116; 3,183,950; 3,334,554; 3,722,933 / 3,723,730; 40 s; 218 lines; 1,560 (600+300+60+300+300); 30 MISMATCH lines; 4,358,193.
- The sha256 pairs for 11 and 10 bins match in `organiser_sha256_and_lines.txt`.
- I reduced `out/{b2,vb}_{6_3,7_3,8_4,9_4}.txt` with `certtool.py minimize`: each pair is identical and equals the file in `cert/`.
- The accepted "added line" in `damage_11.log` is redundant: its prefix `01 02 03 45 46` is a certificate line.
- Pruning P1 is in both `back2.cpp:36-38` and `vb.cpp:44-45`; the NOPRUNE runs stop at 9 bins.
- `check.cpp`: zero parts divides by zero (line 214), any `NOPRUNE` value disables pruning (line 172), only part 0 walks the seeds (line 213).
- The `gen/` copies differ from the originals by one dump statement and a comment.
- Spot-checked `count_check.txt` rows by hand (n=6: k=1, 2, 5).

**MODERATE**

1. README.md:118-119 — "a second mode that walks all 974,272,116 winning positions without that proof also accepts."
   - Why: mode `full` drops only the reduction of PROOF.md section 3. It still relies on section 5's argument, the F3 seed walk and pruning P1 (`check.cpp:213` runs `walkSeeds` in both modes).
   - Replacement: "a second mode that walks all 974,272,116 winning positions, and needs only the short argument of the proof's section 5 instead of the reduction to three families, also accepts (in 9.5 minutes)."

2. README.md:126-128 — "its own literal mode … on 1,560 damaged certificates up to 8 bins".
   - Why (a): `difftest.py` has a `nobrute` option, and `difftest.log` prints "all modes agreed" either way. The log does not show that the literal mode ran in every batch.
   - Why (b): 212 of the 1,560 are of kind `intact` (73+52+4+45+38), so not all were damaged.
   - Replacement: "on 1,560 certificates (intact, damaged and random) up to 8 bins, by the follow-up's account; the log does not record which modes ran".
   - Alternative: rerun and log the command lines.

**LOW**

3. README.md:134-136 — "reran it on the 11-bin certificate and on a certificate it damaged itself."
   - Why: after "11-bin" this reads as a damaged 11-bin certificate; `organiser_rerun.log` shows a 9-bin one (`damaged_9_4.cert`).
   - Replacement: "…and on a 9-bin certificate it damaged itself (rejected)."

4. README.md:112 — "It then wrote a checker".
   - Why: the preceding sentence's subject is "Reviewers", so "It" has lost its antecedent.
   - Replacement: "The follow-up instance then wrote a checker".

5. README.md:146-147 — "the folder says how to regenerate it."
   - Why: `certificate/` has no written recipe. There is only the `DUMP=file` comment in `gen/*.cpp` and the `certtool.py` docstring, and nothing on the larger table `vb` needs at 11 bins.
   - Replacement: "it can be regenerated from `gen/` (run with `DUMP=file`, then `certtool.py minimize`); no command line is recorded."

6. README.md:245 (Files) — "small certificates for 6, 8 and 9 bins".
   - Why: `cert/` also holds `vb_7_3.cert` (the single line "-") and `bad_7_3_core_without_empty.cert`; `BRIEF.md` is unlisted.
   - Replacement: "small certificates for 6, 8 and 9 bins, two 7-bin files that must be rejected, the follow-up's brief, and logs."

7. README.md:205 — "The corrected wording has not been through a second round."
   - Why: with a second part now below it, the scope is unclear.
   - Replacement: "At that time the corrected wording had not been through a second round."

**Clean sections**
- Short answer: clean.
- Results row "g(11) = 3, so N(4) ≥ 12": clean. "Two-page" is generous for a 127-line, 8 kB proof, but harmless.
- "How the adversary uses expiry", the k = 4 paragraph: clean. Each number has a source that exists: `referee.md:16` for the 10-bin any-removal set, and 1 + 4·5/2 = 11.
- "Rough edges" bullet: clean apart from item 5.
- Review, both parts: consistent with the page as edited. The reviewer-attributed statements (who confirmed what, the 7,500 random lists) have no file in the folder, so I could not check them.
- Top-level README.md row 21 (line 52): clean and consistent with the page.
- Stale-text sweep: nothing contradictory. Line 100-102 ("the lower bounds … are computations") is still true.
- `reports/referee.md:39` says there is no certificate for 10 or 11 bins. That is a verbatim report predating the follow-up, not a defect of the page.
