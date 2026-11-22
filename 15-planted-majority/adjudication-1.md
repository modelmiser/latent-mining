<!-- Archived by the organiser on 2026-11-21 from the session transcript. This is the final report of the first of the two instances asked to adjudicate Step 4 of Theorem 2, verbatim as it was returned as text (the harness did not let sub-instances write report files), except that absolute paths are shortened: (staging)/ is the instance's working folder, (scratch)/ a temporary folder. Statements in it reflect what the instance knew when it wrote; later corrections are on the experiment's README. -->

**report.md was NOT written.** The Write tool refused it ("Subagents should return findings as text, not write report files") and I did not route around the block. The full report is below; save it to `(staging)/report.md` if you want the file. Nothing under (workspace) was modified, and no script was run.

---

# Referee report: Theorem 2, Step 4 (15-planted-majority, solver-a)

**Verdict: Position Q is right. Theorem 2 is proved.** Position P's premise is true but is aimed at something the proof never uses. I reached this from 3-question.txt and proofs.txt only; README.md and reports/ were not read, before or after.

## A. The staged sequence is a legitimate object
- **Legality is local.** By 3-question.txt line 5, a move is legal iff d(p, q) != (p, q) for the two chosen agents' states. Every staged move is therefore a legal move of the whole population.
- **Maximality.** A complete play may not stop while a legal move exists, so the staged sequence is in general a prefix of a play, not a complete one. P is right about that, and it does not matter.
- **Where winning is used.** Winning is defined through R (configurations reachable by legal moves) and nothing else. Step 4 applies it only to instances whose entire population is the active set. The big population is never claimed to be a won instance or to be terminal.
- **Step 3 covers arbitrary sequences.** Its statement says "every play", but its proof is a potential argument: Phi drops by at least delta per legal move and stays within n*[min r, max r] over Q0. That bounds any finite sequence of legal moves from a pure population.
- **Extension.** If wanted, the staged sequence extends to a complete play, since by Step 2 every such sequence is finite. Extension only adds moves.

## B. The odd-stage instances are right
- **Legal instances.** For j = 1, 5, 9, ... beta_j = alpha_j + 1, and (alpha_j - 1, beta_j, plant q_A) has b - a = 2, majority B. For j = 3, 7, ... (alpha_j, beta_j - 1, plant q_B) has a - b = 2, majority A. In both, a + b = 2k + j - 1 >= 2, the plant is in Q, and alpha_j, beta_j >= k >= 1.
- **Reachable.** Every move so far was between agents inside the stage-j active set, which started as exactly the instance's initial configuration. So the stuck configuration is in R of that instance.
- **Terminal.** The instance's population is the active set, so "no two active agents have a legal move" is literally "no legal move" there.
- **Good.** P wins the instance, so every active agent shows its majority at that moment.

## C. The count follows
- The 2k + j >= 2k + 1 agents active at odd stage j all show one letter at stage j's stuck configuration and the other at stage j + 2's.
- Each must change state in between, and a move changes at most two agents, so each interval has at least k + 1 moves.
- The intervals j = 1, 3, ..., 2m - 1 are disjoint and number m: at least k*m legal moves from a pure population of n = 2k + 2m + 1.
- With m = k, Step 3 gives k^2 <= c(4k + 1), false for k > 5c.

## D. Rest of Steps 1 to 5
I found no gap.
- **Reduction to one protocol.** Pigeonhole plus downward closure is fine.
- **Step 1.** I' = (a, a + 2, x) is a legal instance containing I, and C is non-empty. Sound.
- **Step 2.** aA + bB sits inside a legal instance; a finite acyclic graph has only finite plays. Sound.
- **Step 3, Q0.** Closed under d, because disjoint pure populations can run side by side (this also covers p = q).
- **Step 3, zero-sum combination.** The union of private populations is reachable from a pure population. Firing each copy on its private pair is legal and returns the same multiset: a closed walk of at least one move, which contains a cycle. A swap (D_t = 0) is the case y = e_t, a self-loop.
- **Step 3, Gordan.** Correctly applied; T is finite, so delta > 0.
- **Step 3, scope.** The bound holds for every population starting in q_A and q_B only, which is what Step 4 uses.

Wording nits, none affecting correctness:
1. Step 3 should say "every sequence of legal moves", not "every play".
2. "Stage j = 0..2m+1 ... then activate h_{j+1}" overruns by one; there is no h_{2m+2}.
3. "The scheduler forces" and "the play has" invite exactly P's misreading.
4. "At least k moves" could be k + 1.

## E. Rewritten Step 4

**Step 4 (a long sequence of legal moves).** Fix k, m >= 1. Take n = 2k + 2m + 1 agents: k in q_A, k in q_B, and h_1, ..., h_{2m+1} in states q_B, q_A, q_A, q_B, q_B, q_A, q_A, ... (after h_1, alternately two q_A, two q_B). We build one sequence S of legal moves from this configuration. S need not be a complete play; Step 5 needs only that every move in it is legal.

Let V_0 be the first 2k agents and V_j = V_0 + {h_1, ..., h_j}. Build S in rounds j = 0, 1, ..., 2m + 1. In round j, append legal moves whose two agents both lie in V_j, until no two agents of V_j have a legal move. This stops by Step 2, since V_j started as a pure population and only agents of V_j have moved. Call the configuration of V_j at that point Z_j. Agents outside V_j have never moved, so h_{j+1} enters round j + 1 in its original state.

Let j be odd. V_j started with alpha q_A's and beta q_B's, where beta = alpha + 1 if j = 1 mod 4 and alpha = beta + 1 if j = 3 mod 4. In the first case V_j's starting configuration is the initial configuration of the planted instance (alpha - 1, beta, plant q_A), with majority B. In the second it is that of (alpha, beta - 1, plant q_B), with majority A. Both are legal instances: the counts differ by 2 and sum to 2k + j - 1 >= 2. All moves so far are legal moves of that instance, so Z_j is reachable in it, and Z_j has no legal move. P wins the instance, so Z_j is good: every agent of V_j outputs B if j = 1 mod 4 and A if j = 3 mod 4.

Hence between Z_j and Z_{j+2} (j odd, j + 2 <= 2m + 1) each of the 2k + j agents of V_j changes output, so changes state. A move changes at most two agents, so that stretch of S has more than k moves. There are m such disjoint stretches (j = 1, 3, ..., 2m - 1), so S has at least k*m moves.

## Not checked
- Gordan's theorem and the real-to-rational-to-integer step were taken as standard, not re-derived.
- No script was run; everything is by hand.
- Lemmas R and M, Theorem 1 and its corollaries were not reviewed; Theorem 2 does not depend on them.
- README.md and reports/ were not read.
- Word counts (report under 900, Step 4 under 350) are my estimate; the count command had no file to run on. The report may run slightly over 900.
