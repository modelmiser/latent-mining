<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — the game checks heights after both steps and permits one firing only after each add (`25-throttled-chips/chain/3-question.txt:4-12`); A1–A2 and equation (A) follow exactly (`25-throttled-chips/tight/PROOFS.md:20-39`) — none

Q2: CONFIRMED — A3’s three integer solutions, A4’s firing identities, the forced opening pass, and A6’s backward contradiction are valid (`25-throttled-chips/tight/PROOFS.md:41-78`); computed C(4…12) agrees (`25-throttled-chips/tight/out_C.txt:2-11`) — none

Q3: REFUTED — B1’s cases and neighbour switches are sound (`25-throttled-chips/tight/PROOFS.md:101-126`), but “6/5 cannot be lowered” is false because lags are fifth-integral (`25-throttled-chips/tight/PROOFS.md:97,127-128`), so any threshold strictly above 1, e.g. 11/10, is equivalent — “The condition is equivalently M > 1; M = 1 can fail.”

Q4: CONFIRMED — monotonic targets justify the fixed future Wset, B1 puts the fired j in it, fifth-integrality makes cases 0/1 exhaustive, and the top-k algebra gives max(1,H_n) (`25-throttled-chips/tight/PROOFS.md:132-154`) — none

Q5: CONFIRMED — B3’s weight count and j=2,4 arguments are valid (`25-throttled-chips/tight/PROOFS.md:156-173`); recomputation gives requested values 24/5,24/5,24/5,24/5,22/5 and decreasing tail g(10)=4.479 (`25-throttled-chips/tight/out_arith.txt:1-14`) — none

Q6: REFUTED — B5 correctly proves G(h)≥5h−28 using information available to Firer (`25-throttled-chips/tight/PROOFS.md:192-200`), but “2h−1 is better for h≤9” is false at h=9, where both equal 17 (`25-throttled-chips/README.md:15`) — “strictly better for h≤8 and equal at h=9”; B5 is non-vacuous from h=6 and improves the prior bound from h=10

Q7: REFUTED — the outputs support 89,243 plays, 810,997 top-k checks, and maxima 2.4/3.2 (`25-throttled-chips/tight/out_rule_main.txt:1-8`), plus the arithmetic and C-values, but the page falsely says the height bound was not exercised: it is asserted directly (`25-throttled-chips/tight/test_rule.py:45-47,68-70`) — “B1 and B2’s endpoint consequence were tested; height inequalities were asserted but far from binding; B3/B4 were not tested.”

Q8: REFUTED — the bounds, 18-round comparison, history, superseded upper row, and top-level row are accurate (`25-throttled-chips/README.md:8-24,50-57`; `README.md:56`), but the h=9 wording is wrong and older lower-bound rows remain unlabeled despite being superseded (`25-throttled-chips/README.md:53,55,57,325`) — apply the Q6 wording and mark those rows superseded

F: MODERATE — `25-throttled-chips/tight/PROOFS.md:127-128` — numerical sharpness claim is false; 11/10 works — confidence: high — state the sharp cutoff as M>1

F: LOW — `25-throttled-chips/README.md:15` — “better” includes an equality at h=9 — confidence: high — use “strictly better through h=8; equal at h=9”

F: LOW — `25-throttled-chips/README.md:51` — says the height bound was not exercised although tests assert it — confidence: high — distinguish tested inequality from untested B3/B4 derivation

F: LOW — `25-throttled-chips/tight/REPORT.md:14` — “B2 asserted” overstates coverage: only t=t0 with Wset recomputed was checked — confidence: high — say “B2 endpoint consequence”

F: LOW — `25-throttled-chips/tight/test_rule.py:2` — stale claim G(h)≥5h−32 — confidence: high — change to 5h−28

F: LOW — `25-throttled-chips/README.md:53,55,57` — older lower bounds remain unmarked as superseded — confidence: high — label them historical/superseded

F: LOW — `25-throttled-chips/tight/PROOFS.md:213-215` — “the constant c” suggests one fixed c, which is unproved — confidence: high — define deficit d(h)=5h−G(h)∈[10,28]

F: MODERATE — `25-throttled-chips/tight/run_all.sh:3-15` — no working directory satisfies both local script paths and `test_rule.py`’s repository-relative paths; it also requires an absent binary and omits recorded search runs — confidence: high — anchor paths, compile solve, and include all canonical commands

F: LOW — `25-throttled-chips/tight/solve.cpp:83-91` — search score claims post-fire states but skips final-round fires — confidence: high — describe it as safe states immediately after the final add

F: LOW — `25-throttled-chips/tight/PROOFS.md:3-4` — “nobody else has read it” conflicts with the later review status — confidence: medium — qualify “at time of writing”

Unasked: the reproduction-script and fixed-c wording defects above are the principal additional issues; no proof gap was found.
