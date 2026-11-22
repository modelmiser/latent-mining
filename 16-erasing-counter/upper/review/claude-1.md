<!-- Archived by the organiser on 2026-11-21. Report of the first Claude seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# Claude seat 1: experiment 16 upper bounds

No CRITICAL or MODERATE defect found. Every proof was traced by hand; six LOW wording findings below.

Own check (script outside the repository, written from `chain/3-question.txt` and the text of PROOFS.md, not from `check_F.py`): 18,500 random trees with heavy index sharing, 4 to 7 indices, depth up to 5, full functional graph of each.
- Theorem E: 21,520 cycles of length >= 2, at most one empty step on each.
- Construction of F: built from the text for the 1,934 cycles with >= 2 home states (4,282 home states). F was always a legal tree off Z with fewer indices, f_F(x_t) = x_(t+1) held, the empty-leaf correspondence held, no dummy was reached. F was deeper than T in 20 cases.
- Theorem RD: 1,500 disjoint trees, 0 violations of the bound, 0 of the "moreover" on 3,718 mixed cycles. With a shared index both fail (20 and 222), so the test can fail.
- Q(T): 264 random shapes, longest cycle = L(T), all-ones leaf empty and used once.
- Both Section 5 examples replayed.

## Verdicts

**Q1 CONFIRMED** (PROOFS.md:16-22). Weight change is |R| - c with |R| <= 1. In (F2) S_i may read some z_j with j > i, which only lowers Z. λ0's path is exactly Z, so its write lies on Z.

**Q2 CONFIRMED** (PROOFS.md:32-46). Case A: weight cannot rise, so it is constant; every step reads a 1, so an empty step would lower it. Case B: an empty λ0 makes the Z = 0 state a fixed point; Z <= 1 follows from (F2); home states are distinct states of C that agree off the data bits, so the x_t are distinct; (α)/(β)/(γ) is exhaustive because a leaf of S_i writes nothing, a z_j (j <= i from above, or j > i read in S_i) or a data index.

**Q3 CONFIRMED** (PROOFS.md:48-59). Termination: nesting of grafts is at most (data indices + 1)·k. A leaf of S_i writing data index u: u is on its S_i path, so u was kept in this copy or was in K, and K is exactly the F-path so far, including earlier modes; either way u is on the leaf's F-path. Induction is on |idx|, for trees of any depth (PROOFS.md:3-4, 29), so a deeper F is allowed. The dummy rule as worded is ambiguous at the start (finding 1).

**Q4 CONFIRMED** (PROOFS.md:61-74). Only (α) steps occur before the last step of an excursion and they write only on Z, so data bits in K stay 0 and T's data is x_t with K zeroed. At a dummy T is in mode j with the same K as at an earlier entry into mode j, hence in the same state, so it never reaches home. The x_t are distinct, so H is the exact period. Could not break it by program.

**Q5 CONFIRMED** (PROOFS.md:82-115). Both step formulas are right (a is on neither subtree's paths). I, II, III are right; Theorem E is applied to T0°, T1°, which are trees of the model. Case I needs M(T1°) >= 1, which holds. T0° has depth <= r - 1 and is disjoint above level j - 1. With j = r - 4, a cycle longer than W_r forces a sharing node at depth <= r - 5. This rests on the computed D(4) (finding 5).

**Q6 CONFIRMED** (PROOFS.md:119-135; solver-a/proofs.txt:5-31). T0°, T1° are read-once with unchanged shape, so unchanged L. In Q(T), T0° is the Q(T0) labelling itself, so its cycle survives and exits once per p steps; the states (0, x, y) and (1, x0, y) are distinct; the all-ones leaf of T is that of T1, used once. The equivalence is accurate.

**Q7 CONFIRMED** (PROOFS.md:139-150). Case A: Z is non-increasing and so constant, the one 1 read is the first 1 on Z, so the write is on Z. Nodes <= 2^r - 2^(r-k); the exponent identity checks; values 6, 64, 10240, 402,653,184.

**Q8 CONFIRMED** (PROOFS.md:154-164). (a) 01, 00, 10, back to 01; M0° = M1° = 1; L = 6. (b) the ten-state orbit is as printed, four states with bit 1 set, M(T1°) = L(T1) = 3, M(T0°) = 1, L = 15. Zero P2 violations in `out_brute_r2.txt` and `out_brute3_r3_m3.txt`.

**Q9 CONFIRMED with LOW findings.** 5,478,539 and 383,161: `out_brute3_r3.txt` last block. 40,000 = two runs of 20,000 in `out_checkF.txt` (9,980 + 10,029 cycles with >= 2 home states). `out_lazy2_r4.txt`: longest 1806, empty steps <= 1, 0 cycles longer than L(T'); L is monotone under extending T', so this covers L(T). The conjecture rows are marked "conjecture ... not proved" (README.md:64-65). Top-level row 16 is accurate.

## Findings

1. **LOW, confidence high. PROOFS.md:53.** "has already been entered since K last grew" does not cover the first mode j0 (K never grew), nor clearly the mode entered by the graft at which K grew. `check_F.py` and my script both count them. Replacement: "except that if K' = K and mode j has already been entered with this same K on the way from the root of F (j0 counts as entered with K = ∅), put an empty leaf".

2. **LOW, confidence high. PROOFS.md:68-69.** The dummy argument is one clause. Replacement: "at a dummy, T is in mode j with data x_t zeroed on K, which is exactly its state at the earlier entry into mode j with the same K; the orbit is then periodic through (α) states and never returns to a home state".

3. **LOW, confidence high. README.md:61.** "the longest cycle is exactly the value of the shape function L" is false for an individual tree (a read-once tree with all leaves empty has only fixed points). Replacement: "every cycle has length at most L of the tree, and every shape has a labelling that attains it; so the maximum at depth at most r is exactly W_r".

4. **LOW, confidence high. README.md:114.** "builds a smaller tree": F has fewer positions but can be deeper (20 cases in my runs). Replacement: "builds a tree on fewer positions (the positions off the path only; it may be deeper)".

5. **LOW, confidence high. PROOFS.md:114.** "Since D(ρ) = W_ρ for ρ ≤ 4" is stated without qualification; D(4) is computed by one search design. README.md:62 says "computed". Replacement: "Since D(ρ) = W_ρ for ρ ≤ 4 (computed; ρ = 4 by one design of search)".

6. **LOW, confidence medium. README.md:134-135 against PROOFS.md:168-172.** README says "four natural strengthenings"; PROOFS lists five, (v) being the product bound, which already fails at depth 2 (Section 5a) although the list is headed "FALSE at depth 3". Replacement in PROOFS: "(v) the product bound of Theorem RD (already false at depth 2)"; README: "lists five".

Also: README.md:62 "at depth r − 5 or higher" would read better as "at depth r − 5 or nearer the root". The top-level README diff also changes row 25, which is outside this brief.

Not checked: the C++ programs' source (only their outputs were read), and README.md:144 "the organiser ... traced the two small examples by hand".
