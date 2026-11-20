## KNOWN

This is the bit-probe (decision assignment tree) model of counters, studied under the name quasi-Gray codes. A tree of depth r can touch at most 2^r - 1 distinct bits over all its paths, so reading fewer than about log2 n bits is impossible for a counter that uses n essential bits (an observation I attribute to Fredman, 1978). Constructions with d slightly above n that read log n + O(1) bits and write O(1) exist (Brodal, Greve, Pandey, Rao; Bose et al.; exact constants: unsure). For d = n and a full 2^n cycle, a linear read lower bound is, I believe, due to Raskin (2017); Chakraborty, Das, Koucky, Saurabh give d = n codes of length 2^n - O(n) with logarithmic read. Flaws of the seed: it asks four things over two parameters, and does not say whether the counter wraps or how states decode to numbers.

## SUCCESSOR QUESTION

Fix integers r >= 1 and 1 <= w <= r. Memory is a string s in {0,1}^d, for any finite d you like.

An (r,w)-incrementer is a rooted binary tree T in which every internal node carries a bit index in {1..d} and has a 0-child and a 1-child; no index occurs twice on one root-to-leaf path; every root-to-leaf path has at most r internal nodes. Each leaf carries a list of new values for some of the indices on its own path, such that at most w of those indices receive a value different from the value implied by the path (the branch taken at that index).

T defines a map f: {0,1}^d -> {0,1}^d. To compute f(s): start at the root; at an internal node with index i go to the s_i-child; at the leaf, overwrite the listed bits with the listed values; all other bits are unchanged. The map uses no other state. Note that f need not be a bijection.

A cycle of length N is a state s with f^N(s) = s and f^k(s) != s for 0 < k < N. Let

L(r,w) = the largest N such that some (r,w)-incrementer, on some d, has a cycle of length N.

(Since T touches at most 2^r - 1 indices, d = 2^r - 1 suffices and L(r,w) <= 2^(2^r - 1).) So L(r,w) is the largest modulus of a wrapping counter whose increment reads at most r bits and changes at most w, with memory free.

Questions.
1. Compute the table L(r,w) for all 1 <= w <= r <= 3.
2. Determine L(r,r) and L(r,1) for every r, as formulas. In particular: for which r, if any, does L(r,r) = 2^(2^r - 1) hold, and is L(r,1) = L(r,r) for all r, for no r >= 2, or for some?
3. Determine the least w = w(r) with L(r,w) = L(r,r).

## WHY IT IS PROBABLY UNCHARTED

The literature fixes the counter size n and minimises reads; here reads are fixed and the modulus is maximised with memory free, which forces the regime where the tree uses nearly every bit it can reach, d close to 2^r - 1, far from the "d = n + small" regime of the known constructions. Non-bijective maps are allowed, so permutation arguments do not apply directly. Doubts: small cases may have been found in computer searches reported in the quasi-Gray papers, and the known constructions may already give log2 L(r,1) within a constant factor of 2^r, leaving only exact values open.

## HOW TO CHECK AN ANSWER

For r <= 3: enumerate trees up to renaming of indices (at most 7 indices; subtrees may share indices across branches), enumerate all leaf labellings respecting w (at most 8 choices per leaf, 8 leaves), build f on at most 128 states, and take the longest cycle of its functional graph. A claimed value is confirmed by an explicit tree achieving it plus the exhaustive maximum. A general formula must reproduce this table and come with a construction and a matching upper-bound proof.
