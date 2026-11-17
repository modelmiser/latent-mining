## 1. KNOWN

I believe the seed has a short answer. The relevant field is dynamic omission faults (Santoro and Widmayer, "Time is not a healer"); the exact statements there I am unsure of. By my own reasoning: losses are detectable (a message is due every round) and the ring is oriented, so full-information views let a process place every input it has causally heard of. With f = 1 the adversary can permanently starve one process of its left input, so everything is computable in n-1 rounds and no faster for OR, parity or count. With f >= 2 the adversary cuts both links entering any arc forever, so no non-constant function is computable. Threshold: f = 2, by a cut argument. Majority's exact time at f = 1 I am unsure of.

## 2. SUCCESSOR QUESTION

Forbid the permanent cut. Let n >= 3 and let the processes be the elements of Z_n. There are 2n directed links: (i -> i+1) and (i -> i-1) for each i. Processes are identical, deterministic, know n and share a consistent left/right; each holds one input bit. In every round each process sends one message of unbounded size on each of its two outgoing links.

An adversary who sees everything chooses in each round t = 1, 2, ... a set D_t of links whose messages are destroyed, subject to two rules: |D_t| <= 2, and D_t is disjoint from D_{t-1} (D_0 is empty). So no link may be silenced in two consecutive rounds.

Define influence sets: K_0(i) = {i}, and K_t(i) is the union of K_{t-1}(i) with K_{t-1}(j) for each neighbour j of i such that the link (j -> i) is not in D_t. Let T(n) be the maximum, over all legal sequences D_1, D_2, ..., of the least t for which K_t(i) = Z_n for every i. (T(n) is finite: every link delivers at least every second round, so T(n) <= 2*floor(n/2). T(n) is also the exact number of rounds needed and sufficient for every process to output the number of ones among the inputs, or their OR, or their parity, under this adversary.)

Determine T(n) for every n from 3 to 12, give a closed formula for T(n) valid for all n >= 3, and describe an adversary strategy attaining it. Then say whether the formula changes when the budget 2 is replaced by 3.

## 3. WHY IT IS PROBABLY UNCHARTED

The no-repeat rule kills the cut argument that decides the seed, so impossibility turns into a question of exact delay. Budget 2 is too small for the obvious strategy (silence everything on alternate rounds, which gives 2*floor(n/2)), and starving a single process alternately from left and right leaves its neighbours fast, so the adversary must spend its second loss shaping what the neighbours know. I know no theorem giving this constant. Doubts: the answer may be a dull linear formula with a parity term, and bounded-recurrence dynamic-graph flooding results may cover a near-identical model.

## 4. HOW TO CHECK AN ANSWER

The game is finite and inputs are irrelevant. A state is the vector of influence sets (each a contiguous arc, so two offsets per process) together with D_{t-1}. Compute the longest legal play by memoised search over adversary moves (at most 1 + 2n + C(2n,2) per round), using rotation and reflection symmetry; n <= 12 should finish in minutes. Compare the table with the claimed formula, and replay the claimed strategy to confirm it attains each value. A proof needs a matching upper bound argument for all n.
