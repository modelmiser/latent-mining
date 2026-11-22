<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: experiment 25, lag is logarithmic

All line numbers are in `25-throttled-chips/` unless the path says otherwise. I re-derived every step myself. My only computation was a scratch evaluation of the U2 condition.

## Verdicts

**Q1: CONFIRMED.**
- (b): I expanded 5(k_x − 2T_x + T_{x±1}) and got k_{x−2}+…+k_{x+2}. Substituting F = T − l into (*) gives (b) (solver-a/proofs.txt:26-28).
- (c): T_x ≤ 3(k_{x−1}+k_x+k_{x+1})/5 ≤ 3η*_x (solver-a:29).
- An add raises lags by 1/5, 3/5, 1/5. A firing lowers only F_z, so only l_z changes.
- (d'): if M > 3, then η_x ≥ η*_x ≥ M/3 > 1. η_x is an integer, so η_x ≥ 2 and x is ready with lag ≥ 1 (lag/PROOFS.md:16-18).

**Q2: CONFIRMED.**
- Case 0 holds because 3k ≤ B_k.
- Case 1a: the add raises any subset's sum by at most 1, and the firing takes off 1.
- Case 1b: j ∈ V because its lag is above 0, so k+1 ≤ n. Since j ∉ A, l''_j = M − 1 ≥ max_A l' − 1 = max_A l'' − 1. Then s(1 + 1/k) ≤ B_{k+1} + 1. The factor k/(k+1) times (B_{k+1} + 1) is k(3 + H_n − H_{k+1} + 1/(k+1)) = B_k. I checked this algebra.
- After an add the bound is 2.6 + H_n because the previous round's bound is S_1 ≤ 2 + H_n and an add raises a lag by at most 3/5.
- Lags vanish outside V: there T = 0, and F = 0 because a firing needs lag ≥ 1.
- The proof never uses the tie rule, so "any tie-breaking" holds (PROOFS.md:30-39).

**Q3: CONFIRMED.**
- Derivation: η ≤ η* + 2l with η* ≤ t/5, and l ≤ 2.6 + H_{3m}. For any integer m < m0 = 5h − 26 − 10H_{15h}, using H_{3m} ≤ H_{15h}, the condition holds, so G(h) ≥ m0.
- My own evaluation for h = 59: at m = 199 the condition gives 58.940 < 59, and at m = 200 it gives 59.150. So G ≥ 200.
- For h = 100: at m = 397 it gives 99.920, and at m = 398 it gives 100.125. So G ≥ 398.
- Against 2h − 1: at h = 27 they tie (53 = 53). The bound is strictly better from h = 28 through 1499 (exact fractions), and asymptotically it is about 5h.
- The crossover with 5h − 176 is first reached at h ≈ 135,000 in my float estimate (H_n ≈ ln n + γ + 1/2n). "Near 10⁵" and "not computed" are honest (README diff row 2; PROOFS.md:50-52).

**Q4: CONFIRMED.**
- During the reservoir η = k ≤ 1, so nothing fires. Every interior vertex ends at level 5 and has η* ≥ 1, and η* never decreases.
- R2: the vertex is the maximum and one neighbour is strictly lower, so 2l − l₋ − l₊ ≥ 1/5. That gives η ≥ 1.2, hence η ≥ 2, and the lag is ≥ 1. So the largest ready lag equals the global maximum.
- R1 is the special case with a unique maximum (PROOFS.md:69-81).

**Q5: CONFIRMED.** I traced every move:
- Pad: (6,8,6), fired to (6,3,6).
- B-adds: buffer at 8 is the unique maximum, fired, giving (6,3,6,5,6,3,6).
- First C-add: centre 8 is the unique maximum (everything else ≤ 7), fired to 3, and the flanks reach 7.
- Second C-add: the flanks reach 8 and the centre 6. Only raised flanks sit at 8, and R2 fires one of them.

The result meets S(8,N)(i)-(ii) (PROOFS.md:98-106). `gen.py:90-99` asserts each of these states.

**Q6: CONFIRMED.** I looked for a move by an adversarial history-dependent firer that the proof does not expect, and found none.
- Phase 1: only alive flanks are at W.
- Phase 2, claim (2):
  - Both new values ≤ W: any level-W vertex may be fired. Rebuilt flanks (≤ W−1) and centres (W−2) cannot be.
  - b+3 ≠ o+1, with the larger above W: R1 fires the larger, which falls to ≤ W−2. The other may stay above W, and the flush handles it.
  - b+3 = o+1 > W, which forces o = W and b = W−2: the buffer's flank neighbour is lower, so R2 applies.
- Only B-add rounds can lose an alive flank, at most 8m' of them. The proof's 16m' is conservative.
- Phases 3 and 4: the trigger centre W+1 is the unique maximum. In resolve, level W+1 is held only by resolved flanks next to centres at W−1.
- Counting: m ≥ 34q gives m' ≥ ⌊(17q−1)/16⌋ ≥ q.
- Each stage takes at most ⌈m/2⌉ + 18m' ≤ m + 2m' rounds. Summed over stages, using m_{i+1} < m_i/32, this is ≤ 1.1N. The total is 16N+76 + 2N+8 + 4N + 1.1N ≤ 24N+84.
- Lag (8+j)/5 follows.

**Q7: CONFIRMED (one wording defect, F1).**
- The construction is adaptive: the selection and the flushes depend on what was fired. So it covers exactly the non-anticipating deterministic tie rules.
- An anticipating firer is correctly excluded. Randomised tie rules are not covered either, and the page does not mention them (F4).
- The coefficients check out: 0.2/ln 34 = 0.0567, and 2 + H_{3t} ≤ ln t + 3 + ln 3 = ln t + 4.10.

**Q8: CONFIRMED with two exceptions (F2, F3).**
- The base construction reaches 1.8, 2.0, 2.2 (`gen_out_1156.txt`, `gen_out_39304.txt`). The greedy variant reaches 2.6, 3.6, 4.0 (`gen_out_1156.txt:29`, `gen_out_39304.txt`, `gen_out_300000.txt`).
- The replays exit 0 and reach the expected levels. The NEG replay exits 1 (`replay_out_1156.txt:29-32`).
- `neg_out.txt` shows assertions failing under `--no-reservoir` and `--overdraw`.
- There is no replay output for the N = 34 lists, although README says "each list replayed".
- "Factor of about 18": 1/0.0567 = 17.6, so this is correct.

## Findings

**F1. LOW. Confidence high. `lag/PROOFS.md:60`.** The text says "between 0.056 ln t and ln t". Without the additive constant, ln t is not a proved upper bound.
Replacement: "between about 0.056 ln t and ln t + 4.1".

**F2. MODERATE (a stated number is wrong). Confidence high. `README.md` "From √h to a constant" ("2 to 4 in the greedy runs") and `lag/PROOFS.md:155-156`.** The greedy per-level factors in m range from about 1.0 to 5.2:
- 18743→18734 and 286→277 (`gen_out_300000.txt:5-6,10-11`);
- 66→57 and then 57→11 (`gen_out_1156.txt:30-34`);
- 25→5.

The geometric mean is about 2.5 (300000→5 over 12 levels).
Replacement: "between 1 and 5 per level, about 2.5 on average, in the greedy runs".

**F3. LOW. Confidence high. `README.md` same paragraph, "Rebuilding the survivors one level higher".** The proof rebuilds the gadgets whose flanks were both fired. The surviving top flanks are spent as decoys, not rebuilt (PROOFS.md:110-115).
Replacement: "Gadgets whose flanks were both fired are rebuilt one level higher while the firer is kept busy shooting surviving top flanks; this costs a factor of 34 per level in the proof…".

**F4. LOW. Confidence medium. `README.md` Results row 1, and top-level `README.md` row 25.**
- The top-level row says "the firing rule's shortfall grows like log t" without the qualifier "for tie rules that depend only on the play so far".
- "Depends only on the play so far" should also say "deterministic".

Replacement for the top-level row: "…the firing rule's shortfall grows like log t (lower bound for deterministic tie rules that do not look ahead)".

**F5. LOW. Confidence high. README Results row 1, "each list replayed by a separate program".** No `replay_out` covers `list_34_{L,R,H}.txt`.
Fix: either replay them, or write "the lists for N = 1156, 39304 and 300000 were replayed".

**F6. LOW. Confidence high. `lag/PROOFS.md:121-122`.** "If exactly one of them is the single largest (> W)" reads as though it excludes the case where both values are above W but unequal. That case is handled by the same argument.
Replacement: "If b+3 ≠ o+1 and the larger exceeds W, it is the single largest; R1 fires it…".

**F7. LOW. Confidence high. `lag/REPORT.md:22`.** "Every later top-lag point is ready" is false in general. Readiness is proved only for a top vertex with a strictly lower neighbour (R2), or for a lag above 3 (d').
Replacement: "every later top-lag point with a lower neighbour is ready".

No CRITICAL defects. Theorems U and L, the stage lemma, the counting and Corollary U2 hold as I traced them.
