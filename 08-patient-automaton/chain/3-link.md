## KNOWN

Perfect-information synchronization games on automata (two players alternate letters, one wants the image of Q to become a single state) have been studied; I believe the authors are Fominykh, Martyugin and Volkov, and that deciding the winner is polynomial, but I am unsure of details. Games where one player sees the other late ("delay games") are a known area; I recall bounded lookahead sufficing for regular conditions, but that compares finite lookaheads with each other, not with full clairvoyance, which is what "blind" gives the Spoiler here.

The question as given is well posed. Its weakness is breadth: three parts, several tables, and s(n) may simply be infinite, which no enumeration over d can show. For fixed n the chain W_d(n) stabilises, but possibly strictly above W_inf(n). That gap is the real question. The sanity check s(2) = 0 looks right.

## FINAL QUESTION

Objects. A is a complete deterministic finite automaton with state set Q = {1, ..., n} and two letters a, b; each letter is an arbitrary map Q -> Q, so there are n^(2n) labelled automata. For a word v, Q·v is the image of Q under v (letters applied left to right). Fix an integer d >= 0 (the delay).

Play. In rounds t = 1, 2, 3, ... the Synchronizer chooses a letter x_t, then the Spoiler chooses a letter y_t. The played word is x_1 y_1 x_2 y_2 ....

Information. Both players know A and d. The Synchronizer's strategy is deterministic and x_t may depend only on x_1, ..., x_(t-1) and y_1, ..., y_(t-1-d) (on no Spoiler letter while t <= d+1). The Spoiler knows that strategy and sees every letter played so far.

Winning. A is d-winnable if some Synchronizer strategy and some number T give |Q·(x_1 y_1 ... y_(T-1) x_T)| = 1 against every Spoiler play. A is blind-winnable if one fixed word x_1 ... x_T, chosen before play, has that property for all 2^(T-1) choices of y_1 ... y_(T-1). Write W_d(n) and W_inf(n) for the sets of d-winnable and blind-winnable automata on Q; then W_0(n) ⊇ W_1(n) ⊇ ... ⊇ W_inf(n). Call A a patient automaton if it is d-winnable for every d >= 0 but not blind-winnable. Let s(n) be the least d with W_d(n) = W_inf(n), and s(n) = infinity if no such d exists (equivalently, if a patient automaton with n states exists).

HEADLINE (yes/no): Does a patient automaton exist for some n?
If yes: find the least such n, give one explicit transition table, and give the Synchronizer's strategy as a function of d.
If no: find s(n) exactly as a function of n, with a family of automata that are (s(n)-1)-winnable but not blind-winnable.

Small instances (required with either answer): the table of |W_d(n)| for n = 3, 4 and d = 0, 1, ..., 6, together with |W_inf(n)|, and the first d (if any, within that range) at which the two numbers agree.

UNVERIFIED beliefs.
- s(2) = 0: on two states a non-constant map is a permutation, so winnable at any delay forces a constant letter, which alone is a blind winning word.
- A uniform T costs nothing: the game tree is finitely branching, so a strategy that wins every play wins within a bounded number of rounds.
- If a patient automaton exists it has at least 3 states, and W_0(n) ≠ W_inf(n) for some n <= 4.
- If s(n) is finite it is at most exponential in n (pumping on the family of reachable images).

GUESS: I would bet yes, a patient automaton exists with fewer than ten states, because the Spoiler's clairvoyant advantage in the blind game can hinge on one early choice that the Synchronizer can outwait at any finite delay if the automaton offers a harmless waiting letter; the interleaved Spoiler letters are what could make this false.

## WHY IT IS PROBABLY UNCHARTED

Synchronization games are usually studied with perfect information, and delay games usually give the lookahead to the reacting player and compare finite delays. Here the delayed player is the one building a reset word, the limit case is a classical object (a word that synchronizes against every interleaving), and the question is whether finite delay ever differs from it in the limit. Doubts: a general delay-game theorem may settle it as a corollary once the winning condition is recognised as a safety condition for the Spoiler, and the two-letter, shared-alphabet restriction may make small cases degenerate.

## HOW TO CHECK AN ANSWER

Fixed A and d: solve a perfect-information reachability game. A position is (current image S ⊆ Q, queue of d letters the Synchronizer has already committed); the Synchronizer first commits x_1 ... x_(d+1), then each round the next queued letter is applied, the Spoiler applies y, and the Synchronizer appends one letter. He wins iff he forces |S| = 1; at most 2^n · 2^d positions. Blind: breadth-first search over families of images {Q·w}, one letter x at a time, until every member is a singleton or a family repeats. Enumerate all n^(2n) tables for n = 3, 4.

A "yes" needs a strategy proved correct for every d, plus the blind search failing. A "no" needs an upper bound proof and the matching family.
