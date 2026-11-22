<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `solver-b/proofs.txt:8-24` — The invariant $\text{cost}(L) + d(D_L, D_A) \le \text{cost}(A)$ holds round by round against arbitrary flips, but Lemma 1 as written proves optimality only for a fixed (oblivious) Adversary sequence; lifting it to adaptive adversaries requires the shadow orientation.
Q2: CONFIRMED — `reports/open-limit-referee.md:12-18`, `solver-b/proofs.txt:13-24` — none
Q3: CONFIRMED — `open-limit/proofs.txt:12-50` — none
Q4: CONFIRMED — `open-limit/proofs.txt:52-83`, `reports/open-limit-referee.md:12-18` — The invariant is inductive from every position in the normal-form game and on the shadow orientation, but fails if evaluated directly on raw positions after non-normal replies.
Q5: CONFIRMED — `open-limit/proofs.txt:72-80`, `README.md:51-53` — none
Q6: CONFIRMED — `solver-b/proofs.txt:26-55`, `solver-a/proofs.txt:37-88`, `solver-a/cert_W2_14_ge4.txt:1`, `solver-a/cert_W2_16_ge5.txt:1`, `solver-b/cert_n30_k7.txt:1` — none
Q7: CONFIRMED — `open-limit/proofs.txt:88-128`, `README.md:46` — none
Q8: CONFIRMED — `README.md:9-140`, `reports/referee.md:1-35`, `reports/open-limit-referee.md:1-42`, `prior-art-deep.md:1-45` — In `README.md:39`, replace "to the nearest vertex with a free slot" with "to the first unsaturated vertex along the path".

ROUNDS MODE. After the verdict lines and before anything else, list every problem you found, one per line:
F: MODERATE — `solver-b/proofs.txt:11-13` — Lemma 1 claims normal-form optimality against every adversary, but its proof fixes the adversary's move sequence; it fails to cover adaptive adversaries without a shadow simulation — confidence: high — Qualify Lemma 1 to fixed move sequences and explicitly invoke the shadow orientation transfer for adaptive strategies.
F: MODERATE — `open-limit/proofs.txt:81` — Remark (i) claims the invariant is inductive from every position, but non-normal replies destroy witness types in real positions — confidence: high — State that inductiveness holds from every position in the normal-form game or on the shadow orientation.
F: LOW — `README.md:39` — Normal form describes path reversal as ending at the "nearest" free slot, but Lemma 1 allows any path through saturated vertices ending at the first unsaturated vertex — confidence: high — Change "nearest vertex with a free slot" to "first unsaturated vertex along the path".
F: LOW — `open-limit/proofs.txt:111-113` — Claim that no one-witness strategy can prove more than $L \ge 1/3$ is proved only for the pair game reduction, not for all possible single-witness real-forest strategies — confidence: high — Restrict the impossibility claim to the pair game as defined (as `README.md:46` already does).
F: LOW — `open-limit/proofs.txt:134` — Corollary 7 claims an optimal algorithm must let $h$ grow without bound, but this is established only for normal-form algorithms — confidence: high — Add the qualification "under normal-form replies".
F: LOW — `README.md:43` — Upper bound $T(16) = 3$ rests on a single unverified solver run (`solver-a/run_n16.txt`) that neither referee verified due to memory limits — confidence: high — Add explicit caveat in text that $T(16) \le 3$ is unverified by an independent program.

### Additional Important Items Not Asked About

1. **Corollary 7 Scope (`open-limit/proofs.txt:130-136`)**: The deduction that an optimal Algorithm must let $h$ grow unbounded assumes normal-form play. Non-normal bounded-depth algorithms are not ruled out because Lemma 1's distance-tracking transformation does not preserve bounds on $h$.
2. **Failure of Potential on Real Trees (`open-limit/proofs.txt:138-148`)**: The natural Algorithm candidate minimizing cost plus the Theorem 3 potential fails on a 9-vertex tree counterexample. Consequently, the potential does not transfer to establish $L \le 1/3$, leaving the upper bound at $L \le 1$.
3. **Missing Large Certificate (`README.md:138`, `solver-b/`)**: The 10 MB certificate for $T(64) \ge 18$ is omitted from the folder, leaving $T(64) \ge 18$ dependent on running generator scripts, unlike $T(30) \ge 7$ and $T(32) \ge 8$ whose certificates are checked directly.
