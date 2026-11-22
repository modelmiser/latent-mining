<!-- Archived by the organiser on 2026-11-21. Report of the Gemini seat. Verbatim except that absolute paths are shortened ((staging)/ = a working folder outside the repository, (scratch)/ = a temporary folder). It was written about the working tree BEFORE the organiser applied the findings, so line numbers and quoted sentences may no longer match the files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — `16-erasing-counter/shape/PROOFS.md:18-35`, `16-erasing-counter/shape/cex_small.txt:1-5`, `16-erasing-counter/shape/cex_mid.txt:1-5`, `16-erasing-counter/shape/cex_big.txt:1-5`, `16-erasing-counter/shape/out_cex.txt:1-10`, `16-erasing-counter/shape/organiser_check_out.txt:1-3` — none
Q2: CONFIRMED — `16-erasing-counter/shape/PROOFS.md:38-44` — none
Q3: CONFIRMED — `16-erasing-counter/shape/PROOFS.md:48-68` — none
Q4: CONFIRMED — `16-erasing-counter/shape/PROOFS.md:72-129`, `16-erasing-counter/upper/PROOFS.md:24-78` — none
Q5: CONFIRMED — `16-erasing-counter/shape/PROOFS.md:130-163`, `16-erasing-counter/upper/PROOFS.md:152-166` — none
Q6: CONFIRMED — `16-erasing-counter/README.md:65-67,139-151,154-158`, `16-erasing-counter/shape/out_testZ.txt:1`, `16-erasing-counter/shape/test_Z.py:180-196` — none

ROUNDS MODE. After the verdict lines and before anything else, list every problem you found, one per line:
F: LOW — `16-erasing-counter/README.md:67` — "tested on 6,003 random trees of the class" conflates 6,000 randomly generated trees with 3 deterministic witness trees (examples a, b, and \(T^*\)) tested in `shape/test_Z.py:180-196` and recorded in `shape/out_testZ.txt:1` — confidence: high — replace "6,003 random trees" with "6,000 random trees and 3 fixed witnesses"
F: LOW — `16-erasing-counter/shape/PROOFS.md:85` — Theorem Z1 "moreover" clause specifies destination mode as "\(m_{u+1}\) if \(u < l\)" but leaves the destination mode unspecified when \(u = l\) and mode \(m_l\) is a jumper (which loops back to some \(m_p\) with \(p \le l\)) — confidence: high — change "\(m_{u+1}\) if \(u < l\)" to "\(m_{u+1}\) if \(u < l\), or some \(m_p\) with \(p \le l\) if \(u = l\)"

### Important points not asked about

1. **Subtree data sharing remains entirely open for \(D(r) = W_r\)**: Theorem Z and class \(\mathcal{C}\) strictly require hypothesis \((H)\) (\(D_i = \text{idx}(S_i) \setminus Z\) pairwise disjoint). If two subtrees hanging off the spine share data indices not on the spine (\(D_i \cap D_j \neq \emptyset\)), the state space no longer factors into independent sub-cycles, and the \(W_r\) upper bound remains unproven for depth \(r \ge 5\).
2. **Lemma A universally disposes of Case A for all trees**: Lemma A does not require hypothesis \((H)\) and applies to all trees of any depth. Because any cycle avoiding home states has length \(N \le k \le r \le W_r\), no counterexample to the headline conjecture \(D(r) \le W_r\) can ever avoid home states; every potential violation must visit at least one all-zero spine state.
3. **Failure of the excursion-tree reduction under \(L\)**: In `upper/PROOFS.md:173-176`, the author noted that bounding cycle lengths using \(L\) via the excursion tree \(F\) failed because \(F\) duplicates subtrees across multiple jump leaves. The relay leaf \(S_2 = [1]\) in \(T^*\) demonstrates this failure concretely: \(S_2\) takes 1 level of depth on the spine and is traversed on every home visit, yet contributes only a factor of 1 to \(L(T)\).
