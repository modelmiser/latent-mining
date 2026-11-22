<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: constant bound, experiment 25

Paths are relative to `25-throttled-chips/`. Method: I re-derived every step by hand. I ran none of the repository's programs and no simulation of my own, so Q5 is answered by argument only.

## Verdicts

**Q1: CONFIRMED.** (`constant/proofs.txt:13-18`, `solver-a/proofs.txt:25-32`)
- (a) Only a firing at x lowers l_x; it needs l_x >= 1 and lowers it by 1.
- (b) Putting F = T - l into eta_x = k_x - 2F_x + F_(x-1) + F_(x+1) gives coefficients 1 - 6/5 + 2/5, -2/5 + 3/5 and 1/5: all five are 1/5. The sum is (t - e_x)/5.
- (d) A largest lag exists because only finitely many lags are non-zero. At such an x, eta_x >= eta*_x >= (k_(x-1)+k_x+k_(x+1))/5 >= T_x/3 >= l_x/3 >= 2. The rule takes the largest lag among ready vertices with lag >= 1, and x is one of them, so the fired lag is M under any tie rule. A ready vertex of smaller lag cannot be chosen.

**Q2: CONFIRMED.** (`constant/proofs.txt:21-26`) max(u - a, 0) is 1-Lipschitz and the rises sum to 1. The fired lag goes from M >= a+1 to M-1 >= a, a drop of exactly 1. In case (ii) a firing only lowers a lag. "After round" and "after add" are used consistently.

**Q3: CONFIRMED**, one wording slip (F6). (`constant/proofs.txt:28-43`)
- tau = t is impossible when L > a+7, and harmless anyway. tau = 0 gives Phi = 0 < L - a, a contradiction, so tau >= 1.
- |Z| is an integer above L - a, so |Z'| >= |Z| - 7 > 0.
- For |z - y| >= 4 the points z-1, z, z+1 lie outside [y-2, y+2]. l_z <= T_z because F_z >= 0.
- One chip gives 3 + 1 + 1 <= 5 to the sum of 5T_z over distinct z.
- a need not be an integer.

**Q4: CONFIRMED**, one range slip (F5). (`constant/proofs.txt:45-59`)
- After a round: (t-e)/5 + 2(17 + e/10) = t/5 + 34.
- After the add: lag <= 17 + e(t-1)/10 + 0.6 <= 17.6 + e(t)/10, and (b) holds with the current t and e(t), giving t/5 + 35.2.
- For t <= 5h-177: t/5 + 35.2 <= h - 0.2, so integer heights are at most h-1 at both steps. G(h) >= 5h-176 for every h (vacuous for small h).

**Q5: CONFIRMED (no break found; argument only).** Large lag at y with few far chips is exactly what Lemma 2 forbids: e = 0 forces l_y <= 12 (a = 5). The proof needs only a finite list. It assumes no bounded support and no tie rule.

**Q6: CONFIRMED for the proof paragraphs; the lag paragraph is partly UNSUPPORTED** (F2, F3). (`README.md:107-127`) Every quoted sentence matches the proof: seven points = [y-3, y+3]; weights 3/5 + 1/5 + 1/5 = 1; -e/5 + 2e/10 = 0. The lag paragraph is presented as evidence.

**Q7: mostly CONFIRMED; one status claim UNSUPPORTED** (F1).
- "170 rounds": (5h-6) - (5h-176), for h >= 2. Correct.
- "until h reaches 59": 5h-176 > 2h-1 iff h >= 59. Correct.
- One author, not machine-checked, lazy variants only: all stated at `README.md:162-169`.

**Q8: CONFIRMED with reservations** (F4). The assertions at `constant/test_lemmas.py:48-49, 59-63, 71-76` match (d), Lemma 1 and the Lemma 2 chain, with units in fifths correct. The Theorem A assertions (lines 41, 82, 87) are never near binding: largest excess 15.2 against 34, largest lag 12.4 against 17.

## Findings

**F1. MODERATE, confidence high.** `README.md:45` says "read by the review panel (see Review)". The Review section (`README.md:205-224`) covers only the first round and ends "The corrected wording has not been through a second round".
Replacement: drop the phrase until the panel's result is written into Review, then cite that paragraph.

**F2. LOW to MODERATE, confidence high.** `README.md:46` says "two of them replayed by a separate exact program". The folder holds one replay output, `constant/replay_n16384.txt`. `list_n512.txt` has no replay output, and the 6.6 run (n = 32768) has neither list nor replay.
Replacement: "one of them (n = 16384, lag 6.4) replayed by a separate exact program", or add the second output.

**F3. LOW, confidence high.** "About 0.3 per doubling" (`README.md:46`, `constant/proofs.txt:76`) understates the trend and hides the scatter.
- The data run from 2.4 at n = 16 to 6.6 at n = 32768, about 0.38 per doubling.
- Identically labelled runs differ: n = 4096 gives 5.4 (`lagadv2_out.txt`) and 4.8 (`lagadv2_big.txt:6,9`); n = 128 gives 3.4 and 2.8 (`lagadv2_big.txt:4,7`).
- The seed is not printed, so "`lagadv2.cpp` regenerates it" (`README.md:237`) is not reproducible as stated.
- `proofs.txt:76` cites a `REPORT.md` that is not in the folder.
- "Within 0.8 of t/5" is true, but 0.8 is the value at t = 1.

Replacement: "the lag grows by 0.3 to 0.4 per doubling of the number of points, with differences of up to 0.6 between runs", and record seeds.

**F4. LOW, confidence high.** Negative tests and stale output.
- `constant/test_out.txt:7-9` shows `--wrong 3` passing. `test_out2.txt:1` shows it failing after a correction, so `test_out.txt`, including its "full" run, came from an earlier script than the one in the folder. Say so, or regenerate it.
- `--wrong 1` fails at round 1 with a negative level (a = -1.2, theta = 1) and `--wrong 4` at round 2. Both show the assertion is live, not that it discriminates near the threshold.
- `proofs.txt:65` says "lag reaches 7.2 and 12"; `test_out2.txt:6-7` says 7.2 and 12.2.
- `test_lemmas.py:51` (`before`) is dead code.

**F5. LOW, confidence high.** `constant/proofs.txt:56` says "for all h", but at h = 1 the upper bound 5h - 6 = -1 is false. The top-level `README.md` row for experiment 25 also drops the range.
Replacement: "5h - 176 <= G(h) for all h; G(h) <= C(h) <= 5h - 6 for h >= 2".

**F6. LOW, confidence high.** `constant/proofs.txt:35-36`: "Phi_a < |Z|" fails as written when Z is empty (0 < 0).
Replacement: "Phi_a >= L - a > 0, so Z is not empty, and then Phi_a < |Z|".

**F7. LOW, confidence medium.** The bold Short answer (`README.md:9-14`) states the constant as fact; the caveat comes 150 lines later.
Replacement: add "(proved by hand by one instance on 21 November, not machine-checked)" after "5h − 176 ≤ G(h)". In the top-level row, "the lower one by two single authors in two steps" is obscure; use "the lower bound by one instance building on another's strategy".

No CRITICAL finding: I traced Lemma 1, Lemma 2, Theorem A and the Corollary line by line and found no gap.
