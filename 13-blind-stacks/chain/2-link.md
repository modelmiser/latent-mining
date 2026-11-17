## 1. KNOWN

The question is well posed (a finite perfect-information game), but parts are easy. Monotonicity: Adversary (or the fixed order) can begin with 1, 2, ..., n-m, which Sorter can do no better than pass straight through an empty system, so losing for m implies losing for every n >= m; that sub-question is settled. Upper bound: withholding 1 until last forbids every OUT, and 2k other items fill the system, so A(k) <= B(k) <= 2k+1. k = 1 gives 3 because the order 2,3,1 jams. Offline, a single capacity-2 stack produces, I believe, 2^(n-1) permutations of length n, and bounded-capacity permuting machines in series give finite-state (regular) permutation classes, studied by Atkinson and coauthors; exact citation unsure. So B(k) is a finite-automaton computation for each k; a closed formula is something I cannot name. The adaptive value A(k) I believe is not in the literature.

## 2. SUCCESSOR QUESTION

Fix an integer k >= 1. There are k stacks S_1, ..., S_k in a row, each able to hold at most 2 items, all initially empty. The numbers 1, ..., n arrive one at a time in an order (a permutation of 1..n) fixed by Adversary before play. Sorter knows k and n but is BLIND: Sorter never sees the arrival order or the value of any item. Sorter does know how many items each stack holds (this follows from its own successful moves).

Sorter repeatedly attempts one move:
(IN) Legal if S_1 holds fewer than 2 items and some number has not yet arrived. The next number of the arrival order is pushed onto S_1.
(SHIFT i), for 1 <= i < k. Legal if S_i is non-empty and S_{i+1} holds fewer than 2 items. The top item of S_i is popped and pushed onto S_{i+1}.
(OUT) Legal if S_k is non-empty and its top item equals the smallest number not yet output. That item is popped and leaves for good.
For k = 1 there are no SHIFT moves.

A legal attempt is executed and Sorter is told "done". An illegal attempt changes nothing and Sorter is told "refused". Refusals are free and unlimited. Hence Sorter's only information about values is the done/refused answer to each OUT attempt on a non-empty S_k.

A Sorter strategy is a deterministic function from the history of attempts and answers to the next attempt. It beats an arrival order if, played against it, all n numbers are output after finitely many attempts. It fails if a position with no legal move is reached before that, or if play never ends. Adversary knows Sorter's strategy when choosing the order.

Define C(k) as the smallest n for which every Sorter strategy fails against at least one arrival order of 1..n. For comparison let A(k) be the same threshold when Sorter sees all stack contents and Adversary may choose each arriving number at the moment of the IN move (perfect information). Then C(k) <= A(k), and C(1) = A(1) = 3.

Question: determine C(k) for every k >= 1 as a closed formula in k. In particular: what is the smallest k with C(k) < A(k), or do they coincide for all k? And for n = C(k) - 1, what is the least possible worst-case number of refused OUT attempts of a strategy that beats every arrival order, as a function of k?

## 3. WHY IT IS PROBABLY UNCHARTED

Stack-sorting literature assumes the sorter sees the items; sorting where the only feedback is "is the exit item the next one" belongs to no class-enumeration framework I know. The prefix trick that gave monotonicity no longer obviously works, since a blind Sorter cannot recognise the prefix. Doubts: C(k) might simply equal A(k), or both might equal a trivial bound such as 2k+1 or k+2, making blindness irrelevant; the refusal-count part may then carry the only content. Related "sorting with partial information" work may exist under another name.

## 4. HOW TO CHECK AN ANSWER

For small k (n <= 2k+1 suffices, since withholding 1 still jams), run an AND-OR search over belief states: a state is the stack configuration written in arrival indices plus the set of arrival orders consistent with all answers so far; Sorter picks an attempt, the answer splits the set. Sorter wins for n iff the root is winning; minimise worst-case refusals by the same recursion. Compute A(k) by ordinary game search and compare. A claimed formula must match these values for k = 1..3 or 4, then needs a strategy and an adversary proof.
