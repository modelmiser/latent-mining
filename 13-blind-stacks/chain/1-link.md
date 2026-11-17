## 1. KNOWN

With no bypass, a capacity-1 stack is a pipe: nothing can overtake, so for c = 1 only the identity is sortable for every s. The seed's "as c increases from 1" therefore starts at a degenerate point; c = 2 is the first real case. One stack of capacity c sorts exactly the permutations avoiding 231 and the decreasing pattern of length c+1 (for c = 2: avoid 231 and 321, giving 2^(n-1)); the counts are bounded-height Dyck-path numbers with rational generating functions. For bounded stacks in series the system is a finite-state "token passing network" (I believe Atkinson, Livesey and Tulley): the sortable class has a rational generating function for each fixed s, c. Whether its basis is always finite, and explicit bases for s >= 2: unsure. The seed is also three questions with three parameters (n, s, c), so it has no single answer.

## 2. SUCCESSOR QUESTION

Fix an integer k >= 1. There are k stacks S_1, ..., S_k in a row, each able to hold at most 2 items, all initially empty. Two players, Sorter and Adversary, play with the numbers 1, ..., n; both know k and n and both see the full contents of every stack at all times (perfect information). The order in which the numbers arrive is NOT fixed in advance: Adversary chooses it during play.

Sorter makes one move at a time, choosing any legal move from:
(IN) Legal if S_1 holds fewer than 2 items and some number has not yet arrived. Adversary then picks any number of {1, ..., n} that has not yet arrived, and it is pushed onto S_1.
(SHIFT i), for 1 <= i < k. Legal if S_i is non-empty and S_{i+1} holds fewer than 2 items. The top item of S_i is popped and pushed onto S_{i+1}.
(OUT) Legal if S_k is non-empty and its top item equals the smallest number not yet output. That item is popped and leaves the system for good.
For k = 1 there are no SHIFT moves; IN pushes onto S_1 and OUT pops from it.

Sorter wins when all n numbers have been output (necessarily in the order 1, 2, ..., n). Adversary wins if Sorter has no legal move before that. Every item only moves forward, so play ends within n(k+1) moves.

Define A(k) as the smallest n for which Adversary has a winning strategy. For comparison define the offline quantity B(k) as the smallest n for which there is a fixed arrival order (a permutation of 1..n, announced to Sorter before play) that Sorter cannot sort using the same moves. Clearly A(k) <= B(k), and A(1) = B(1) = 3.

Question: determine A(k) and B(k) for every k >= 1, as closed formulas in k. In particular: what is the smallest k with A(k) < B(k) (or do they coincide for all k), and does Adversary win for every n >= A(k)?

## 3. WHY IT IS PROBABLY UNCHARTED

Stack-sorting literature counts sortable permutations and finds pattern bases, with the whole input known. For one stack greedy play is optimal, so foresight is worthless; for stacks in series the right move can depend on the future, and I know no work pricing that foresight as a game against an adaptive input. Doubts: B(k) may follow from token-passing-network results I do not remember (the decreasing permutation gives a linear upper bound, and the truth may simply be linear), and A(k) = B(k) could hold for a dull reason, e.g. one worst permutation defeats every strategy regardless of adaptivity.

## 4. HOW TO CHECK AN ANSWER

A state is (stack contents, set of arrived numbers); the output count is implied. For B(k): for each permutation of 1..n, depth-first search over states with memoisation; B(k) is the first n with a failure. For A(k): memoised minimax over the same states, Sorter choosing moves, Adversary choosing the arriving number. Both are feasible for k <= 4 or so (n around 10) in minutes. A claimed formula is refuted by one mismatch; confirmed in general only by a proof giving an explicit Sorter strategy below the threshold and an Adversary strategy (or permutation) at it.
