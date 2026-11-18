/-
M4.  THE FULL GAME.  The normal-form lemma (solver-b Lemma 1) in the form of the shadow
construction of reports/open-limit-referee.md section 0: Adversary keeps a shadow orientation
`L` of the same forest, chooses its moves from `L`, and answers every reply of Algorithm
(an arbitrary valid re-orientation `A'`) by a normal reply on `L`.  Invariant of the proof:
    (what Adversary still forces on the shadow)  ≤  (value of the real position) + d(L, A),
`d` = number of edges on which the two orientations differ.
-/
import FO.Strategy
namespace FO
variable {n : Nat}

/-! ### max and min of lists -/

theorem le_maxL : ∀ {l : List Nat} {a : Nat}, a ∈ l → a ≤ maxL l
  | b :: l, a, h => by
    rcases List.mem_cons.mp h with rfl | h
    · exact Nat.le_max_left _ _
    · exact Nat.le_trans (le_maxL h) (Nat.le_max_right _ _)

theorem minL_mem : ∀ {l : List Nat}, l ≠ [] → minL l ∈ l
  | [a], _ => List.mem_cons_self ..
  | a :: b :: l, _ => by
    have ih := minL_mem (l := b :: l) (by simp)
    have e : minL (a :: b :: l) = min a (minL (b :: l)) := rfl
    rw [e]
    by_cases h : a ≤ minL (b :: l)
    · rw [Nat.min_eq_left h]; exact List.mem_cons_self ..
    · rw [Nat.min_eq_right (by omega)]; exact List.mem_cons_of_mem _ ih

/-! ### distance between aligned orientations -/

theorem al_mem : ∀ {L B : Pos n}, Al L B → ∀ a, a ∈ L → a ∈ B ∨ sw a ∈ B
  | [], [], _ => by intro a h; cases h
  | e :: L, f :: B, h => by
    intro a ha
    rcases List.mem_cons.mp ha with rfl | ha
    · rcases h.1 with e1 | e1
      · exact Or.inl (by rw [e1]; exact List.mem_cons_self ..)
      · exact Or.inr (by rw [e1]; exact List.mem_cons_self ..)
    · rcases al_mem h.2 a ha with h1 | h1
      · exact Or.inl (List.mem_cons_of_mem _ h1)
      · exact Or.inr (List.mem_cons_of_mem _ h1)

theorem flips_triangle : ∀ {L A B : Pos n}, Al L A → Al A B →
    flips L B ≤ flips L A + flips A B
  | [], [], [], _, _ => Nat.le_refl _
  | e :: L, f :: A, g :: B, h1, h2 => by
    have ih := flips_triangle h1.2 h2.2
    have key : (if e = g then 0 else 1) ≤ (if e = f then 0 else 1) + (if f = g then 0 else 1) := by
      by_cases h1 : e = f
      · subst h1; by_cases h2 : e = g <;> simp [h2]
      · by_cases h3 : e = g
        · simp [h3]
        · simp [h1, h3]
    show (if e = g then 0 else 1) + flips L B
      ≤ (if e = f then 0 else 1) + flips L A + ((if f = g then 0 else 1) + flips A B)
    omega

/-- Flipping a set `P` of arcs of `L` on which `B` disagrees with `L` brings `L` closer to `B`
by the number of flipped arcs. -/
theorem flips_flipSet : ∀ {L B : Pos n}, Forest L → Al L B → ∀ P : List (Arc n),
    (∀ a, a ∈ P → a ∈ L → sw a ∈ B) →
    flips (flipSet L P) B + L.countP (fun a => decide (a ∈ P) && true) = flips L B
  | [], [], _, _, _, _ => rfl
  | (u, v) :: L, f :: B, hF, hA, P, hP => by
    have hnadj : ¬ adj L u v := fun h => hF.1 (RT.single h)
    have huv : u ≠ v := fun e => hF.1 (e ▸ RT.refl _)
    have ih := flips_flipSet hF.2 hA.2 P (by
      intro a ha haL
      rcases List.mem_cons.mp (hP a ha (List.mem_cons_of_mem _ haL)) with e | h
      · exfalso
        obtain ⟨x, y⟩ := a
        rcases hA.1 with e1 | e1
        · rw [e1] at e
          simp only [sw, Prod.mk.injEq] at e
          obtain ⟨rfl, rfl⟩ := e
          exact hnadj (Or.inr haL)
        · rw [e1] at e
          simp only [sw, Prod.mk.injEq] at e
          obtain ⟨rfl, rfl⟩ := e
          exact hnadj (Or.inl haL)
      · exact h)
    have e1 : flipSet ((u, v) :: L) P
        = (if (u, v) ∈ P then sw (u, v) else (u, v)) :: flipSet L P := rfl
    have fl : ∀ (x : Arc n) (X : Pos n),
        flips (x :: X) (f :: B) = (if x = f then 0 else 1) + flips X B := fun _ _ => rfl
    have cc : ((u, v) :: L).countP (fun a => decide (a ∈ P) && true)
        = L.countP (fun a => decide (a ∈ P) && true) + (if (u, v) ∈ P then 1 else 0) := by
      simp [List.countP_cons]
    rw [e1, fl, fl, cc]
    by_cases hin : (u, v) ∈ P
    · have hf : f = sw (u, v) := by
        rcases hA.1 with e2 | e2
        · exfalso
          rcases List.mem_cons.mp (hP (u, v) hin (List.mem_cons_self ..)) with e3 | h3
          · rw [e2] at e3
            simp only [sw, Prod.mk.injEq] at e3
            exact huv e3.2
          · exact hnadj (al_adj (al_symm hA.2) _ _ (Or.inr h3))
        · exact e2
      have hne : (u, v) ≠ f := by
        rw [hf]; intro e
        simp only [sw, Prod.mk.injEq] at e
        exact huv e.1
      rw [if_pos' hin, if_pos' hin, if_pos' hf.symm, if_neg' hne]
      omega
    · rw [if_neg' hin, if_neg' hin]
      omega

theorem pathArcs_length : ∀ (t : Fin n) (ws : List (Fin n)), (pathArcs t ws).length = ws.length
  | _, [] => rfl
  | _, w :: ws => by simp [pathArcs, pathArcs_length w ws]

/-! ### the path of the normal-form lemma -/

theorem walk_reach : ∀ {L : Pos n} {v : Fin n} {ws : List (Fin n)},
    (∀ a, a ∈ pathArcs v ws → a ∈ L) → ∀ c, c ∈ ws → ∃ w, (v, w) ∈ L ∧ Reach L w c
  | _, _, [], _ => by intro c h; cases h
  | L, v, w :: ws, h => by
    intro c hc
    have hvw : (v, w) ∈ L := h _ (List.mem_cons_self ..)
    refine ⟨w, hvw, ?_⟩
    rcases List.mem_cons.mp hc with rfl | hc
    · exact RT.refl _
    · obtain ⟨w', h1, h2⟩ := walk_reach (L := L) (v := w) (ws := ws)
        (fun a ha => h a (List.mem_cons_of_mem _ ha)) c hc
      exact RT.step h1 h2

theorem walk_nodup {L : Pos n} (hF : Forest L) : ∀ {v : Fin n} {ws : List (Fin n)},
    (∀ a, a ∈ pathArcs v ws → a ∈ L) → (v :: ws).Nodup
  | _, [], _ => by simp
  | v, w :: ws, h => by
    refine List.nodup_cons.mpr ⟨?_, walk_nodup hF (v := w) (ws := ws)
      (fun a ha => h a (List.mem_cons_of_mem _ ha))⟩
    intro hv
    obtain ⟨w', h1, h2⟩ := walk_reach h v hv
    exact dag hF h1 h2

/-- Follow arcs of `L` that are reversed in `B`, as long as the current vertex is saturated in
`L`: either this reaches an unsaturated vertex, or it runs for `fuel` steps. -/
theorem dis_path {L B : Pos n} (hV : Valid L)
    (hQ1 : ∀ v o, (v, o) ∈ L → (o, v) ∈ B → outdeg L o = 2 → ∃ o', (o, o') ∈ L ∧ (o', o) ∈ B) :
    ∀ (fuel : Nat) (v : Fin n), (outdeg L v = 2 → ∃ o, (v, o) ∈ L ∧ (o, v) ∈ B) →
      (∃ ws, NPath L v ws ∧ ∀ a, a ∈ pathArcs v ws → sw a ∈ B) ∨
      (∃ ws, ws.length = fuel ∧ ∀ a, a ∈ pathArcs v ws → a ∈ L) := by
  intro fuel
  induction fuel with
  | zero => intro v _; exact Or.inr ⟨[], rfl, by intro a h; cases h⟩
  | succ fuel ih =>
    intro v hQ
    by_cases hs : outdeg L v = 2
    · obtain ⟨o, ho, hoB⟩ := hQ hs
      rcases ih o (hQ1 v o ho hoB) with ⟨ws, hws, hB⟩ | ⟨ws, hlen, hw⟩
      · refine Or.inl ⟨o :: ws, ⟨hs, ho, hws⟩, ?_⟩
        intro a ha
        rcases List.mem_cons.mp ha with rfl | ha
        · exact hoB
        · exact hB a ha
      · refine Or.inr ⟨o :: ws, by simp [hlen], ?_⟩
        intro a ha
        rcases List.mem_cons.mp ha with rfl | ha
        · exact ho
        · exact hw a ha
    · refine Or.inl ⟨[], ?_, by intro a h; cases h⟩
      have := hV v
      show outdeg L v < 2
      omega

/-- NORMAL-FORM LEMMA, one round (solver-b Lemma 1).  `L` is a valid orientation of a forest,
`B` another orientation of the same forest such that `B` plus the arc `t → hd` is valid.
Then `L` has a normal path from `t` all of whose arcs are reversed in `B`; flipping it lowers
the distance to `B` by exactly its length. -/
theorem shadow_path {L B : Pos n} {t hd : Fin n} (hF : Forest L) (hV : Valid L) (hA : Al L B)
    (hB : Valid ((t, hd) :: B)) :
    ∃ ws, NPath L t ws ∧ flips (flipSet L (pathArcs t ws)) B + ws.length = flips L B := by
  have hFB : Forest B := al_forest hA hF
  have hNB := forest_nodup B hFB
  have hNL := forest_nodup L hF
  have hBv : ∀ v, outdeg B v ≤ 2 := fun v => by
    have := hB v; rw [outdeg_cons] at this; omega
  have hBt : outdeg B t ≤ 1 := by
    have := hB t
    rw [outdeg_cons] at this
    have h1 : ind (t, hd).1 t = 1 := ind_self t
    omega
  have hQ0 : outdeg L t = 2 → ∃ o, (t, o) ∈ L ∧ (o, t) ∈ B := by
    intro hs
    obtain ⟨a, b, hab, ha, hb, _⟩ := two_out hNL hs
    rcases al_mem hA _ ha with ha' | ha'
    · rcases al_mem hA _ hb with hb' | hb'
      · have := two_le hNB ha' hb' hab; omega
      · exact ⟨b, hb, hb'⟩
    · exact ⟨a, ha, ha'⟩
  have hQ1 : ∀ v o, (v, o) ∈ L → (o, v) ∈ B → outdeg L o = 2 →
      ∃ o', (o, o') ∈ L ∧ (o', o) ∈ B := by
    intro v o hvo hov hs
    obtain ⟨a, b, hab, ha, hb, _⟩ := two_out hNL hs
    rcases al_mem hA _ ha with ha' | ha'
    · rcases al_mem hA _ hb with hb' | hb'
      · have hva : v ≠ a := fun e => by subst e; exact forest_antisymm L hF _ _ hvo ha
        have hvb : v ≠ b := fun e => by subst e; exact forest_antisymm L hF _ _ hvo hb
        have := three_le hNB hov ha' hb' hva hvb hab
        have := hBv o; omega
      · exact ⟨b, hb, hb'⟩
    · exact ⟨a, ha, ha'⟩
  rcases dis_path hV hQ1 n t hQ0 with ⟨ws, hws, hdis⟩ | ⟨ws, hlen, hw⟩
  · refine ⟨ws, hws, ?_⟩
    have h1 := flips_flipSet hF hA (pathArcs t ws) (fun a ha _ => hdis a ha)
    have h2 : L.countP (fun a => decide (a ∈ pathArcs t ws) && true)
        = (pathArcs t ws).countP (fun _ => true) :=
      countP_mem_eq L (pathArcs t ws) (fun _ => true) hNL
        (pathArcs_nodup (npath_nodup hF hws)) (npath_sub hws)
    have h3 : (pathArcs t ws).countP (fun _ => true) = ws.length := by
      rw [← pathArcs_length t ws]; simp
    omega
  · exfalso
    have := (walk_nodup hF hw).length_le_of_subset (l₂ := List.finRange n)
      (fun x _ => List.mem_finRange x)
    rw [List.length_finRange, List.length_cons, hlen] at this
    omega

/-! ### legal moves and replies of the Spec -/

theorem mem_moves {A : Pos n} {u v : Fin n} (h : ¬ Conn A u v) : (u, v) ∈ moves A := by
  unfold moves
  refine List.mem_flatMap.mpr ⟨u, List.mem_finRange u,
    List.mem_map.mpr ⟨v, List.mem_filter.mpr ⟨List.mem_finRange v, ?_⟩, rfl⟩⟩
  have : conn A u v = false := by
    cases hc : conn A u v with
    | false => rfl
    | true => exact absurd ((conn_iff A u v).mp hc) h
  simp [this]

theorem mem_replies {A A' : Pos n} {u v : Fin n} :
    A' ∈ replies A u v ↔ Al ((u, v) :: A) A' ∧ Valid A' := by
  unfold replies
  rw [List.mem_filter, mem_reorient, valid_iff]

theorem npath_of_not_hge {D : Pos n} (hV : Valid D) : ∀ (k : Nat) (v : Fin n), ¬ Hge D k v →
    ∃ ws, NPath D v ws := by
  intro k
  induction k with
  | zero => intro v h; exact absurd trivial h
  | succ k ih =>
    intro v h
    by_cases hs : outdeg D v = 2
    · have hex : ∃ o, (v, o) ∈ D ∧ ¬ Hge D k o :=
        Classical.byContradiction (fun hne =>
          h ⟨hs, fun o ho => Classical.byContradiction (fun hh => hne ⟨o, ho, hh⟩)⟩)
      obtain ⟨o, ho, hh⟩ := hex
      obtain ⟨ws, hws⟩ := ih o hh
      exact ⟨o :: ws, hs, ho, hws⟩
    · refine ⟨[], ?_⟩
      have := hV v
      show outdeg D v < 2
      omega

/-- `h` is finite: from every vertex there is a normal path (so Algorithm always has a reply). -/
theorem exists_npath {D : Pos n} (hF : Forest D) (hV : Valid D) (t : Fin n) :
    ∃ ws, NPath D t ws := by
  apply npath_of_not_hge hV n t
  intro h
  have h1 := size_bound hF h
  have h2 := Nat.lt_two_pow_self (n := n + 1)
  omega

/-! ### the transfer -/

/-- If Adversary forces `g` in `k` rounds of the normal-form game from the shadow `L`, then the
value of the real `k`-round game from any valid orientation `A` of the same forest is at least
`g - d(L, A)`. -/
theorem transfer : ∀ (k : Nat) (L A : Pos n) (g : Nat), Core.Forces NLegal NReply k L g →
    Forest L → Valid L → Al L A → Valid A → g ≤ value k A + flips L A := by
  intro k
  induction k with
  | zero =>
    intro L A g h _ _ _ _
    have : g = 0 := h
    omega
  | succ k ih =>
    intro L A g h hF hV hA hVA
    rcases h with h | ⟨m, hm, hrep⟩
    · omega
    · obtain ⟨u, v⟩ := m
      have hnc : ¬ Conn L u v := hm
      have hncA : ¬ Conn A u v := fun hc => hnc ((al_conn hA).mpr hc)
      have hFA : Forest A := al_forest hA hF
      have key : ∀ A', A' ∈ replies A u v → g ≤ cost A A' + value k A' + flips L A := by
        intro A' hA'
        obtain ⟨hal, hVA'⟩ := mem_replies.mp hA'
        match A', hal, hVA' with
        | e :: A'', hal, hVA' =>
          obtain ⟨he, hAA''⟩ := hal
          have hLA'' : Al L A'' := al_trans hA hAA''
          have hgen : ∀ t hd, e = (t, hd) → ((t = u ∧ hd = v) ∨ (t = v ∧ hd = u)) →
              ¬ Conn L t hd → g ≤ cost A (e :: A'') + value k (e :: A'') + flips L A := by
            rintro t hd rfl hth hc
            obtain ⟨ws, hws, hfl⟩ := shadow_path hF hV hLA'' hVA'
            have hforces := hrep ws.length (nreply L t hd ws) ⟨t, hd, ws, hth, hws, rfl, rfl⟩
            have hal' : Al (nreply L t hd ws) ((t, hd) :: A'') :=
              ⟨Or.inl rfl, al_trans (al_symm (al_flipSet L _)) hLA''⟩
            have h1 := ih _ _ _ hforces (forest_nreply hF hc) (valid_nreply hF hV hws) hal' hVA'
            have h2 : flips (nreply L t hd ws) ((t, hd) :: A'')
                = flips (flipSet L (pathArcs t ws)) A'' := by
              show (if (t, hd) = (t, hd) then 0 else 1) + _ = _
              simp
            have h3 := flips_triangle hA hAA''
            have h4 : cost A ((t, hd) :: A'') = flips A A'' := rfl
            omega
          rcases he with he | he
          · exact hgen u v he (Or.inl ⟨rfl, rfl⟩) hnc
          · exact hgen v u he (Or.inr ⟨rfl, rfl⟩) (fun h => hnc h.symm)
      obtain ⟨ws0, hws0⟩ := exists_npath hFA hVA u
      have hne : nreply A u v ws0 ∈ replies A u v :=
        mem_replies.mpr ⟨⟨Or.inl rfl, al_flipSet A _⟩, valid_nreply hFA hVA hws0⟩
      have hval : minL ((replies A u v).map fun D' => cost A D' + value k D')
          ≤ value (k + 1) A := by
        show _ ≤ maxL ((moves A).map fun m =>
          minL ((replies A m.1 m.2).map fun D' => cost A D' + value k D'))
        exact le_maxL (List.mem_map.mpr ⟨(u, v), mem_moves hncA, rfl⟩)
      have hmem := minL_mem (l := (replies A u v).map fun D' => cost A D' + value k D') (by
        intro e
        have := List.mem_map_of_mem (f := fun D' => cost A D' + value k D') hne
        rw [e] at this; cases this)
      obtain ⟨A', hA', hA'eq⟩ := List.mem_map.mp hmem
      have hA'eq' : cost A A' + value k A'
          = minL ((replies A u v).map fun D' => cost A D' + value k D') := hA'eq
      have := key A' hA'
      omega

/-- THE THEOREM OF THE SPEC:  `n + 3 ≤ 3 T(n) + 7 ⌊log₂(n+1)⌋` for every `n ≥ 1`. -/
theorem main : MainStatement := by
  intro n hn
  have key : ∀ g, 3 * g + 7 * Nat.log2 (n + 1) ≤ n + 5 → g ≤ T n := by
    intro g hg
    have h := transfer (n - 1) [] [] g (normal_game_bound n hn g hg) trivial
      (fun v => Nat.zero_le _) trivial (fun v => Nat.zero_le _)
    have h0 : flips ([] : Pos n) [] = 0 := rfl
    rw [h0] at h
    exact h
  by_cases h : 7 * Nat.log2 (n + 1) ≤ n + 5
  · have := key ((n + 5 - 7 * Nat.log2 (n + 1)) / 3) (by omega)
    omega
  · omega

end FO
