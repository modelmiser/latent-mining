## KNOWN

Rivest and Shamir (1982) founded write-once-memory codes; rewriting one fixed-size value t times is well studied. Restricted update patterns were treated too: floating and buffer codes for flash cells (Jiang, Bohossian, Bruck), trajectory codes where data walks along a graph (authors: unsure). Those results are mostly asymptotic.

Flaw: for the counter and the pointer on a line the question is trivial. Every operation changes the value, so each needs a new cell; filling ones from the left for +1 and from the right for -1 decodes as (left run) - (right run). On t cells the all-ones state after t moves is ambiguous, so exactly t+1 cells are needed. Only structures whose operations branch more than two ways, such as a stack of bits, look open.

## SUCCESSOR QUESTION

A memory has n cells, each 0 or 1, all initially 0. A cell may be changed from 0 to 1, never back. A configuration is an element of {0,1}^n; write c <= c' when every cell that is 1 in c is 1 in c'.

The data is a stack of bits: a finite binary string, initially the empty string. Three operations exist: PUSH0 and PUSH1 append the given bit to the end of the string; POP deletes the last bit and is legal only when the string is non-empty.

A scheme is a pair (D, E). D maps each configuration to a binary string or to the symbol "invalid", with D(0^n) = the empty string. E maps a pair (configuration c, operation) to a configuration c' with c <= c'. E is deterministic and sees only the current configuration and the requested operation; D sees only the configuration.

Play: an adversary who knows the scheme chooses a sequence of operations, each legal for the current stack. After each operation the memory is changed from its configuration c to E(c, operation). The scheme survives the sequence if after every operation D of the memory's configuration equals the current stack exactly (the whole string, in order).

Let T(n) be the largest t such that some scheme survives every legal sequence of at most t operations.

(Spending two fresh cells per operation - 01 for a pushed 0, 10 for a pushed 1, and overwriting the last live pair with 11 on a pop - gives T(n) >= floor(n/2). Since each operation changes the stack, each must set at least one cell, and all strings of length at most t need distinct configurations, so T(n) <= n - 1.)

Questions:
1. Determine T(n) exactly for n = 1, 2, ..., 8.
2. Determine T(n) for all n, or at least the constant lambda = lim T(n)/n (prove the limit exists and give its value, which lies in [1/2, 1]).

## WHY IT IS PROBABLY UNCHARTED

Rewriting theory counts bits per write for a value that may jump anywhere; here the data walks on the infinite binary tree, the reader needs the whole path, and pops force old labels onto fresh configurations. Which cell is set carries information, so neither bound looks tight. Doubts: trajectory-code results may give lambda as a corollary (degree-3 data graph), and lambda may simply be 1. The exact small values are probably unpublished either way.

## HOW TO CHECK AN ANSWER

For fixed n and t, "T(n) >= t" is a finite search over labelings of configurations by strings of length at most t; backtracking or a SAT solver decides it for small n, using cell permutations and the 0/1 swap as symmetries. Hand sanity values: T(1) = 0, T(2) = 1, T(3) = 1. A claimed scheme is verified by enumerating all 3^t sequences; a claimed lambda needs a construction plus a matching upper-bound proof.
