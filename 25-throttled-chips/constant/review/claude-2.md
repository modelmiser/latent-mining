<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: constant bound for experiment 25

All algebra below was re-derived by hand. As a small check of my own (outside the repo), I replayed `constant/list_n512.txt` under the rule with integer fifths: max lag 3.8, min lag 0, max height − t/5 = 0.8, which matches the n=512 line of `lagadv2_out.txt`.

## Verdicts

**Q1: CONFIRMED.** (a) `proofs.txt:13`: T only grows, a firing needs lag ≥ 1 and lowers it by 1. (b) `:14`: I expanded 5(k_x − 2T_x + T_{x−1} + T_{x+1}) and got k_{x−2}+…+k_{x+2}. Substituting F = T − l gives the identity, and the window sum is t − e_x. (d) `:15-18`: at a global maximum, 2l_x − l_{x−1} − l_{x+1} ≥ 0, so η_x ≥ η*_x ≥ (k_{x−1}+k_x+k_{x+1})/5 ≥ T_x/3 ≥ M/3 ≥ 2. So x is ready with lag ≥ 1. The rule takes the largest lag among ready vertices, and that set contains x, so the fired lag is M. The tie rule doesn't matter. A maximum exists because only finitely many lags are nonzero.

**Q2: CONFIRMED.** `:24-26`. max(·−a, 0) is 1-Lipschitz and the add's increments sum to 1, so Φ_a rises by at most 1. In (i) the fired lag goes from M ≥ a+1 to M−1 ≥ a, so its term drops by exactly 1. In (ii) all lags after the add are ≤ M < a+1, and a firing only lowers a lag. "Before round t" = "after round t−1", and the bookkeeping is consistent.

**Q3: CONFIRMED.** `:30-43`. τ ≥ 1 always holds, because M_1 ≤ 3/5. The case τ = 0 is harmless anyway: Φ = 0 would contradict L − a > 7. If τ = t, the chain is trivial and in fact inconsistent with L > a+7. L − a ≤ Φ_a(after t) is the y-term alone. Φ_a(after τ) < |Z| because every term is < 1; Z empty would give Φ = 0 < L − a, a contradiction. For z ∈ Z' we have |z−y| ≥ 4, so z−1, z, z+1 all lie at distance ≥ 3 from y. l_z ≤ T_z because F ≥ 0. Over distinct z, a chip at x carries weights 3, 1, 1 into 5T_x and 5T_{x±1}, at most 5 in total. e_y(τ) ≤ e_y(t). Non-integer a is fine. Z' is nonempty because |Z'| > L − a − 7 > 0.

**Q4: CONFIRMED** for the bound. `:49-59`. a = 10 gives 17 + e/10, and −e/5 + 2e/10 = 0. After the add I get lag ≤ 17 + e_y(t−1)/10 + 0.6 ≤ 17.6 + e_y(t)/10, so η ≤ t/5 + 35.2. For m ≤ 5h − 177: t/5 + 35.2 ≤ h − 0.2. Heights are integers, so they are ≤ h − 1 after both steps. G(h) ≥ 5h − 176 holds for every h (it is vacuous for h ≤ 35). **Defect:** the Corollary says "for all h" (`:56`), but its upper bound 5h − 6 is P3 for h ≥ 2 (`solver-b/proofs.txt:36`). At h = 1, G(1) = 1 > −1.

**Q5: CONFIRMED (no break).** The accounting is local. A large lag at y forces many vertices with lag > a at time τ, and once the 7 nearest y are removed, their targets can only be paid for by chips at distance ≥ 3 from y. Spreading chips then bursting at one point, or two distant bursts, only add to e_y, which the bound already credits. The proof assumes no finite board, no bounded support and no tie rule.

**Q6: CONFIRMED with two defects.** Every quoted sentence in README:103-127 follows from the proof. "One chip counts towards at most three targets, with total weight 1" is right (3/5 + 1/5 + 1/5). "The two terms in e cancel exactly" is right. In the evidence paragraph (README:129-135), the lag reaching 6.6 and height within 0.8 of t/5 match `lagadv2_big.txt:3` and `replay_n16384.txt:5`, and the paragraph is presented as evidence. The defects are in Results row README:46 (see findings).

**Q7: UNSUPPORTED in parts.** The arithmetic is right: 5h − 6 − (5h − 176) = 170, and 5h − 176 > 2h − 1 first at h = 59 (119 vs 117). The status wording has problems; see findings.

**Q8: CONFIRMED with caveat.** The assertions match the proof's steps. `test_lemmas.py:59-63` is Lemma 1. Lines `:71-76` are the chain, with e taken from the τ snapshot `ks` and the [y−3, y+3] removal. Line `:87` is lag < 17 + e/10 in fifths. Lines `:41, :82` are 35.2 and 34. Wrong variants 1 and 2 are real negative tests: they remove exactly the hypotheses a ≥ θ−1 and the 7-point exclusion. Wrong 4 (t/5 + 1) only shows that the check is live. The original wrong 3 *passed* (`test_out.txt`, "ALL CHECKS PASSED (wrong=3)"). Only the corrected window [y−4, y+4] fails (`test_out2.txt:1-3`). In real data no check comes near its bound (excess ≤ 15.2 vs 34), and (d) is never exercised at θ = 1 (`test_out2.txt:5`).

## Findings

1. **MODERATE, high confidence.** README:45 says the bound was "read by the review panel (see Review)". The Review section describes the earlier round and says the corrected wording "has not been through a second round" (README:208). Replacement: "…tested by program on lazy variants of the rule; not machine-checked; under review (no panel verdict yet)". Update it once this round ends.
2. **MODERATE, high confidence.** README:46 says lists "on which the lag grows by about 0.3 each time their length doubles … two of them replayed by a separate exact program". Only one replay output is in the folder (`replay_n16384.txt`). The runs disagree under identical printed parameters: n=4096 gives 5.4 (`lagadv2_out.txt:9`) vs 4.8 (`lagadv2_big.txt:6`), and n=128 gives 3.4 vs 2.8. The steps per doubling range from 0.2 to 0.8. Replacement: "…lag rises with n, from 3.4 at n = 128 to 6.6 at n = 32768 (runs with the same parameters differ by up to 0.6); one list (n = 16384, lag 6.4) replayed by a separate exact program". Alternatively, commit the second replay output.
3. **LOW, high confidence.** `proofs.txt:56` says "for all h". Replacement: "for all h ≥ 2 (G(h) ≥ 5h − 176 for all h)". The top-level `README.md` row 25 ("between 5h − 176 and 5h − 6") also needs "for h ≥ 2".
4. **LOW, high confidence.** `proofs.txt:76` cites "REPORT.md", which isn't in the folder. Replace with "`lagadv2_out.txt` and `lagadv2_big.txt`".
5. **LOW, medium confidence.** The Short answer (README:9-14) states "no list beats the single-point list by more than 170 rounds, whatever h is" in bold with no status. Add: "(the lower bound is a one-author hand proof, one day old, not machine-checked)".
6. **LOW, medium confidence.** The "What to distrust" bullet (README:165) says lags of 6 or more "hardly occur" under the rule itself. The search lists reach 6.6 under that rule. The relevant regime for a = 10 (M ≥ 11) never occurs in any data. Replacement: "…not on the rule itself, where no list tried reaches the lag ≥ 11 at which the a = 10 argument's Lemma 1(i) applies".
7. **LOW, high confidence.** `test_out.txt` keeps a run in which the deliberately wrong variant 3 passed, and nothing explains it. Note in the Files list that `test_out2.txt` supersedes it, or remove it.
8. **LOW, low severity.** `proofs.txt:65` says the lag reaches "7.2 and 12". The outputs show 7.2/12.2 (`test_out2.txt`) and 7.4/12.4 (`test_out.txt` full).

**Bottom line:** Lemma 1, Lemma 2, Theorem A and G(h) ≥ 5h − 176 are correct as written. I found no gap. All defects are in status and evidence wording.
