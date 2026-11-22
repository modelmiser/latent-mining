# PROOFS (experiment 23, growth of A(n))

Notation. K_x = set of secrets agent x holds. "x knows y" means y ∈ K_x. A pair {x,y} is
DEAD if x knows y and y knows x; a call is legal iff the pair is not dead. Sets only grow,
so a dead pair stays dead. An EXPERT knows all n secrets. Two experts are always a dead pair.
A non-expert x is always active (if y ∉ K_x then {x,y} is legal).

## Theorem 1 (COMPLETE PROOF).  A(n) ≥ 3n − 6 for every n ≥ 3.

(For n = 3..6 this is the exact value. The previous general bound was 2n − 3.)

### Delayer's strategy D*

Phase 1. Name one fixed agent c in each of the first n − 2 rounds.

As in solver-a's P1: every call of phase 1 contains c, the partners l_1, l_2, ... are
distinct (a pair is never called twice) and each was untouched before its call. By induction
after j calls K_c = P_j := {c, l_1, ..., l_j}, K_{l_i} = P_i (i ≤ j), everyone else holds only
its own secret. While j < n − 1 agent c does not know everybody, so it is active and may be
named. After n − 2 calls exactly one agent p is untouched. Put m = n − 2, W = P_m = everybody
except p, P_0 = ∅. So: K_c = W, K_{l_i} = P_i, K_p = {p}, and P_0 ⊂ P_1 ⊂ ... ⊂ P_m = W.

Closure. From now on every K_x has the form P_j or P_j ∪ {p}. (True now, with K_p = P_0 ∪ {p};
the union of two sets of this form is again of this form.) Write level(x) = j and say x is
FLAGGED if p ∈ K_x, TOP if level(x) = m. After a call {x,y} both have level
max(level x, level y) and both are flagged iff one of them was. Expert = top and flagged.
Levels and flags never decrease.

Tasks. T = #{agents that are not top} + #{agents that are not flagged}. At the start of
phase 2 the non-top agents are l_1..l_{m−1} and p (m agents) and the unflagged agents are the
n − 1 members of W, so T = m + n − 1 = 2n − 3. At the end T = 0. For a call, let Δ be the
amount by which T drops. In a call at most one participant changes level (the lower one) and
at most one changes flag, so Δ ≤ 2, and Δ = 2 happens only in two ways:
 (i)  x becomes top and the other one, y, becomes flagged: before the call y was top and
      unflagged, x was flagged and not top;
 (ii) the same agent x becomes top and flagged: before the call y was an expert and x was
      neither top nor flagged.
Call such a call a DOUBLE. Summing Δ over phase 2: 2n − 3 = Σ Δ ≤ (calls) + (doubles).

Phase 2. In each round:
 R1. If no expert exists and some agent is neither top nor flagged: name such an agent z.
 R2. If no expert exists and every agent is top or flagged: name p.
 R3. If an expert exists: name an active expert.

### The rule is always applicable

R1: z is not an expert, hence active.
R2: c is top (always). No expert exists, so c is unflagged, i.e. p ∉ K_c; {c,p} is legal, p is
active.
R3: see Lemma B below.

### Stage A: while no expert exists

R1 call {z,y}. It is not of type (ii) (no expert exists) and not of type (i) (z is neither
"top unflagged" nor "flagged non-top"). So Δ ≤ 1. It creates no expert: afterwards both have
level max and flag "or"; z contributes neither top level nor the flag, so y would have had to
be top and flagged already.

R2 call {p,y}. p is flagged and not top (else p would be an expert). Now every agent is top
or flagged. If y is top it is unflagged (no experts): the call is a double of type (i) and
both become experts. If y is not top it is flagged: afterwards both are flagged and not top,
Δ = 0, no expert.

An agent that is neither top nor flagged now was so at every earlier time. Hence once R2
has been applied R1 never applies again, and from then on every agent is top or flagged.

The game ends with n experts, so some call creates the first expert. By the above it is an R2
call {p,y} with y top and unflagged; it is a double; and Stage A contains no other double.
After it: p is an expert, and every agent is an expert, or top and unflagged, or flagged and
not top.

### Stage B: after the first experts

Lemma B. In a non-terminal position of Stage B some expert is active.
Proof. If some agent x is unflagged then p ∉ K_x, {p,x} is legal and the expert p is active.
Otherwise all agents are flagged, and the non-experts are exactly the non-top agents. Suppose
no expert is active: every pair containing a current expert is dead, now and for ever. The
play continues to a terminal position whatever is done; look at the first later call in which
some agent becomes top (there is one: a non-expert exists and must become top). Its partner
is top before that call; as this is the first call in which anybody becomes top, the partner
is top already now; being flagged, it is a current expert — but pairs with current experts
are dead. Contradiction. ∎

R3 call {e,y}, e an expert. y is not an expert (two experts form a dead pair). So y is "top
and unflagged" or "flagged and not top" (no other kind exists), the call makes y an expert,
and Δ = 1 exactly. In particular it is not a double, and the description of the agents in
Stage B is preserved.

### Count

Phase 2 contains exactly one double. So (calls of phase 2) ≥ (2n − 3) − 1 = 2n − 4 whatever
Hastener answers, and the whole play has at least (n − 2) + (2n − 4) = 3n − 6 calls. ∎

Check n = 3: phase 1 is one call, T = 3, R2 gives the double, one R3 call: 3 calls.
n = 4 reproduces solver-b's Theorem A.

Machine check (COMPUTED, not part of the proof): `dcheck.cpp` plays D* against every Hastener
reply; the minimum over all plays is exactly 3n − 6 for n = 3..11 (`dcheck.log`). So this
rule cannot give more than 3n − 6; the computed A(n) exceeds 3n − 6 from n = 7 on.

## Lemma 2 (COMPLETE, a restatement of solver-b's Lemma 3 in the form used below)

If some expert z is known by no non-expert, exactly u more calls are made under Hastener's
rule "non-expert named → answer z; expert named → any legal answer", u = number of
non-experts. (Each call then turns exactly one non-expert into an expert, and only experts
ever know z.)

## Upper bound: NOTHING PROVED

No bound of the form A(n) ≤ c·n and no bound o(n²) is proved here. What follows are
negative results about natural Hastener rules; they are SKETCHES plus computations, recorded
so that nobody repeats them.

S1 (SKETCH + COMPUTED n ≤ 8). "Lift to the top": keep a set M and the agents knowing all of
M, always answer with the most knowledgeable legal partner. Delayer: name a top agent whom
all touched agents know (forces a call with an untouched agent, which demotes every other top
agent), then name the demoted agents one by one. Each of them is lifted again after every
newcomer: about n²/2 calls. Computed: policies 1 and 3 of `g.cpp` give 10, 15, 21, 28 =
n(n−1)/2 for n = 5..8 (`hast.log`).

S2 (SKETCH, not computed). "First fit" (answer with the lowest-numbered legal partner):
Delayer names t = n/2 high-numbered agents in turn, round after round; all of them call 1,
then all call 2, ...; every one of these t·(n−t) ≈ n²/4 calls is legal because the j-th
collector is untouched when the first of them reaches it.

S3 (COMPUTED). Greedy rules: "least-known legal partner, then largest union" gives 9, 13,
17, 22, 26 for n = 5..9 against an optimal Delayer (A(n) = 9, 12, 16, 19, 23); "largest
union, then least-known" gives 9, 13, 18, 23 for n = 5..8. Neither is optimal; the data are
too short to tell whether they are linear.

Why the obvious invariant fails: by Lemma 2 Hastener needs only to produce an expert whom no
non-expert knows, and the only cheap source of such an agent is an untouched one. Delayer's
counter is the clique: a set of agents who all hold the same knowledge is pairwise dead, and
each member needs its own call with an outsider for every batch of outside secrets.
