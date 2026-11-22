<!-- Archived by the organiser on 2026-11-21 from the review of this experiment before its publication. Report of one reviewer seat (or, for summary.md, the machine-made summary of the round). Verbatim except that absolute paths are shortened. It was written about the files BEFORE the findings were applied, so line numbers and quoted sentences may not match the current files. -->

# claude:claude-fable-5-1

**Verdicts**

Q1: CONFIRMED — NOTE.md:48-54 defines encoding (each position constant or determined by one bit and differing between its two values) and protocol (cyclic equality ⟺ equal f). For non-constant f this is equivalent to "z_0,z_1 not cyclically equal, s(x)~z_{f(x)}" (take z_b = any row with f=b; conversely transitivity of rotation); for constant f both are vacuous. Pairs (x_i,x̄_i) under a known permutation with inserted fixed words are an encoding with |P_i|=2 (prior-art-deep.md:10-11 agrees). — none.

Q2: CONFIRMED — I re-derived every step at NOTE.md:66-136: (∗)/(∗∗) by substitution of 2a_i = W−z^{s_i}W; the unordered-pair claim (intersection of two unit circles); (N) gives r·s_ik=θ_i+θ_k; (X) gives θ_i=θ_k+m/2, r·s_ik=m/2, and −2θ_k; "at most one exceptional pair" (l.108-112) correct; r·t formula (l.115) checks since Σ_pairs(s_ik−s_i−s_k) = Σs_ik − 2Σs_i; both final cases give ω^{rt}; t is independent of r, so inverse DFT gives s(1,1,1)=z^tW; corner reduction (l.132-136) gives a_i'=±a_i, W' = opposite corner. Parenthetical (l.72-74) is true and is actually needed for the corner step. I also random-tested the lemma in the general affine form (rational entries, overlapping supports, m≤12): 4040 configurations with seven rotations, 0 failures; and reproduced the m≤9 frequency count exactly (1,011,693 cases, 0 violations). — none.

Q3: CONFIRMED — NOTE.md:140-145 is a correct application (fixed bits make their cards constant). AND/OR (l.150-151), equality n≥4 with x_4..x_n=0 (l.152-154), majority-5 with two bits fixed to 1 = OR of three (l.155-156) all check. "Exactly seven of eight" ⟺ indicator of one point or its complement ⟺ AND of three literals or its negation (l.147-148). The two non-settled items (l.168-175) match prior-art-deep.md:16-17. — l.148: "or the negation of such an AND".

Q4: CONFIRMED — Proof at NOTE.md:194-214 is complete: inversion formula is Σ_j Π_{q∈R}(1+t_q t^{(j)}_q)/2 expanded; B_v = c[v=0]+2^{−n}δ(−1)^{|v|} from Σ_x(−1)^{x·v}=2^n[v=0]. Matches solver-b/proofs.txt:22-34. Further facts match T5 (l.119-123), T4 (l.88-89,109), C3 (l.66), L1 (l.16-20). The three 36-char strings at NOTE.md:233-235 are character-identical to solver-a/w_n3_m36.txt:4-6; R = multiples of 6 (file l.7); owner 0 ⇒ u0=u1 at all such positions; I re-verified from the definition: seven AND-0 inputs share one multiset of 33 distinct words, M(1,1,1) has 16 disjoint words. Colour convention 1=♥ matches solver-a/check.py:7. — none.

Q5: UNSUPPORTED (two sentences) — NOTE.md:250-252 attributes the n≥4 equality question to [7] "as reported in [2]"; no repository file records this (prior-art-deep.md:34 only says P2 lists it as open). NOTE.md:263-264 says [3] shows "for three-variable functions no new protocols exist" with one helper colour; prior-art-deep.md:17 documents only the AND result. Everything else (l.9-16, 56-62, 191, 239-240, 269-277) is consistent with prior-art-deep.md and hedged (l.303-307, 279-280). — l.251: drop "who asked … (as reported in [2])" or cite [2] directly; l.263: "show, for three-bit AND (and, per their abstract, other three-variable classes), that…" only if verified.

Q6: CONFIRMED with one gap — Panel: README.md:225-245 (five reviewers, four families). Referee m≤40 test: reports/referee.md:34, verifier/tests.py:74-94, tests.out:32 (3519/1140/0). fullR m≤18: solver-b/proofs.txt:64, fullR.out. Agreement with definition-level search: solver-b/slow.out (n=2 m≤7; n=3 m≤6), referee.md:41. Normalisation caveat: referee.md:64. Gap: the "m≤9, 1,011,693 cases" test (l.296-298) has no file in the folder (I reproduced the number, but the note says the record is in the folder, l.329-330). Provenance is at l.3-5, l.31-33, l.289-290 and Section 6 — adequate. — l.298: add the script path or "(not recorded)".

Q7: CONFIRMED (minor) — l.18-20 uses "depends on" and "row" before Section 1 defines them; l.148 "negation of one" is ambiguous; l.110 "θ₂ = θ₁ + m/2" silently uses m/2 ≡ −m/2; l.125 "ŝ(1,1,1)(r)" notation never introduced; l.77 rotation direction unspecified (harmless). — see F lines.

**Findings**

F: MODERATE — NOTE.md:263-264 — "[3] … show that for three-variable functions no new protocols exist when all additional cards have the same colour": repository evidence (prior-art-deep.md:17) covers only three-bit AND; the Summary (l.14-15) claims only AND. Overstated relative to documented reading. — confidence: medium — restrict to what prior-art-deep records, or mark as the instance's reading.

F: MODERATE — NOTE.md:250-252 — attribution of the n≥4 equality question to Heather–Schneider–Teague/Shinagawa–Mizuki "as reported in [2]" has no source in the folder; [6],[7] were not read (l.306-307). — confidence: medium — cite only [2]'s open-problem list.

F: MODERATE — NOTE.md:161-164 — the "consistency check" enumerating Table 2 of [4] (eight restrictions, 4/4 split) has no script, output or truth table in the folder; unverifiable. — confidence: high — include the truth table or drop the sentence.

F: LOW — NOTE.md:296-298 — the m≤9 test (1,011,693 cases) is not in the folder although l.329-330 says the full record is; count reproduces. — confidence: high — add file or say "not recorded".

F: LOW — NOTE.md:288-289 — "adds the remark on m even": solver-b/proofs.txt:51 already says "(m even)"; the remark was missing only from README's write-up. — confidence: high — "adds the remarks on the choice of two pairs and on an arbitrary corner".

F: LOW — NOTE.md:148 — "the negation of one" could mean a negated literal. — confidence: high — "or the negation of such an AND".

F: LOW — NOTE.md:18-20 — Lemma 1 in the Summary uses "depends on" (meaning: the card is flipped) and two-colour cards before Section 1 defines them. — confidence: medium — add "(Section 1)" or "each card is ♣ or ♥ and is changed by at most one of them".

F: LOW — NOTE.md:110, 125 — "θ₂ = θ₁ + m/2" needs the reader to note m/2 ≡ −m/2; ŝ(1,1,1)(r) undefined notation. — confidence: high — write "θ₂ ≡ θ₁ − m/2 ≡ θ₁ + m/2" and "the transform of s(1,1,1) at r".

**Not asked, worth noting**

- The headline mathematics (Lemma 1, Theorem 2, Proposition 3, 36-card witness) all hold under my independent re-derivation and computation; the defects are confined to literature attribution and record-keeping.
- The "none with m ≤ 15" claim (l.222-224) correctly flags shared normalisation; referee only recomputed m ≤ 13 (referee.md:17) — the note states this accurately.
