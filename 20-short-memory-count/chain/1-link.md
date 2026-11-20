## KNOWN

Exact counting of n bits needs n+1 states (the streams 1^k must reach distinct states). A saturating counter (count to s-1, then stick) guarantees absolute error (n-s+1)/2, and pigeonhole on the n+1 possible counts gives only a lower bound near n/(2s). I believe the truth is linear in n-s, close to the saturating counter, but I am unsure of the exact value. Ratio error is ill-posed as stated: a true count of 0 makes any nonzero estimate infinitely wrong, so the machine must isolate count 0. With unknown length, zeros can be withheld by the adversary, so the problem collapses to a unary counter (a tail plus a cycle), which is an easy exercise. Randomised machines do far better (Morris's approximate counter), but the seed is deterministic. The "second pass" and "chosen order" variants are underspecified: a machine with s states cannot remember positions either.

## SUCCESSOR QUESTION

Fix an integer w >= 1. A machine is a deterministic finite automaton M = (Q, q0, d) with exactly w+1 states: Q is a set of size w+1, q0 in Q is the start state, and d : Q x {0,1} -> Q is the transition function. There is no output function and no other memory. M reads a binary stream x_1 x_2 x_3 ... one bit at a time; after reading x_1...x_t it is in state d*(q0, x_1...x_t).

For a binary string x of length t >= w, let c(x) be the number of 1s among its last w bits, x_{t-w+1} ... x_t. So c(x) is one of the w+1 values 0, 1, ..., w: the machine has exactly as many states as there are possible answers.

For each state q, let C(q) be the set of all values c(x) taken over every binary string x of length at least w (every length, with no upper limit) such that d*(q0, x) = q. If C(q) is non-empty, the spread of q is max C(q) - min C(q); states with empty C(q) are ignored. The spread of M is the largest spread of any of its states. (Half the spread is the best worst-case absolute error M can guarantee when it must name the count of 1s in the last w bits at every moment from time w onward, against an adversary who chooses the whole stream and the stopping time, knowing M.)

Define e(w) as the minimum spread of M over all machines with w+1 states (all choices of d and q0).

Determine e(w): compute it exactly for w = 1, 2, 3, 4, 5, and give a closed formula for e(w) valid for all w, with proof, together with a description of a family of machines achieving it. If no exact formula is reached, determine the limit of e(w)/w as w grows, and prove it exists.

## WHY IT IS PROBABLY UNCHARTED

Sliding-window counting is well studied (Datar, Gionis, Indyk, Motwani; later deterministic schemes), but always asymptotically, in bits, and usually for relative error. Those results suggest e(w) grows linearly or nearly so, but say nothing exact about the diagonal case "as many states as answers", where the machine can name every count yet cannot track which bit leaves the window. I know no theorem giving e(w), and the obvious machine (count trailing 1s) gives only w-1. Doubt: the answer may turn out dull (w-1 for all w >= 2), or the asymptotic constant may follow from known lower bounds.

## HOW TO CHECK AN ANSWER

For fixed w, enumerate all (w+1)^(2w+2) transition tables (start state fixed by symmetry; about 10^7 for w = 4, and w = 5 with state-renaming symmetry removed). For each, explore the product of M with the last-w-bits register (at most (w+1)*2^w pairs) from the w-bit prefixes, collect C(q), and take the minimum spread. By hand I get e(1) = 0 and e(2) = 1, which a program should reproduce. A claimed formula must match the computed values, with a matching construction and a lower-bound proof for general w.
