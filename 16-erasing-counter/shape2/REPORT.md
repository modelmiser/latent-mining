<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the third follow-up instance (Theorems NC and T), verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

# Report: experiment 16, attempt B

The W_depth bound is still not proved for all trees. I proved it for a class strictly larger than attempt A's, one that allows data shared between different S_i. P1 is false, as your note says; I had found the same relay mechanism independently.

The harness refused the write of `REPORT.md`, so this text is the report. All other files are in `(staging)/`; proofs are in `PROOFS.md` (sections §).

Notation: all-zero path z_1..z_k, S_i the subtree at the 1-child of z_i, mode i = states whose only 1 on the path is z_i, home = path all zero, data = indices off the path.

## Established

1. **P1 is false — PROVED (§1) and COMPUTED.** For any tree K with a c-cycle and any q ≥ 1, an explicit tree T(q,K) has a cycle of length (q+2)c while L = 2q + 4L(K), so cycle/L is unbounded. Depth-5 examples give 60 > 56 and 52 > 50 (`cex.txt`, `sim.py` written from the definition, `out_cex.txt`). Nothing exceeds W_depth.

2. **Theorem MD — PROVED (§2), not new.** It is the same statement as attempt A's Theorem Z1, found independently: with pairwise disjoint data, a cycle through home is an odometer of modes, each with one overflow state, and |C| ≤ Πc + Σ suffix products. Corollaries K, K4 and K\* (§3) add the depth ≤ 4 base case and Theorem RD.

3. **Theorem NC — PROVED (§4), new.** For any tree, the same bound holds for every cycle on which no mode changes a data bit that another mode reads; indices may be shared freely. Given the repository's D(ρ) = W_ρ for ρ ≤ 4, a cycle longer than W_5 in a depth-5 tree must pass through home and have two modes communicating through a data bit.

4. **Theorem T and Lemma T — PROVED (§5), new.** This is the part that covers data shared between different S_i.
   - Split the path at some i ≥ 1. Prefix modes 1..i have private data; the tail Θ (everything below) may share data internally in any way.
   - Prefix modes may read tail path indices and jump into the tail, provided all entries into the tail on the cycle go to one tail mode (condition EC).
   - The tail then acts as one more odometer digit: a cycle of a modified tree Θ\* with one overflow. The total is ≤ W_r whenever the pieces obey their own W bounds.
   - This is unconditional at depth 5, given the repository's depth ≤ 4 computation. The recursive class K\*\* strictly contains K\* and attempt A's class, and it strictly extends Theorem RD, since S_1 may now jump into T0.

5. **Candidate replacement for P1 — COMPUTED only, not proved.** Lhat is L with the values Lhat(S_i) sorted decreasingly before multiplying; L ≤ Lhat ≤ W_depth. No violation turned up in 150,000 random chain machines with shared indices or in up to 4,000,000 hill-climbing evaluations (depth ≤ 7, ≤ 12 indices). The same searches with L as the measure do find P1 violations (63 and 53 trees). The generators are narrow, so this is weak evidence.

## Failed or not done

- **D(r) = W_r for all trees.** Two cases remain uncovered:
  - a prefix mode and another mode exchanging information through a data bit;
  - a tail entered from outside at two different modes (for example λ0 writes one tail index and S_1 jumps to another).

  At depth 5 a cycle longer than W_5 would need one of these for every split i ≥ 1.
- **Per-node bounds charged by depth are false.** "#(root = 1 states) ≤ W_(r-1)" fails at depth 4: the tree in `root1.txt` has a 258-cycle with 126 > 42 such states (COMPUTED, `out_root1.txt`). Only the total is bounded, so an induction through the root needs a joint statement, and I did not find one.
- **No depth-charging measure on the excursion tree F was found.** F's depth is a sum over stages; the odometer structure in Theorems MD, NC and T replaces it only where some coordinate evolves on its own.
- **`lhat_search.py` is abandoned.** Its control could not find known P1 violations, so its nulls mean nothing.
- **Cycles avoiding home** are only bounded by 2^k here; attempt A's Lemma A gives ≤ k.

## What a reader must still trust

- [U] Theorems E and RD as given, and the repository's D(ρ) = W_ρ for ρ ≤ 4 wherever "unconditional at depth 5" or K4, K\*, K\*\* are used.
- The proofs in §2, §4 and §5 are unreviewed.
- The mechanical checks test the structural steps and count bounds on random small trees (≤ 12 indices), and Lemma T numerically. They do not test the W_r conclusion at depth ≥ 5.
- Every check was watched failing on a wrong input:

| Check | Passing run | Failing runs |
|---|---|---|
| `sim.py` on counterexamples | `out_cex.txt` | `out_cex_mut.txt` |
| `mdcheck.py` (Theorem MD) | 40,000 trees, 83,005 cycles through home | `out_md_mut1-3.txt` |
| `nccheck.py` (Theorem NC) | 20,592 qualifying cycles, 15,121 in trees with shared data | `out_nc_mut1-2.txt` |
| `tailcheck.py` (Theorem T) | 82,420 cycles, 3,862 with a prefix mode jumping into the tail, 11,294 in trees whose tail shares data | `out_tail_mut1-3.txt` |
| `lemma_t.py` (Lemma T) | 35,387 exhaustive + 165,091 random cases, max X/W_r = 1 | `out_lemma_t_mut1-2.txt` |

## Files

- `PROOFS.md`
- `sim.py`, `cex.txt`, `cex_mut.txt`, `out_cex*.txt`
- `mdcheck.py`, `out_md*.txt`
- `nccheck.py`, `out_nc*.txt`
- `tailcheck.py`, `out_tail*.txt`
- `lemma_t.py`, `out_lemma_t*.txt`
- `root1.py`, `root1.txt`, `out_root1.txt`
- `lhat_rand.py`, `lhat_climb.py`, `out_lhat_*.txt`, `out_climb_*.txt`
- `lhat_search.py` (abandoned)

Nothing was written under `(workspace)`, and no compute process is left running.
