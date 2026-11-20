## KNOWN

The n = 2, R = Z_m case is the classical five-card trick (den Boer); five cards is what a single random cut gives there. The received question's claim "m ≥ 2^n − 1 when R = Z_m" is right: AND-0 rows are distinct (equal rows would force u_i^0 = u_i^1 and break correctness) rotations of one word. Flaws: (1) it asks for four values plus an open-ended general part; r(2) = f(2) = 5 is an easy case check, so only n = 3 is a real target. (2) The general form "is r(n) finite for every n / is r(n) < 2^n − 1" presumes existence; a degree argument (below) suggests protocols may not exist at all for n ≥ 3, and for R = Z_m probably not for n ≥ 7. Whether any single-cut, no-further-shuffle protocol for 3-bit AND with free encodings is published: unsure.

## FINAL QUESTION

Objects. Fix integers n ≥ 2 and m ≥ 1. A cut-and-peek protocol (n, m) is the public data: a partition of Z_m = {0,…,m−1} into P_1,…,P_n,H (each P_i non-empty, H possibly empty); for each player i two words u_i^0, u_i^1 in {♣,♥}^{P_i}; a word h in {♣,♥}^H; a non-empty reveal set R ⊆ Z_m.

Play. Player i holds a secret bit x_i and lays u_i^{x_i} face down on P_i, unseen by the others; h is laid face down on H. This gives a row w(x) in {♣,♥}^m, x in {0,1}^n. Then j is drawn uniformly from Z_m, nobody learns j, and the card at position p moves to p+j mod m, for every p. Then exactly the cards on the positions of R are turned up; everyone sees the word t in {♣,♥}^R, t_q = w(x)_{q−j} for q in R. Nothing else happens.

Validity. Let M(x) be the multiset of the m words t obtained as j runs over Z_m. The protocol computes AND securely iff (privacy) M(x) = M(x′) for all x, x′ ≠ 1^n, and (correctness) no word occurs both in M(1^n) and in M(0^n).

Let E(n, m) = 1 if a valid protocol (n, m) exists, else 0; r(n) = min{m : E(n, m) = 1}, or ∞; f(n) the same with R = Z_m required.

HEADLINE. Find r(3) exactly: the least m with a valid three-player protocol, or a proof that E(3, m) = 0 for every m.

General form (secondary): determine the set of n with r(n) < ∞, and r(n) on it.

UNVERIFIED beliefs (from thought only):
(a) Positions where u_i^0 and u_i^1 agree can be moved to H, so WLOG u_i^1 is the complement of u_i^0 on P_i. Averaging the number of ♥ seen forces equal ♥-counts on all AND-0 rows, so every |P_i| is even and u_i^0 is half ♥. Hence r(n) ≥ 2n.
(b) The number of j giving a fixed t is a multilinear polynomial in x of degree at most the number of players whose cards lie in R−j. A polynomial constant on {0,1}^n minus 1^n but not at 1^n has degree n, so some translate of R meets every P_i; |R| ≥ n.
(c) r(2) = f(2) = 5 (five-card trick; m = 4 fails by a short case check).
(d) f(3) ≥ 8: for m = 7 the AND-0 rows would be all 7 rotations of one word W, at Hamming distances 2,2,2,4,4,4 from W, but distances to rotations by a and −a are equal, so each value occurs an even number of times.
(e) With R = Z_m, by (b) the row w(1^n) has the same k-point pattern counts as W for all k < n. If, as I recall (unsure: Grünbaum–Moore), subsets of Z_m are determined up to translation by 6-point counts, then f(n) = ∞ for n ≥ 7. This does not bound r(n), since with R ≠ Z_m the AND-0 rows need not be rotations of each other.
(f) No valid protocol has shift(x) = Σ a_i x_i with w(x) = rotation of W by shift(x) on AND-0 inputs: it forces w(1^n) to be a rotation too.

GUESS: I would bet r(3) = ∞, because privacy demands exact equality of seven window multisets built from only three local edits, and every "linear" way to achieve it drags the all-ones row along; but the freedom in R makes a finite value below 12 a live possibility.

## WHY IT IS PROBABLY UNCHARTED

Card-based cryptography studies committed-format protocols with several shuffles, or richer shuffles (random bisection cuts); lower bounds usually count cards for two-input AND. A single cut, free per-player encodings, a partial reveal and n = 3 is an odd corner. Doubts: multi-input "five-card-trick-like" single-cut protocols have been studied (I recall work on symmetric functions with one cut, details unsure), and one may already settle n = 3 with R = Z_m. The partial-reveal variant I have not seen. The impossibility side connects to homometric sets, which is known mathematics.

## HOW TO CHECK AN ANSWER

Fixed m: enumerate each position's label among 8 options (player 1–3 with u^0 = ♣ or ♥ and u^1 the complement; helper ♣ or ♥), keep labelings with each player present and half-♥; quotient by rotation, reflection, face swap, player permutation. For each, build the 8 rows and, for every non-empty R containing 0, the multisets M(x); test privacy and correctness. Feasible for m ≤ 9 or 10. Confirm the checker accepts the five-card trick (n = 2, m = 5) and rejects all n = 2, m = 4. A finite r(3) needs one explicit protocol plus exhaustive E(3, m′) = 0 below it. r(3) = ∞ needs a proof for all m and all R; computation only supports it.
