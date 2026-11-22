<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-opus-5-5

Q1: CONFIRMED. I traced every step in `open-upper/proofs.txt`. W(iii) (L26–30) holds: by W(i) there is always an element ≤ b, and l−j ≤ c−1. L1 (L42–50) holds: the Reach induction is sound, the undo only reads preconditions on nodes ≤ x−2, and |S|+r = q. L2 (L51–55) holds: the strong diamond gives a unique red(S) ⊆ S. L3 (L56–58) holds in both directions. L4 and L5 (L62–71) hold because every configuration on the path contains A, or contains A′. L6 (L72–77) holds: skipping the moves at z leaves the original configuration plus z, so preconditions still hold and there are at most q pebbles. — none

Q2: CONFIRMED. `proofs.txt:94–101` covers three exhaustive cases: e = z; e ≠ z with pred ≠ z, where the gap is unchanged and (q−1)−|A''| = c, so the contradiction is valid; and e = succ(z). In the last case V = S0−w lies in comp(A), the order pred_V(z) < w < succ_{S0}(w) holds, and w−z ≤ 2^(q−|A''|−1). — none

Q3: CONFIRMED. `proofs.txt:105–119`. (1): u ∉ Q and u′ ∉ V, so the gaps agree and the order survives deleting y, including y = p. (2): A−u = ∅ uses |A| ≤ 1, which is stated. (3a) holds. (3b): s−u′ ≤ 2^φ < s−u, so u′ > u. V has two or more elements, so V ≠ A. e ≠ s because gap_V(s) > 2^φ. If e ≠ u then e ∉ {u′, s}. If e = u, the backward shift s−u′ ≤ 2^φ = 2^(q−(k−1)−1) is the bound allowed at size k−1. |A| ≤ 1 is used only in (2) and (3b), and both places say so. — none

Q4: CONFIRMED. `proofs.txt:146–168`: I rechecked every inequality.
- m = 0: 2^q−2^(r′)+2^(r′−1) = 4U−2^(r′−1), and v_1+shift ≤ 3U.
- m = 1: max V ≤ a+2U−2^(q−m′), via W(iii) with l ≤ m′−1.
- m ≥ 2: in every subcase b_1 < 3U, or b_1 ≤ a_m + 2^(r−1) ≤ (f+3)U.
- J(f) ⇒ J(f+1) holds trivially.
- m = q (r = 0): the cases still go through, since T_m = ∅ and the half-integer bound is harmless. It only arises on unreachable boards. — L138: add "(for reachable boards)" after "Always m ≤ q−1".

Q5: CONFIRMED. `proofs.txt:171–176`. f = 0 forces A = ∅, so n ≤ 2U. A board {n} with n ≤ 2U is trivial, and with n > 2U it is reduced. The A = ∅ case of (b) uses W(ii). In the real game, F is a relaxed F, each F lowers the total charge, so f ≤ pk, and the bounds are monotone in f. — L176: add "since each F lowers the total charge pk by one"

Q6: CONFIRMED. `solver-a/proofs.txt:90–102`: L(q)'s reversed pass is legal, and pebble (j−1) mod p has been forgotten ⌊(j−1)/p⌋ ≤ k−1 times. `solver-b/proofs.txt:13–32`: Clean/Frontier is sound, and the {c, c−1} invariant gives c ≥ 1 while the total charge is positive. m ≥ 2^(p−1) ≥ 2. — none

Q7: CONFIRMED. `solver-a/proofs.txt:106–117`: all six transitions check, including x = 1 for R on x. `solver-b/proofs.txt:59–66`: node y−2 ≥ 1 is empty while both pebbles are down, so the first removal of y−1 is an F-move. — none

Q8: REFUTED, on three points (detailed in F2–F5 below):
- The README's prior-art sentence (`README.md:93`) contradicts the arithmetic in `prior-art.md`.
- `README.md:73–75` says "sliding forward", but the proof allows backward shifts.
- `README.md:47` says "two solvers agree" for values only one solver computed.

The Status cells for C1–C4 match `reports/referee.md:11–12, 53` and `reports/open-upper-referee.md:3, 48`. — see F lines

F1: none CRITICAL — the headline proof chain (C1–C4) holds under my trace.
F2: MODERATE — `README.md:93`, `prior-art.md:8,12` — The claim that Li and Vitányi's strategies "reach about half as far per erasure" contradicts prior-art.md's own figures.
- Item 1 gives m·2^(n−1) for 2m−1 erasures, which is (e+1)·2^(n−2).
- Item 3 gives (f+1)·2^(p−2).
- Both have the same per-erasure slope, 2^(p−2), as this work. They fall short by only an additive 2^(p−2) or less.
— confidence: medium (depends on prior-art.md's transcription being faithful) — fix: "found their strategies, which gain the same 2^(p−2) per erasure but reach one step less (and not as a clean delivery)".
F3: LOW — `README.md:73–75` — "sliding one pebble forward by at most half the reach". The proof allows |u′−u| (`proofs.txt:88`), and in (3b) the pebble "retreats" (L119). Also, the lemma describes the new anchor set; it does not say "forgetting is equivalent" — confidence: high — fix: "the new anchor set is a board of the old class with one pebble shifted, forward or back, by at most half the reach of the free pebbles, without passing a neighbour".
F4: LOW — `README.md:9–10` — "Each forgetting buys 2^(p−2) more nodes and no more, however the forgettings are spent" reads as an exact per-forgetting gain. Forgettings can be wasted, and the README's own unequal-charge data (3,0,0 → 6, L98) gains less than this — confidence: high — fix: "Each forgetting buys at most 2^(p−2) more nodes, however it is spent, and the rotation strategy gets exactly that."
F5: LOW — `README.md:47` — "two solvers agree" is wrong for some entries. Solver A has no (2,6), (2,7), (2,9), (2,10) or (3,7) (`reports/solver-a.md` table). Solver B has no (2,15) or (3,15) (`reports/solver-b.md` table) — confidence: high — fix: "each value computed by at least one solver (both agree where both ran); referee recomputed all but …".
F6: LOW — `README.md:76–77` — "an invariant … holds for every board satisfying it, reachable or not" is circular. The intended meaning is that the invariant is preserved by any deletion from any board satisfying it — confidence: high — fix: "an invariant that any deletion preserves, from every board satisfying it, reachable or not".
F7: LOW — `open-upper/proofs.txt:138` — "Always m ≤ q−1" sits next to the "reachable or not" claim, but it holds only for reachable boards. The m = q case does go through (r = 0, T_m = ∅) — confidence: high — fix: as in Q4.
F8: LOW — `README.md:67–68` — "deleting any pebble that sits close enough behind another pebble". The deleted pebble is the higher one, and the threshold 2^(free count) changes as pebbles are deleted — confidence: medium — fix: "repeatedly deleting any pebble within 2^(free pebbles) of the pebble below it".
F9: LOW — `open-upper/proofs.txt:28` — W(iii) relies on "as in B's proof of W(ii)" for the bound of c−1 elements; the actual reason is W(i) — confidence: high — fix: "by W(i) a nonempty set always has an element ≤ b, so the nodes > b number ≤ c−1".
F10: LOW — `README.md:45` — "I read it once and checked the arithmetic of Part 3" has no artifact in the repository to back it — confidence: low — fix: none needed beyond noting it is unrecorded.

**Not asked about:**
- Everything I traced is by hand, as is everything the panel traced. My check agrees with the same-family referee, so it is still not a machine-checked proof; a proof-assistant formalisation of Lemma Step and Theorem I would close this.
- The README (`README.md:47`) omits solver B's q = 6, f ≤ 6 relaxed run, and its C(6,1) = 128 conclusion (an upper bound from that run plus the proved lower bound). The referee lists C(6,1) = 128 as unsupported by them (`reports/referee.md:18`). This is harmless now that C3 is proved.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
