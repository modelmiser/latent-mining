## KNOWN

A write-once-memory (WOM) problem in the Rivest–Shamir tradition, with a stack as data and adversarial updates. Well-posed; the stated bounds look right, but the upper bound is weak. Easy sharpening: a stack of length t is reachable only after exactly t operations, when at least t cells are set, so 2^t <= number of configurations of weight >= t. With t = cn this forces c <= H(c) (binary entropy), so lambda <= about 0.773. Whether the pair scheme is optimal: unsure; I know no theorem settling it. The counting relies entirely on decoding the whole string.

## SUCCESSOR QUESTION

A memory has n cells, each 0 or 1, all initially 0. A cell may be changed from 0 to 1, never back. A configuration is an element of {0,1}^n; write c <= c' when every cell that is 1 in c is 1 in c'.

The data is a stack of bits: a finite binary string, initially empty. PUSH0 and PUSH1 append the given bit to the end of the string; POP deletes the last bit and is legal only when the string is non-empty. The top of a non-empty stack is its last bit.

A top-scheme is a pair (Q, E). Q maps every configuration to one of the symbols EMPTY, 0, 1, with Q(0^n) = EMPTY. E maps every pair (configuration c, operation) to a configuration c' with c <= c' (c' = c is allowed). Both are deterministic; E sees only the current configuration and the requested operation, Q only the configuration. Neither sees the history or the number of operations so far.

Play, for a fixed integer t >= 0: the scheme is fixed first and may be designed for this t. An adversary who knows the scheme and t then chooses a sequence of at most t operations, each legal for the current stack. After each operation the memory goes from its configuration c to E(c, operation). The scheme survives the sequence if after every operation Q of the configuration equals the top of the current stack when the stack is non-empty, and EMPTY when it is empty. Only the top is ever checked: different stacks may share a configuration provided no legal continuation within the remaining budget exposes the difference.

Let R(n) be the largest t such that some top-scheme survives every legal sequence of at most t operations.

(Unverified bounds. Lower: use cells in pairs, writing 01 for a pushed 0 and 10 for a pushed 1 into the first untouched pair, overwriting the rightmost pair equal to 01 or 10 with 11 on a pop; Q reads the rightmost such pair. So R(n) >= floor(n/2). Upper: PUSH0, PUSH1, PUSH0, ... and PUSH1, PUSH0, PUSH1, ... change the top at every step, so each step sets a cell; after n steps both reach 1^n with different tops, so R(n) <= n - 1.)

Questions:
1. Determine R(n) exactly for n = 1, ..., 7.
2. Determine R(n) for all n, or at least prove that rho = lim R(n)/n exists and give its value, which lies in [1/2, 1].

## WHY IT IS PROBABLY UNCHARTED

Checking only the top destroys the entropy bound: deep content matters only if enough budget remains to pop down to it, so late configurations may merge many stacks, and top-preserving operations may be free near the end. I have not seen this horizon trade-off in WOM work. Doubts: merging may gain only an additive constant, making rho equal the original lambda; generalized WOM codes for finite-state machines might cover it. By hand: R(1) = 0, R(2) = 1, unverified.

## HOW TO CHECK AN ANSWER

For fixed n and t, existence of a surviving top-scheme is a finite search: backtrack over E and Q on configurations reachable within t operations, breaking symmetry by cell permutation; feasible for n <= 7. An independent checker replays every legal sequence of at most t operations against a claimed scheme. A general claim needs an explicit scheme family and a matching impossibility proof.
