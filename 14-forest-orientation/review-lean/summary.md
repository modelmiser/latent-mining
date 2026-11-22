<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-182008 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $1.511; this month $146.27 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $1.187, input 24 + cache write 34,714 + cache read 203,479 tokens — cache hit 85%
- claude:claude-opus-5-5: $0.324, input 12 + cache write 25,738 + cache read 46,823 tokens — cache hit 64%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q8** (split) | REFUTED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q5 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 3 MODERATE, 22 LOW
- **MODERATE** [claude:claude-opus-5-5] `SpecCheck.lean:57-69` vs `FalseVariants.lean.txt` — The comment's variant 3 (`cleanup` in Core.lean with loss 1) is not in `FalseVariants.lean.txt`. That file's variants 3 (Type Lemma, index always rises) and 4 (constant 4 in place of 3) are not in the comment. The file contains no outputs, despite "exact inputs and outputs". `README.md:158-159` says "Four ... that Lean refuses are recorded", but the refusals are not recorded anywhere for variants 3 and 4. — high — Make the two lists the same and store the actual error output, or say "inputs only".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] 14-forest-orientation/lean/SPEC.md:1,37-61 — “every choice” omits the off-play behavior of `replies` and unequal-length `flips`; this does not affect `T` because generated replies are legal and aligned — confidence: high — document those behaviors or restrict the definitions
- **MODERATE** [cursor:grok-4.7-medium] SpecCheck.lean:65 — the comment says `FalseVariants.lean.txt` has the cleanup-loss input and Lean's output; that file has four other counterexamples and no compiler output — high — point the comment at those four examples, or add the cleanup input and the error
- **LOW** [claude:claude-fable-5-1] lean/FO/SpecCheck.lean:57-69 vs lean/FalseVariants.lean.txt — variant lists disagree (3 vs 4, different sets), promised error outputs absent — high — make the two lists identical and add the outputs
- **LOW** [claude:claude-fable-5-1] lean/FalseVariants.lean.txt:13 — comment "constant 4 in place of 3" but the statement uses n + 6 — high — "n + 6 in place of n + 3"
- **LOW** [claude:claude-fable-5-1] lean/SPEC.md:55-56 — "Both conventions can only make T smaller" is wrong for maxL [] = 0 (equal to the natural convention, larger than −∞); "neither happens in play" for moves is not formally proved — medium — wording above
- **LOW** [claude:claude-fable-5-1] lean/SPEC.md:67 — "T 5 = 0 … #eval" not reproducible from repository — high — drop or add a log
- **LOW** [claude:claude-fable-5-1] lean/SPEC.md:37-62 — unlisted choices: `value` is total on arbitrary lists (non-forests, loops); `flips` truncates unequal lengths; `valid` includes the new edge — medium — add as items 11-12
- **LOW** [claude:claude-fable-5-1] README.md:53 "positive only from n = 34" — the Lean integer form gives T(33) ≥ 1 (36 ≤ 3T+35) — medium — "the hand bound is positive from 34, the Lean form from 33"
- **LOW** [claude:claude-opus-5-5] `SpecCheck.lean:2,16-18`; `README.md:154`; `SPEC.md:66` — "Each of them can fail" is false for T 0, T 1 and T 2:
- **LOW** [claude:claude-opus-5-5] `SpecCheck.lean:11-12` — The `conn_iff` "check" only restates the lemma, so it cannot fail on its own. It is listed as a check that can fail (`README.md:157-158`). — high — Call it a lemma, not a check.
- **LOW** [claude:claude-opus-5-5] `SpecCheck.lean:53-55` — "Not vacuous" is shown only at n=1, where T 1 = 0 by definition. The theorem first constrains T at n=33, and no check evaluates anything near that size. — medium — State the n ≥ 33 threshold. Treat Q6's large-n reasoning as the argument that the statement has content.
- **LOW** [claude:claude-opus-5-5] `SPEC.md:37-62` (unlisted choice) — A state is the ordered arc list, so it records the order edges were added, which is more than (G,D). Values of states with the same (G,D) are never identified. This is harmless because the game tree rooted at `[]` is the real game tree, but it is a choice the list leaves out. — medium — Add item 11: "the state also records the order edges were added; the value is computed on that richer tree, which equals the game tree."
- **LOW** [claude:claude-opus-5-5] `FalseVariants.lean.txt` (last example) — The "constant 4" variant is not shown to be false. At n=1 it holds (7 ≤ 7). It is only not derivable from `key` by that proof. Calling it "deliberately false" is inaccurate. — medium — Call it "not derivable from `key`".
- **LOW** [claude:claude-opus-5-5] `README.md:144-145` — "Trusting ... those 89 lines and the Lean kernel" leaves out the core-library definitions they use (`Nat.log2`, `List.finRange`, `countP`, `flatMap`). — low — Add "and the core definitions they use".
- **LOW** [claude:claude-opus-5-5] `SPEC.md:56` — "Neither happens in play" is proved only for the Algorithm's side. The Adversary always having a move is argued in the text (counting components) but not proved. This is harmless, because an empty maximum only lowers T. — medium — Say "proved for replies; for moves by component counting".
- **LOW** [cursor:gpt-5.6-sol-xhigh] 14-forest-orientation/lean/FO/Main.lean:11-16 — “The same without logarithms” suggests equivalence, but the power theorem is weaker: at abstract `n=40,T=2` the power inequality holds while the integer inequality fails — confidence: high — say “A weaker log-free corollary”
- **LOW** [cursor:grok-4.7-medium] Spec.lean:27 — the comment places `conn_iff` in `SpecCheck.lean`; the proof is `Graph.lean:127`, and SpecCheck.lean:12 only calls it — high — cite `Graph.lean`
- **LOW** [cursor:grok-4.7-medium] Spec.lean:52 — a longer tail is ignored, so a mis-aligned reply would under-count flips; unused on real replies, and not listed in SPEC.md:37-61 — medium — state that choice next to item 6
- **LOW** [cursor:grok-4.7-medium] SPEC.md:53 — "no legal reply ⇒ the rest of the game is worth 0"; `minL []` zeros only that move (Spec.lean:74-75) — high — say that move scores 0
- **LOW** [cursor:grok-4.7-medium] SPEC.md:67 — the one-off `T 5` timing cannot be checked from the repo — medium — delete it or keep a log
- **LOW** [cursor:gemini-3.8-flash-high] `lean/FO/SpecCheck.lean:69` — Comment states "See FalseVariants.lean.txt for the exact inputs and outputs", but `FalseVariants.lean.txt` contains only Lean code inputs without compiler error outputs — confidence: high — Append compiler error diagnostic outputs to `FalseVariants.lean.txt` or adjust comment to "for the exact inputs"
- **LOW** [cursor:gemini-3.8-flash-high] `lean/SPEC.md:38-62` — The ten listed encoding choices omit the explicit syntactic convention that `replies` places the new edge at list head for `cost` to drop via `.tail`, and omits integer discretization from the numbered list — confidence: high — Add explicit entries in `SPEC.md` documenting head-placement of new edges and cross-referencing integer rounding
- **LOW** [cursor:gemini-3.8-flash-high] `lean/FalseVariants.lean.txt:13` — Comment says "constant 4 in place of 3" but the code replaces constant 3 with 6 (`n + 6 ≤ 3 * T n + 7 * Nat.log2 (n + 1)`) — confidence: high — Change comment to "constant 6 in place of 3"
- **LOW** [cursor:gemini-3.8-flash-high] `lean/FO/SpecCheck.lean:64-67` — Comment describes a third false variant in `Core.lean` cleanup missing from `FalseVariants.lean.txt` (which contains a Type Lemma variant and an `n + 6` variant instead) — confidence: high — Synchronize the listed false variants in `SpecCheck.lean` comments with `FalseVariants.lean.txt`

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
