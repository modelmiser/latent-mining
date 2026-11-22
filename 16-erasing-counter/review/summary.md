<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261119-171600 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $2.779; this month $180.47 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $2.153, input 38 + cache write 62,919 + cache read 565,001 tokens — cache hit 90%
- claude:claude-opus-5-5: $0.625, input 28 + cache write 44,976 + cache read 338,565 tokens — cache hit 88%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q3** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q5** (split) | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED | CONFIRMED |
| **Q6** (split) | CONFIRMED | REFUTED | REFUTED | REFUTED | REFUTED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q4 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q8 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q3, Q5, Q6
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 9 MODERATE, 28 LOW
- **MODERATE** [claude:claude-opus-5-5] README.md:124-125 — "Solver A reported that one of its wait loops ran for about 100 seconds outside the run rules" has no source. It is not in `reports/solver-a.md`, `results.txt`, or anywhere else in the folder ("wait" appears only in the README), and README:132 says the reports are verbatim — high — remove it, or add the source file and change "reported" to "was observed".
- **MODERATE** [claude:claude-opus-5-5] README.md:49, :95-96 — "solver A's search for r = 4 did not finish" / "is what failed to finish". Solver A only ran its search restricted to m=6 positions (timed out at 1500 s); it never attempted unrestricted r=4 and relied on annealing (`reports/solver-a.md:73,80`) — high — "solver A ran its trajectory search at r = 4 only with at most 6 positions, where it timed out; for D(4) it has annealing only and claims ≥ 1806".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/check.py:24,33` — nonbinary start characters are silently treated as zero, so malformed witnesses can be accepted — confidence: high — validate every character is `0` or `1`
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:51-52` — false “exactly one 1” assertion; an empty all-zero-path leaf gives a zero-read fixed point — confidence: high — add the fixed-point exception
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:13-14` — “no better upper bound is known” is unsupported by a literature search that read no source fully (`prior-art.md:35-37`) — confidence: high — say “we found no better upper bound”
- **MODERATE** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:124-125` — “Solver A reported” a ~100s out-of-rules wait loop, but that report is not in `reports/` or the solver logs — high — delete the sentence
- **MODERATE** [cursor:gemini-3.8-flash-high] `16-erasing-counter/README.md:124-125` — States solver A reported a wait loop ran ~100s outside run rules, but solver A's report and repository files contain no such statement — confidence: high — Delete sentence or document source
- **MODERATE** [cursor:gemini-3.8-flash-high] `16-erasing-counter/README.md:44` — Results table asserts D(1), D(2) computed by "all three instances over every tree", but solver A ran trajectory search lazy.cpp (482 nodes), not all 1201 trees — confidence: high — Replace with "computed by all three instances (solver B and referee over every tree, solver A via lazy trajectory search)"
- **MODERATE** [cursor:gemini-3.8-flash-high] `16-erasing-counter/solver-b/bad_idx.txt:1` — Described in solver-b report as testing repeated index rejection, but index 14 is in separate branches; rejected by checker only because leaf [15] is off-path — confidence: high — Place an index twice on one path so it tests repeated index detection
- **LOW** [claude:claude-fable-5-1] README.md:125 — "Solver A reported that one of its wait loops ran for about 100 seconds outside the run rules" has no source anywhere in the folder (`grep` of reports/, solver-a/results.txt, intermediate/ finds nothing) — high — delete or cite the transcript it came from
- **LOW** [claude:claude-fable-5-1] README.md:47 — "computed by all three instances over every tree": solver A's D(1), D(2) come from `lazy.cpp`, a symmetry-reduced trajectory search (`reports/solver-a.md:9-10`), not an enumeration of all 5 / 1201 trees — high — "by solver B and the referee over every tree, and by solver A's search"
- **LOW** [claude:claude-fable-5-1] README.md:93-96 — "solver A's program, of the same design, is what failed to finish": A's `lazy.cpp` lacks the minimum-weight pruning (`solver-a/lazy.cpp:7`) and was never run with all 15 positions; it timed out at m=6 (`solver-a/results.txt:3`) — high — "solver A's program, same design without the minimum-weight pruning, timed out already at 6 positions and was not run on 15"
- **LOW** [claude:claude-fable-5-1] README.md:56 — "over all trees": `solver-b/prod3.cpp:1` fixes root=1 and a canonical X; `verifier/brute.cpp:3` fixes two indices — high — "over all trees up to relabelling"
- **LOW** [claude:claude-fable-5-1] README.md:63-64 — hypothesis stated as uniqueness of any empty-leaf use rather than one designated leaf used once (`solver-a/proofs.txt:7-9`); true for the construction, but not what the induction carries — medium — as in Q5
- **LOW** [claude:claude-fable-5-1] README.md:109 vs prior-art.md:9 — author order differs (prior-art lists Jansens first); the README order is the paper's — low — align prior-art.md
- **LOW** [claude:claude-opus-5-5] README.md:61 vs :79 — the section says it follows solver A's proof but uses solver B's base case — high — fix as in Q5.
- **LOW** [claude:claude-opus-5-5] README.md:47 — "computed by all three instances over every tree": solver A used the pruned trajectory search, not an enumeration of every tree (`reports/solver-a.md:70-71`) — high — "by B and the referee over every tree; by A with an exhaustive search pruned by symmetry".
- **LOW** [claude:claude-opus-5-5] README.md:54 — solver B also found the 720 optimal r=3 trees (`reports/solver-b.md:18`), and the "Where" cell omits `solver-b/ex3_full*.out` and the referee — medium — "computed by both solvers and the referee".
- **LOW** [claude:claude-opus-5-5] README.md:98-99 — "could only make the reported value too small" is not true of every bug. It holds here because every recorded length is a real orbit of a legal tree (`verifier/lazy.cpp:37-44`) and 1806 is also a proved lower bound — medium — "Any recorded length is a real cycle of a legal tree and 1806 is proved as a lower bound, so an error could only hide a longer cycle."
- **LOW** [claude:claude-opus-5-5] README.md:14 — "no upper bound better than about 2^(2^r)": the proved bound is 2^(2^r−1)−1, which is about half of 2^(2^r) — low — "no upper bound better than 2^(2^r − 1) − 1".
- **LOW** [claude:claude-opus-5-5] solver-a/proofs.txt:76 — Lemma 2(c) "every step reads exactly one 1" fails at a fixed point. The README already discloses this — high — "every step of a cycle of length > 1 reads exactly one 1".
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:43` — Lemma 2 assumes a root although a single-leaf tree is legal — confidence: high — dispatch the identity-map case first
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:44-48` — literal “every tree” wording hides canonical/trajectory enumeration — confidence: high — say “representatives covering every cycle”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `reports/solver-a.md:26` — “24 relabelings” means symmetry-reduced representatives; the referee reports 720 with root fixed (`reports/referee.md:23,35`) — confidence: high — state the symmetry convention
- **LOW** [cursor:gpt-5.6-sol-xhigh] `reports/solver-b.md:26` — `bad_idx.txt` is rejected for an off-path write, not a repeated path index — confidence: high — correct the test description
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:109` — bibliographic author order disagrees with `prior-art.md:7` — confidence: high — use “Jansens et al.”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:125` — wait-loop claim has no repository evidence — confidence: high — remove or source it
- **LOW** [cursor:gpt-5.6-sol-xhigh] `verifier/lazy.cpp:3` — comment says states are XOR-encoded, but only visitation keys are (`verifier/lazy.cpp:39-45`) — confidence: high — correct the comment
- **LOW** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:109` — 2010 author order disagrees with `prior-art.md:9` — high — use Jansens, Bose, Carmi, Maheshwari, Morin, Smid
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:51-52` — “every step reads exactly one 1” is false for an empty all-zero-path fixed point; length 1 still holds — high — except a creation-free fixed point, which reads no 1
- **LOW** [cursor:grok-4.7-medium] `solver-b/proofs.txt:27-28` — the sketch caps the period by 2^r (“≤ r bits”); a depth-(r−1) subtree has up to 2^(r−1)−1 indices — high — keep it labeled as a sketch and drop the 2^r count
- **LOW** [cursor:grok-4.7-medium] `16-erasing-counter/README.md:93-96` — “one design” covers A, B, and the referee; A and the referee XOR against the start (`solver-a/lazy.cpp:6`, `verifier/lazy.cpp:3`), B uses packed absolute states (`solver-b/lazy.cpp:2-4`), and A does not prune on weight — medium — one trajectory idea, two encodings
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:65-70` — the write-up never says that a cycle which misses all-ones has length ≤ 2^d−1 — medium — add that sentence before the all-ones case
- **LOW** [cursor:grok-4.7-medium] `verifier/brute.cpp:42` — empty loop, no effect on the histogram — high — delete it
- **LOW** [cursor:gemini-3.8-flash-high] `16-erasing-counter/solver-a/proofs.txt:51` — Lemma 2(c) states every step on a zero-creation cycle reads exactly one 1; fails for fixed point 0^d -> 0^d with empty leaf (reads 0 ones) — confidence: high — Qualify with "for cycles of length > 1"
- **LOW** [cursor:gemini-3.8-flash-high] `16-erasing-counter/README.md:109` — Author list puts Bose first; prior-art.md:9 lists Jansens first — confidence: high — Change citation to "(Jansens, Bose, Carmi, Maheshwari, Morin, Smid, 2010)"
- **LOW** [cursor:gemini-3.8-flash-high] `16-erasing-counter/README.md:86` — "uses every one of the 2^r - 1 positions once" is ambiguous between node count and step usage — confidence: medium — Clarify to "uses each of the 2^r - 1 positions at exactly one internal node"

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
