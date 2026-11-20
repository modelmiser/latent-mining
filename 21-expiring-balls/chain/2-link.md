## KNOWN

The question looks well-posed (finite reachability game). Easy facts, by hand and unverified beyond that: f(2)=f(3)=1, f(4)=2, and f(n) <= n-1. Without deletions, the classical tournament-style adversary against two-choice placement (pair up bins, then pair up the loaded ones; I believe this is the Azar-Naor-Rom lower bound for greedy) only ever offers a forest, giving load k from about 2^k bins. With free deletion Adversary does much better: build a bin x of load k-1, delete every ball not in x (x keeps a star of k bins), build a second load-(k-1) bin among the untouched bins, and join them. So the least n for load k is at most the previous value plus k, i.e. roughly k^2/2, f(n) at least about sqrt(2n). This echoes the square-root bounds for load balancing of temporary tasks (Azar-Broder-Karlin, I believe). Whether star leaves can be reused to beat this: unsure.

## SUCCESSOR QUESTION

There are n bins, labelled 1..n, initially empty. A ball, once placed, is a record ({u,v}, w, t): the unordered pair of distinct bins that was offered, the bin w in {u,v} that received it, and its arrival time t (the number of balls offered before it, so present balls are totally ordered by age). The state is the set of balls currently present, together with their age order (only the relative order matters). The offer graph G of a state has the bins as vertices and one edge {u,v} for each present ball. The load of a bin is the number of present balls it received.

Two players, Adversary and Rule, play with perfect information: both see the whole state at all times. Repeatedly, Adversary makes one of two moves:

- Expire: legal only if at least one ball is present. The OLDEST present ball is removed (its edge leaves G and the load of its bin drops by one). Adversary has no choice of which ball leaves. Rule does nothing.
- Offer: name two distinct bins u, v. This is legal only if u and v lie in different connected components of the current G (so G is always a forest and at most n-1 balls are present). Rule must answer immediately by choosing w in {u,v}; the ball ({u,v}, w, t) is added as the newest ball. Rule can never move or remove a ball. A pair may be offered again after its earlier ball has expired.

Play may continue for any finite number of moves. Adversary achieves L if at some moment some bin has load L. Define g(n) as the largest L such that Adversary has a strategy achieving L against every Rule. (States are forests on n labelled vertices with a chosen endpoint per edge and a linear order on the edges, so this is a finite reachability game and g(n) is well defined.)

For comparison, let h(n) be the same quantity when Expire is forbidden altogether, and f(n) the same quantity when Adversary may instead remove ANY present ball of its choice. Clearly h(n) <= g(n) <= f(n).

Determine g(n) exactly for n = 2,...,7. Then, for every k >= 1, determine N(k), the least n with g(n) >= k, as a closed formula or exact characterisation, together with an optimal Rule. In particular decide: is g(n) = h(n) for every n (first-in-first-out deletion never helps Adversary), and if not, what is the least n with g(n) > h(n), and does N(k) grow exponentially, polynomially, or otherwise in k?

## WHY IT IS PROBABLY UNCHARTED

The trim-to-a-star trick needs to delete junk while keeping a loaded bin's balls; under first-in-first-out expiry the loaded bin's earliest ball is typically older than much of the junk, so the easy quadratic recursion fails, yet expiry is not obviously useless. I know sliding-window and temporary-task load balancing as fields, but no result for forest-restricted two-choice offers with adversarial FIFO expiry. Doubts: g may simply equal h (then the theorem is the content), and h itself might not be exactly floor(log2 n).

## HOW TO CHECK AN ANSWER

Enumerate states (ordered, endpoint-marked forests on n labelled bins, up to bin relabelling) and compute Adversary's attractor of "some load >= L" by backward fixed point; this gives g(n), and the analogous runs give h(n) and f(n), for n up to 7. A claimed formula for N(k) must match these values; a general claim needs an explicit Adversary strategy (lower bound) and an explicit Rule with an invariant preserved by Offer and Expire (upper bound).
