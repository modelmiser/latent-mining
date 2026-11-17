## 1. KNOWN

The question as received appears degenerate: I believe F(n) = floor(n/2) for every n, i.e. two destructions per round never delay the first completion at all. Sketch (unverified by computation): write each process's clockwise and counter-clockwise knowledge extents as t minus a "deficit". In a frame moving with the information, a destruction sets one deficit to (neighbour's deficit + 1) and deficits otherwise persist, so each destruction creates or raises one (process, direction) entry. To block every process at round floor(n/2) the adversary needs total deficit >= 2 at each process (n even, n destructions available) or >= 1 at each (n odd, n-1 available); the first entry created is worth only 1 and raising it wastes a destruction, so the count fails. Checks: F(3) = 1 directly. The upper bound 2*floor(n/2) and the arc structure of K_t(i) are correct. Related literature (dynamic networks, message adversaries, Santoro-Widmayer): unsure of any exact match.

## 2. FINAL QUESTION

First to know everything, with a destruction budget k.

Setting. Let n >= 3 and k >= 1 be integers. Processes are the elements of Z_n on a ring. There are 2n directed links: (i -> i+1) and (i -> i-1) for each i. Time runs in synchronous rounds t = 1, 2, ... In every round every process sends everything it knows on both outgoing links; there is no protocol to choose.

Adversary. The only player. It has full information and picks, in each round t, a set D_t of links whose messages are destroyed in round t, subject to: |D_t| <= k, and D_t is disjoint from D_{t-1} (D_0 is empty). Nothing is random or hidden, so a strategy is a legal sequence D_1, D_2, ...

Knowledge. K_0(i) = {i}. K_t(i) is the union of K_{t-1}(i) and of K_{t-1}(j) for each j in {i-1, i+1} with (j -> i) not in D_t. Process i is complete at round t if K_t(i) = Z_n.

Value. F(n,k) is the maximum, over legal sequences, of the least t at which at least one process is complete. Since no link fails twice in a row, floor(n/2) <= F(n,k) <= 2*floor(n/2), and F is non-decreasing in k.

Tasks.
(a) Compute F(n,k) for all 3 <= n <= 10 and 1 <= k <= 4.
(b) HEADLINE: for each fixed integer k >= 3, find the exact value of c(k) = lim_{n -> infinity} F(n,k)/n (prove the limit exists), as a closed formula in k, with an adversary strategy attaining it and a matching upper-bound proof. The first open case is k = 3: find c(3) exactly.

UNVERIFIED beliefs.
- F(n,1) = F(n,2) = floor(n/2) for all n (counting argument: by the deadline floor(n/2) the adversary has made at most n, resp. n-1 for odd n, destructions, each of which gives one lasting unit of delay to one (process, direction) pair, and that is too few to delay every process).
- F(n,2n) = 2*floor(n/2) (destroy every link in every odd round), and k = n already gives about n.
- F(5,3) = 3 > floor(5/2), so k = 3 is the least budget that ever delays the first completion.
- c(4) >= 5/8, via "ramps": a destruction that tracks information moving at speed one stacks delay, so j successive tracked destructions build delays 1, 2, ..., j on j consecutive pieces of information; opposing clockwise and counter-clockwise ramps give every process the same total delay.
- 1/2 < c(3) < c(4) < 1, and c(k) -> 1 as k -> infinity.

GUESS (one sentence): I would bet c(4) = 5/8 exactly and c(k) = 1 - O(1/k), because the rule against hitting the same link twice in a row halves the growth rate of every ramp and I see no way around that.

## 3. WHY IT IS PROBABLY UNCHARTED

Message-adversary work mostly asks whether broadcast or consensus is solvable, or bounds the time for ALL processes to learn, under per-round connectivity conditions. Here the measure is the FIRST completion, the constraint is a per-round count plus a no-repeat rule, and the answer sought is an exact constant per budget. Doubts: the deficit dynamics are a max-plus recurrence, so a known scheduling or pursuit result may settle c(k) quickly; and my own ramp heuristic might already be tight, making the answer a short exercise. The limit might also fail to exist as stated (parity effects), requiring separate even/odd constants.

## 4. HOW TO CHECK AN ANSWER

Fixed (n,k): a state is (l_i, r_i for all i, D_{t-1}), where K_t(i) is the arc from i-l_i to i+r_i. Depth-first search with memoisation over all D_t of size <= k disjoint from D_{t-1} (subsets of 2n links; at most 6,196 choices for n = 10, k = 4), to depth 2*floor(n/2); return the largest first-completion round. Reduce by rotation and reflection. Sanity checks: F(n,2n) = 2*floor(n/2), monotonicity in k, F(n,2) = floor(n/2). A general claim needs: an explicit strategy valid for all n (legality proved, including the no-repeat rule), an upper bound proof for every adversary, existence of the limit, and agreement with the table for n <= 10.
