## 1. KNOWN

The classical problem is settled as stated: minimal time 2n - 2 (Goto, Waksman, Balzer), a six-state minimal-time solution (Mazoyer), four states insufficient (Balzer, later re-checked by exhaustive search); five states open. The three weakenings are uneven. One bit per neighbour, with the designer choosing which bit each state emits, is known territory: 1-bit-communication cellular automata synchronise in optimal time (Mazoyer; Umeo and co-authors, I believe). "Sees only its left neighbour" on an open line is degenerate: cell 1 then runs autonomously, never learns n, so no rule works for all n (for a single fixed n a countdown with n + 1 states does it). One-way rings are a studied repair. The deaf-tick variant is ill-posed until "deaf" is defined; if deaf means "does not update", deafness on the last tick kills every rule. Fault-tolerant variants exist (Kutrib and Vollmar, defective cells); the adversarial transient version: unsure.

## 2. SUCCESSOR QUESTION

Equality-blind firing squad. Fix an integer s >= 3 and a state set S of size s containing three distinct named states: Q (asleep), G (general), F (fire). Cells 1..n stand in a line, n >= 2. A cell never sees a neighbour's state. It sees only its own state x and, for each side, one symbol from {=, !=, #}: "#" if there is no cell on that side (the end of the line), "=" if the neighbour on that side is currently in exactly the state x, and "!=" otherwise. A rule is a function d: (S \ {F}) x {=, !=, #} x {=, !=, #} -> S, taking (own state, left symbol, right symbol) to the next state, subject to the sleep constraint: d(Q, a, b) = Q whenever both a and b lie in {=, #}. All cells use the same d and update simultaneously at each tick. At time 0 cell 1 is in G and every other cell is in Q. The rule d synchronises length n if there is a time t >= 1 such that every cell is in F at time t and no cell is in F at any earlier time (behaviour after time t is irrelevant). The rule has no access to n other than through the symbols it sees.

Define L(s) as the largest N such that some single rule d on s states synchronises every length n with 2 <= n <= N; put L(s) = infinity if some rule on s states synchronises every n >= 2.

Questions.
(1) Determine L(3), L(4) and L(5) exactly.
(2) Decide whether L(s) = infinity for some s. If so, determine the least such s, and determine the least function T(n) such that some equality-blind rule (with any finite number of states) synchronises every n >= 2 and fires on length n at time T(n); in particular decide whether T(n) = 2n - 2, the optimum when cells see their neighbours' states in full, is still attainable. If L(s) is finite for every s, determine L(s) as a function of s.

## 3. WHY IT IS PROBABLY UNCHARTED

In the 1-bit literature the designer chooses each state's emitted bit; here the bit is forced to be "same as me?", so what a cell learns depends on its own state, and reading a neighbour means probing it over several ticks. I know symmetric (left/right-blind) and 1-bit variants, but no equality-only one. Doubts: a slowed simulation of an ordinary solution (neighbour holds still while I cycle states) probably gives L(s) = infinity for some large s, making the existence half easy; the least s and the exact T(n) would remain. A related "colour-blind" model may exist under a name I do not know.

## 4. HOW TO CHECK AN ANSWER

For L(s): backtracking search over rule tables, filling an entry only when a simulation of lengths 2, 3, ... first needs it, pruning on premature or partial firing and on repeated configurations (a cycle means no firing). A claimed L(s) = N needs a witness table verified by direct simulation on lengths 2..N, plus search exhaustion for 2..N+1. The table has 9(s - 1) entries, so s = 3, 4 are quick; s = 5 may need symmetry reduction. A claimed infinite solution needs an explicit table, simulation to large n, and an invariant proof; a T(n) claim needs an indistinguishability lower bound.
