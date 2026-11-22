<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: A(n) ≥ 3n − 6 (experiment 23 growth)

All paths are relative to `23-gossip-tug/`. I traced the proof by hand and wrote my own checker (`seat2_gossip23/mine.py` in the session scratchpad, written from `chain/3-question.txt`).

## Verdicts

**Q1 CONFIRMED.** The legality, active-agent, naming and termination rules in `growth/PROOFS.md:3-6` match `chain/3-question.txt:1-3`. Delayer names, Hastener picks the partner. R3 names an active expert, which the definition allows. "Two experts are dead" and "a non-expert is active" are both correct.

**Q2 CONFIRMED.** These hold whatever the Hastener answers (`PROOFS.md:16-27`):
- In phase 1, every earlier partner l_i knows c and c knows it, so each new partner is untouched and K_c = P_j. c is active while j ≤ n − 3.
- Closure holds because the P_j form a strict chain (P_0 = ∅, so K_p = {p} has level 0).
- After a call, both agents have the maximum level and the OR of the flags.

**Q3 CONFIRMED.** Only the lower-level agent can change level, and only the unflagged agent of a mixed pair can gain the flag, so Δ ≤ 2. For Δ = 2, the flag goes either to the top agent, which is case (i), or to the agent that becomes top, so the top partner was already flagged, which is case (ii). No other case exists (`PROOFS.md:32-37`). T = (n − 2) + (n − 1): c and l_m are both top. I asserted T = 2n − 3 mechanically for n ≤ 7.

**Q4 CONFIRMED.**
- R1: z is not of either double type, and z adds nothing to its partner.
- R2: p is flagged and not top, and {c, p} is legal.
- Monotonicity means R1 never applies after R2.
- Lemma B (`PROOFS.md:74-82`) is sound: the agent that first becomes top must call a partner that is already top and flagged, but every pair containing such an expert is dead from then on.
- In R3 the partner y is non-expert, so the call has Δ = 1 exactly.

I found no Hastener reply the proof misses. Mechanically, across all phase-1 partners, all Delayer tie-breaks inside R1 and R3, and all Hastener replies (n = 3…7): max Δ = 2; no R1 double; R1 never fires after R2; every R3 call has Δ = 1; Lemma B never fails; **every complete play contains exactly one double**.

**Q5 CONFIRMED.** 2n − 3 ≤ calls + 1. Checked by hand:
- n = 3: 1 + 2 = 3 calls.
- n = 4: 2 + 4 = 6 calls.

These match A(3) = 3 and A(4) = 6.

**Q6 CONFIRMED for n ≤ 7.** My checker gives a guaranteed number of calls of 3, 6, 9, 12, 15 for n = 3…7, under both the worst and the best Delayer tie-break, with full phase-1 branching. n = 8 hit my 2 GB cap. This agrees with `growth/dcheck.log:1-9,13-16`. Negative control: replacing R3 with "lowest active agent" gives 8 at n = 5 and 13 at n = 7, with up to 3 doubles; this matches dcheck variant 2.

**Q7 Partly CONFIRMED; findings below.** The bound, "exact for 3…6", and the plain-language proof are correct. "Allows exactly one call that completes two" holds on every play. The 4n − 13 claim is marked as a guess. "No upper bound better than n(n − 1)/2" is true. The source and wording problems are listed below.

## Findings

No CRITICAL. The proof is correct as written.

1. **MODERATE (conf. high).** The conjecture A(n) = 4n − 13 and the claim that "the excess over 3n − 6 comes from leaving two or more people out" (`README.md:113-118`) appear nowhere in `growth/`. PROOFS.md does not state them, and `star.log`, which partly supports the second claim, is cited by neither file.
   - **Repair:** add both to `PROOFS.md` with their basis: A(8..10) = 19, 23, 27 for the conjecture, and `star.log` for the "left out" claim. star.log shows that after a forced star with k leaves, k ≤ n − 3 reaches A(n) for n ≤ 8, while k = n − 2 gives exactly 3n − 6.
   - Also note in the README that A(10) itself rests on a pruned search.

2. **LOW (conf. high).** The checker evidence is described too strongly in `README.md:54` ("guarantees exactly 3n − 6 each time") and `README.md:123-126`. `dcheck.cpp:24,29-30` fixes one tie-break inside R1 and R3 (the lowest z, with p preferred), so it tests one instance of the rule. The log's second block (`dcheck.log:13-16`), which is presumably the FULLPH1 run, is not labelled.
   - **Repair:** "…against every reply of the Hastener, with one fixed choice where the rule leaves a choice, for n = 3, …, 11 (n = 5…8 also without the phase-1 shortcut)."
   - Also label the second block in the log. An independent checker covered all tie-breaks for n ≤ 7.

3. **LOW (conf. high).** `dcheck.log:17` says the deliberately wrong rules "must print BELOW", but variant 1 prints OK (`:18-20`). This is not a checker failure: c's only legal partner in Stage A is p, because every touched agent knows c and c knows every member of W. So "name c" forces the same call as "name p", and variant 1 is not actually a wrong rule.
   - **Repair:** note that in the log, or drop variant 1.

4. **LOW (conf. medium).** `README.md:113-115` says the Hastener rules tested "do no better than the trivial bound or are far from optimal". The greedy rules are 3 calls over A(n) at n = 8 and 9 (`hast.log:17-23` against `PROOFS.md:126-129`), and PROOFS itself says only "neither is optimal".
   - **Repair:** "…either reach the trivial n(n − 1)/2 or are not optimal (3 above A(n) at n = 8, 9); whether the latter grow linearly is unknown."

5. **LOW.** `PROOFS.md:10` says "For n = 3..6 this is the exact value", and `PROOFS.md:99` says "exceeds 3n − 6 from n = 7 on". Exactness for n = 5, 6 rests on computed values, and the excess is known only up to n = 10.
   - **Repair:** "(exact for n = 3..6 by the computed values; exceeded for n = 7..10)".

6. **LOW.** `README.md:78` ("Why at least 2n − 3 in general") is still correct, but it is now superseded.
   - **Repair:** retitle it "Why at least 2n − 3 (the first version's bound, superseded by 3n − 6 for n ≥ 4)".
   - The Review section (`:175-196`) is historical and accurate, and `@@GREVIEW@@` will cover this round.

7. **LOW.** The top-level `README.md` row 23 status says "the general lower bound proved by hand" without the caveat the experiment page carries.
   - **Repair:** append "(3n − 6, one author, checked by program to n = 11)".

8. **LOW (conf. medium).** `PROOFS.md:95` says "n = 4 reproduces solver-b's Theorem A". Solver B names agent 2 again where D* names p, so the opening is the same but the continuation differs.
   - **Repair:** "for n = 4 it gives 6, as solver-b's Theorem A".
