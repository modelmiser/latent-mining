# Prior-art search: mutual exclusion on one counter mod b (read / blind add)

## CLOSEST PRIOR WORK

1. **Data Requirements for Implementation of N-Process Mutual Exclusion Using a Single Shared Variable** — Burns, Jackson, Lynch, Fischer, Peterson, JACM 1982. https://groups.csail.mit.edu/tds/papers/Lynch/jacm82.pdf
   Shows: with one variable and an atomic general test-and-set, 2 values suffice and are needed for deadlock-free exclusion for every N; lockout-freedom needs about sqrt(2N) values, bounded waiting N+1.
   Differs: the step fetches and stores atomically, so the 2-value answer does not transfer; here read and blind add are separate steps.
2. **A Complexity-Based Hierarchy for Multiprocessor Synchronization** — Ellen, Gelashvili, Shavit, Zhu, PODC 2016. https://arxiv.org/pdf/1607.06139
   Shows: one location with exactly {read(), add(x)} solves obstruction-free n-consensus, packing n bounded counters as base-3n digits (Thm 3.3).
   Differs: consensus, not exclusion; location size unbounded, and its size is named as future work.
3. **Bounds on Shared Memory for Mutual Exclusion** — Burns, Lynch, Inf. Comput. 1993. https://www.semanticscholar.org/paper/Bounds-on-Shared-Memory-for-Mutual-Exclusion-Burns-Lynch/252844376fd0319359cc6fc0f512f744a3c9362d
   Shows (abstract, secondary sources): n binary read/write variables are necessary and sufficient for deadlock-free exclusion; source of the one-bit algorithm.
   Differs: n overwriting registers, not one commutative-add location.
4. **Obstruction-free Snapshot, Obstruction-free Consensus, and Fetch-and-add Modulo k** — Bowman, Dartmouth TR2011-681. https://digitalcommons.dartmouth.edu/senior_theses/67
   Shows (abstract): algorithms for a fetch-and-add mod k object.
   Differs: implements the object; no exclusion value bounds.
5. **Mutual Exclusion of N Processors Using an O(N)-Valued Message Variable** — Cremers, Hibbard, 1977 (citation only: https://api.crossref.org/works/10.1016%2F0022-0000%2882%2990010-1; their 2-process lemmas appear as Lemmas 4.6–4.8 of item 1).
   Shows: value counts for fair exclusion with atomic test-and-set.
   Differs: same gap as item 1.
6. **Automatic Discovery of Mutual Exclusion Algorithms** — Bar-David, Taubenfeld, DISC 2003. https://unpaywall.org/10.1007%2F978-3-540-39989-6_10
   Shows (abstract): brute-force generation plus model checking of short algorithms.
   Differs: read/write bits; no modular counter or minimal-b question.

## VERDICTS

- **g(2) = 2 (toggle bit, 6 states):** NOT FOUND. Not a corollary of item 1 (atomic model), not contradicted by item 3 (add mod 2 is not an overwrite).
- **b = 2 and b = 3 impossible for n ≥ 3, so g(n) ≥ 4:** NOT FOUND. The proof style (indistinguishability, hidden process) is standard; the statements were not found.
- **g(3) = 4:** NOT FOUND.
- **g(n) ≤ 2^n − 1 (solver B says 2^(n−1); the reports differ):** LIKELY EASY COROLLARY of the one-bit algorithm (item 3) with flags packed as summands in one read/add location, as in item 2 Thm 3.3. Neither source states it.
- **Weighted variants (1,1,2 mod 4; g(4) ≤ 8; g(5) ≤ 14) and the state-count tables (least s = 6 at n = 2, b = 2; g_4(2) = 4; counts 19300, 116680):** NOT FOUND.
- **Sequence 2, 4, 8, 14** (least modulus of the weighted construction, n = 2..5; the only sequence with four terms): looked up in OEIS, 270 matches (A014206, A015128, A005598, …), none tied to this setting; four terms cannot identify it.

Queries (22): JACM 1982 title; Cremers Hibbard (two); counter fetch-and-add modulo space bound; Burns Lynch (two); Peterson values of one variable; historyless/counter objects; Ellen Gelashvili Shavit Zhu read add; Styer Peterson; one location read and add minimum size; one-bit flags as weighted sum; modulo counter three-process impossibility; Peterson Fischer; OEIS 2,4,8,14; Bowman fetch-and-add modulo k; Bar-David Taubenfeld; increment/decrement/read object; Fich Hendler Shavit; Alur Taubenfeld register size; read and flip/toggle bit; commuting operations Fich Herlihy Shavit.

## WHO TO ASK

A shared-memory distributed-computing theorist working on space complexity of synchronization (the Ellen / Zhu / Gelashvili line; Taubenfeld for mutual exclusion).

## NOT CHECKED

- Full texts: two downloaded and extracted, each read in substantial part, neither cover to cover — item 1 (abstract, model, Section 3.1, theorem list) and item 2 (introduction, Section 3, conclusions). The rest is snippets or abstracts.
- Not obtained: Burns–Lynch 1993, Cremers–Hibbard, Styer–Peterson 1989 (title not confirmed by search), Peterson's work on value counts, Taubenfeld's textbook, Bowman's full report.
- Pre-1990 technical reports and textbook exercises are poorly indexed; a b = 2 or b = 3 argument could sit there unseen.
