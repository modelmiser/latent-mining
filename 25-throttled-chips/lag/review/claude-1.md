<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: experiment 25, the lag is logarithmic

No CRITICAL or MODERATE defect found. Both proofs traced line by line; the construction was also rebuilt from the proof text in my own simulator (not the repository's programs). Paths are relative to `25-throttled-chips/`.

## Verdicts

**Q1 CONFIRMED.** Add and firing effects follow from T_x = (3k_x + k_{x-1} + k_{x+1})/5 (`lag/PROOFS.md:13-14`). I re-expanded (b): k_x − 2T_x + T_{x−1} + T_{x+1} = (k_{x−2}+…+k_{x+2})/5, and with (*) of `solver-a/proofs.txt:2` gives (**) (`solver-a/proofs.txt:26-28`). (c) is `solver-a/proofs.txt:29`, correct since F ≥ 0. (d') (`PROOFS.md:16-18`): a vertex of largest lag M > 3 has eta ≥ eta* ≥ M/3 > 1, integer, so ≥ 2; ready with lag ≥ 1, so the rule fires at lag M.

**Q2 CONFIRMED** (`PROOFS.md:30-39`). Case 0: 3k ≤ B_k since H_n ≥ H_k. 1a: the add raises any subset sum by at most 1. 1b: j has lag > 0 so j ∈ V and k+1 ≤ n; max_A l'' ≤ M because firing only lowers; algebra k/(k+1)·(B_{k+1}+1) = k(3 + H_n − H_{k+1} + 1/(k+1)) = B_k checked. B_1 = 2 + H_n; an add adds at most 3/5. Lags vanish outside V since T_x = 0 there; n ≤ 3m. Nothing uses how ties are broken, so the claim for any tie-breaking (including clairvoyant) holds; for U1 apply the theorem to the prefix.

**Q3 CONFIRMED.** Derivation: m < 5h − 26 − 10H_15h implies 3m < 15h and m/5 + 5.2 + 2H_3m < h. Recomputed with exact fractions: h = 27, 28, 59, 100, 1000 give 53, 57, 200, 398, 4873, matching `lag/corollary_out.txt`; U2 > 2h − 1 for every h from 28 to 399 and not at 27. Crossover with 5h − 176 by a float estimate of H: U2 is ahead up to about h = 122,300, so "near 10⁵, not computed exactly" is cautious enough. One wording defect (F4).

**Q4 CONFIRMED** (`PROOFS.md:69-81`). After the reservoir, interior k ≡ 1 on five points, so level 5 and eta* ≥ 1 for ever; adds at distance ≥ 4 touch lags only at distance ≥ 3, all interior. R1/R2: 2l_x − l_{x−1} − l_{x+1} ≥ 1/5 gives eta ≥ 6/5, so ≥ 2.

**Q5 CONFIRMED** (`PROOFS.md:100-106`). Levels followed by hand: pad (6,8,6) → (6,3,6); B-adds give (6,3,6,5,6,3,6); first C-add (7,8,7) → (7,3,7); second gives flanks 8, centre 6; the only 8s are unfired flanks. S(8, N) holds.

**Q6 CONFIRMED** (`PROOFS.md:108-149`). The three sub-cases of claim (2) are exhaustive (both ≤ W; one strictly larger and > W, which includes "both above W, unequal"; equal and > W). The firer's only freedom is among ready vertices of the largest level, and in every round the proof only needs "the fired vertex has the largest level"; firing a level-W buffer or outer instead of an alive flank helps the adder. Flushes lose no alive flank, so 16m' is conservative (8m' suffices). Counting: m ≥ 34q gives m' ≥ q; pads ≤ 0.75m per stage; length ≤ 23.1N + 84. Independent check: my simulator, written from the proof text, with tightly packed gadgets and pads, random reservoir order, and five tie rules (leftmost, rightmost, random, prefer non-flanks, prefer flanks), reached S(9,1) for N = 34 and S(10,1) for N = 1156 with every claim asserted; a greedy variant reached level 13 and exercised "both above W" plus flush 5 to 32 times per run without failure.

**Q7 CONFIRMED, with wording notes (F1, F2).** Excluding a clairvoyant tie-breaker is right: the list is built adaptively, which gives one fixed list only for a deterministic rule that ignores the future. 0.2/ln 34 = 0.0567 and 2 + H_3t ≤ ln t + 3.7 (so "+ 4.1" is safe).

**Q8 CONFIRMED, with notes (F3, F5, F6, F7).** `gen_out_39304.txt` shows the proof's construction at 2.2; greedy 2.6, 3.6, 4.0 in `gen_out_1156.txt`, `gen_out_39304.txt`, `gen_out_300000.txt`; replays agree; `neg_out.txt` has three assertion failures; `replay.cpp` is a separate implementation. ln 34 / 0.2 = 17.6, "about 18" is right. Lengths 859, 26482, 897727 match.

## Findings

**F1 LOW (confidence high).** `README.md:56` (top level) "the firing rule's shortfall grows like log t" and `25-throttled-chips/README.md:47` headline are worst-case statements and the lower half needs the tie-rule scope. Replacement (top level): "the firing rule's worst-case shortfall grows like log t (for tie-breaking that does not look ahead)". At line 47 write "any deterministic tie-breaking that depends only on the play so far"; a randomised rule is not covered by one fixed list.

**F2 LOW (high).** `lag/PROOFS.md:59-60`: "between 0.056 ln t and ln t" drops the additive constant, and the floor expression is undefined for t < 108. Replacement: "for t ≥ 108 … between 0.056 ln t and ln t + 3.7".

**F3 LOW (high).** `README.md:144-146` and `lag/PROOFS.md:155-156`, `lag/REPORT.md:21`: "2 to 4 in the greedy runs". In the outputs the per-level ratio of surviving gadgets ranges from 1.0 (18743 → 18734) to 11 (22 → 2); the averages are 2.5 (N = 300000) and 2.7 (N = 39304). Replacement: "about 2.5 on average in the greedy runs". Also "Rebuilding the survivors" is wrong: the fired flanks are rebuilt and survivors are spent to pay for it. Replacement: "Refilling fired groups to one level higher, paid for with survivors, costs a factor of 34 in the proof".

**F4 LOW (high).** `README.md:48` and `lag/PROOFS.md:48-49`: the examples 200, 398 come from the exact condition; the closed form 5h − 26 − 10H_15h gives 196 and 396 (4873 at h = 1000). Replacement: "…; the exact condition m/5 + 5.2 + 2H_3m < h gives G(59) ≥ 200, …".

**F5 LOW (medium).** `README.md:47` "every claim of its proof asserted, up to lag 2.2" and `README.md:188-189` "runs that reach 12 stages". The 12-stage runs are the greedy variant; the proof's own construction reaches 3 stages. In those runs "pads used" equals the burn count exactly (17; 578 + 18 = 596; 19652 + 614 + 19 = 20285), so no flush happened: claim (3) and the third sub-case of claim (2) were never exercised there (same in my replication; the third sub-case never occurred in any run of mine). Replacement: "asserted in runs of the proof's construction (3 stages; its flush step never arose) and of the greedy variant (12 stages)". Repair: print flush and sub-case counts from `gen.py`.

**F6 LOW (high).** "each list replayed by a separate program" (`README.md:47`): no replay output exists for `list_34_L/R/H`. Replacement: "each list from N = 1156 up".

**F7 LOW (high).** `README.md:49`: duplicated phrase "lists found by a search: lists found by a search over". Delete the first.
