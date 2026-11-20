## KNOWN

Tracking the window count exactly needs 2^w states (the last w bits must be remembered), a standard fact; with w+1 states some confusion is forced for w >= 2. The question as given is well posed: the game reading of f matches the max-min-over-cycles definition (take F = states of spread <= s), and non-simple cycles add nothing, since a simple sub-cycle has an equal or larger minimum. Flaws: it asks five things at once (f, e, their equality, a formula, growth rates), so it has no single headline; and "exactly w+1 states" is the same as "at most w+1", since unreachable states are inert. By hand, unverified: f(1) = e(1) = 0, f(2) = e(2) = 1, and the saturating up/down counter seems to give spread floor(w/2) at every state, so both functions are at most about w/2, far below the w-1 of the trailing-ones counter. No literature result known to me; unsure.

## FINAL QUESTION

Fix an integer w >= 1. A machine is a deterministic finite automaton M = (Q, q0, d) with |Q| = w+1, start state q0 in Q and transition function d : Q x {0,1} -> Q. There is no output and no acceptance condition. Write d*(q0, x) for the state M is in after reading the binary string x from q0.

For a binary string x of length at least w, c(x) is the number of 1s among its last w bits. For a state q, C(q) is the set of all values c(x) over binary strings x of length at least w (no upper limit on length) with d*(q0, x) = q. When C(q) is non-empty, spread(q) = max C(q) - min C(q).

A simple cycle of M is a sequence of m >= 1 distinct states q_1, ..., q_m, each reachable from q0, with bits b_1, ..., b_m such that d(q_i, b_i) = q_{i+1} for i < m and d(q_m, b_m) = q_1 (m = 1 is a self-loop). Every state on a cycle has non-empty C.

Define
  g(M) = max over simple cycles Z of M of ( min over states q on Z of spread(q) ),
  f(w) = min of g(M) over all machines M with w+1 states.

Game reading (equivalent, not a second definition). The designer moves first and publishes M together with a set F of "lit" states and an integer s. The adversary, knowing all three, then picks one infinite binary stream. The designer's choice is legal if (1) on every infinite stream M is in a lit state at infinitely many times, and (2) every lit state q with non-empty C(q) has spread(q) <= s. f(w) is the least s admitting a legal choice. The machine may fall silent when confused, but never forever.

HEADLINE: find the exact function f(w) for all w >= 1, as a closed formula with proof and an explicit family of optimal machines. Finite part of the same question: the exact values f(1), ..., f(6).

UNVERIFIED beliefs of the author:
- f(1) = 0, f(2) = 1, and f(3) <= 1.
- f(w) <= floor(w/2) for all w, from the saturating up/down counter (states 0..w, bit 1 moves up and stops at w, bit 0 moves down and stops at 0), in which every state seems to have spread at most floor(w/2).
- f(w) <= w-1 from the machine that counts trailing 1s up to w.
- f(w) >= 1 for w >= 2.
- If lit states are forced to be all of Q (maximum spread over all states with non-empty C, minimised over M), call the value e(w); then f(w) <= e(w), and e(2) = 1.

GUESS (clearly a guess): I would bet f(w) = floor(w/2) for all w, because a lit state is needed on both the all-0 loop and the all-1 loop and w+1 states look too few to do better than one saturating counter; my confidence is low, and a counterexample at w = 4, 5 or 6 would not surprise me.

## WHY IT IS PROBABLY UNCHARTED

Sliding-window counting is studied with approximation guarantees that hold at every time and with space measured in bits; here the budget is exactly w+1 states, the error is worst-case spread per state, and the guarantee is only required infinitely often on every stream. I know of no paper with that combination. Doubts: the answer may simply be floor(w/2) with a short proof, which would make it an exercise; and automata-theoretic work on "window" or "delay" languages might contain an equivalent statement under other names.

## HOW TO CHECK AN ANSWER

For fixed w, enumerate all DFAs on w+1 states up to renaming of non-start states ((w+1)^(2w+2) raw; canonical BFS numbering cuts this by w!). For each, build the product with the window automaton: start from the pairs (d*(q0, x), x) for all 2^w strings x of length w, close under both bits, giving C(q) for every q. Then g(M) <= s iff the reachable states with spread > s induce an acyclic graph; f(w) is the least s achieved. A general formula needs a machine family with g computed for all w (upper bound) and a proof that every (w+1)-state machine has a cycle whose states all exceed the bound minus one (lower bound).
