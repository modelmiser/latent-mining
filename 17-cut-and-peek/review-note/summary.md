<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-220413 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.217; this month $194.65 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $1.621, input 22 + cache write 46,600 + cache read 257,015 tokens — cache hit 85%
- claude:claude-opus-5-5: $0.596, input 12 + cache write 39,118 + cache read 80,402 tokens — cache hit 67%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q4** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q5** (split) | UNSUPPORTED | REFUTED | CONFIRMED | REFUTED | CONFIRMED |
| **Q6** (split) | CONFIRMED | UNSUPPORTED | REFUTED | REFUTED | CONFIRMED |
| **Q7** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q5, Q6, Q7
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 11 MODERATE, 36 LOW
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:263-264 — "[3] … show that for three-variable functions no new protocols exist when all additional cards have the same colour": repository evidence (prior-art-deep.md:17) covers only three-bit AND; the Summary (l.14-15) claims only AND. Overstated relative to documented reading. — confidence: medium — restrict to what prior-art-deep records, or mark as the instance's reading.
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:250-252 — attribution of the n≥4 equality question to Heather–Schneider–Teague/Shinagawa–Mizuki "as reported in [2]" has no source in the folder; [6],[7] were not read (l.306-307). — confidence: medium — cite only [2]'s open-problem list.
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:161-164 — the "consistency check" enumerating Table 2 of [4] (eight restrictions, 4/4 split) has no script, output or truth table in the folder; unverifiable. — confidence: high — include the truth table or drop the sentence.
- **MODERATE** [claude:claude-opus-5-5] NOTE.md:263-264 — The note attributes to [3] a result for all three-variable functions; the repository supports it only for AND — medium — restrict it to AND, or quote [3].
- **MODERATE** [claude:claude-opus-5-5] NOTE.md:286-287,300,309 — "Five reviewing models from four vendors confirmed": no reports are in the folder — high — archive the reviews or drop the claim.
- **MODERATE** [claude:claude-opus-5-5] NOTE.md:296-298 — There is no code or log for the 1,011,693-case test — high — add the script and its output.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] NOTE.md:215-220 — general further facts omit n≥2; n=1 refutes two of them — confidence: high — add “For n≥2”
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] NOTE.md:329-330 — the promised verbatim panel record is absent — confidence: high — describe the record as partial and the panel material as a summary
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] NOTE.md:296-298 — the exact 1,011,693-case check has no archived code or log — confidence: high — call it an unarchived check
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] NOTE.md:161-163 — the claimed enumeration of [4]’s truth table has no table, script, or log in the repository — confidence: high — archive the calculation or label it unarchived
- **MODERATE** [cursor:grok-4.7-medium] NOTE.md:329–330 — the five reviewers’ verbatim reports are not in the folder, so “the full record … is in this folder” is false — high — state that only the solver and referee reports are present
- **LOW** [claude:claude-fable-5-1] NOTE.md:296-298 — the m≤9 test (1,011,693 cases) is not in the folder although l.329-330 says the full record is; count reproduces. — confidence: high — add file or say "not recorded".
- **LOW** [claude:claude-fable-5-1] NOTE.md:288-289 — "adds the remark on m even": solver-b/proofs.txt:51 already says "(m even)"; the remark was missing only from README's write-up. — confidence: high — "adds the remarks on the choice of two pairs and on an arbitrary corner".
- **LOW** [claude:claude-fable-5-1] NOTE.md:148 — "the negation of one" could mean a negated literal. — confidence: high — "or the negation of such an AND".
- **LOW** [claude:claude-fable-5-1] NOTE.md:18-20 — Lemma 1 in the Summary uses "depends on" (meaning: the card is flipped) and two-colour cards before Section 1 defines them. — confidence: medium — add "(Section 1)" or "each card is ♣ or ♥ and is changed by at most one of them".
- **LOW** [claude:claude-fable-5-1] NOTE.md:110, 125 — "θ₂ = θ₁ + m/2" needs the reader to note m/2 ≡ −m/2; ŝ(1,1,1)(r) undefined notation. — confidence: high — write "θ₂ ≡ θ₁ − m/2 ≡ θ₁ + m/2" and "the transform of s(1,1,1) at r".
- **LOW** [claude:claude-opus-5-5] NOTE.md:1,151,154 — The title and "This answers" are unconditional for an unrefereed AI proof — medium — add "if Lemma 1 holds" or "we claim".
- **LOW** [claude:claude-opus-5-5] NOTE.md:161-164 — The [4] consistency check has no artifact — medium — add it.
- **LOW** [claude:claude-opus-5-5] NOTE.md:272-273 — "Cannot be a random cut" lacks n ≥ 3 and "uniform" — low — add both.
- **LOW** [claude:claude-opus-5-5] NOTE.md:288 — The m-even remark is already in T2 (`proofs.txt`:51) — high — write "expands the remark on m even".
- **LOW** [claude:claude-opus-5-5] NOTE.md:147-148 — "the negation of one" is ambiguous — medium — use the wording from Q3.
- **LOW** [claude:claude-opus-5-5] NOTE.md:35,170,194,223-225,285 — Terms are used before they are defined — high — define them on first use.
- **LOW** [claude:claude-opus-5-5] NOTE.md:38 — "Exhaustive" without "pruned" — medium — write "pruned exhaustive".
- **LOW** [claude:claude-opus-5-5] NOTE.md:108-109 — The without-loss-of-generality relabelling is implicit — low — add "relabel so the shared index is 1".
- **LOW** [claude:claude-opus-5-5] NOTE.md:3-5 — The provenance warning is not in the opening sentence — low — put "AI-generated, unrefereed" in the subtitle.
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:43-44 — the conclusion about what the observer learns assumes the cut offset remains secret — confidence: high — say “uniformly and secretly”
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:47 — m≥1 is unstated although the Fourier proof requires it — confidence: medium — define n,m as positive integers
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:61 — “(k₀,k₁)-SCFO” is not defined — confidence: high — spell out the numbers of additional cards of each colour
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:148 — “the negation of one” has an ambiguous antecedent — confidence: high — write “an AND of three literals or its negation”
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:169-171 — “cyclically equalizable” is undefined — confidence: high — define it in terms of inserting additional words
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:244 — M(1,1,1) has 36 occurrences supported on 16 distinct words, not 16 words — confidence: high — write “has 16 distinct words in its support”
- **LOW** [cursor:gpt-5.6-sol-xhigh] NOTE.md:275-276 — “private-simultaneous-messages protocol” is introduced without explanation — confidence: high — briefly define it
- **LOW** [cursor:grok-4.7-medium] NOTE.md:25 — “either colour” can be read as monochrome helpers, the case [3] already closed — medium — “both colours, in any numbers”
- **LOW** [cursor:grok-4.7-medium] NOTE.md:147 — “the negation of one” does not say one what — high — “or the negation of that AND”
- **LOW** [cursor:grok-4.7-medium] NOTE.md:89–123 — θ_i is mod m, but r·t equalities are written in Z; they are only congruences (harmless for ω^{r·t}; README.md:113 says “modulo m”, the note does not) — high — write ≡ (mod m)
- **LOW** [cursor:grok-4.7-medium] NOTE.md:210 — the bracket [v=0] is never defined — high — “c when v=0, and 0 otherwise”
- **LOW** [cursor:grok-4.7-medium] NOTE.md:218 — “player” is used before any definition — high — “each input bit”
- **LOW** [cursor:grok-4.7-medium] NOTE.md:288 — “adds … m even” is already in proofs.txt:51 — high — say only the corner reduction is new relative to T2
- **LOW** [cursor:grok-4.7-medium] NOTE.md:298 — 1,011,693 occurs nowhere else in the folder — high — cite the script or drop the count
- **LOW** [cursor:grok-4.7-medium] NOTE.md:313–314 — the 6-card agreement is cited as a check, but README.md:196–197 says both sides are zero — high — say both searches find none, so this does not test a positive find
- **LOW** [cursor:grok-4.7-medium] NOTE.md:161–164 — the Table 2 balance count is not in the repo — high — include the 16-row table
- **LOW** [cursor:grok-4.7-medium] NOTE.md:66 versus NOTE.md:231 — ♣=+1, ♥=−1 in the proof, then 1=♥, 0=♣ in the witness — high — restate the witness convention at the strings
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/NOTE.md:26` — "no card depends on two bits" literally permits cards depending on \(\ge 3\) bits, whereas the proof requires each card to depend on at most one bit — high — replace with "in which each card depends on at most one bit"
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/NOTE.md:148` — "the AND of three literals or the negation of one" is syntactically ambiguous between negating one literal versus negating the 3-literal conjunction — high — replace with "the conjunction of three literals or the negation of such a conjunction"
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/NOTE.md:132` — "the exceptional row" collides with "exceptional pair" defined at line 102 for a frequency zero-sum pair — high — replace with "the unique row taking the minority value of f"
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/NOTE.md:217` — "player's bit" introduces the undefined term "player"; the note uses "input bit" or "variable" elsewhere — medium — replace with "input bit"
- **LOW** [cursor:gemini-3.8-flash-high] `17-cut-and-peek/NOTE.md:238` — "strand" is specialized jargon from [5] used without explicit definition — low — replace with "strand (sub-sequence of cards at a fixed residue class modulo 6)"

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
