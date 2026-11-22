<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# Panel 20261117-100750 — (repository) — round 1

Seats: claude:claude-fable-5-1, claude:claude-opus-5-5, cursor:gpt-5.6-sol-xhigh, cursor:grok-4.7-medium, cursor:gemini-3.8-flash-high
API spend this run: $4.632; this month $133.71 of $200.00 (Cursor spend not metered: check Cursor's usage page)

- claude:claude-fable-5-1: $3.512, input 34 + cache write 95,653 + cache read 759,530 tokens — cache hit 89%
- claude:claude-opus-5-5: $1.121, input 34 + cache write 76,832 + cache read 611,042 tokens — cache hit 89%

| Q | claude:claude-fable-5-1 | claude:claude-opus-5-5 | cursor:gpt-5.6-sol-xhigh | cursor:grok-4.7-medium | cursor:gemini-3.8-flash-high |
|---|---|---|---|---|---|
| **Q4** (split) | CONFIRMED | CONFIRMED | REFUTED | CONFIRMED | CONFIRMED |
| **Q5** (split) | CONFIRMED | CONFIRMED | REFUTED | REFUTED | CONFIRMED |
| **Q8** (split) | REFUTED | REFUTED | REFUTED | REFUTED | CONFIRMED |
| Q1 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q2 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q3 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q6 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |
| Q7 | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED | CONFIRMED |

Disagreements (read these first): Q4, Q5, Q8
Reports: claude__claude-fable-5-1.md, claude__claude-opus-5-5.md, cursor__gpt-5.6-sol-xhigh.md, cursor__grok-4.7-medium.md, cursor__gemini-3.8-flash-high.md

## Findings (UNVERIFIED — check each in the code; record the result in dispositions.md)
Raw counts across seats, duplicates included: 0 CRITICAL, 14 MODERATE, 39 LOW
- **MODERATE** [claude:claude-fable-5-1] README.md:42 — Claims replay "against every online reply"; the logs restrict Online to inclusion-maximal replies except 12 small cases. Lemma (b) justifies that restriction for the game value, not for the forced-fault count of a fixed cache-reading adversary, so the computational check of the lower-bound adversaries is weaker than stated (proofs themselves cover all replies). — high — wording above.
- **MODERATE** [claude:claude-opus-5-5] README.md:64 — "replayed each adversary against every online reply for k ≤ 13, m ≤ 6" is false. adva.txt and advb*.txt run in mode 1, which only tries inclusion-maximal Online replies (wk.cpp:10, :207, :219). Lemma D justifies that for game values, not for testing a fixed adversary. All replies were tried only in 12 small cases each (adva_mode0.txt, advb_mode0.txt). — high — "…against every inclusion-maximal Online reply for k ≤ 13, m ≤ 6, and every reply for 12 small cases".
- **MODERATE** [claude:claude-opus-5-5] README.md:131–135 — "did not find the two combined" leaves out prior-art.md:15–16: Gaysinsky–Itai–Shachnai do combine access-graph walks with prefetching (different objective). — high — "…found one work combining access-graph requests with prefetching (Gaysinsky, Itai, Shachnai 2001; execution-time objective), but none with fault counts or an exact game value".
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:80` — false odd-\(k\) case equivalence — confidence: high — use the parity-specific condition above
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/proofs.txt:69-72` — stated credit invariant cannot hold initially — confidence: high — include initial credit and the required eviction argument
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:42` — replay did not enumerate every legal Online reply at the claimed range — confidence: high — qualify it as inclusion-maximal replies
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `README.md:112-113` — prior-art summary denies a combination that `prior-art.md:15-16` reports — confidence: high — narrow the claim to the exact model
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-b/verify.py:131` — revisiting a state returns success, accepting zero-fault infinite plays — confidence: high — detect hit-only cycles
- **MODERATE** [cursor:gpt-5.6-sol-xhigh] `solver-a/adv.py:61-64` — stopping at the first terminal leaves other branches’ legality and zero-fault cycles unchecked — confidence: high — exhaust reachable states and detect cycles
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/proofs.txt:71-72` — Virtual-hit case states `|V\R|−1` for an unspecified real fault; evicting a page of `V` breaks `credit≥|V\R|`. — high — Keep `V∩R`, add `p`, evict only from `R\V`.
- **MODERATE** [cursor:grok-4.7-medium] `solver-a/proofs.txt:102` — “Always prefetch an uncached neighbour” is not implied by the domination test and is not what `online_options` does once eviction is required. — high — Say online may be limited to inclusion-maximal caches.
- **MODERATE** [cursor:grok-4.7-medium] `solver-b/proofs.txt:75` — Claims unpruned agreement also for `k=2..4`, `m=1..3`; `solver-b/table_py_noprune.txt` has only the 18 cases `k=3..8`, `m=2..4`. — high — Delete the clause, or add the log.
- **MODERATE** [cursor:gemini-3.8-flash-high] `solver-b/verify.py:131` — `check_adv` returns `True` on any revisited state (`if kk in seen: return True`), which would falsely accept an adversary that enters an infinite zero-fault cycle with `f < target` — confidence: high — Track recursion stack ancestors to reject zero-fault cycles or assert forward progress
- **MODERATE** [cursor:gemini-3.8-flash-high] `solver-a/adv.py:61` — Dijkstra search halts upon popping the first terminal state, so legality assertions are not executed on unpopped reachable states and zero-fault cycles would go undetected — confidence: high — Traverse the full reachable state space of the adversary against all legal online branches
- **LOW** [claude:claude-fable-5-1] README.md:112-113 — "did not find the two combined": `prior-art.md:15-16` item 5 (Gaysinsky–Itai–Shachnai) combines access-graph walks with prefetching, differing in objective. — medium — "did not find neighbour-only prefetch on a path with an exact bounded-opt value".
- **LOW** [claude:claude-fable-5-1] README.md:73 — "away from the side the online cache prefetched": if Online loads nothing extra in round 1, either side is used (`solver-b/proofs.txt:38`). — high — "toward a side whose first page is not in the online cache".
- **LOW** [claude:claude-fable-5-1] README.md:58-59 — "once m ≥ k/2": the saving is ⌊k/2⌋−1 already for m ≥ ⌊k/2⌋ (k=3, m=1: D=0). — high — "once m ≥ ⌊k/2⌋".
- **LOW** [claude:claude-fable-5-1] README.md:79 — "k − 1 once, for odd k": the (k−1)-window persists through all free steps before the first cycle completes. — medium — "k − 1 until the first cycle, for odd k".
- **LOW** [claude:claude-fable-5-1] solver-b/proofs.txt:75 — claims unpruned agreement on "all k=2..4, m=1..3"; `table_py_noprune.txt` has only k=3..8, m=2..4 (k=2 and m=1 absent). — high — delete the k=2..4, m=1..3 clause or add the log.
- **LOW** [claude:claude-fable-5-1] solver-b/proofs.txt:68-73, solver-a/proofs.txt:96-102 — pruning (b) is proved for a fixed future; the transfer to the perfect-information game (adversary sees the real, not the simulated, cache) needs the remark that legality depends only on the walk. — medium — add one sentence.
- **LOW** [claude:claude-fable-5-1] solver-a/proofs.txt:20-27 — Belady's theorem is invoked for "all NP schedules", which may evict more than one page per fault; the classical theorem is for lazy demand paging (the lazy conversion is standard but unstated). — low — add "non-lazy NP schedules can be made lazy without more faults".
- **LOW** [claude:claude-opus-5-5] README.md:35 — "computed exhaustively" hides that the large cases rely on the pruning lemma. — high — add "with pruning proved safe; unpruned to (8,4)".
- **LOW** [claude:claude-opus-5-5] solver-b/proofs.txt:69 — the credit invariant is misstated (Q5). — high — as in Q5.
- **LOW** [claude:claude-opus-5-5] solver-b/proofs.txt:75 (also reports/solver-b.md) — "agrees on all k=2..4, m=1..3" has no log in solver-b/; table_py_noprune.txt starts at k=3, m=2. — high — drop it, or cite verifier/small.txt.
- **LOW** [claude:claude-opus-5-5] README.md:104 — "steps one page past an end of the window, where online must fault" leaves out that the end is chosen so the outside page is uncached (proofs.txt:48). For |O| = k−1 the other end can be a hit. — high — "past an end whose outside page online does not hold".
- **LOW** [claude:claude-opus-5-5] README.md:95–98 — "away from the side the online cache prefetched" (it may not have prefetched) and "pays min(m,h) more faults" (it is at least that). — high — "away from an uncached neighbour of 0 … at least min(m,h) more faults".
- **LOW** [claude:claude-opus-5-5] README.md:101 — "(k − 1 once, for odd k)": O has k−1 pages from the end of stage 0 until the first cycle, possibly over several free steps. — medium — "(k − 1 until the first cycle, for odd k)".
- **LOW** [claude:claude-opus-5-5] README.md:66 — "(one entry timed out)": solver A's (7,5) timed out (solver B covers it), but solver B also timed out on four m = 6 entries. — medium — "solver A timed out on (7,5), which B covers; B timed out at k = 5..8, m = 6".
- **LOW** [claude:claude-opus-5-5] README.md:80–81 — "once m ≥ k/2" is not sharp for odd k; m ≥ ⌊k/2⌋ is enough. — high — "once m ≥ ⌊k/2⌋".
- **LOW** [claude:claude-opus-5-5] README.md:118–119 — "two independent proofs": both upper bounds analyse the same strategy, and everything is from one model family. — medium — "two different arguments".
- **LOW** [claude:claude-opus-5-5] README.md:120–122 — leaves out the referee's second finding on adv.py: legality is checked only on nodes popped before the first terminal. — medium — add that.
- **LOW** [claude:claude-opus-5-5] solver-b/proofs.txt:25 — garbled phrase "|W|>=k-1+... precisely". — high — delete it.
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/proofs.txt:98-100` — \(D\) is never initialized and the capacity-preserving eviction is omitted — confidence: high — define \(D=C\setminus C'\) and evict outside protected pages
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:63-66` — strategy description omits round 1 — confidence: high — add “load \(\{0,1\}\) on round 1”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:73-74` — assumes a first-round neighbour was retained — confidence: high — choose either absent side
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:79-80` — “\(k-1\) once” is misleading during repeated free steps — confidence: medium — say “until the first cycle completes”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `README.md:68-69` — “checked by program” overstates a capped diagnostic; `verifier/wk.cpp:122,130` labels reaching CAP as unbounded — confidence: medium — say “observed as unbounded in the tested cases”
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/game.py:57,97` — displayed cap differs from the cap used — confidence: high — print `2*m+3`
- **LOW** [cursor:gpt-5.6-sol-xhigh] `solver-a/mkwit.py:21,24` — witness header is glued to data and advertised recomputation is skipped for many rows — confidence: high — insert a newline and label unchecked rows explicitly
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:80` — “`2m≥k` (i.e. `m≥j`)” is false for odd `k` (`k=5`, `m=2`). — high — Drop the “i.e.”.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:61-62` — `B:=j` is written only in the even branch; the odd loop uses `B` anyway. — medium — Set `B:=j` before the parity split.
- **LOW** [cursor:grok-4.7-medium] `solver-a/proofs.txt:90` — A frontier request is outside `O` and outside the online cache, so it is not a hit inside both caches. — high — Say only the walk up to the frontier stays in `O`.
- **LOW** [cursor:grok-4.7-medium] `README.md:73-74` — “Prefetched on its first fault” excludes a first cache of `{0}`. — high — “toward a neighbour of 0 that round 1 did not cache”.
- **LOW** [cursor:grok-4.7-medium] `README.md:86-87` — “Had to drop pages … to make room” covers only the full-window counting case. — high — “then at least one window page is missing (at least two if `|O|=k`)”.
- **LOW** [cursor:grok-4.7-medium] `README.md:42` — “Every online reply” does not match mode-1 logs (`verifier/advb.txt`, `adva.txt`, `wk.cpp:10`). — high — “every inclusion-maximal reply”.
- **LOW** [cursor:grok-4.7-medium] `README.md:109-113` — Omits `prior-art.md:15-16` (Gaysinsky et al.: access-graph walks plus prefetch, different objective). — high — Cite it as the nearest partial combination.
- **LOW** [cursor:grok-4.7-medium] `verifier/table_mode2.txt:1` — The `k≤14`, `m≤6` table is mode 2 (domination pruning); unpruned `table_mode0.txt` is only a sparse set through `(8,4)`. — high — Say “exact under the domination prune”.
- **LOW** [cursor:gemini-3.8-flash-high] `solver-a/witnesses.txt:2` — The first data record `k=2 m=1 ...` is concatenated onto the header text without a newline, 12 of 24 rows lack unpruned verification, and all sample walks are straight lines that never exercise bounce or stage 2 — confidence: high — Add a newline before data records and provide sample plays that exercise cycles and bounces
- **LOW** [cursor:gemini-3.8-flash-high] `solver-a/game.py:97` — Output string reports `"(cap %d)"%(2*m+4)` but line 57 initializes `cap = 2*m+3` — confidence: high — Format the print statement with `cap` rather than `2*m+4`
- **LOW** [cursor:gemini-3.8-flash-high] `reports/solver-b.md:18` — Report claims unpruned checks agree on `k=2..4, m=1..3`, but `solver-b/table_py_noprune.txt` logs only 18 cases for `k=3..8, m=2..4` — confidence: high — Log the missing small cases into `table_py_noprune.txt` or adjust the text
- **LOW** [cursor:gemini-3.8-flash-high] `10-walk-paging/README.md:46` — Pruning status cell states "proved by hand, twice" but the 'Where' column cites only `solver-b/proofs.txt`, omitting Solver A's Lemma D — confidence: high — Add `solver-a/proofs.txt Lemma D` to the 'Where' column
- **LOW** [cursor:gemini-3.8-flash-high] `10-walk-paging/README.md:62-65` — "What the online cache does" assumes an existing interval window and omits the round-1 rule (loading {0, 1} from empty cache) — confidence: high — Clarify that round 1 loads {0, 1} from the empty cache
- **LOW** [cursor:gemini-3.8-flash-high] `10-walk-paging/README.md:72-89` — "What the adversary does" details only Solver B's adversary (straight walk then cycles), omitting Solver A's distinct construction (bounce, fill, pair/single blocks) — confidence: high — Note that the text describes Solver B's adversary and contrast with Solver A's strategy

Stop rule (memory js1-cold-review-convergence-lesson): stop when no CRITICAL and no real-bug MODERATE survives verification; MODERATE wording findings never run out.
