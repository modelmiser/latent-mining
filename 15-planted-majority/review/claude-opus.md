<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. P_S has a rule for every token pair: proofs.txt:41-43 says "in both orders", and the (+u,−v) rules always change the pair. Splits exist because of (split). Every move lowers (Σ|w|, #weightless): netting by 2v, 2v or 2c (0<c<v keeps the result a −token), and a split lowers the second count by one (:47). W ≥ 2K−K holds for every plant, including −K, weightless and +K (:49). A +token and a −token always have a legal move, so a stuck configuration has one sign. The token count ≤ (a+b+1)−1 holds because the wb agent is not a token. Goodness covers every agent, the plant included. For Theorem 1′ (:58-61), majority A ends with +tokens and n (n shows A), which is good. Majority B with n present forces weight-1 −tokens, giving K ≤ a+b, a contradiction. I re-ran 1′ by brute force on S={1,2,4} and {1,2,3,6}: they win up to 3 and 5. — none

Q2: REFUTED (one identity is false; the claim still holds). proofs.txt:75 says "e_j − j′ = (M−1)M + (2M−j−j′)" when j+j′>M. That sum equals M²+M−j−j′. Counterexample: M=3, j=1, j′=3 gives e_1−3 = 5 but the formula gives 8. Every other listed identity checks out, including the boundary cases j=M, c′=M, c=1 and zero summands. I checked closedness (two-condition definition) and |S|=4M−3, max = M³ by program for M=2..20. For M=3 the set {1,2,3,6,7,8,9,18,27} is closed, and the 19-state protocol wins up to N=9 (I enumerated no further). — replacement: "(M−2)M + (2M−j−j′)"

Q3: CONFIRMED. The sub-population principle holds because legality depends only on the two states, and the scheduler may pick any legal move (:5-8). I′=(a,a+2,x) is a legal instance, and initial(I)+E is its start (:88-91). Q0 is closed under d: run two pure populations side by side (:95-98). Firing y_t private copies returns to C, which is a cycle; swaps give D_t=0 and are covered. Gordan's theorem is applied correctly: there is no y≥0, y≠0 with yᵀD=0, so some r has Dr<0 componentwise, all t at once. Every state stays in Q0, so Φ ∈ [n·min, n·max], which gives ≤ c·n moves. — none

Q4: CONFIRMED. The scheduler moves only among active agents, which is legal, and inactive agents stay in qA/qB. The differences 0,−1,0,+1,… are correct. The two instances are (α, β−1, plant qB) and (α−1, β, plant qA), with margin 2 and a+b = 2k+j−1 ≥ 2. Stuck active agents form a terminal configuration of that instance. So 2k+j agents each change state, which needs ≥ k+1 moves over m intervals. k² ≤ c(4k+1) fails for k > 5c. — Minor: stage 2m+1 "activates h_{2m+2}", which does not exist (:109-110).

Q5: REFUTED (README wording; the lemma itself holds). Lemma R (:16-20) is sound: each P′ edge is a P edge on multisets, and a P′-terminal configuration has only swaps or nothing in P. But README:57 says G(3) was "computed by both solvers … in the model as stated". Solver A's ordered-model search "timed out for s=3, N=4, so that cross-check is missing" (solver-a.md:146). I independently confirmed G(2)=1 by brute force over all 2048 protocols. — replacement: "G(3): ordered model by solver B and the referee; solver A in the reduced model only"

Q6: REFUTED. README:90-91 says the weight is "at most the number of agents, which is less than K when a+b ≤ K−1". But the number of agents is a+b+1 ≤ K, which is not less than K. The proof uses tokens ≤ a+b (proofs.txt:52, 61). README:79 says a weightless agent shows "the opinion of the token that survived". That is false under 1′ (always A) and undefined on a tie. README:108 says "add q_B" without the mirror case. README:113 says lowered "by a fixed amount" where it should be "at least". README:114 says "a play on n agents" where it should be from a pure population. — replacement: "at most the number of other agents, a+b, which is less than K"

Q7: REFUTED. README:162-164 attributes process notes to solver A ("three … for about a minute", "four early crashes … comment line"). Neither reports/solver-a.md nor solver-a/report_copy.md contains them, and grep finds no source. README:54 credits "both solvers" for G(4M−1), but solver B proves G(4m) ≥ m²−1 (solver-b.md:3). The other cells match: referee.md:11-19, 25, 39, 44-47 and prior-art.md:63-87. The two-condition definition is consistent: referee.md:45 shows no set needs the third option. — replacement: drop the solver A process sentence or cite its source; "solver B proves G(4M) ≥ M²−1"

Q8: CONFIRMED. ../README.md:46 matches the folder. "The game" matches 3-question.txt except for small omissions: self-loops and swaps count as cycles, a,b ≥ 0, and the G=1/∞ conventions. — Add "(a swap, which leaves the multiset unchanged, counts as a cycle)".

**Findings**

F: MODERATE — solver-a/proofs.txt:75 — the identity e_j−j′ = (M−1)M+(2M−j−j′) is false for every case with j+j′>M (M=3: 5 ≠ 8). The README calls the cubic bound "proved by hand", and the referee "read it" and missed this. — high — replace with (M−2)M+(2M−j−j′) (a zero summand when M=2).

F: MODERATE — README:162-164 — process notes are attributed to solver A with no source in the repository. — high — remove them or add the source.

F: MODERATE — README:57 — G(3) "in the model as stated" by both solvers; solver A's ordered search timed out (solver-a.md:146). — high — use the attribution given in Q5.

F: LOW — README:89-91 — "number of agents … less than K" is false (a+b+1 can equal K); the argument needs the token count. — high — "number of other agents, a+b".

F: LOW — README:79 — under 1′ the weightless agent always shows A, and in Theorem 1 a tie gives (wa,wb). — high — "becomes weightless (in Theorem 1′ the single weightless state shows A)".

F: LOW — README:54 — "both solvers" for G(4M−1): solver B has G(4M). — high — say so.

F: LOW — README:113-114 — "by a fixed amount" should be "by at least a fixed amount"; the c·n bound holds only from pure populations. — medium — add both qualifiers.

F: LOW — README:108 — "add agents in q_B" covers only an A-majority instance. — high — "add agents of the minority's starting state".

F: LOW — solver-a/proofs.txt:110 — stage 2m+1 activates the nonexistent h_{2m+2}; :118 should say k+1 moves, not k (referee.md:32). — high — "stages 0..2m, then play until stuck".

F: LOW — solver-a/proofs.txt:41 — "whichever case of (net) applies" is ambiguous when several cases apply; any choice works. — high — "the first case that applies".

F: LOW — README:31-34 — swap self-loops are not stated to count as cycles, yet Lemma R depends on that. — high — add the sentence given under Q8.

F: LOW — README:152 — calling the CONCUR 2026 paper a "sequel" is an interpretation; its author list differs (prior-art.md:64-65). — medium — "a related 2026 paper".

**Not asked about**

- None of the "proved by hand" theorems is mechanised. Theorem 2 looks correct to me after a full trace.
- The referee's closure check covers only M ≤ 12. My own check extends that to M ≤ 20, and the corrected identity covers all M.
