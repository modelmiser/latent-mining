<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Q1: CONFIRMED** — `open-upper/proofs.txt:26-30` (W(iii): j≥1 by W(i), l−j≤c−1, exponent c−1−l+j≥c−l), `:42-50` (Reach(r): ≤r−1 extras, +1 placement, undo, then ≤r−1 more; undo moves have preconditions on nodes ≤x−2 only), `:51-55` (L2 gap sum ≤2^(r+1); termination+diamond ⇒ Newman), `:57-58` (L3 both directions via gap 1 ≤ 2^(q−|S'|) and L1), `:66-68`, `:70-71`, `:75-77` (lifted configs are originals ∪{z}; moves at z become no-ops; n≠z keeps n empty/pebbled). — none.

**Q2: CONFIRMED** — `proofs.txt:94-101`. Cases e=z / (e≠z, pred≠z) / (e≠z, pred=z) are exhaustive; the middle case gives gap_{A''}(e)=gap_{S0}(e)≤2^(q−|S0|)=2^((q−1)−|A''|), contradicting A'' reduced in the (q−1)-game; V=S0−w is a reduction step so V∈comp(A), z∈V, pred_V(z)<w<succ_V(z), w−z≤2^(q−|A''|−1). — none.

**Q3: CONFIRMED** — `proofs.txt:109-119`. (1) y∉{u',s} ⇒ pred_Q(y)=pred_V(y); (2) via Lemma N, Q0=A−u empty only because |A|≤1; (3a) direct; (3b) s−u'≤2^φ<s−u ⇒ u'>u; V≠A needs A∈{∅,{a}} (stated); e≠s since gap_V(s)>2^φ; e=u reducible ⇒ V1∈comp(A); pred_{V1}(s)=p<u'<s, shift s−u'≤2^φ=2^(q−(k−1)−1) exactly the size-(k−1) bound. No hidden use of A beyond (2) and (3b). — none.

**Q4: CONFIRMED** — `proofs.txt:136-168`. m=0: b_l≤v_l+2^(q−m'−1) (order preserved), b_m'≤4U−2^(r'−1), b_1≤3U (m'≥1). m=1: b_1≤a+2^(r'−1)≤a+U; max V≤a+2U−2^(q−m') (≤m'−1 elements above a) ⇒ b_m'≤a+2U−2^(r'−1). m≥2: max S≤(f+3)U+2^(r−1)−1≤(f+3)U+U/2−1; r'≤q−2 vs r'=q−1 split; (i') three sub-cases use L5 and W(i). Monotone in f so J(f)⇒J(f+1). m=q (r=0): 2^(r−1)=½, inequalities still hold but text `:138` says "Always m≤q−1" beside a theorem for unreachable boards. — add "for m=q read 2^(r−1)=½; the inequalities hold verbatim".

**Q5: CONFIRMED** — `proofs.txt:171-176`: n≤2U trivial; n>2U ⇒ {n} reduced ⇒ A={n}, f≥1; (b) A=∅ by W(ii), else L4 with r≤q−1; charged moves are relaxed moves and total charge pk drops by 1 per F, never below 0, so f≤pk. Independently re-ran a 0-1 BFS (q=2..4, with and without the F-legality rule): equality at every f. — expand `:176` to one sentence stating f≤pk's reason.

**Q6: CONFIRMED** — `solver-a/proofs.txt:12-15` (base pebble (j−1) mod p forgotten ⌊(j−1)/p⌋≤k−1 times; top pebble j mod p is next base), `:4-11` (L(q) reversal touches only g+1..g+h); `solver-b/proofs.txt:59-78` ({c,c−1} invariant with board pebble at c; total>0 ⇒ c≥1). — none.

**Q7: CONFIRMED** — `solver-a/proofs.txt:18-31` (all 8 transitions, y>x+1 pairs only arise from placing at 1 with x≤s+2); `solver-b/proofs.txt:104-112` (y−2≥1 empty while both down ⇒ removal of y−1 is F). — none.

**Q8: REFUTED (minor)** — `README.md:93` "reach about half as far per erasure" vs `prior-art.md:12`: Vitányi 2005 reaches (f+1)·2^(p−2), the same 2^(p−2) per erasure; item 1's own numbers (2m−1 erasures, m·2^(n−1)) also give 2^(n−2) per erasure. `README.md:9` "buys … and no more, however spent" is only true as "no more" (`README.md:98` shows 3,0,0 reaches 6). `README.md:67` "behind" reverses `proofs.txt:36`; `README.md:74` "forward" contradicts `proofs.txt:88,119` (retreat). `README.md:47` "two solvers agree" fails for (3,7) (`reports/solver-a.md:11`) and (2,15),(3,15) (`reports/solver-b.md` table). — see fixes below.

F: MODERATE — README.md:93, prior-art.md:8 — "about half as far per erasure" contradicts prior-art.md:12's own translation ((f+1)·2^(p−2)); the prior strategies have the same per-erasure rate, only the constant differs — medium — "whose strategies gain the same 2^(p−2) per erasure but start one round behind; no upper bound was found".
F: LOW — README.md:9 — "buys … and no more" overstates; only the upper bound holds for arbitrary spending — high — "No forgetting ever buys more than 2^(p−2) nodes, however spent; the strategy gets exactly that from each."
F: LOW — README.md:67-68 — reducible pebble sits close enough *above* its predecessor, not "behind another pebble" — high — "any pebble close enough above the pebble below it for the free pebbles to reach".
F: LOW — README.md:73-75 — shift may be backward (proofs.txt:119); "equivalent to" should be "has the same anchors as" — high.
F: LOW — README.md:47 — "two solvers agree" false for (3,7), (2,15), (3,15); referee covers them — high — "solvers and referee together cover every entry, each by ≥2 programs".
F: LOW — open-upper/proofs.txt:138-139 — "Always m≤q−1" is reachable-only; Theorem I covers m=q where 2^(r−1)=½ — high — state it.
F: LOW — open-upper/proofs.txt:63 — a_{m+1} undefined (should be ∞) — high.
F: LOW — open-upper/proofs.txt:29 — "as in B's proof of W(ii)" should say "by W(i) a nonempty set has an element ≤b" — high.
F: LOW — open-upper/proofs.txt:176 — f≤pk unjustified in text — high — add the charge-sum sentence.
F: LOW — open-upper/proofs.txt:153 — "f≥1" listed as a hypothesis in case m=1 but unused — low.

**Not asked:** nothing severe. The proof is correct as written; every lemma traced holds for general q. The only substantive weakness is that mechanical confirmation stops at q=6 and no proof assistant was used, which the README already says. The lower-bound witnesses and certificates were not re-run by me.

<!-- stderr: Warning: no stdin data received in 3s, proceeding without it. If piping from a slow command, redirect stdin explicitly: < /dev/null to skip, or wait longer.
 -->
