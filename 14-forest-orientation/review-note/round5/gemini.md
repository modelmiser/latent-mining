<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gemini-3.8-flash-high

Q1: CONFIRMED — In attach5.txt:19-58 (Attachment A), out-neighbours after the flip are exact (a); out-trees of \(c_i, z, q\) cannot contain path vertices in an acyclic forest (b); Step 1 induction establishes \(g_{i-1} \ge k - i + 1\) and monotone non-decrease of \(h\) (c); Step 3 subcases for \(w_i, w_0, w_k\), boundary values \(k \in \{0, 1, 2\}\), and \(\deg^+(w_k) = 1\) correctly yield the claimed gains (d); and Step 2 monotonicity covers all off-path edges without omission (e). — none
Q2: CONFIRMED — In attach5.txt:60-77 (Attachment A), sum of in-degrees is \(n - 1\) so an in-degree 0 vertex \(r\) exists; deleting \(r\) leaves saturation and directed reachability unchanged in \(T - r\); out-degrees 1 and 2 exhaust non-trivial trees; base case \(n = 1\) holds (\(0 \le 0\)); and all three subcases for \(H(T)\) are exhaustive and algebraically exact. — none
Q3: CONFIRMED — In attach5.txt:79-85 (Attachment A) and 14-forest-orientation/NOTE.md:38-48,258-260, telescoping Lemma A over \(n - 1\) merges yields total cost \(\le \Phi(T) \le (n - 1)/2 - H(T)\); paying cost \(> 0\) forces some merge with \(\min(h(p), h(q)) \ge 1\), whence \(H(T) \ge 1\) by Step 1 monotonicity and cost \(\le (n - 3)/2\); this holds for any tie-breaking; bounding optimal play bounds \(T(n)\); and \(L \le 1/2\) follows from Fekete's lemma (NOTE.md:328). — none
Q4: CONFIRMED — In 14-forest-orientation/NOTE.md:15-18,333-345 and upper/greedy_recurrence.py:32-37, \((n - 3)/2\) strictly exceeds Theorem 1's lower bound \(n/3 - O(\log n)\), is consistent with exact values \(T(n) = \max(0, \lfloor n/4 \rfloor - 1)\) for \(n \le 15\) and known certificates at \(n \in \{30, 32, 64\}\), and does not contradict \(f(n)\), which is an independent, looser upper bound charging merges by tree size alone. — none
Q5: CONFIRMED — In attach5.txt:83-85 (Attachment A) and 14-forest-orientation/NOTE.md:18,264-273, the algebraic crossover \(9n/16 - \log_2 n > (n - 3)/2 \iff n/16 > \log_2 n - 3/2\) has real root \(n \approx 75.9\) ("about \(n = 80\)"), but Theorem 2 only applies for \(n \ge 19\) and is superseded by \(f(n)\) or exact \(T(n)\) for smaller \(n\). — "The new bound improves on Theorem 2 for \(n \ge 76\); for \(n < 76\), Theorem 2 (for \(n \ge 19\)) or exact values / \(f(n)\) (for small \(n\)) provide tighter bounds."
Q6: CONFIRMED — In attach5.txt:88-167 (Attachments B and C), `allcheck.py` and `lemma2.py` exhaustively generate canonical forms of all oriented trees with out-degree \(\le 2\), test all valid endpoint pairs with \(h(p) \le h(q)\), all shortest directed paths, and the exact potential \(\Phi\) (cap 2); the only gap is their finite scope (\(n_A, n_B \le 7, n_A + n_B \le 12\) and \(n \le 10\)) versus general \(n\). — none

ROUNDS MODE:
F: LOW — attach5.txt:3 — Title states "pays at most (n - 3)/2", which is negative for \(n \in \{1, 2\}\) where cost is 0 — confidence: high — State the bound as \(\max(0, \lfloor(n - 3)/2\rfloor)\) or restrict the headline inequality to \(n \ge 3\).
F: LOW — attach5.txt:84 — "The best statement is the minimum of the two" is ambiguous after mentioning Theorem 2, Theorem 3, and the table of \(f(n)\), and Theorem 2 does not hold for \(n < 19\) — confidence: high — Specify the piecewise minimum across the relevant domains: exact values / \(f(n)\) for small \(n\), Theorem 2 for \(19 \le n \le 75\), and Theorem 3 for \(n \ge 76\).

### Important Points Not Asked About

1. **Dependence on Shorter-Path Rule vs. Smaller-Tree Rule:**
   Theorem 3 strictly requires the shorter-path rule (\(h(p) \le h(q)\)). If an algorithm instead breaks ties or selects the endpoint based on smaller tree size \(a \le b\) when \(h(u) > h(v)\), then \(b < k\) and Lemma A Step 3 fails. NOTE.md introduces the smaller-tree rule before mentioning [7]'s shorter-path rule; adopting Theorem 3 requires standardizing on the shorter-path rule.

2. **Superseding the Recurrence Analysis:**
   Theorem 3 establishes \(L \le 1/2\) directly via a global potential argument. This completely bypasses the recurrence \(f(n)\) from NOTE.md Section 5 (which proved \(9/16\) was the barrier for size-only charging), narrowing the open asymptotic rate of \(L\) from \([1/3, 9/16]\) to \([1/3, 1/2]\).

3. **Suboptimality at Small \(n\):**
   For \(n \le 7\), \(\lfloor(n - 3)/2\rfloor\) permits values up to 2 (at \(n = 7\)), whereas NOTE.md Section 5 proves \(T(n) = 0\) for all \(n \le 7\). Theorem 3 is purely an asymptotic upper bound improvement.
