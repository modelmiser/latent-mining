## KNOWN

This is the decision-assignment-tree (bit-probe) model of counting; I believe it goes back to Fredman's quasi-Gray codes, with later work by Rahman–Munro, Brodal et al., Bose et al., Raskin, and Chakraborty–Das–Koucký–Saurabh (exact statements: unsure). I am fairly confident that cycling through all 2^n states of n bits needs Omega(log n) reads, and that O(log n) reads with O(1) writes reach nearly all states, so log2 L(r,w) = Theta(2^r) already for constant w. The question is well-posed and finite; L(1,1) = 2. Weakness: only constants and tiny cases remain open.

## SUCCESSOR QUESTION

Destructive-read counters. Fix integers r >= 1 and 0 <= w <= r. Memory is a string s in {0,1}^d, for any finite d you like.

An (r,w)-destructive incrementer is a rooted binary tree T in which every internal node carries a bit index in {1..d} and has a 0-child and a 1-child; no index occurs twice on one root-to-leaf path; every root-to-leaf path has at most r internal nodes (paths may be shorter and of different lengths). Each leaf carries a set R of at most w indices, all occurring on that leaf's own root-to-leaf path.

T defines a map f: {0,1}^d -> {0,1}^d. To compute f(s): start at the root; at an internal node with index i go to the s_i-child; at the leaf reached, let P be the set of indices on the path followed. Then f(s)_i = 1 for i in R, f(s)_i = 0 for i in P \ R, and f(s)_i = s_i for i not in P. In words: reading a bit erases it; the increment may then set at most w of the bits it read to 1 (the choice may depend on everything read); unread bits are untouched. No other state exists. f need not be a bijection.

A cycle of length N is a state s with f^N(s) = s and f^k(s) != s for 0 < k < N. Let

D(r,w) = the largest N such that some (r,w)-destructive incrementer, on some d, has a cycle of length N.

T mentions at most 2^r - 1 indices, so d = 2^r - 1 suffices and D(r,w) <= 2^(2^r - 1). Anchors: D(r,0) = 1 (the number of 1s never increases, so on a cycle no 1 is ever read and the state never changes); D(r,r) is the unrestricted case where the leaf may give the read bits any values.

Questions.
1. Compute the table D(r,w) for all 0 <= w <= r <= 3.
2. Determine D(r,1) for every r as a formula. In particular decide which holds: log2 D(r,1) is bounded by a polynomial in r, or log2 D(r,1) >= c * 2^r for some constant c > 0 and all r.
3. Determine, as a formula in r, the least w = w(r) with D(r,w) = D(r,r).

## WHY IT IS PROBABLY UNCHARTED

Known work charges writes per changed bit, symmetrically. Here erasure is forced and free while only 1s cost, so on a cycle the average number of 1s read per step is at most w; Gray-code constructions ignore that. Doubts: write-once-memory or asymmetric-cost literature may be close, and sparse encodings might reduce D(r,1) to known quasi-Gray codes.

## HOW TO CHECK AN ANSWER

For r <= 2 enumerate all trees on 3 bits and all leaf sets, build f on 8 states, take the longest cycle: seconds. For r = 3 (7 bits, 128 states) enumerate trees up to index relabelling with pruning; I have not verified this takes only minutes. A general formula needs a matching table, an explicit construction simulated for r <= 5, and an upper-bound proof.
