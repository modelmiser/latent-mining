<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# cursor:gpt-5.6-sol-xhigh

Q1: CONFIRMED — W(iii) reserves one pebble below its split; L1’s construction stays within q pebbles; L2’s doubled threshold gives the diamond; L3–L6 then follow (`open-upper/proofs.txt:25-31`, `open-upper/proofs.txt:41-78`) — none

Q2: CONFIRMED — the exhaustive alternatives are e=z, e≠z with predecessor not z (contradicting reducedness in the q−1 game), and e=succ(z), which constructs the required V (`open-upper/proofs.txt:92-101`) — none

Q3: CONFIRMED — in (3b), s−u′≤2^φ<s−u implies u′>u; V≠A supplies reducible e≠s; when e=u, the new size k−1 permits exactly shift 2^φ (`open-upper/proofs.txt:103-119`) — none

Q4: CONFIRMED — the m=0, m=1, and m≥2 estimates establish both bounds (`open-upper/proofs.txt:146-168`); for m=q, S=A and the same algebra works with 2^(r−1)=1/2 — “For reachable post-forgetting boards m≤q−1; arbitrary boards may have m=q, for which the formulas remain valid.”

Q5: CONFIRMED — the n≤2U case, reduced-singleton case, empty-anchor case, and nonempty-anchor case are all covered (`open-upper/proofs.txt:170-176`); every real F-move consumes one of pk charge units (`solver-b/proofs.txt:6-7`) — none

Q6: CONFIRMED — solver A’s cyclic base has been forgotten floor((j−1)/p)≤k−1 times and its reversible subroutine may select the top pebble (`solver-a/proofs.txt:4-16`); solver B’s maximal-charge invariant gives c≥1 whenever charge remains (`solver-b/proofs.txt:13-32`) — none

Q7: CONFIRMED — solver A exhausts every one- and two-pebble move (`solver-a/proofs.txt:18-31`); solver B proves removal of y−1 after y’s last placement must be forgetful (`solver-b/proofs.txt:58-66`) — none

Q8: REFUTED — “about half as far per erasure” (`README.md:92-94`) contradicts the cited formulas: 2m−1 erasures reaching m·2^(n−1) has the same asymptotic gain 2^(n−2) per erasure (`prior-art.md:7-12`); the “forward shift” explanation also omits deletion of the sole anchor (`README.md:72-75`; `open-upper/proofs.txt:92-101`) — “Published strategies have the same asymptotic gain per erasure but a lower additive baseline; a deletion may instead remove the sole anchor, leave the class unchanged, or produce one forward shift.”

F: MODERATE — `README.md:92-94` — “about half as far per erasure” is arithmetically false under the repository’s own prior-art formulas — confidence: high — say “same asymptotic gain, one 2^(p−2) block shorter for fixed f”

F: MODERATE — `README.md:72-75` — a sole-anchor deletion yields empty, not a shifted same-class board — confidence: high — enumerate Lemma N’s three outcomes

F: MODERATE — `open-upper/proofs.txt:138-141` — “Always m≤q−1” is false for the theorem’s unreachable boards; e.g. q=2, A={2,4}, f=2 satisfies J with m=q — confidence: high — restrict the assertion to reachable post-F boards

F: LOW — `open-upper/proofs.txt:62-68` — a_{m+1} is never defined although T_m is later used — confidence: high — define a_{m+1}=+∞

F: LOW — `open-upper/proofs.txt:88` — succ_V(u) is undefined when u is maximal — confidence: high — define a missing successor as +∞

Unasked: no additional correctness issue found.
