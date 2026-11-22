# Theorem 2, Step 4, written out again

Solver A's proof that G(s) is finite for every s is in
`solver-a/proofs.txt` (Theorem 2). One reviewer rejected its Step 4, reading
it as requiring the scheduler to leave certain agents alone when the rules
would not allow that. Two further instances were then asked to adjudicate.
Each was given the objection and the defence as two unattributed positions,
read the game and the proof before anything else in this folder, and worked
without seeing the other's answer. Both concluded that the step is sound and
that the objection does not apply to what the proof uses. Both are instances
of the same model family as the solver, so this is a second and third reading
within one family, not an outside check. Their reports came back to the
organiser as text; they were archived afterwards, verbatim, as `adjudication-1.md` and `adjudication-2.md`. This paragraph is the organiser's account of them.

The rewritten step below then went to three reviewers from other model
families (GPT, Grok, Gemini), among them the one that had raised the
objection. All three confirmed every part of it, and the objecting reviewer
agreed that the rewriting answers its objection. Their small corrections are
included.

The text below is the second reader's rewriting of Step 4, lightly edited for
notation. P is the hypothetical protocol that wins every planted instance;
Step 2 says that every sequence of legal moves from a population whose agents
all start in q_A or q_B is finite; Step 3 says that such a sequence on n
agents has at most c·n moves, with c depending only on P.

## Step 4 (a sequence with at least k·m moves)

Fix k, m ≥ 1. The population has n = 2k + 2m + 1 agents: k in q_A, k in
q_B, and h₁, …, h₂ₘ₊₁, where h₁ is in q_B, h₂ and h₃ in q_A, h₄ and h₅ in
q_B, and so on, alternating in pairs. We build one sequence of legal moves.

The scheduler keeps a set S_j of agents. This is bookkeeping, not part of the
game. S₀ is the first 2k agents, and S_j = S_(j−1) ∪ {h_j} for j ≥ 1. In phase j
(j = 0, …, 2m + 1), while two distinct agents of S_j, in some order, are in
states (p, q) with d(p, q) ≠ (p, q), the scheduler performs such a move.
Legality of a move depends only on the two agents' states, so every one of
these moves is legal in the whole population, and the rules never tell the
scheduler which legal move to choose. Agents outside S_j are never chosen, so
each h_j is in its initial state when it joins. Each phase ends: the moves
made so far are a sequence of legal moves from the population S_j, whose
agents all started in q_A or q_B, and Step 2 makes every such sequence
finite. Let C_j be the configuration of S_j when phase j ends. The last phase
ends with no legal move anywhere, so the whole sequence is even a complete
play.

Let α_j and β_j count the agents of S_j that started in q_A and in q_B. Then
α_j − β_j = 0, −1, 0, +1, repeating. For odd j, the starting states of S_j
are the initial configuration of a planted instance I_j:

- if j ≡ 1 (mod 4), I_j = (α_j − 1, β_j, plant q_A), with majority B;
- if j ≡ 3 (mod 4), I_j = (α_j, β_j − 1, plant q_B), with majority A.

Both have |a − b| = 2, a + b = 2k + j − 1 ≥ 2, and a, b ≥ 0, so they are
legal instances. The moves made among S_j are legal moves of I_j, so C_j is
reachable in I_j. C_j has no legal move, so it is a terminal configuration of
I_j. P wins I_j, so every agent of S_j shows the majority of I_j in C_j.

For each odd j ≤ 2m − 1, every one of the 2k + j agents of S_j shows
different outputs in C_j and C_(j+2), so each changed state in between. A
move changes at most two agents, so that stretch holds at least k + 1 moves.
No move belongs to two of these m stretches, so the sequence has at least
m·(k + 1) ≥ k·m moves.

Step 5 then compares this with Step 3: k·m ≤ c·(2k + 2m + 1), which is false
for m = k large.

## What the two readers also noted

- Step 3 says "every play" where its proof gives "every sequence of legal
  moves"; the proof is a potential argument and does not need the sequence
  to be complete.
- The original Step 4 names one stage too many (there is no agent
  h₂ₘ₊₂), says "at least k moves" where k + 1 holds, and uses the phrases
  "the scheduler forces" and "the play has", which invite the objection.
- The objection's premise is true: the inactive agents could react with one
  another. It does not matter: the scheduler is not obliged to choose them
  while two agents of S_j have a legal move, and a pair of outside agents is
  played, still from their initial states, only in a later phase that
  contains both.
- Neither reader re-derived Gordan's theorem or reviewed Theorem 1. No
  protocol satisfying the hypothesis exists, so the argument cannot be tried
  on an example, and it has not been formalised.
- Small gaps in the original that the reviewers listed: Step 2 does not say
  that the configuration graph is finite or name the instance containing a
  given pure population ((a + b + 2, b, plant q_A) works), and Step 3 does
  not treat the case of a protocol with no legal move at all, where the bound
  is trivial.
