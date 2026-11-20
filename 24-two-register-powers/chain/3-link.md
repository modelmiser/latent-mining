## KNOWN

The modular machine is the image of the integer machine: a move sequence run over Z reduces mod m to the same sequence run over Z_m. Hence g_m(r) = min{G(n) : n >= 1, n ≡ r mod m}, with G the integer cost. After t integer steps every register is at most 2^t, so at most 2^t residues are reachable in t steps and D(m) >= ceil(log2 m). So part (iii) as received is flawed: "L_inf < 1" is trivially false; wrap-around can never beat log2 m in the worst case. The binary method in A with B = 1 gives G(n) = floor(log2 n) + popcount(n) - 1 at most, so D(m) <= 2 log2 m and both limits are finite, in [1, 2]. The live issue is whether the constant is exactly 1 or strictly larger. Parts (i) and (ii) are pure computation or probably messy. Citations for the two-register no-copy model: unsure.

## FINAL QUESTION

Definitions. A state is a pair (a, b) of positive integers; the start state is (1, 1). One step replaces (a, b) by exactly one of (2a, b), (a, 2b), (a+b, b), (a, a+b). There are no other moves (no copy, no subtraction, no constants). For an integer n >= 1, G(n) is the least t such that some sequence of t steps from (1, 1) ends in a state with a = n or b = n; G(1) = 0. For an integer m >= 2 and r in {0, ..., m-1}, put

g_m(r) = min { G(n) : n >= 1, n ≡ r (mod m) },   D(m) = max over r of g_m(r).

(Equivalently: run the same four moves on pairs in Z_m x Z_m from (1, 1), all arithmetic mod m, 0 an ordinary value; g_m(r) is the least number of steps after which a register holds r.) Also let N(t) = #{ n >= 1 : G(n) <= t }.

Headline (yes/no): Is

L = liminf over m -> infinity of D(m) / log2(m)

strictly greater than 1?

General form expected of an answer: a proof, either of a constant c > 1 with D(m) >= c log2(m) for all large m, or of an infinite family of m with D(m) <= (1 + o(1)) log2(m). Secondary, only after the headline: the exact value of L.

Small instances: D(m) for 2 <= m <= 400 and N(t) for 0 <= t <= 12 are computable by breadth-first search.

UNVERIFIED easy facts and values:
- ceil(log2 m) <= D(m) <= 2 floor(log2 m) for m >= 2.
- D(m) >= min{ t : N(t) >= m }, since the m residues need m distinct integers of cost at most D(m). Hence if limsup N(t)^(1/t) = lambda < 2, then L >= 1/log2(lambda) > 1 and the answer is yes.
- N(t) <= 2^t; N(t) grows at least like phi^t, phi = (1+sqrt 5)/2 (binary method count), so 1/log2(lambda) <= 1.4405.
- G(n) = ceil(log2 n) only for powers of 2.
- N(0..3) = 1, 2, 4, 7 (costs: 2 -> 1; 3, 4 -> 2; 5, 6, 8 -> 3; 7 -> 4).
- D(2) = 1, D(3) = 2, D(4) = 2, D(5) = 3, D(6) = 3, D(7) = 4, D(8) = 4.

Guess (clearly a guess): yes, L > 1, because a step that does not lose growth is forced (doubling the larger register), while steps that carry a real choice seem to lose a constant fraction of a bit of growth on average, which should force lambda < 2.

## WHY IT IS PROBABLY UNCHARTED

Addition-chain literature concentrates on unlimited storage, on l(n) for single n, or on Euclidean/continued-fraction chains (add moves only). The two-register, overwrite-only machine with doubling, measured in the worst case over residues of a cyclic group, is a narrow corner. Doubts: counting integers by restricted-chain cost may exist under "two-register addition chains" or straight-line-program width 2; the growth rate lambda of N(t) may already be known there, which would settle the headline at once. The question could also be easier than it looks if the entropy-versus-growth argument closes in a few lines.

## HOW TO CHECK AN ANSWER

Fixed m: breadth-first search on Z_m x Z_m from (1, 1) with the four moves; record the first depth at which each residue appears in either register; D(m) is the largest such depth (at most m^2 states). Fixed t: breadth-first search on integer pairs to depth t, deduplicating states and using the swap symmetry; N(t) is the number of distinct register values seen. Tabulate D(m)/log2(m) for m <= 400 and N(t)^(1/t) for t <= 12 as evidence only. A "yes" proof must bound N(t) <= C lambda^t with lambda < 2 for all t (or bound D directly); a "no" proof must exhibit infinitely many m with an explicit strategy for every residue, including 0.
