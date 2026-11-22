<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-055550 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.805; this month $112.62 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.143, input 28 + cache write 62,230 + cache read 357,789 tokens — cache hit 85%
- claude:claude-opus-5-5: $0.662, input 22 + cache write 48,972 + cache read 211,451 tokens — cache hit 81%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q4** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q7** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| **Q8** (split) | CONFIRMED | REFUTED | CONFIRMED | REFUTED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q7, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 6 MODERATE, 35 LOW
- **MODERATE** [claude:claude-opus-5-5] README.md:39-40 — "three independent programs" overstates independence: all three use the same reversed-input plus dominance-reduction method for n = 6, 7, and only one of them works from the definition (n ≤ 5) — medium — reword as in Q8.
- **MODERATE** [claude:claude-opus-5-5] solver-b/proofs.txt:5 — the cited base run "slow.py 4 9 up" has no output in the repository; the base is supported only by fast output (runup.txt) and the referee's brute force — high — cite those logs instead.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `prior-art.md:22,37-39` — “EASY COROLLARY” is asserted although the key identification and cited lemma were not verified from the source — confidence: high — replace with “likely a corollary, pending source verification”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:5` — claims an all-input slow.py run whose output is absent; other logs still establish the base — confidence: high — retain the slow-run log or remove that provenance claim
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/proofs.txt:39-45` — L2(c) is false: `[1,4,5,2,6]` is 2-low, and `I_6` on `y = 3` yields `[1,2,4,5,3,6]`, neither sorted nor 2-low — high — replace the `y > x` case; the same `x` need not survive
- **MODERATE** [cursor:grok-4.7-medium] `README.md:61-66` — idle-letter existence is asserted without deletion-proofness; length-N insertion sort is a counterexample — high — use the Q7 replacement
- **LOW** [claude:claude-fable-5-1] `solver-b/proofs.txt:5` — cites `slow.py 4 9 up` as evidence for u(4)=9 but no slow.py output exists in `solver-b/`; base rests on fast (reversed input only) and the verifier's 0-1 enumeration — high — cite runup.txt:5-6 and verifier/out_lemmas.txt:31-40.
- **LOW** [claude:claude-fable-5-1] `solver-a/proofs.txt:53` — T3(i) omits x=1^m (needs a zero to be "c=0") — high — add the one-line case.
- **LOW** [claude:claude-fable-5-1] `README.md:36` — "resting on one computed base case" omits that n=3 in this route is the hand check L3, not L4 (which gives n≥4 only, `solver-b/proofs.txt:97`) — high — "resting on one computed base case (n = 4) and a hand check for n = 3".
- **LOW** [claude:claude-fable-5-1] `README.md:34` — "tested it word by word for n ≤ 5" overstates `referee.md:24` (n=5 only at size 14) — high — as above.
- **LOW** [claude:claude-fable-5-1] `README.md:39-40` — "independent" programs share design and model family; the README's own distrust section concedes the latter — medium — "separately written, same design".
- **LOW** [claude:claude-fable-5-1] `README.md:9-10` — "Surviving a slip … same as surviving the removal" unqualified; false at n=2 (`out_lemmas.txt:8`) — high — prefix "For n ≥ 3".
- **LOW** [claude:claude-fable-5-1] `README.md:56` — "the last comparator c_(n−1)" reads as positional — medium — "letters c_(n−1)".
- **LOW** [claude:claude-fable-5-1] `solver-b/proofs.txt:107` — stale "n = 3..6" vs `run7dom.txt:3,13` (already noted in referee.md:49) — high — "n = 3..7".
- **LOW** [claude:claude-fable-5-1] `solver-a/out_cross.txt:1-12` — paired runs are unlabeled, so "reduction off" vs "on" (`reports/solver-a.md:28`) cannot be told apart from the log — medium — label runs.
- **LOW** [claude:claude-opus-5-5] README.md:55-56 — "upward slips" means c_i→c_{i+1}, which is one wire down in the README's own geometry (L47-48) — high — define the slip direction explicitly.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:50-53 — T3(i) misses x = 1^m — high — add the one-line case.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:58 — "before it" should be "before D_k" — high — reword.
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:42 — the distinctness of the q_v is unjustified (it is true) — high — add the reason from Q3.
- **LOW** [claude:claude-opus-5-5] solver-b/proofs.txt:101-104 — L5 omits that deleting a wasted letter leaves w* unchanged, and it shares L4's decomposition — medium — add that sentence, and describe L5 as "independent of T2".
- **LOW** [claude:claude-opus-5-5] README.md:72-78 — the paragraph leaves out that the deletion fault model is Yao–Yao's passive fault (prior-art.md:8), so a reader may think A(n) is a new notion — medium — mention Yao–Yao.
- **LOW** [claude:claude-opus-5-5] reports/referee.md:51 — the referee says A has no size-26 run for n = 7, but out_n7_m0.txt shows level 26 with no GOAL, which already settles it — medium — correct the referee note or the README's reliance on it.
- **LOW** [claude:claude-opus-5-5] README.md:34 — "tested word by word for n ≤ 5" covers only certain lengths (referee.md:24) — high — state the ranges.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:53,57` — all-ones vectors are omitted from two T3 cases — confidence: high — state that comparators fix 1^m
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:36,55` — “upward” names the physically downward c_i→c_{i+1} slip — confidence: high — rename it “downward”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:10-13` — s_0 is used but undefined — confidence: high — define s_0=0
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:108` — stale range says n=3..6 although n=7 is logged — confidence: high — change to n=3..7
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:24` — “v c u x” reverses the repository’s word notation — confidence: high — write “u c v acts on x”
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:49-53` — (i) skips `x = 1^m`, and (iii) never says a sorted vector is idle — high — add the Q4 sentence
- **LOW** [cursor:grok-4.7-medium] `verifier/lemmas.py:150-154` — `xlow` is existential, so the 0 failures in `out_lemmas.txt:50-54` do not test L2(c) — high — track the same displaced value
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:17-18` — (d) is proved only on 0-1 inputs; the lift is L0 at line 8, which B states and A does not — high — add "hence all inputs, by L0"
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:42` — "`q_v` are distinct" has no sentence; one letter holds one pair — high — add that clause
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:108` — count equality is written "n = 3..6" while `run7dom.txt:3` and `:13` both give `7144675968` — high — write n = 3..7
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:5` — cited `slow.py 4 9 up` has no saved stdout — high — cite `verifier/out_lemmas.txt:39`
- **LOW** [cursor:grok-4.7-medium] `README.md:39` — "exhaustively" for n = 7 sits above the admission that dominance pruning was not rerun unpruned there (`README.md:82-84`) — medium — say "exhaustive under dominance pruning"
- **LOW** [cursor:gemini-3.8-flash-high] 09-slipped-comparator/README.md:39 — Claims S(3..7) lower bound was computed by three independent programs, but solver A omitted a standalone n=7 size-26 run (reports/referee.md:51) and all implementations share one model family (README.md:70) — confidence: high — State that two programs ran standalone size-26 runs at n=7 and note single model family origin.
- **LOW** [cursor:gemini-3.8-flash-high] 09-slipped-comparator/solver-a/proofs.txt:49 — T3(i) states "x sorted is the case c=0", which omits x = 1^m (the all-ones vector has no zeros) — confidence: high — Add explicit note that x = 1^m is trivially preserved.
- **LOW** [cursor:gemini-3.8-flash-high] 09-slipped-comparator/verifier/wit_bad.txt:9,18 — Two negative test witnesses (3: 2 1 2 1 2 1 and 4: 3 2 1 3 2 1 3 2 1 3 2 1) are genuinely slip-proof and deletion-proof due to redundant descending passes (reports/referee.md:52, out_wit_bad.txt:9,18) — confidence: high — Replace with genuinely defective words.
- **LOW** [cursor:gemini-3.8-flash-high] 09-slipped-comparator/solver-b/verify.py:31 — Belief (a) check at n=5 tests words up to length 8, where 0 words sort reversed input, making the check vacuous (verify.out:28, reports/referee.md:50) — confidence: high — Increase test length to at least N=10 for n=5 or document vacuity.
- **LOW** [cursor:gemini-3.8-flash-high] 09-slipped-comparator/solver-b/proofs.txt:109 — States word count equality between slip-proof and deletion-proof holds for "n = 3..6", but solver B's report and logs computed it through n = 7 — confidence: high — Update range to n = 3..7.
- **LOW** [cursor:gemini-3.8-flash-high] 09-slipped-comparator/solver-b/proofs.txt:102 — L5 base cases include A(1) = 0, but adjacent comparator networks require n >= 2 — confidence: high — Anchor the induction directly at A(2) = 2.

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
