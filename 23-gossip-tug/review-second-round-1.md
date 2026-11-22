<!-- Archived by the organiser on 2026-11-21. Report of a second-round reviewer (one Claude instance) on this experiment's page, taken from the session transcript. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

Second-round review, experiment 23 (`23-gossip-tug/README.md`, `growth/`). No CRITICAL defects. I recompiled `growth/dcheck.cpp` in the scratchpad and reran small n; every figure I reran matches `dcheck.log`.

**Defects**

1. **MODERATE — README.md:103.** "one of a chain of sets (what c knew after 0, 1, 2, … calls) … (the chain starts with the empty set …)". The sentence contradicts itself: after 0 calls c knows {c}, not the empty set. `growth/PROOFS.md:18-21` has P_j = {c, l_1..l_j} for j ≥ 1 and sets P_0 = ∅ separately.
   Replacement: "one of a chain of sets (the empty set, then what c knew after 1, 2, … calls), with or without p's secret (so p's own knowledge counts as the bottom member plus p's secret)".

2. **MODERATE — README.md:119.** "that variant names c where the rule names p, and c's only legal partner is then p, so it is the same call and not a wrong rule". It is not the same call. When p is named the Hastener may pick any legal partner of p; naming c forces {c, p}, which is only one of those replies. The log shows the play trees differ: 38 / 971 / 46972 positions for variant 1 against 56 / 1529 / 75777 for variant 0.
   Replacement: "…c's only legal partner is then p, so this variant forces the call {c, p}, one of the replies the Hastener has when p is named; it narrows the Hastener's choice and cannot fall below, so it is not a wrong rule".

3. **MODERATE — top-level README.md:54 (row 23).** "the general lower bound 3n − 6 proved by hand … and checked by program up to 11 people". This is the over-strong wording the page's own Review (README.md:198) says was corrected: the program tests one fixed version of the rule, not the bound.
   Replacement: "…proved by hand by one follow-up instance; its Delayer rule played by program against every reply up to 11 people".

4. **LOW — README.md:113.** "are not optimal (3 calls above A(n) at n = 8 and 9)". That holds for one greedy rule only (`hast.log` pol 2: 22 and 26 against 19 and 23). The other (pol 0) gives 23 at n = 8, which is 4 above, and has no n = 9 run.
   Replacement: "(3 or 4 calls above A(n) at n = 8; 3 above at n = 9 for the one rule run there)".

5. **LOW — README.md:54.** "(where the rule leaves a choice it takes the lowest-numbered agent)". True for R1 only. In R3, `dcheck.cpp:29-30` names p first if it is an active expert, and only then the lowest-numbered active expert.
   Replacement: "(lowest-numbered candidate in the first stage; p, else the lowest-numbered active expert, once someone knows everything)".

6. **LOW — README.md:119 and :54.** "The log also contains an unlabelled crash (out of memory at n = 12)". The log shows `std::bad_alloc` after the N=11 line with a literal `$n`; 12 is an inference, so say "presumably at n = 12". Separately, the bullet does not say that the block the "all orders for n = 5, …, 8" claim rests on (`dcheck.log:14-17`) is itself unlabelled. Nothing in the log says those are FULLPH1 runs. My reruns with `FULLPH1=1` give 565 / 7606 / 132877 positions for n = 5, 6, 7, matching the log, so the claim is true.
   Suggested addition: "the four lines after the crash are the all-orders runs (FULLPH1), also unlabelled".

7. **LOW — README.md:54.** "by its own account the organiser read the proof" is garbled; "its" has no clear referent. Replacement: "read by the organiser".

8. **LOW — README.md:119.** "a different statement with no proof". `PROOFS.md:105-106` gives a one-line parenthetical justification. Replacement: "with only a one-line justification".

**Clean**

- **Short answer:** clean (3n − 6 for n ≥ 3; values; "proved by hand only up to four").
- **Results rows 54–55:** the numbers are right (15, 18, 21, 24 against 16, 19, 23, 27; n = 3..11 all OK in the log; "superseded for n ≥ 4" is right, since the two bounds are equal at n = 3). Only items 5 and 7 apply.
- **"Why at least 2n − 3":** clean. `star.log` with nobody left out gives 5, 7, 9, 11, 13 = 2n − 3, which is consistent.
- **"Why at least 3n − 6":** the task count 2n − 3, the two double types, rules R1–R3, exactly one double, and (n − 2) + (2n − 4) all match `PROOFS.md`. Only item 1 applies.
- **`star.log` claim:** correct for n = 4..8. Two or more left out gives A(n) (e.g. n = 8, k ≤ 5 gives 19; n = 7, k ≤ 4 gives 16); exactly one left out gives 3n − 6 (6, 9, 12, 15, 18).
- **`hast.log` claims:** `PROOFS.md` S1 and S3 match the log exactly. The n(n − 1)/2 claim holds for policies 1 and 3, n = 5..8.
- **Log statements:** variant 1 prints OK at n = 5, 7, 9; variants 2 and 3 print BELOW at n = 5, 7, 9. Correct.
- **Lemma 2 claim:** it does differ from solver B's Lemma 3 (`solver-b/proofs.txt:13`: "≤ u" under a different hypothesis) and is not used in Theorem 1.
- **4n − 13:** arithmetic is right (19, 23, 27, 31) and is consistent with A(11) ≥ 29/30. The report is not in the folder, as the page says.
- **Review, both parts:** no internal contradiction; the listed edits are present on the page. Reviewer-side claims (their own checkers, n up to 7 and 9) cannot be checked from the folder.
- **Files:** clean.
- **Stale text:** none found. The literature bullet's "2n − 3" is covered by the line-119 note that the search predates 3n − 6.

Nothing was written under `(workspace)`; the scratch build is in `(scratch)/r23-gossip-q7x/`.
