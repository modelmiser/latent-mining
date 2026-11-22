<!-- Archived by the organiser on 2026-11-21. Report of the GPT seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — The identities and nonnegative lags are derived at `25-throttled-chips/solver-a/proofs.txt:24-29`; a global lag maximizer with M≥6 is ready, so every eligible maximum—including any tie—has lag M (`25-throttled-chips/solver-a/proofs.txt:30-33`) — none

Q2: CONFIRMED — The add contributes at most 3/5+1/5+1/5=1; firing M≥a+1 lowers Φ exactly 1, while a low-M round leaves every lag below a+1 (`25-throttled-chips/constant/proofs.txt:20-26`) — none

Q3: CONFIRMED — The τ monotonicity, Φ and Z inequalities, seven-point deletion, target locality, weighted double count, and strict conclusion are valid (`25-throttled-chips/constant/proofs.txt:28-44`); τ=t contradicts L>a+7, τ=0 forces Z empty and hence the same contradiction, and real a causes no integrality issue — none

Q4: REFUTED — The height calculation and losing-list inference are correct (`25-throttled-chips/constant/proofs.txt:46-59`), but its displayed chain says “for all h” although P3 only proves 5h−6 for h≥2 (`25-throttled-chips/solver-b/proofs.txt:35-44`); at h=1 it asserts G(1)≤−1 — “5h−176≤G(h) for all h, and 5h−176≤G(h)≤C(h)≤5h−6 for h≥2”

Q5: CONFIRMED — Lemma 2 charges every large remote lag through T_z only to additions outside y±2 and bounds each chip’s total weight by 1 (`25-throttled-chips/constant/proofs.txt:36-44`); finite t gives finite support, and fact (d) permits every tie rule (`25-throttled-chips/constant/proofs.txt:12-18`) — none

Q6: CONFIRMED — The explanatory steps match Lemmas 1–2 and Theorem A (`25-throttled-chips/README.md:103-128`); the unbounded-lag wording is explicitly probabilistic, with search reaching 6.6 and exact replay reaching 6.4 (`25-throttled-chips/constant/lagadv2_big.txt:1-6`, `25-throttled-chips/constant/replay_n16384.txt:1-5`) — none

Q7: REFUTED — The experiment README correctly restricts the bound to h≥2 and clearly discloses authorship/testing limitations (`25-throttled-chips/README.md:8-16,162-169`), but the top-level row omits that range (`README.md:56`) — “For h≥2, the shortest length lies between 5h−176 and 5h−6”

Q8: REFUTED — The assertions cover facts (a),(d), Lemmas 1–2, and both height moments (`25-throttled-chips/constant/test_lemmas.py:35-91`), and all corrected wrong modes fail (`25-throttled-chips/constant/test_out.txt:1-12`, `25-throttled-chips/constant/test_out2.txt:1-3`), but line 87 tests strict l_y<17+e/10 whereas the proof only establishes ≤ — replace `<` with `<=` at `test_lemmas.py:19,87`

F: MODERATE — `25-throttled-chips/constant/proofs.txt:56-58` — “for all h” makes the upper bound false at h=1 — confidence: high — restrict the upper-bound chain to h≥2

F: MODERATE — `README.md:56` — the unqualified top-level bound is false at h=1 — confidence: high — add “for h≥2”

F: LOW — `25-throttled-chips/constant/test_lemmas.py:19,87` — the test can reject a valid equality case not excluded by the proof — confidence: high — test `<=`

F: LOW — `25-throttled-chips/README.md:46` — “two … replayed” has only one recorded exact replay result — confidence: medium — say “one replayed” or include the second output

F: LOW — `25-throttled-chips/constant/proofs.txt:76` — it references absent `REPORT.md` — confidence: high — reference `lagadv2_big.txt` and `replay_n16384.txt`

F: LOW — `25-throttled-chips/constant/test_out.txt:5-9` — stale output says wrong variant 3 passed, although the corrected test fails — confidence: high — label it obsolete or retain only the corrected output

Unasked: no additional correctness defect found.
