## 1. KNOWN

The question is well-posed. Without feedback this is the single-deletion code problem: Varshamov–Tenengolts codes give about 2^n/(n+1) words, optimal sizes known for small n (Levenshtein; Sloane's tables), redundancy about log2 n. With instant feedback, 2^(n-1) is reached (resend the deleted bit, pad otherwise), so redundancy is 1. With one-step-late feedback I expect constant redundancy too: only the last two bits are unprotected, so M(n)/2^n probably stays bounded below, and part (3) is likely a short argument (unsure of the constant). The calibration M(2) = 2 checks by hand; M(3) = 3 I did not verify. Practical flaw: a strategy has n + (n-1)(n-2)/2 free bits, so at n = 7 there are 2^22 strategies and exact maximum packing is not obviously a few-minutes computation. The interesting regime (between log n and constant redundancy) is skipped.

## 2. SUCCESSOR QUESTION

Fix integers n >= 1 and t with 0 <= t <= n. A sender transmits exactly n bits, one per step, to a receiver. An adversary who knows the message, the sender's whole strategy and every bit sent may silently delete at most one of the n bits (over the whole transmission). The receiver ends with the surviving bits in order (a string of length n or n-1), with no mark of where, or whether, a deletion happened, and no timing information.

The sender gets exactly one bit of feedback in the whole transmission, at a moment fixed in advance: after sending bit t and before sending bit t+1, it is told whether one of the first t bits was deleted. It is NOT told which one, and it never learns anything about bits t+1, ..., n. (t and the codebook are known to everyone, including the adversary and the receiver.)

Equivalently, a strategy for one message is a triple (u, a, b) with u in {0,1}^t and a, b in {0,1}^(n-t): the sender sends u, then a if told "no deletion so far", or b if told "a deletion happened". For a string s let D(s) be the set of strings obtained from s by deleting exactly one symbol (D of the empty string is empty). The outcome set of (u, a, b) is

  O(u,a,b) = { ua } ∪ { u a' : a' in D(a) } ∪ { u' b : u' in D(u) }.

Let F(n,t) be the largest number of strategies whose outcome sets are pairwise disjoint (the largest message set the receiver can always decode), and let F(n) = max over t of F(n,t).

For t = 0 and for t = n the feedback is useless and F(n,0) = F(n,n) is the maximum size of a binary single-deletion-correcting code of length n. Also F(n,t) <= 2^(n-1) always, since every outcome set contains a string of length n-1. Calibration: F(2,1) = 2.

Determine F. Concretely:
(1) give the full table F(n,t) for 0 <= t <= n <= 6 (and n = 7 if you can), and for each n the set of t attaining F(n);
(2) decide whether one well-placed feedback bit ever strictly beats no feedback, i.e. find the least n with F(n) > F(n,0), or prove there is none;
(3) let r(n) = n - log2 F(n) be the redundancy. Determine its growth: is r(n) bounded, of order log log n, of order log n, or something else? Give matching upper and lower bounds up to an additive constant if possible, and say where the optimal t sits asymptotically (a constant, about n/2, n minus about log n, n minus a constant, ...).

## 3. WHY IT IS PROBABLY UNCHARTED

Deletion codes with full, delayed or noiseless feedback are studied, and so is partial feedback for erasure/substitution channels, but I know no result on a single yes/no feedback bit that says "a deletion happened" without its position. The tension is new: b can carry a syndrome of u, yet the receiver cannot tell u·a' from u'·b by length, so the two halves interfere. Doubts: the literature on deletion channels with limited feedback is large and I may be missing it; the answer might collapse to a known code via a simple reduction.

## 4. HOW TO CHECK AN ANSWER

For fixed (n,t) there are 2^(2n-t) strategies; build the conflict graph (two strategies adjacent when outcome sets intersect) and compute a maximum independent set exactly (ILP or branch-and-bound clique solver), merging strategies with identical outcome sets first. n <= 6 is at most a few thousand vertices. Check F(n,0) against the known single-deletion optima (1, 2, 2, 4, 6, 10 for n = 1..6). A claimed asymptotic needs an explicit construction, verifiable by the same disjointness test at moderate n, plus a counting or packing upper bound proof.
