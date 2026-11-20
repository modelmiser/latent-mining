## KNOWN

The question is well-posed; I found no degeneracy. Easy facts: each step at most doubles the largest register, so f(n) >= ceil(log2 n), giving W(k) >= k-1; the binary method gives W(k) <= 2k-2. So any limit of W(k)/k lies in [1, 2]. Every reachable state is M(1,1)^T for a product M of four fixed 2x2 nonnegative integer matrices, so only 4^t states exist after t steps; that counting gives only about k/2, weaker than the doubling bound. For unrestricted addition chains the worst case is log2 n (1 + o(1)) (Brauer), but Brauer's method stores many intermediate values, so it does not transfer. The moves (a+b, b), (a, a+b) alone are the subtractive Euclidean algorithm run backwards; "Euclidean / continued-fraction addition chains" exist in the literature (Bergeron, Berstel, Brlek, Duboc, I believe). Whether the limit exists, and its value, I do not know; existence proof: unsure.

## SUCCESSOR QUESTION

Fix an integer m >= 2. A two-address machine has exactly two registers, A and B, each holding an element of Z_m = {0, 1, ..., m-1} (think of it as the exponent of a power of x, where x generates a cyclic group of known order m). Initially A = 1 and B = 1. The only instruction is "dst := dst * src", where dst and src are each one of A, B and may be the same register. In exponents, with all arithmetic mod m, one step takes the state (a, b) to exactly one of four states:

- (2a, b)    [A := A*A]
- (a, 2b)    [B := B*B]
- (a+b, b)   [A := A*B]
- (a, a+b)   [B := B*A]

A result always overwrites one of its own operands; a register cannot be copied, there is no inversion, no constant can be loaded, and no other instruction exists. Each step costs 1. The value 0 is an ordinary register content (from (0, b) the moves are still as listed).

For r in Z_m let g_m(r) be the least number of steps after which at least one of the two registers holds r (so g_m(1) = 0). This is finite for every r: over the integers the binary method run in A with B = 1 reaches any positive integer congruent to r. Define the worst-case cost in the cyclic group of order m:

D(m) = max { g_m(r) : r in Z_m }.

Question: determine D(m) for every m >= 2. Concretely:

(i) Give the exact values D(m) for 2 <= m <= 400, and for each such m the smallest residue r with g_m(r) = D(m).

(ii) Give and prove a formula for D(2^k) for all k >= 1, and a formula or tight two-sided bound for D(p) over primes p. Note the structural split: for odd m all four moves are bijections of Z_m x Z_m, while for even m the two doubling moves lose information.

(iii) Determine L_sup = limsup D(m)/log2(m) and L_inf = liminf D(m)/log2(m) as m goes to infinity (prove both are finite and give their values). In particular decide whether L_inf < 1, that is, whether wrap-around mod m lets the two-register machine beat, for infinitely many m and in the worst case over residues, the bound ceil(log2 n) that holds for every integer exponent n; and decide whether L_sup equals L_inf.

## WHY IT IS PROBABLY UNCHARTED

Reducing mod m destroys the monotonicity behind the log2 n lower bound: only the counting bound (4^t states, so D(m) >= about (1/2) log2 m) survives, leaving a factor-4 gap up to the binary method. Addition-chain literature works over the integers; diameter results for walks like x -> 2x, x+1 on Z_m (Chung–Diaconis–Graham) concern one register with a constant, not a copy-free two-register machine. Doubts: for odd m this is an orbit-diameter question for a subsemigroup of GL2(Z_m), and expander-type results may already give O(log m) with unspecified constants; exact constants I believe are open.

## HOW TO CHECK AN ANSWER

Breadth-first search from (1,1) over the at most m^2 states of Z_m x Z_m, recording for each residue the first depth at which it appears in either register; D(m) is the largest such depth. All m <= 400 take about 2*10^7 state visits in total, seconds on one core; D(2^k) is checkable to k = 11 or 12. A claimed formula is refuted by one mismatching m; claimed limits need proof: an explicit program family for the upper bound, a counting or invariant argument for the lower.
