<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-214816 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $3.154; this month $149.43 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.308, input 30 + cache write 64,161 + cache read 474,034 tokens — cache hit 88%
- claude:claude-opus-5-5: $0.846, input 28 + cache write 64,709 + cache read 450,795 tokens — cache hit 87%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q5** (split) | CONFIRMED | REFUTED | REFUTED | CONFIRMED | CONFIRMED |
| **Q7** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | REFUTED |
| **Q8** (split) | REFUTED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q5, Q7, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 19 MODERATE, 33 LOW
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:318,312 — source [8] (Gupta slides) and the "3 amortized flips" claim appear nowhere in the recorded search; provenance sentence is false for this item — high — add [8] to prior-art-deep.md or say "read for this note, not part of the recorded search".
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:350-351 — "known only through [6]" but prior-art-deep.md:17 says Chaudhuri et al. was read directly — high — "Chaudhuri et al. was read in part; Grove et al. only through [6] and [9]".
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:327 — "O(1) amortized time and flips with out-degree 3" unsupported by prior-art files, which record O(log log n) w.h.p. — medium — state what prior-art.md says or cite where the amortized claim was read.
- **MODERATE** [claude:claude-fable-5-1] NOTE.md:390-391 — "a proof would need an Algorithm whose analysis sees more than one vertex per tree" overclaims; Section 5 refutes one potential, and that potential already reads every vertex — high — reword as above.
- **MODERATE** [claude:claude-opus-5-5] NOTE:317-319 — Gupta's survey slides [8] appear nowhere in prior-art.md or prior-art-deep.md, so the claim "we read this in slides" cannot be traced. — high — Either add the source to prior-art-deep.md or cite only the derivation from Brodal–Fagerberg Lemma 1, with its Δ = 2δ caveat (prior-art.md:21).
- **MODERATE** [claude:claude-opus-5-5] NOTE:327-328 — "Bender et al. obtain O(1) amortized … out-degree 3" contradicts prior-art.md:9, which records O(log log n) per insertion w.h.p. — high — Use the prior-art wording.
- **MODERATE** [claude:claude-opus-5-5] NOTE:349-351 — "we know those results only through [6]" is false for Chaudhuri et al., which was read directly (prior-art-deep.md:14, 27). — high — "Chaudhuri et al. read (introduction); Grove et al. only through [6] and Chaudhuri et al."
- **MODERATE** [claude:claude-opus-5-5] NOTE:390-392 — Overclaim, see Q7. — high — Use the Q7 wording.
- **MODERATE** [claude:claude-opus-5-5] NOTE:262-271 — The wider claim and the undefined inequality, see Q5. — medium — Use the Q5 wording.
- **MODERATE** [claude:claude-opus-5-5] NOTE:119-126 — Lemma 2 leaves out "y keeps its type", but the pair game in Section 5 depends on it. — high — Add the clause.
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:258-268` — the purported self-contained method barrier omits the four-option inequality proving the potential invariant — confidence: high — include the proof or label it as an external supporting result
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:266-268` — Theorem 3 rules out only the defined pair game, not every proof using the same invariant — confidence: high — narrow the conclusion to that game
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:268-271` — “the inequality” and the real-forest potential G are undefined, and one failed extension does not rule out all Algorithms — confidence: high — define G and state only that this extension fails
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:390-392` — the asserted necessity of seeing multiple vertices in both directions is unproved — confidence: high — replace with the narrower pair-game limitation
- **MODERATE** [cursor:grok-4.7-medium] NOTE.md:390-392 — Open question 2 presents two necessities Section 5 does not prove; a one-witness analysis other than g, or a one-witness adversary that does not use only Lemma 2, is not ruled out — high — replace with the Q7 wording.
- **MODERATE** [cursor:grok-4.7-medium] NOTE.md:327-328 — The O(1) sentence contradicts the only Bender bound stored in prior-art.md:8 — high — use the Q8 wording.
- **MODERATE** [cursor:grok-4.7-medium] NOTE.md:340-341 — "does not give a worst-case lower bound" is stronger than the recorded "does not imply an Ω(log n) worst-case lower bound" (prior-art-deep.md:25) — high — restore the Ω(log n) qualifier.
- **MODERATE** [cursor:grok-4.7-medium] NOTE.md:348-351 — "only through [6]" covers Chaudhuri, whom prior-art-deep.md:15-16 says was read; Grove is the unread one (prior-art-deep.md:20) — high — split the two citations.
- **MODERATE** [cursor:gemini-3.8-flash-high] 14-forest-orientation/NOTE.md:390-392 — Open question 2 claims proving L = 1/3 requires an algorithm seeing >1 vertex per tree and disproving it requires an adversary that does; repository only proves the specific potential g fails on 9 vertices (`open-limit/proofs.txt:158`) and the abstract pair game stops at 1/3 (Note N3), leaving general one-witness strategies on real trees unrefuted — confidence: high — Replace with: "2. Is L = 1/3? The candidate potential of Section 5 does not transfer to real trees, and the abstract pair game cannot force more than 1/3, leaving L open."
- **LOW** [claude:claude-fable-5-1] NOTE.md:252 — "36 ≤ n ≤ 63" should be 36–61 (62, 63 follow from certificates + superadditivity) — high — change range.
- **LOW** [claude:claude-fable-5-1] NOTE.md:251 — T(64) ≥ 18 "by certificates": that 10 MB certificate is not in the repository (README.md:247-249; solver-b/ has only n=8,12,30,32) — high — say "certificate not included; generator is".
- **LOW** [claude:claude-fable-5-1] NOTE.md:119-127 — Lemma 2 omits "y keeps type (p′,q′)", used at 261 — high — add clause.
- **LOW** [claude:claude-fable-5-1] NOTE.md:87-88 — validity of the flipped path (result is a position) not argued — high — add one clause.
- **LOW** [claude:claude-fable-5-1] NOTE.md:64-67 vs 95-99 — Lemma 1 stated for a fixed sequence, used adaptively; proof is per-round so fine, statement should say so — medium — restate as a one-round invariant.
- **LOW** [claude:claude-fable-5-1] NOTE.md:242-244 — superadditivity sketch omits that Algorithm flips in the first tree during the second phase only add cost (solver-b/proofs.txt:52-54) — medium — add clause.
- **LOW** [claude:claude-fable-5-1] NOTE.md:249 — "three separate programs" hides that n=14,15 were searched with normal replies only (README.md:217-222) — medium — add "n = 14, 15 under normal replies".
- **LOW** [claude:claude-fable-5-1] NOTE.md:329 vs 323 — "random-walk rule" conflicts with prior-art.md's description of the folklore rule as path flipping — medium — "the path-flipping rule".
- **LOW** [claude:claude-fable-5-1] NOTE.md:269-271 — the 9-vertex failure is presented as a fact; source marks it COMPUTED (proofs.txt:159). I verified it by hand, so true — low — say "(checked by hand and by program)".
- **LOW** [claude:claude-opus-5-5] NOTE:344 — "(α = 1)" sits inside the quotation marks but is not in the quote as recorded (prior-art-deep.md:26). — high — Move it outside the quote.
- **LOW** [claude:claude-opus-5-5] NOTE:338-339 — "[3] … with insertions and deletions" is not supported by the prior-art files. — medium — Remove it or source it.
- **LOW** [claude:claude-opus-5-5] NOTE:333, 352-353 — [9] is listed under "statements we read", but it was only keyword-scanned (deep:15), and its bound is Ω(1/Δ), not a constant. — high — "scanned only: an Ω(1/Δ) amortized lower bound…"
- **LOW** [claude:claude-opus-5-5] NOTE:310-311 — The note promises to say, for each statement, whether the source was read or relayed, but most items ([2], [3], [4], [5]) don't say. — high — Tag each item "(read: §…)" or "(scanned)".
- **LOW** [claude:claude-opus-5-5] NOTE:309 — "the same AI instances" is wrong; the searches were done by separate instances of the same model. — medium — "instances of the same model".
- **LOW** [claude:claude-opus-5-5] NOTE:242-244 — The superadditivity argument is missing the restriction step. — high — Add "payments restricted to B are at most real payments".
- **LOW** [claude:claude-opus-5-5] NOTE:81-88 — Legality of the path flip is never argued. — high — See Q1.
- **LOW** [claude:claude-opus-5-5] NOTE:232-233 — f is "defined" with ≤. — high — "f(n) = max …".
- **LOW** [claude:claude-opus-5-5] NOTE:276 — Line count; see Q6. — high — "about 2,000".
- **LOW** [claude:claude-opus-5-5] NOTE:298 — Not stated that the bound forces nothing at n = 31, 32. — high — Add it.
- **LOW** [claude:claude-opus-5-5] NOTE:251 — The certificates rely on Lemma 1 (normal-form replay, reports/solver-b.md:32), but the note doesn't say so. — medium — Add "(against normal replies; valid by Lemma 1)".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:250-253` — the T(64) certificate is omitted from the repository (`README.md:247-249`), so its reported verification is not reproducible here — confidence: high — disclose that omission
- **LOW** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:309-329` — read-versus-relayed provenance is not consistently stated — confidence: high — mark each paragraph explicitly
- **LOW** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:1` — the title literally says n/3 although the finite bound is n/3−O(log n) — confidence: medium — use “An asymptotic n/3 lower bound”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `14-forest-orientation/NOTE.md:28-29` — “contributes the constant” sounds like a novelty claim despite the acknowledged unread likely source (`NOTE.md:355-360`) — confidence: high — say “Our search did not find this constant.”
- **LOW** [cursor:grok-4.7-medium] NOTE.md:329 — prior-art.md:9 calls the folklore rule a directed path to out-degree ≤1, not a random walk — medium — "the path-flip rule, which [3] calls folklore".
- **LOW** [cursor:grok-4.7-medium] NOTE.md:51-53 — 2^{k+1}−1 needs the unused fact that each saturated vertex has two out-edges — high — add that count.
- **LOW** [cursor:grok-4.7-medium] NOTE.md:208-212 — the jump from F(b_{r+1})−F(i_r) to Σ(F(b_r)−F(i_r)) is correct but not written — high — insert the reindex.
- **LOW** [cursor:grok-4.7-medium] NOTE.md:242-244 — superadditivity does not say why flips into the finished part cannot spoil the second game — medium — add that those flips only add payment.
- **LOW** [cursor:grok-4.7-medium] NOTE.md:298-299 — the integer bound forces T≥1 on 26..30 and from 33 on, and not at 31 or 32 (README.md:166-168) — high — add that exception.
- **LOW** [cursor:grok-4.7-medium] NOTE.md:343-344 — "(α = 1)" is not in the quotation recorded in prior-art-deep.md:26 — medium — move it outside the quotation.
- **LOW** [cursor:gemini-3.8-flash-high] 14-forest-orientation/NOTE.md:137 — Fact (b) states the unique tree path from o' to path vertices begins with "the edge o′x, which is directed towards o′", where writing "o'x" for an edge oriented x -> o' can momentarily confuse direction — confidence: medium — Clarify as "begins with the edge between o′ and x, which is directed towards o′ (i.e. x → o′)"
- **LOW** [cursor:gemini-3.8-flash-high] 14-forest-orientation/NOTE.md:200-204 — Cleanup assumes m trees with indices i₁ < ... < i_m and runs r from m - 1 down to 1; if main phase finishes with m = 1 tree, this loop is vacuous and the notation i₁ < ... < i_m is degenerate — confidence: high — Add a parenthetical: "(if m = 1, no cleanup rounds occur and paid(Y₁) ≥ n/3 − F(i₁))"
- **LOW** [cursor:gemini-3.8-flash-high] 14-forest-orientation/NOTE.md:147 — Deducing p = -1 from an empty slot is phrased as "so P = -1 = p", compressing the type definition (-1 <= p <= P = -1) — confidence: low — Rephrase to: "A free reply needs an unsaturated vertex, so P = -1, which forces p = -1; it costs 0 = p + 1..."

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
