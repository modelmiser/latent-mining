## KNOWN

The question is well posed and reduces to pure combinatorics: since j is uniform on the cyclic group ⟨π⟩, s is uniform on the orbit of w(x). So a protocol is valid iff the 2^n−1 rows with AND 0 lie in one ⟨π⟩-orbit and w(1…1) lies outside it. Consequences (my own hand arguments, unverified): the rows are distinct, so ord(π) ≥ 2^n−1, and by Landau's function m grows at least like n²/log n; u_i^0 and u_i^1 have equal heart counts, so |P_i| ≥ 2 and m ≥ 2n; a short case check excludes m=4, so c(2)=5. Player-local coprime cycles fail (the orbit becomes a product set). Whether c(3) is finite: unsure. "Single-shuffle full-open" protocols are a studied topic (I believe Shinagawa and co-authors); exact results for n-input AND: unsure.

## SUCCESSOR QUESTION

Fix n ≥ 2. A cut-and-peek protocol with m cards is the following public data: a partition of the positions Z_m = {0,…,m−1} into sets P_1,…,P_n,H (each P_i non-empty, H possibly empty); for each player i two words u_i^0, u_i^1 in {♣,♥}^{P_i}; a helper word h in {♣,♥}^H; and a non-empty set R ⊆ Z_m of reveal positions. Cards with the same face are indistinguishable, all backs are identical, and any number of cards of each face is available.

Play. Each player i holds a secret bit x_i and, unseen by the others, lays the word u_i^{x_i} face down on the positions P_i; the word h is laid face down on H. This gives a row w(x) in {♣,♥}^m. Then one random cut happens: an integer j is drawn uniformly from {0,…,m−1}, nobody learns j, and the card at position p moves to position p+j mod m, for every p. Then exactly the cards now lying on the positions in R are turned face up, and everyone sees the resulting word t in {♣,♥}^R (they know which face lies on which position of R). The other m−|R| cards are never turned. Nothing else happens.

The protocol securely computes AND if (correctness) there is a function g with g(t) = x_1 ∧ … ∧ x_n for every input x in {0,1}^n and every j, and (privacy) for any two inputs x, x′ with the same AND, the probability distributions of t (over the uniform j) are identical.

Example: m=5, R=Z_5, P_1={0,1}, H={2}, P_2={3,4}, u_1^0=♥♣, u_1^1=♣♥, h=♥, u_2^0=♣♥, u_2^1=♥♣ is the classical five-card trick, a valid protocol for n=2. When R=Z_m, validity forces the 2^n−1 rows w(x) with AND 0 to be distinct rotations of one another, hence m ≥ 2^n−1; with R smaller, this argument no longer applies, because different rows can show the same multiset of windows.

Let r(n) be the least m for which a cut-and-peek protocol securely computing the n-bit AND exists (r(n)=∞ if none exists for any m), and let f(n) be the same minimum when R is required to be all of Z_m. Determine r(2), r(3), f(2), f(3) exactly. For general n: decide whether r(n) is finite for every n, and give either a formula for r(n) or a proof that r(n) < 2^n−1 for all large n (or that r(n) = f(n) always).

## WHY IT IS PROBABLY UNCHARTED

Card-protocol lower bounds I know of count cards for committed-format AND under general shuffles; here the shuffle is fixed to one random cut and the freedom is which positions to peek at. Partial reveal destroys the orbit argument, so the condition becomes equality of window multisets of binary necklaces, a design-like condition I cannot match to a named theorem. Doubts: random-cut-only multi-input AND may have been treated in the card-cryptography literature (f(n) especially); and r(n) might simply equal f(n), making the twist empty, though proving that would itself be a result.

## HOW TO CHECK AN ANSWER

Merge helpers into the row: a protocol is a word v = w(1…1) in {0,1}^m, disjoint non-empty flip sets D_1,…,D_n ⊆ Z_m, and R. For each m in increasing order, enumerate (v, D, R) up to rotation; for each input compute the multiset of the m windows of its row on R; accept iff all AND-0 multisets are equal and disjoint in support from the AND-1 one. This is seconds for n=2 and feasible for n=3 up to m≈9 with pruning. A claimed general formula needs a construction plus an impossibility proof, checked against these values.
