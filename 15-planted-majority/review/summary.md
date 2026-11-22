<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-222931 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.558; this month $198.21 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.757, input 32 + cache write 74,872 + cache read 645,059 tokens — cache hit 90%
- claude:claude-opus-5-5: $0.801, input 24 + cache write 53,032 + cache read 273,571 tokens — cache hit 84%
- **claude:claude-fable-5-1 produced no answer:** empty

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q2** (split) | — | REFUTED | REFUTED | REFUTED | CONFIRMED |
| **Q3** (split) | — | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q4** (split) | — | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q5** (split) | — | REFUTED | REFUTED | REFUTED | CONFIRMED |
| **Q6** (split) | — | REFUTED | REFUTED | REFUTED | CONFIRMED |
| **Q8** (split) | — | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | — | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | — | REFUTED | REFUTED | REFUTED | REFUTED |

Disagreements (read these first): Q2, Q3, Q4, Q5, Q6, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 1 CRITICAL, 10 MODERATE, 25 LOW
- **CRITICAL** [cursor:grok-4.7-medium] `solver-a/proofs.txt:107-110` — Theorem 2’s staged scheduler is not a legal play: after the active set sticks, inactive `q_A`/`q_B` pairs can still be legal, and the scheduler must take one, so later agents are not added in their original states and the monochromatic stages need not occur. Headline “`G(s)` is finite” is unproved. — high — add an argument that the next agent always has a cross move, or replace Step 4.
- **MODERATE** [claude:claude-opus-5-5] solver-a/proofs.txt:75 — the identity e_j−j′ = (M−1)M+(2M−j−j′) is false for every case with j+j′>M (M=3: 5 ≠ 8). The README calls the cubic bound "proved by hand", and the referee "read it" and missed this. — high — replace with (M−2)M+(2M−j−j′) (a zero summand when M=2).
- **MODERATE** [claude:claude-opus-5-5] README:162-164 — process notes are attributed to solver A with no source in the repository. — high — remove them or add the source.
- **MODERATE** [claude:claude-opus-5-5] README:57 — G(3) "in the model as stated" by both solvers; solver A's ordered search timed out (solver-a.md:146). — high — use the attribution given in Q5.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/solver-a/proofs.txt:104` — empty T makes δ undefined — confidence: high — dispose of T=∅ first
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/README.md:36-37` — G(2)=1 is undefined under the abbreviated definition — confidence: high — restore both conventions
- **MODERATE** [cursor:grok-4.7-medium] `solver-a/proofs.txt:75` — `e_j−j'=(M−1)M+(2M−j−j')` is larger than the difference by `M` whenever `j+j'>M`. Closure still holds with `(M−2)M`. — high — correct the summand.
- **MODERATE** [cursor:grok-4.7-medium] `15-planted-majority/README.md:89-91` — the stuck-configuration count drops the weightless agent and treats `a+b+1` as `<K`, so the boundary `a+b=K−1` does not contradict `W≥K`. The proof file’s count is the one that works. — high — state `W≤a+b≤K−1`.
- **MODERATE** [cursor:grok-4.7-medium] `15-planted-majority/README.md:54` — solver B does not prove `G(4M−1)≥M²−1`; monotonicity from `G(4m)` does not recover it. — high — attribute that bound only to solver A.
- **MODERATE** [cursor:grok-4.7-medium] `15-planted-majority/README.md:57` — solver A did not finish the ordered-model search for `G(3)`. — high — see the Q5 replacement.
- **MODERATE** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-b/proofs.txt:59-60 — Solver B claims quadratic growth is the limit of the construction because |W| is at least order sqrt(K), but Corollary 1b's cubic family satisfies B's conditions (C) and (S) with U=1 — confidence: high — retract the remark claiming an Omega(s^2) barrier
- **LOW** [claude:claude-opus-5-5] README:89-91 — "number of agents … less than K" is false (a+b+1 can equal K); the argument needs the token count. — high — "number of other agents, a+b".
- **LOW** [claude:claude-opus-5-5] README:79 — under 1′ the weightless agent always shows A, and in Theorem 1 a tie gives (wa,wb). — high — "becomes weightless (in Theorem 1′ the single weightless state shows A)".
- **LOW** [claude:claude-opus-5-5] README:54 — "both solvers" for G(4M−1): solver B has G(4M). — high — say so.
- **LOW** [claude:claude-opus-5-5] README:113-114 — "by a fixed amount" should be "by at least a fixed amount"; the c·n bound holds only from pure populations. — medium — add both qualifiers.
- **LOW** [claude:claude-opus-5-5] README:108 — "add agents in q_B" covers only an A-majority instance. — high — "add agents of the minority's starting state".
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:110 — stage 2m+1 activates the nonexistent h_{2m+2}; :118 should say k+1 moves, not k (referee.md:32). — high — "stages 0..2m, then play until stuck".
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:41 — "whichever case of (net) applies" is ambiguous when several cases apply; any choice works. — high — "the first case that applies".
- **LOW** [claude:claude-opus-5-5] README:31-34 — swap self-loops are not stated to count as cycles, yet Lemma R depends on that. — high — add the sentence given under Q8.
- **LOW** [claude:claude-opus-5-5] README:152 — calling the CONCUR 2026 paper a "sequel" is an interpretation; its author list differs (prior-art.md:64-65). — medium — "a related 2026 paper".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/solver-a/proofs.txt:66,72-75` — boundary identities silently use 0 or undefined e_M — confidence: high — state parameter ranges and boundary cases
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/solver-a/proofs.txt:109-110` — final stage activates h beyond its range — confidence: high — condition activation on j<2m+1
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/README.md:45-46` — “every difference” includes zero unless u>v is stated — confidence: high — quantify u>v in S
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/README.md:90-91,107-109` — two explanatory sentences are literally false at boundary/opposite majority — confidence: high — use the Q6 wording
- **LOW** [cursor:gpt-5.6-sol-xhigh] `15-planted-majority/README.md:54,57,130,162-164` — exact-bound, model, panel, and process attributions exceed their sources — confidence: high — narrow or remove them
- **LOW** [cursor:grok-4.7-medium] `15-planted-majority/README.md:11` and top-level `README.md:46` — “population size” is `a+b+1`, while `G` is defined as `a+b` at `README.md:36-37` and `chain/3-question.txt:9`. — high — call `G` the largest `a+b`.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:66` — `(M−1)M=(M−2)M+M` is not a split for `M=2` (zero summand). `2=1+1` still splits. — high — give the `M=2` split separately.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:71` — for `M=1`, `{M²−j:0≤j≤M}` contains `0`, which Theorem 1 excludes. — high — state `M≥2`.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:67` — for `j=M` and `c≥2`, `cM−j=(c−1)M` is in `S`, but the parenthetical only allows the first summand to be zero. — high — allow either summand to be zero.
- **LOW** [cursor:grok-4.7-medium] `15-planted-majority/README.md:162-164` — solver A’s crashes and “three processes for a minute” do not appear in `reports/solver-a.md`. — high — cite a source or delete.
- **LOW** [cursor:grok-4.7-medium] `15-planted-majority/README.md:151-153` — the 2026 paper’s authors are Czerner, Esparza, Fischer, Guttenberg, Pins, Reilich (`prior-art.md:8`), not a sequel under the 2024 Lossin et al. byline. — high — name item 2 separately.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:118` — each block changes at least `2k+1` outputs, hence at least `k+1` moves, not `k`. The weaker bound still contradicts for large `k`. — high — write `k+1`.
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/README.md:162-164 — README falsely attributes to Solver A reporting three processes running for about a minute and four early crashes on comment lines, which is absent from reports/solver-a.md — confidence: high — remove the attribution to Solver A or state that Solver A reported no process violations
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-b/proofs.txt:58 — Solver B lists a 10-element closed set with max W = 36, conflicting with reports/solver-b.md:23 (which lists 40) and the true maximum 42 — confidence: high — harmonize the maximum for |W|=10 to 42 across files
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-a/proofs.txt:72 — Split identity (M-1)M = (M-2)M + M fails at M=2 because (M-2)M = 0 is not in S — confidence: high — restrict the identity to M >= 3 and note that for M=2 the element (M-1)M = 2 splits as 1+1
- **LOW** [cursor:gemini-3.8-flash-high] 15-planted-majority/solver-a/proofs.txt:118 — Move count states >= k moves between stage j and j+2, but since >= 2k+1 agents change output and each move changes at most 2, it forces >= ceil((2k+1)/2) = k+1 moves — confidence: high — replace 'at least k moves' with 'at least k+1 moves'

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
