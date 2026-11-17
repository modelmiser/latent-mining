## KNOWN

With no feedback (d infinite) this is the theory of deletion-correcting codes: Levenshtein showed the Varshamov-Tenengolts codes correct one deletion with about log2(n+1) bits of redundancy, which is asymptotically optimal; exact maximum code sizes are known only for small lengths, and for t >= 2 even the asymptotic redundancy constant is open. With d = 0 the sender sees each deletion at once and resends, so N(k, t, 0) = k + t (the adversary can always delete the last t bits, so the receiver must tell messages apart from N - t bits). Interactive coding for synchronisation errors and coding with noiseless feedback for bit flips exist as fields; I am unsure whether exact results for delayed count-feedback with deletions exist. The seed as stated has three parameters and no single answer; it is a programme, not a question.

## SUCCESSOR QUESTION

Fix n >= 1. A sender transmits exactly n bits x_1, ..., x_n, one per step. An adversary who knows the message, the sender's strategy and everything sent may delete at most one of the n bits. The receiver ends with the string of surviving bits in order (length n or n-1), with no mark of where, or whether, a deletion happened, and no timing information. Feedback is one step late: before choosing x_i the sender knows, for each j <= i-2, whether x_j was delivered; it knows nothing about x_{i-1}. So a deletion of x_i with i <= n-2 is learned just in time to choose x_{i+2}, ..., x_n freely, and a deletion of x_{n-1} or x_n is never learned in time.

Equivalently: a strategy for one message is a word w in {0,1}^n together with, for each i in {1, ..., n-2}, a suffix s_i in {0,1}^{n-i-1}. Its outcome set is

  { w } ∪ { w with bit n-1 removed, w with bit n removed } ∪ { w_1 ... w_{i-1} w_{i+1} s_i : 1 <= i <= n-2 }.

Let M(n) be the largest number of strategies whose outcome sets are pairwise disjoint (the largest message set the receiver can always decode).

Determine M(n). Concretely: (1) give the exact values M(1), ..., M(7); (2) give a formula or a tight characterisation for all n; (3) decide whether M(n)/2^n tends to 0 or stays bounded below by a positive constant, and find the limit if it exists.

Calibration: M(n) <= 2^(n-1) always (the adversary deletes x_n). M(2) = 2 and M(3) = 3, whereas the best one-deletion code without feedback has 2 words at length 3 and instant feedback would give 4.

## WHY IT IS PROBABLY UNCHARTED

The two ends are classical (VT codes; resend), but the one-step-late middle is a new finite combinatorial object: an adaptive code whose last two positions must be deletion-proof on their own while earlier damage is repaired blindly, since the receiver never learns where repair began. Already at n = 3 the answer lies strictly between both ends. Doubts: feedback-deletion papers I do not know may cover delayed feedback asymptotically; the sequence might turn out to be something simple such as a shifted known sequence, and question (3) might have a short argument.

## HOW TO CHECK AN ANSWER

For n <= 6 (perhaps 7) enumerate all strategies (2^n words times 2^((n-1)(n-2)/2) suffix choices), compute outcome sets, and solve the maximum set-packing exactly by integer programming, SAT or branch and bound with symmetry reduction (complement, and pruning strategies whose outcome sets contain another's). Compare with the claimed values. A claimed formula needs an explicit strategy family achieving it plus an upper-bound proof, e.g. a weighting or counting argument on the length-(n-1) outcomes; a claimed limit can be probed against the computed ratios M(n)/2^n.
