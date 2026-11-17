## KNOWN

Without feedback this is the single-deletion code problem: Varshamov–Tenengolts codes give A(n) >= 2^n/(n+1), and Levenshtein's bound gives A(n) ~ 2^n/n. The question as received is largely easy, which is its flaw. Part (2): n = 3 already works, since t = 1 with b = u a' gives F(n,1) = 2A(n-1), and 2A(2) = 4 > A(3) = 2 (my derivation, unverified by machine). Part (3): every outcome set has at least r(u)+r(a)-1 strings of length n-1, so the same sphere-packing argument gives F(n) ~ 2^n/n and r(n) = log2 n + o(1); one feedback bit changes only lower-order terms. Also F(n,n-1) = A(n) because {u} ∪ D(u)b = D(ub). What remains genuinely open to me is the exact optimum over t. Literature on deletion channels with feedback exists; details unsure.

## FINAL QUESTION

Setting. Fix integers n >= 2 and t with 0 <= t <= n. A sender transmits exactly n bits, one per step. An adversary who knows the message, the codebook and every bit sent may silently delete at most one of the n bits. The receiver sees only the surviving bits in order (length n or n-1): no deletion mark, no timing. The sender gets one bit of feedback at a moment fixed in advance: after sending bit t and before bit t+1 it learns whether one of the first t bits was deleted (not which one); it learns nothing about later bits. t and the codebook are public.

Formal form. For a binary string s let D(s) be the set of strings obtained by deleting exactly one symbol of s (D(empty) is empty). A strategy is a triple (u, a, b), u in {0,1}^t, a, b in {0,1}^(n-t): send u, then a if told "no deletion", b if told "deletion". Its outcome set is

  O(u,a,b) = { ua } ∪ { u a' : a' in D(a) } ∪ { u' b : u' in D(u) }.

F(n,t) is the largest number of strategies with pairwise disjoint outcome sets; F(n) = max over 0 <= t <= n of F(n,t). Let A(m) be the largest size of a set C of binary strings of length m with D(c), c in C, pairwise disjoint (optimal single-deletion-correcting code); A(m) = F(m,0).

HEADLINE (yes/no). Is F(n) = 2*A(n-1) for every n >= 3, i.e. is feedback after the very first bit always an optimal placement? Answer "yes" with a proof that F(n,t) <= 2*A(n-1) for all t, or "no" with the least n, a t, and a family of more than 2*A(n-1) strategies with pairwise disjoint outcome sets.

Required small data: the full table F(n,t) for 0 <= t <= n <= 6 (n = 7 if feasible).

UNVERIFIED beliefs (check them):
- F(n,t) >= A(n) for all t (take b = a; then O = {ua} ∪ D(ua)).
- F(n,1) = 2*A(n-1) for n >= 2: taking b = u a' for some a' in D(a) makes the length-(n-1) outcomes exactly uD(a); conversely uD(a) is always contained in O.
- F(n,n-1) = A(n), because {u} ∪ D(u)b = D(ub).
- For 1 <= t <= n-1 the fewest length-(n-1) outcomes of a strategy is r(u)+r(a)-1 (r = number of runs), attained by b = u_t a'; hence F(n) = (1+o(1)) 2^n/n and feedback gains only lower-order terms.
- A(1..10) = 1, 2, 2, 4, 6, 10, 16, 30, 52, 94.
- F(3,1) = 4 > A(3) = 2; F(4,t) = 4 for all t; F(5,2) <= 8.

Guess (not a claim): yes, t = 1 is always optimal, because for interior t every strategy with small outcome set seems forced to use one of a few "constant-run" strings, as happens for n = 5, t = 2.

## WHY IT IS PROBABLY UNCHARTED

Feedback for deletion channels is usually studied asymptotically or with feedback after every symbol; a single feedback bit at a fixed time, with exact finite optimisation over its placement, is an unusual cut. The comparison with 2A(n-1) arises only from the t = 1 trick above. Doubts: the answer may follow from a short shifting argument I did not find, making it an exercise; or it may fail at some small n, making it a table lookup. A(m) itself is unknown for larger m, so a "yes" must be a reduction, not a formula.

## HOW TO CHECK AN ANSWER

For fixed (n,t): enumerate all 2^t * 4^(n-t) triples, compute outcome sets, and solve exact maximum set packing (ILP or branch and bound; keep only inclusion-minimal sets among those sharing the string ua). Compute A(m) the same way with sets D(c). Compare max over t with 2*A(n-1) for n <= 6. First reproduce the calibrations F(2,1) = 2, F(3,1) = 4, F(n,n-1) = A(n). A "no" needs an explicit family verified disjoint by machine, plus exactness of A(n-1). A "yes" needs a proof valid for all n and t that does not use numerical values of A, and must agree with the table.
