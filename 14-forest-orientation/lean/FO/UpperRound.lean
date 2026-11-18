/-
U3, part 2.  One round of rule L: the vertices of the flipped path gain at least three times
the cost (Lemma 8 on real positions), no other weight decreases (Lemma 7), hence
`Phi` rises by at least `3 · cost` (`round_gain`).
-/
import FO.UpperPath
namespace FO
open Upper
variable {n : Nat}

theorem flagNe_shift (l a b : Nat) : flagNe (l + a) (l + b) = flagNe a b := by
  simp only [flagNe]; ifs_omega

theorem flagNe_base (l a : Nat) : flagNe l (l + a) = flagNe a 0 := by
  simp only [flagNe]; ifs_omega

/-- The out-neighbours of a saturated vertex `w` with out-neighbour `s`: `s` and one other. -/
theorem other_out {D : Pos n} (hN : D.Nodup) {w s : Fin n} (hs : outdeg D w = 2)
    (hws : (w, s) ∈ D) : ∃ c, c ≠ s ∧ (w, c) ∈ D ∧ ∀ o, (w, o) ∈ D ↔ o = s ∨ o = c := by
  obtain ⟨a, b, hab, ha, hb, hall⟩ := two_out hN hs
  rcases hall s hws with rfl | rfl
  · exact ⟨b, Ne.symm hab, hb, fun o => ⟨hall o, by rintro (rfl | rfl) <;> assumption⟩⟩
  · exact ⟨a, hab, ha, fun o => ⟨fun h => (hall o h).symm, by rintro (rfl | rfl) <;> assumption⟩⟩

/-- THE PROFILE OF A SUFFIX of the flipped path.  The path is `x :: (pre ++ w :: suf)`, a
shortest path from `x` to an unsaturated vertex.  There is a profile `(rest, z)` in the sense
of UpperCore such that the old weights of `w :: suf` add up to `oldRest rest z`, and, IF the
predecessor of `w` has risen by `o` and has new flag `e`, the new weights add up to
`newRest o e rest z`; moreover `2h + ε` does not decrease on `w :: suf`. -/
theorem suffix_profile {D : Pos n} {x y : Fin n} (hF' : Forest D) (hV : Valid D)
    (hc : ¬ Conn D x y) : ∀ (suf pre : List (Fin n)) (w : Fin n),
    NPath D x (pre ++ w :: suf) → (pre ++ w :: suf).length = hh D x →
    ∃ (rest : List (Nat × Nat)) (z : Option (Nat × Nat)),
      rest.length = suf.length ∧ (∀ c, c ∈ rest → c.2 ≤ 1) ∧ (∀ c, z = some c → c.2 ≤ 1) ∧
      eps D w = headFlag rest z ∧
      ((w :: suf).map (wv D)).sum = oldRest rest z ∧
      ∀ o e, hh (nreply D x y (pre ++ w :: suf)) (lastV x pre) = suf.length + 1 + o →
        eps (nreply D x y (pre ++ w :: suf)) (lastV x pre) = e → e ≤ 1 →
        ((w :: suf).map (wv (nreply D x y (pre ++ w :: suf)))).sum = newRest o e rest z ∧
        ∀ q, q ∈ w :: suf → 2 * hh D q + eps D q
          ≤ 2 * hh (nreply D x y (pre ++ w :: suf)) q + eps (nreply D x y (pre ++ w :: suf)) q := by
  have hN := forest_nodup D hF'
  intro suf
  induction suf with
  | nil =>
    intro pre w hp hlen
    have hF2 : Forest (nreply D x y (pre ++ [w])) := forest_nreply hF' hc
    have hN2 := forest_nodup _ hF2
    have hlt : outdeg D w < 2 := npath_suffix pre hp
    have hpD : (lastV x pre, w) ∈ D := npath_sub hp _ (by
      rw [pathArcs_append]; exact List.mem_append_right _ (List.mem_cons_self ..))
    have harc := arc_nreply_mid (y := y) hF' hp
    have hhw : hh D w = 0 := hh_unsat (by omega)
    rcases outs_shape hV hN w with ho | ⟨z0, ho⟩ | ⟨a, b, _, ho⟩
    · -- the end vertex had no out-neighbour
      have hno : ∀ o, (w, o) ∉ D := by
        intro o h; have := mem_outs.mpr h; rw [ho] at this; cases this
      refine ⟨[], none, rfl, (by intro c h; cases h), (by intro c h; cases h), ?_, ?_, ?_⟩
      · rw [eps_nil ho]; rfl
      · simp only [List.map_cons, List.map_nil, List.sum_cons, List.sum_nil, wv_nil ho, oldRest]
        rfl
      · intro o e ho' he _
        have h1 : ∀ o', (w, o') ∈ nreply D x y (pre ++ [w]) ↔ o' = lastV x pre := by
          intro o'; rw [harc]
          constructor
          · rintro (⟨h, _⟩ | h)
            · exact absurd h (hno o')
            · exact h
          · exact Or.inr
        obtain ⟨s1, s2, s3⟩ := spec1 hN2 h1
        have ho'' : hh (nreply D x y (pre ++ [w])) (lastV x pre) = 1 + o := by
          rw [ho']; simp only [List.length_nil]
        constructor
        · simp only [List.map_cons, List.map_nil, List.sum_cons, List.sum_nil, newRest]
          rw [s3, ho'', he]; omega
        · intro q hq
          simp only [List.mem_cons, List.not_mem_nil, or_false] at hq
          subst hq
          rw [s1, s2, eps_nil ho, hhw]; omega
    · -- the end vertex had one out-neighbour `z0`
      have hz0 : (w, z0) ∈ D := mem_outs.mp (by rw [ho]; simp)
      have hmemD : ∀ o, (w, o) ∈ D ↔ o = z0 := by intro o; rw [← mem_outs, ho]; simp
      have hzp : z0 ≠ lastV x pre := fun e => forest_antisymm D hF' _ _ hpD (e ▸ hz0)
      refine ⟨[], some (hh D z0, eps D z0), rfl, (by intro c h; cases h), ?_, ?_, ?_, ?_⟩
      · intro c h; cases h; exact eps_le D z0
      · rw [eps_one ho]; rfl
      · simp only [List.map_cons, List.map_nil, List.sum_cons, List.sum_nil, wv_one ho, oldRest]
        omega
      · intro o e ho' he _
        have h2 : ∀ o', (w, o') ∈ nreply D x y (pre ++ [w]) ↔ o' = z0 ∨ o' = lastV x pre := by
          intro o'; rw [harc]
          constructor
          · rintro (⟨h, _⟩ | h)
            · exact Or.inl ((hmemD o').mp h)
            · exact Or.inr h
          · rintro (h | h)
            · exact Or.inl ⟨(hmemD o').mpr h, by intro s e; cases e⟩
            · exact Or.inr h
        obtain ⟨k1, k2⟩ := keep hF' hV hc hp (fun q => Reach D z0 q)
          (fun c o hc' ho' => hc'.tail ho')
          (fun q hq => side_avoid hF' hp hz0 (by intro s e; cases e) hq) z0 (RT.refl _)
        obtain ⟨s1, s2, s3⟩ := spec2 hF2 hzp h2
        have ho'' : hh (nreply D x y (pre ++ [w])) (lastV x pre) = 1 + o := by
          rw [ho']; simp only [List.length_nil]
        constructor
        · simp only [List.map_cons, List.map_nil, List.sum_cons, List.sum_nil, newRest]
          rw [s3, k1, k2, ho'', he]; omega
        · intro q hq
          simp only [List.mem_cons, List.not_mem_nil, or_false] at hq
          subst hq
          rw [s1, hhw, eps_one ho]; omega
    · rw [outdeg_eq_length, ho] at hlt; simp at hlt
  | cons s suf' ih =>
    intro pre w hp hlen
    have e : pre ++ w :: s :: suf' = (pre ++ [w]) ++ s :: suf' := by simp
    have hp' : NPath D x ((pre ++ [w]) ++ s :: suf') := e ▸ hp
    have hlen' : ((pre ++ [w]) ++ s :: suf').length = hh D x := e ▸ hlen
    obtain ⟨rest', z, hl, hfl, hzf, hes, hold, hnew⟩ := ih (pre ++ [w]) s hp' hlen'
    rw [← e, lastV_append] at hnew
    have hF2 : Forest (nreply D x y (pre ++ w :: s :: suf')) := forest_nreply hF' hc
    have hpw : NPath D w (s :: suf') := npath_suffix pre hp
    obtain ⟨c, hcs, hwc, hmem⟩ := other_out hN hpw.1 hpw.2.1
    have hhw : hh D w = (s :: suf').length := spath_heights hF' pre hp hlen
    have hhs : hh D s = suf'.length := spath_heights hF' (pre ++ [w]) hp' hlen'
    obtain ⟨d1, d2, d3⟩ := spec2 hF' (Ne.symm hcs) hmem
    simp only [List.length_cons] at hhw
    obtain ⟨al, hal⟩ : ∃ al, hh D c = suf'.length + al := ⟨hh D c - suf'.length, by omega⟩
    have hpD : (lastV x pre, w) ∈ D := npath_sub hp _ (by
      rw [pathArcs_append]; exact List.mem_append_right _ (List.mem_cons_self ..))
    have hcp : c ≠ lastV x pre := fun e => forest_antisymm D hF' _ _ hpD (e ▸ hwc)
    refine ⟨(al, eps D c) :: rest', z, by simp [hl], ?_, hzf, ?_, ?_, ?_⟩
    · intro c' hc'
      rcases List.mem_cons.mp hc' with rfl | hc'
      · exact eps_le D c
      · exact hfl c' hc'
    · rw [d2, hhs, hal, flagNe_base]; rfl
    · rw [List.map_cons, List.sum_cons, hold, d3, hhs, hes, hal]
      simp only [oldRest, hl]
    · intro o e' ho' he hele
      have harc := arc_nreply_mid (y := y) hF' hp
      have h2 : ∀ o', (w, o') ∈ nreply D x y (pre ++ w :: s :: suf') ↔
          o' = c ∨ o' = lastV x pre := by
        intro o'; rw [harc]
        constructor
        · rintro (⟨h, hne⟩ | h)
          · rcases (hmem o').mp h with rfl | rfl
            · exact absurd rfl (hne suf')
            · exact Or.inl rfl
          · exact Or.inr h
        · rintro (h | h)
          · subst h
            exact Or.inl ⟨hwc, fun suf'' e => hcs (by injection e with e1 _; exact e1.symm)⟩
          · exact Or.inr h
      obtain ⟨k1, k2⟩ := keep hF' hV hc hp (fun q => Reach D c q)
        (fun c o hc' ho' => hc'.tail ho')
        (fun q hq => side_avoid hF' hp hwc
          (fun suf'' e => hcs (by injection e with e1 _; exact e1.symm)) hq) c (RT.refl _)
      obtain ⟨n1, n2, n3⟩ := spec2 hF2 hcp h2
      simp only [List.length_cons] at ho'
      rw [k1, ho', hal] at n1 n2
      rw [k1, k2, ho', he, hal] at n3
      have m1 : hh (nreply D x y (pre ++ w :: s :: suf')) w
          = suf'.length + 1 + min al (o + 2) := by rw [n1]; omega
      have m2 : eps (nreply D x y (pre ++ w :: s :: suf')) w = flagNe al (o + 2) := by
        have e2 : suf'.length + 1 + 1 + o = suf'.length + (o + 2) := by omega
        rw [n2, e2, flagNe_shift]
      obtain ⟨hsum, hcode⟩ := hnew (min al (o + 2)) (flagNe al (o + 2)) m1 m2 (flagNe_le _ _)
      constructor
      · rw [List.map_cons, List.sum_cons, hsum, n3]
        simp only [newRest, hl]
      · intro q hq
        rcases List.mem_cons.mp hq with rfl | hq
        · rw [m1, m2, hhw, d2, hhs, hal, flagNe_base]
          have c1 := flagNe_cases al 0
          have c2 := flagNe_cases al (o + 2)
          omega
        · exact hcode q hq

/-- LEMMA 8 ON A REAL POSITION.  `x`, `y` in different trees, `ws` a shortest path from `x` to
an unsaturated vertex, and `x` chosen by the rule `2h(x) + ε(x) ≤ 2h(y) + ε(y)` (this contains
rule L, see SPEC.md).  Then the path vertices together gain at least `3 · |ws|`, and `2h + ε`
does not decrease on the path. -/
theorem path_round {D : Pos n} {x y : Fin n} {ws : List (Fin n)} (hF' : Forest D) (hV : Valid D)
    (hc : ¬ Conn D x y) (hp : NPath D x ws) (hlen : ws.length = hh D x)
    (hrule : 2 * hh D x + eps D x ≤ 2 * hh D y + eps D y) :
    ((x :: ws).map (wv D)).sum + 3 * (ws.length : Int)
        ≤ ((x :: ws).map (wv (nreply D x y ws))).sum ∧
    ∀ q, q ∈ x :: ws →
      2 * hh D q + eps D q ≤ 2 * hh (nreply D x y ws) q + eps (nreply D x y ws) q := by
  have hN := forest_nodup D hF'
  have hF2 : Forest (nreply D x y ws) := forest_nreply hF' hc
  have hN2 := forest_nodup _ hF2
  obtain ⟨ky1, ky2⟩ := keep hF' hV hc hp (fun c => ¬ Conn D x c)
    (fun c o hc' ho h => hc' (h.trans (RT.single (Or.inr ho))))
    (fun c hc' hm => hc' (path_conn hp hm)) y hc
  have hey := eps_le D y
  have hex := eps_le D x
  have harc := arc_nreply_t (hd := y) hF' hp
  cases ws with
  | nil =>
    have hlt : outdeg D x < 2 := hp
    have hhx : hh D x = 0 := hh_unsat (by omega)
    rcases outs_shape hV hN x with ho | ⟨z0, ho⟩ | ⟨a, b, _, ho⟩
    · have h1 : ∀ o, (x, o) ∈ nreply D x y [] ↔ o = y := by
        intro o; rw [harc]
        constructor
        · rintro (h | ⟨h, _⟩)
          · exact h
          · have := mem_outs.mpr h; rw [ho] at this; cases this
        · exact Or.inl
      obtain ⟨s1, s2, s3⟩ := spec1 hN2 h1
      constructor
      · simp only [List.map_cons, List.map_nil, List.sum_cons, List.sum_nil, List.length_nil]
        rw [s3, wv_nil ho]
        have := free_none (hh (nreply D x y []) y) (eps (nreply D x y []) y)
        omega
      · intro q hq
        simp only [List.mem_cons, List.not_mem_nil, or_false] at hq
        subst hq
        rw [hhx, eps_nil ho]; omega
    · have hz0 : (x, z0) ∈ D := mem_outs.mp (by rw [ho]; simp)
      have hmemD : ∀ o, (x, o) ∈ D ↔ o = z0 := by intro o; rw [← mem_outs, ho]; simp
      have hzy : z0 ≠ y := fun e => hc (e ▸ RT.single (Or.inl hz0))
      have h2 : ∀ o, (x, o) ∈ nreply D x y [] ↔ o = z0 ∨ o = y := by
        intro o; rw [harc]
        constructor
        · rintro (h | ⟨h, _⟩)
          · exact Or.inr h
          · exact Or.inl ((hmemD o).mp h)
        · rintro (h | h)
          · exact Or.inr ⟨(hmemD o).mpr h, by intro s e; cases e⟩
          · exact Or.inl h
      obtain ⟨k1, k2⟩ := keep hF' hV hc hp (fun q => Reach D z0 q)
        (fun c o hc' ho' => hc'.tail ho')
        (fun q hq => side_avoid_x hF' hp hz0 (by intro s e; cases e) hq) z0 (RT.refl _)
      obtain ⟨s1, s2, s3⟩ := spec2 hF2 hzy h2
      have hex1 : eps D x = 1 := eps_one ho
      constructor
      · simp only [List.map_cons, List.map_nil, List.sum_cons, List.sum_nil, List.length_nil]
        rw [s3, wv_one ho, k1, k2, ky1, ky2]
        have := free_some (hh D y) (eps D y) (hh D z0) (eps D z0) hey (eps_le D z0) (by omega)
        omega
      · intro q hq
        simp only [List.mem_cons, List.not_mem_nil, or_false] at hq
        subst hq
        rw [s1, hhx, hex1]; omega
    · rw [outdeg_eq_length, ho] at hlt; simp at hlt
  | cons s suf =>
    obtain ⟨rest, z, hl, hfl, hzf, hes, hold, hnew⟩ := suffix_profile (y := y) hF' hV hc suf [] s hp hlen
    obtain ⟨c, hcs, hxc, hmem⟩ := other_out hN hp.1 hp.2.1
    have hhs : hh D s = suf.length := spath_heights hF' [] hp hlen
    simp only [List.length_cons] at hlen
    obtain ⟨d1, d2, d3⟩ := spec2 hF' (Ne.symm hcs) hmem
    obtain ⟨al, hal⟩ : ∃ al, hh D c = suf.length + al := ⟨hh D c - suf.length, by omega⟩
    obtain ⟨be, hbe⟩ : ∃ be, hh D y = suf.length + 1 + be :=
      ⟨hh D y - (suf.length + 1), by omega⟩
    have hcy : c ≠ y := fun e => hc (e ▸ RT.single (Or.inl hxc))
    have hne : ∀ ws', s :: suf ≠ c :: ws' :=
      fun ws' e => hcs (by injection e with e1 _; exact e1.symm)
    have h2 : ∀ o, (x, o) ∈ nreply D x y (s :: suf) ↔ o = c ∨ o = y := by
      intro o; rw [harc]
      constructor
      · rintro (h | ⟨h, hn⟩)
        · exact Or.inr h
        · rcases (hmem o).mp h with rfl | rfl
          · exact absurd rfl (hn suf)
          · exact Or.inl rfl
      · rintro (h | h)
        · subst h; exact Or.inr ⟨hxc, hne⟩
        · exact Or.inl h
    obtain ⟨k1, k2⟩ := keep hF' hV hc hp (fun q => Reach D c q)
      (fun c o hc' ho' => hc'.tail ho')
      (fun q hq => side_avoid_x hF' hp hxc hne hq) c (RT.refl _)
    obtain ⟨n1, n2, n3⟩ := spec2 hF2 hcy h2
    rw [k1, ky1, hal, hbe] at n1 n2
    rw [k1, k2, ky1, ky2, hal, hbe] at n3
    have m1 : hh (nreply D x y (s :: suf)) x = suf.length + 1 + min al (be + 1) := by
      rw [n1]; omega
    have m2 : eps (nreply D x y (s :: suf)) x = flagNe al (be + 1) := by
      have e2 : suf.length + 1 + be = suf.length + (be + 1) := by omega
      rw [n2, e2, flagNe_shift]
    obtain ⟨hsum, hcode⟩ := hnew (min al (be + 1)) (flagNe al (be + 1)) m1 m2 (flagNe_le _ _)
    have dx : eps D x = flagNe al 0 := by rw [d2, hhs, hal, flagNe_base]
    have hL : be = 0 → eps D y = 0 → al = 0 := by
      intro h1 h2
      have c1 := flagNe_cases al 0
      omega
    have hg := path_gain al (eps D c) be (eps D y) rest z (eps_le D c) hey hfl hzf hL
    rw [hhs, hes, hal] at d3
    rw [← hl] at d3 n3
    constructor
    · have e1 : ((x :: s :: suf).map (wv D)).sum = wv D x + ((s :: suf).map (wv D)).sum := rfl
      have e2 : ((x :: s :: suf).map (wv (nreply D x y (s :: suf)))).sum
          = wv (nreply D x y (s :: suf)) x + ((s :: suf).map (wv (nreply D x y (s :: suf)))).sum :=
        rfl
      simp only [List.nil_append] at hsum
      rw [e1, e2, hold, hsum, d3, n3]
      simp only [List.length_cons]
      rw [← hl]
      omega
    · intro q hq
      rcases List.mem_cons.mp hq with rfl | hq
      · rw [m1, m2, dx]
        have c1 := flagNe_cases al 0
        have c2 := flagNe_cases al (be + 1)
        omega
      · exact hcode q hq

/-! ### the vertices off the path (Lemma 7) and the whole potential -/

section
variable {D : Pos n} {x y : Fin n} {ws : List (Fin n)}

/-- No `h` decreases (Step 1 of Lemma 4 of the note), given that it does not on the path. -/
theorem no_decrease (hF' : Forest D) (hc : ¬ Conn D x y) (hp : NPath D x ws)
    (hpath : ∀ q, q ∈ x :: ws → hh D q ≤ hh (nreply D x y ws) q) :
    ∀ (k : Nat) (v : Fin n), Hge D k v → Hge (nreply D x y ws) k v := by
  have hF2 : Forest (nreply D x y ws) := forest_nreply hF' hc
  intro k
  induction k with
  | zero => intro v _; trivial
  | succ k ih =>
    intro v h
    by_cases hv : v ∈ x :: ws
    · exact (le_hh_iff hF2).mp (Nat.le_trans ((le_hh_iff hF').mpr h) (hpath v hv))
    · refine ⟨?_, fun o ho => ih o (h.2 o ((arc_nreply_off hv).mp ho))⟩
      rw [outdeg_nreply hF' hp, ind_ne (fun e => hv (by rw [← e]; exact lastV_mem x ws))]
      exact h.1

theorem hh_le_nreply (hF' : Forest D) (hc : ¬ Conn D x y) (hp : NPath D x ws)
    (hpath : ∀ q, q ∈ x :: ws → hh D q ≤ hh (nreply D x y ws) q) (v : Fin n) :
    hh D v ≤ hh (nreply D x y ws) v :=
  (le_hh_iff (forest_nreply hF' hc)).mpr (no_decrease hF' hc hp hpath _ v (hge_hh hF' v))

/-- `2h + ε` does not decrease at any vertex, and the weight of a vertex off the path does
not decrease. -/
theorem off_path (hF' : Forest D) (hV : Valid D) (hc : ¬ Conn D x y) (hp : NPath D x ws)
    (hcode : ∀ q, q ∈ x :: ws →
      2 * hh D q + eps D q ≤ 2 * hh (nreply D x y ws) q + eps (nreply D x y ws) q) :
    (∀ v, 2 * hh D v + eps D v ≤ 2 * hh (nreply D x y ws) v + eps (nreply D x y ws) v) ∧
    (∀ v, v ∉ x :: ws → wv D v ≤ wv (nreply D x y ws) v) := by
  have hN := forest_nodup D hF'
  have hF2 : Forest (nreply D x y ws) := forest_nreply hF' hc
  have hN2 := forest_nodup _ hF2
  have hpath : ∀ q, q ∈ x :: ws → hh D q ≤ hh (nreply D x y ws) q := by
    intro q hq
    have := hcode q hq
    have := eps_le D q
    have := eps_le (nreply D x y ws) q
    omega
  have hle := hh_le_nreply hF' hc hp hpath
  have hall : ∀ v, 2 * hh D v + eps D v
      ≤ 2 * hh (nreply D x y ws) v + eps (nreply D x y ws) v := by
    intro v
    by_cases hv : v ∈ x :: ws
    · exact hcode v hv
    have harc : ∀ o, (v, o) ∈ nreply D x y ws ↔ (v, o) ∈ D := fun o => arc_nreply_off hv
    rcases outs_shape hV hN v with ho | ⟨a, ho⟩ | ⟨a, b, hab, ho⟩
    · have h0 : ∀ o, (v, o) ∉ nreply D x y ws := by
        intro o h; have := mem_outs.mpr ((harc o).mp h); rw [ho] at this; cases this
      obtain ⟨s1, s2, _⟩ := spec0 h0
      have hd : outdeg D v ≠ 2 := by rw [outdeg_eq_length, ho]; simp
      rw [s1, s2, hh_unsat hd, eps_nil ho]; omega
    · have h1 : ∀ o, (v, o) ∈ nreply D x y ws ↔ o = a := by
        intro o; rw [harc, ← mem_outs, ho]; simp
      obtain ⟨s1, s2, _⟩ := spec1 hN2 h1
      have hd : outdeg D v ≠ 2 := by rw [outdeg_eq_length, ho]; simp
      rw [s1, s2, hh_unsat hd, eps_one ho]; omega
    · have h2 : ∀ o, (v, o) ∈ nreply D x y ws ↔ o = a ∨ o = b := by
        intro o; rw [harc, ← mem_outs, ho]; simp
      obtain ⟨s1, s2, _⟩ := spec2 hF2 hab h2
      rw [s1, s2, hh_two hF' ho, eps_two ho]
      have := hle a
      have := hle b
      have c1 := flagNe_cases (hh D a) (hh D b)
      have c2 := flagNe_cases (hh (nreply D x y ws) a) (hh (nreply D x y ws) b)
      omega
  refine ⟨hall, ?_⟩
  intro v hv
  have harc : ∀ o, (v, o) ∈ nreply D x y ws ↔ (v, o) ∈ D := fun o => arc_nreply_off hv
  rcases outs_shape hV hN v with ho | ⟨a, ho⟩ | ⟨a, b, hab, ho⟩
  · have h0 : ∀ o, (v, o) ∉ nreply D x y ws := by
      intro o h; have := mem_outs.mpr ((harc o).mp h); rw [ho] at this; cases this
    rw [(spec0 h0).2.2, wv_nil ho]; exact Int.le_refl _
  · have h1 : ∀ o, (v, o) ∈ nreply D x y ws ↔ o = a := by
      intro o; rw [harc, ← mem_outs, ho]; simp
    rw [(spec1 hN2 h1).2.2, wv_one ho]
    exact w1_mono (eps_le _ _) (eps_le _ _) (hall a)
  · have h2 : ∀ o, (v, o) ∈ nreply D x y ws ↔ o = a ∨ o = b := by
      intro o; rw [harc, ← mem_outs, ho]; simp
    rw [(spec2 hF2 hab h2).2.2, wv_two ho]
    exact w2_mono (eps_le _ _) (eps_le _ _) (eps_le _ _) (eps_le _ _) (hall a) (hall b)

end

theorem sum_map_sub {α : Type} (l : List α) (f g : α → Int) :
    (l.map fun v => f v - g v).sum = (l.map f).sum - (l.map g).sum := by
  induction l with
  | nil => simp
  | cons a l ih => simp only [List.map_cons, List.sum_cons, ih]; omega

/-- A sum over a duplicate-free list `P` of vertices is at most the sum over all vertices, if
the terms outside `P` are non-negative. -/
theorem sum_le_all : ∀ (P : List (Fin n)) (f : Fin n → Int), P.Nodup →
    (∀ v, v ∉ P → 0 ≤ f v) → (P.map f).sum ≤ ((List.finRange n).map f).sum
  | [], f, _, h => by
    have : ∀ (l : List (Fin n)), 0 ≤ (l.map f).sum := by
      intro l
      induction l with
      | nil => simp
      | cons a l ih =>
        have := h a (by simp)
        simp only [List.map_cons, List.sum_cons]; omega
    simpa using this _
  | a :: P, f, hN, h => by
    have hn := List.nodup_cons.mp hN
    let g : Fin n → Int := fun v => if v = a then 0 else f v
    have hga : g a = 0 := by simp [g]
    have hgv : ∀ v, v ≠ a → f v = g v := by intro v hv; simp [g, hv]
    have h1 := sum_change (List.finRange n) f g a (List.nodup_finRange n) hgv
    rw [if_pos' (List.mem_finRange a), hga] at h1
    have h2 := sum_le_all P g hn.2 (by
      intro v hv
      by_cases e : v = a
      · rw [e, hga]; exact Int.le_refl _
      · rw [← hgv v e]
        exact h v (by
          intro hm
          rcases List.mem_cons.mp hm with hm | hm
          · exact e hm
          · exact hv hm))
    have h3 : (P.map g).sum = (P.map f).sum :=
      sum_map_congr _ _ _ (fun v hv => (hgv v (fun e => hn.1 (e ▸ hv))).symm)
    rw [List.map_cons, List.sum_cons]
    omega

/-- THE ROUND (Lemmas 7 and 8 together).  One round of the rule on a valid forest position:
`x`, `y` in different trees, `2h(x) + ε(x) ≤ 2h(y) + ε(y)`, `ws` a shortest path from `x` to an
unsaturated vertex; the reply flips the path and adds the arc `x → y`.  The potential (in
thirds) rises by at least three times the length of the path. -/
theorem round_gain {D : Pos n} {x y : Fin n} {ws : List (Fin n)} (hF' : Forest D) (hV : Valid D)
    (hc : ¬ Conn D x y) (hp : NPath D x ws) (hlen : ws.length = hh D x)
    (hrule : 2 * hh D x + eps D x ≤ 2 * hh D y + eps D y) :
    Phi D + 3 * (ws.length : Int) ≤ Phi (nreply D x y ws) := by
  obtain ⟨hsum, hcode⟩ := path_round hF' hV hc hp hlen hrule
  obtain ⟨_, hoff⟩ := off_path hF' hV hc hp hcode
  have h1 := sum_le_all (x :: ws) (fun v => wv (nreply D x y ws) v - wv D v)
    (npath_nodup hF' hp) (fun v hv => by have := hoff v hv; omega)
  rw [sum_map_sub, sum_map_sub] at h1
  unfold Phi
  omega

end FO
