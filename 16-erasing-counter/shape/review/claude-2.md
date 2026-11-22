<!-- Archived by the organiser on 2026-11-21. Report of the second Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 2: experiment 16, shape counterexample and Theorem Z

Method: read `shape/PROOFS.md`, `upper/PROOFS.md`, `chain/3-question.txt`, the README diff, `shape/test_Z.py`, and the output files. I wrote my own simulator from the definition (scratchpad `sim.py`: legality, depth, L, full functional graph, orbit of 0), and traced every proof step by hand.

## Verdicts

**Q1: CONFIRMED.** My simulator: T* is legal, depth 5, L = 50, one cycle in the full graph (128 states), length 52, through 0. `cex_mid`: legal, depth 5, L 98, single cycle 114. `cex_big`: legal, depth 5, L 602, single cycle 762. I also checked L by hand: L(S_3)=2·2=4, L(S_1)=1·(4+1)=5, ℓ_2=1, so 5+5+20+20=50 (`shape/PROOFS.md:21-22`). I re-derived (L-spine) (line 12) by unrolling A_i = ℓ_i(A_(i+1)+1) with A_(k+1)=1. The hand trace (lines 24-32) is right: 4 rounds × (4·3+1) = 52. Depth 5 is outside the depth-≤4 computation (`upper/PROOFS.md:183-186`), so there is no contradiction. An upper-bound counterexample with 52 states cannot affect the lower bound D(5) ≥ W_5.

**Q2: CONFIRMED.** L = (c_1+1) + (c_1+1) + 2(c_1+1)c_3 = (c_1+1)(2c_3+2). N = c_3(3c_1+1). N−L = c_1c_3−2c_1−c_3−2, which I re-derived. The instances check out: (4,4)→52/50, (6,6)→114/98, (42,6)→762/602. At depth 4, S_3 has depth ≤ 1, so c_3 ≤ 2 and N−L ≤ −4 (line 42). The "why L undercounts" paragraph (lines 40-41) is a heuristic, not a claim, and it is consistent with the trace.

**Q3: CONFIRMED.** Lines 53-66. The chain of reasoning holds: weight is constant ([U] Case A), so exactly one 1 is read (F1), and it is the first spine 1. Z is constant (F2 plus periodicity), so the write goes to a spine index that was 0; j = i would give a fixed point. No data bit changes. In the max-element argument, the other elements of B are < p, so p is never the minimum and is never removed. Removing p preserves the rule because the new j ≠ p. After c−1 rounds, distinct states give distinct q ∉ F, so N ≤ k−c+1. L ≥ k+1 then gives N < L.

**Q4: CONFIRMED, no gap found.** Each sub-question checks out:
- **S_i° is legal.** Its paths are subsequences of S_i paths. A surviving write is a data index from the S_i path and is kept (line 77).
- **The step acts as g_i.** In mode i, every z_j (j≠i) reads 0, and z_j with j<i cannot occur in S_i. The walk equals the S_i° walk. A leaf writing z_j is empty in S_i° and erases the same bits, so x'_i = g_i(x_i) (lines 93-100).
- **Disjointness.** It is used exactly at "x'_u = x_u for u≠i" (line 98), and it makes the Ω_τ product count valid.
- **The C_i.** By (2), a full orbit of g_i is visited, so every point of C_i, including e_i, occurs at some mode-i state. Theorem E on S_i° gives a unique e_i.
- **Recurring modes.** By (4), the mode at stage t depends only on t, so the sequence is the same in every excursion. Recurrence inside an excursion (m_(l+1) = m_τ) is absorbed because (5) uses the first occurrence τ, and the claim's induction over intermediate stages u' < t is correct.
- **The count.** It is an upper bound, so overlap between the Ω_τ is harmless.

I checked the bound on T*: chain (2,1,3), c = (1,4,4), 2·16+16+4 = 52, which is tight.

**Q5: CONFIRMED.**
- Depth(S_i) ≤ r−i and k ≤ r. The sizes are |C_i| ≤ W_(r−i), with w strictly decreasing and ≥ 1.
- C_t ≤ w_1…w_(l−t+1), because the m_u are distinct.
- Re-indexing gives N ≤ Σ_(q≤l) w_1…w_q + w_1…w_l ≤ Σ_(q≤k)… + w_1…w_k.
- The unrolling W_r = Σ_(q=1..k) w_1…w_q + w_1…w_k·W_(r−k) is right (W_j = W_(j−1) + W_(j−1)²).
- The cycle without a home state has length ≤ k ≤ r ≤ W_r.
- The "more generally" clause needs only M(S_i°) ≤ W_depth. Its depth-≤4 instance rests on D(ρ)=W_ρ for ρ≤4 (`upper/PROOFS.md:114`), as stated (lines 156-158).
- Class membership, which I verified:
  - read-once trees: in 𝒞 by the induction at line 149;
  - (a): Z={1,2}, D_1=D_2=∅, S_1°=[];
  - (b): Z={1,2}, D_1={4}, D_2={3}, S_1°=(4 [4] []);
  - T*: D=({6,7},∅,{4,5}).

  (a), (b) and T* are all non-read-once, so the containment is strict.

**Q6: CONFIRMED, with the LOW defects listed below.**
- The struck row's values match my runs and `organiser_check_out.txt`.
- The P1 row entered the README in commit 70f0c0d (21 Nov 15:52), which is consistent with "a few hours".
- "Three states per return" checks out: home → mode 2 → mode 1, and x moves at the mode-1 step. ℓ_2 = 1 is right.
- The open-case sentence is essentially right. At depth 5, the "more generally" clause plus D(4)=1806 means only trees that violate (H) at the top level remain open.
- Lines 131-133, 187-189 and 113-114 are not stale.

## Findings

1. **LOW (confidence high)**, `README.md:67`: "the two small examples below". Only example (a) appears on the page (line 135). Example (b) exists only in `upper/PROOFS.md` §5. Replacement: "the depth-2 example below, a depth-3 example in `upper/PROOFS.md` §5, and the counterexample above".

2. **LOW (high)**, `README.md:67`: "tested on 6,003 random trees of the class". `test_Z.py:37-62,185-192` shows 6,000 random trees that satisfy (H) at the top level only (membership of S_i° in 𝒞 is never checked), plus the 3 fixed examples. The check is of Theorem Z1 and of the "more generally" clause, not of 𝒞 membership. Replacement: "tested on 6,000 random trees satisfying the condition at the top level, plus the three examples; this checks the bookkeeping of the proof and is not strong evidence".

3. **LOW (medium)**, `README.md:67` and `:148-151`: "the same holds recursively inside them" / "(recursively)". The recursion in 𝒞 is on the pruned subtrees S_i° (spine nodes bypassed, writes to the spine removed), not on S_i. The README's reading is a sufficient condition, because pruning only shrinks the data sets, but it describes a smaller class. The open-case sentence should cover all levels. Replacement: "...and the same holds recursively inside them once the positions of the path are ignored" and "...positions shared between two of those subtrees, at any level of this recursion, are the case still open (at depth 5, only sharing at the top level)".

4. **LOW (medium)**, `README.md:137`: "five natural strengthenings ... of 'length ≤ L(T)' that would allow an induction". This is now strengthenings of a false statement. Suggest appending: "(the statement they strengthen is itself false at depth 5; see below)".

5. **LOW (low)**, `README.md:156-157`: "Agreement up to depth 4 said nothing about depth 5, where a path is long enough for the relay to pay". `shape/PROOFS.md:42` shows only that this family needs depth 5. Replacement: "...said nothing about depth 5; the family found needs depth 5 for the relay to pay".

No CRITICAL or MODERATE defects found. The counterexample is correct, and Lemma A, Theorem Z1 and Theorem Z have no gap I could find.
