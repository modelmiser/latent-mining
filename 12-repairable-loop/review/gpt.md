<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: REFUTED — C3 lacks an n ≥ 2 restriction; for n=1, u=01 is universal with e=0 and ρ=2, contradicting ρ=T=1 (`open-growth/proofs.txt:25-26`, `chain/3-question.txt:1-3`) — “C3. For n ≥ 2…”

Q2: CONFIRMED — E1–E3 and the deductions S(w)=σ+2, ρ(w)=T+2 correctly respect Facts A–C (`open-growth/proofs.txt:35-57`, `solver-b/proofs.txt:8-28`) — none

Q3: CONFIRMED — the excess vector and t=z=0 follow at `open-growth/proofs.txt:59-69`; independent enumeration gives only doubling 101 in Step 5 and the four stated loss contexts in Step 6 (`open-growth-referee/chk1.out:10-15`) — describe the conclusion precisely as “D plus two constrained insertions, possibly at different sites”

Q4: CONFIRMED — the two insertions affect only 0-runs of length ≤2, so the 0001000 occurrence survives; the three cases for the unique long run Z are exhaustive (`open-growth/proofs.txt:86-97`) — none

Q5: CONFIRMED — G2 uses the pass reply and does not assume w universal; G3 correctly subtracts all k through-window occurrences even when their words repeat, while F_s remains a set (`open-growth/proofs.txt:117-130`) — none

Q6: CONFIRMED — G4’s equations, symmetry, factor bound, repeat term, and assembly of (**) hold (`open-growth/proofs.txt:134-158`); the referee’s half-assignment repair is valid (`reports/open-growth-referee.md:30-33`) — add the domain bridge noted below

Q7: CONFIRMED — k=n−4 gives e(19n−78) ≥ 2^(n−3)−19n²+117n−64, hence the displayed weaker bound (`open-growth/proofs.txt:158`); for n=20, (**) is 262142 ≤ 302e+135930, yielding e≥418 (`open-growth/bound.out:15`) — none

Q8: REFUTED — the core values, lower bounds, length-76 witness and prior-art summary are supported (`verifier/route_small.txt:21-25`, `verifier/witness_checks.txt:11,67`, `prior-art.md:3-7,37-39`), but README’s coincidence count and “three computations” wording are false (`README.md:43,98-100`, `reports/solver-b.md:18`) — “two complete L=66 exclusions plus one universal-only search; retain all G4 correction terms”

F: MODERATE — `README.md:98-100` — coincidences are not fewer than 2^(n−3); G4 also has linear and repeat terms — confidence: high — state the full bound from `open-growth/proofs.txt:134-158`

F: MODERATE — `README.md:43`, `open-growth/proofs.txt:30` — solver B excluded only universal length-66 loops, so there were not three complete exhaustive exclusions — confidence: high — say “two complete searches; one universal-only cross-check”

F: LOW — `open-growth/proofs.txt:25-26` — C3 is false for n=1 — confidence: high — add n≥2

F: LOW — `open-growth/proofs.txt:141-143` — the 2^(d−1) factor count omits the residue-class argument — confidence: high — insert the referee’s sentence

F: LOW — `open-growth/proofs.txt:148-150` — `[t+2,t+k−1]` has k−2, not k−1, cells; the bound remains valid but loose — confidence: high — use k−2

F: LOW — `open-growth/proofs.txt:110,134,158` — application silently needs |w|>2n+2 and k=n−4≥2; small n instead follow from M(n)≥2^n — confidence: high — state these cases explicitly

F: LOW — `README.md:11` — “like 2^n/(19n)” suggests the wrong explicit coefficient; the proved simple bound is 2^n/(152n)−n — confidence: high — say Ω(2^n/n)

F: LOW — `README.md:72` — “216+20 loops” mixes n=4 strings with n=5 necklaces — confidence: high — label the counting units

Unasked: no additional concrete defect found.
