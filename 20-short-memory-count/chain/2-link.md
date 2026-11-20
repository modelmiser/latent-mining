## 1. KNOWN

Well-posed: finitely many machines, each spread computable, so e(w) exists. Exact window counting (spread 0) needs 2^w states (Myhill–Nerode), so e(1) = 0 and e(w) >= 1 for w >= 2. Remembering the last k = floor(log2(w+1)) bits gives spread w - k, so e(w) <= w - k, and e(2) = 1. I believe, unproved, that e(w) = w - Theta(log w); then the fallback "limit of e(w)/w" is degenerate (it is 1) and the real content is w - e(w). Sliding-window counting with relative error in polylog bits (Datar–Gionis–Indyk–Motwani) is a different regime. Exact e(w) for w >= 3: unsure.

## 2. SUCCESSOR QUESTION

Fix an integer w >= 1. A machine is a deterministic finite automaton M = (Q, q0, d) with exactly w+1 states: q0 in Q is the start state and d : Q x {0,1} -> Q the transition function. There is no output and no other memory. After reading a binary string x, M is in state d*(q0, x).

For a binary string x of length at least w, let c(x) be the number of 1s among its last w bits. For a state q, let C(q) be the set of values c(x) over all binary strings x of length at least w (no upper limit) with d*(q0, x) = q. If C(q) is non-empty, spread(q) = max C(q) - min C(q).

A cycle of M is a sequence of m >= 1 states q_1, ..., q_m, each reachable from q0, with bits b_1, ..., b_m such that d(q_i, b_i) = q_{i+1} for i < m and d(q_m, b_m) = q_1 (m = 1 is a self-loop). Every state on a cycle has non-empty C.

Define
- e(w) = minimum over all machines M of the maximum of spread(q) over states q with non-empty C(q);
- f(w) = minimum over all machines M of the maximum, over all cycles Z of M, of the minimum of spread(q) over the states q on Z.

Meaning of f: the designer picks M and a set F of "lit" states. An adversary knowing M and F picks an infinite binary stream. Rule 1: on every infinite stream, M is in a lit state at infinitely many times (equivalently, no cycle of M avoids F). Rule 2: every lit state q with non-empty C(q) has spread(q) <= s. f(w) is the least s for which some (M, F) obeys both rules. So the machine may stay silent when confused, but never forever; e(w) is the case F = Q, hence f(w) <= e(w).

Compute f(w) and e(w) exactly for w = 1, ..., 6. Decide whether f(w) = e(w) for every w; if not, find the smallest w with f(w) < e(w) and exhibit a witnessing machine. Give a closed formula for f(w) for all w, with proof and an explicit family of optimal machines; failing that, decide whether e(w) - f(w) is bounded, and determine the growth of w - f(w) up to a constant factor.

## 3. WHY IT IS PROBABLY UNCHARTED

Streaming bounds demand an answer at every time or at the end; a tiny automaton choosing its own moments to answer, under a liveness rule, gives a min-max-min over cycles I cannot match to a named theorem. The last-k-bits machine has equal spread everywhere, so silence gains it nothing; whether spending states as silent waiting rooms buys precision is unclear. Doubts: periodic streams may force f = e for all w, emptying the twist; both may be w - Theta(log w).

## 4. HOW TO CHECK AN ANSWER

Enumerate all (w+1)-state machines up to renaming (w <= 6, with canonical forms). For each, search the product of M with a last-w-bits register from q0 to get every C(q). e: the largest spread. f: the least s such that deleting states with spread <= s leaves the reachable transition graph acyclic. Minimise over machines. A formula must match these values; witnesses are checked by the same search; general lower bounds need proof.
