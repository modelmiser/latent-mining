<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:grok-4.7-medium

Q1: CONFIRMED — `conn` (Spec.lean:29-32) is `a==b` on `[]`, and on `(u,v)::D` is old connectivity or a crossing of undirected `{u,v}` either way; by induction that is the reflexive-transitive closure of adjacency for every arc list, including loops (the extra clause is already transitive), parallel edges, and cycles — none

Q2: CONFIRMED — `moves` (Spec.lean:35-37) keeps every pair with `conn` false, so `u=v` is absent and both orders of each unordered cross-component pair occur (SpecCheck.lean:26); both orders have the same `replies` (Spec.lean:40-42) and the same costs; from `[]`, each added edge joins distinct components and `reorient` only swaps endpoints, so the position stays a simple forest — none

Q3: CONFIRMED — `reorient` (Spec.lean:40-42) keeps or swaps each arc in place; `replies` (Spec.lean:46-47) is exactly the valid orientations of `(u,v)::D`, either direction of the new edge included; `outdeg` (Spec.lean:19) counts tails and `valid` (Spec.lean:22) requires ≤2 on all of `Fin n`; a reply cannot drop, duplicate, or retarget an edge — none

Q4: CONFIRMED — every reply is `dir :: E` with `E` a same-order orientation of `D` (Spec.lean:40-47); `cost` (Spec.lean:56) is `flips D D'.tail` (Spec.lean:50-52), so the new head is free and each reversed old arc counts 1 — none

Q5: CONFIRMED — `value` (Spec.lean:71-75) maximises over `moves`, then minimises over `replies`, of `cost` plus the value with one fewer round; `maxL [] = minL [] = 0` (Spec.lean:59-67) can only lower a score and does not arise for `T n` with `n≥1` (while rounds remain a forest is disconnected; `exists_npath` at Full.lean:260 plus `valid_nreply` at Reply.lean:43 give a reply); `T 1 = value 0 [] = 0`, and `T 0 = value 0 [] = 0` because `0-1=0` in `Nat` (Spec.lean:78), outside the informal game; round payoffs depend only on the current position, so backward induction equals optimal history-dependent deterministic play — none

Q6: CONFIRMED — Spec.lean:86-87 rearranges in ℚ to `T ≥ n/3 + 1 − (7/3)⌊log₂(n+1)⌋`, and `⌊log₂⌋≤log₂` with `1≥5/6` yields the informal bound (slack `1/6+(7/3)` times the fractional part); `Nat.log2` is that floor (`Nat.le_log2` used at Main.lean:19). Hand checks: n=1, `4≤7`; n=2, `5≤7`; n=8, `11≤3T+21` (true for every `T≥0`, and the README upper bound is `<5`); n=64, `log₂65=6`, so `67≤3T+42` forces `T≥9` while `T≤64−log₂65≤58` — not vacuous — none

Q7: CONFIRMED — for `n≥1`, `n-1` does not saturate and `Fin n` is nonempty; the empty max/min cannot make `T` larger than the game value; `T`, `value`, `moves`, `replies`, and `cost` are defined only in Spec.lean:35-78, and `lean/FO/*.lean` has no `notation`, `macro`, or `instance`; `main` (Full.lean:333) proves `MainStatement` (Spec.lean:86-87), and `T_lower_bound` (Main.lean:9) is that statement — none

Q8: REFUTED — SPEC.md:37-61's ten choices omit the `flips` catchall, which returns 0 when the lists differ in length (Spec.lean:52); SPEC.md:67-68's "T 5 = 0, 3 minutes" is not in the tree. The SpecCheck equalities (SpecCheck.lean:16-55) are concrete and can fail; the integer-form paragraphs (SPEC.md:26-34, README.md:127-133) match the arithmetic — add the length-mismatch totalization as a choice, and drop or file the T 5 timing

F: MODERATE — SpecCheck.lean:65 — the comment says `FalseVariants.lean.txt` has the cleanup-loss input and Lean's output; that file has four other counterexamples and no compiler output — high — point the comment at those four examples, or add the cleanup input and the error
F: LOW — Spec.lean:27 — the comment places `conn_iff` in `SpecCheck.lean`; the proof is `Graph.lean:127`, and SpecCheck.lean:12 only calls it — high — cite `Graph.lean`
F: LOW — Spec.lean:52 — a longer tail is ignored, so a mis-aligned reply would under-count flips; unused on real replies, and not listed in SPEC.md:37-61 — medium — state that choice next to item 6
F: LOW — SPEC.md:53 — "no legal reply ⇒ the rest of the game is worth 0"; `minL []` zeros only that move (Spec.lean:74-75) — high — say that move scores 0
F: LOW — SPEC.md:67 — the one-off `T 5` timing cannot be checked from the repo — medium — delete it or keep a log

Nothing else in the definitions makes `n≥1` `T_lower_bound` fail to imply the informal bound. The ordered-pair duplicate is disclosed (SPEC.md:48-49) and does not change the minimax value.
