/-
U2, part 2.  LEMMA 6 of ../NOTE.md (Section 6) in the form used by the final theorem:
    for every valid forest position `D`,   `Phi D ≤ D.length`   (the number of edges).
It is proved in the stronger form `lemma6_gen`: for every list `sel` of vertices in pairwise
different components,
    `Phi D + Σ_{s ∈ sel} (γ(s) − 1) ≤ D.length`.
(With one `s` in every tree, and `#edges = n − #trees`, this is the draft's
`Φ(T) ≤ n − max γ` summed over the trees; the draft's statement itself is not needed.)
Proof as in the draft: delete a vertex `r` of in-degree 0 — here: delete the arcs out of `r`,
keeping the vertex set `Fin n` — which changes no other `h`, flag, `γ` or weight.
-/
import FO.Full
import FO.UpperPhi
namespace FO
open Upper
variable {n : Nat}

/-! ### sums -/

theorem sum_map_zero {α : Type} (l : List α) (f : α → Int) (h : ∀ x, x ∈ l → f x = 0) :
    (l.map f).sum = 0 := by
  induction l with
  | nil => rfl
  | cons a l ih =>
    rw [List.map_cons, List.sum_cons, h a (List.mem_cons_self ..),
      ih (fun x hx => h x (List.mem_cons_of_mem _ hx))]
    rfl

theorem sum_map_congr {α : Type} (l : List α) (f g : α → Int) (h : ∀ x, x ∈ l → f x = g x) :
    (l.map f).sum = (l.map g).sum := by
  induction l with
  | nil => rfl
  | cons a l ih =>
    rw [List.map_cons, List.map_cons, List.sum_cons, List.sum_cons, h a (List.mem_cons_self ..),
      ih (fun x hx => h x (List.mem_cons_of_mem _ hx))]

theorem sum_map_append {α : Type} (l1 l2 : List α) (f : α → Int) :
    ((l1 ++ l2).map f).sum = (l1.map f).sum + (l2.map f).sum := by
  induction l1 with
  | nil => simp
  | cons a l ih => simp only [List.cons_append, List.map_cons, List.sum_cons, ih]; omega

/-- Two functions that differ only at `r`: their sums over a duplicate-free list differ by the
difference at `r` (if `r` is in the list). -/
theorem sum_change {α : Type} [DecidableEq α] (l : List α) (f g : α → Int) (r : α)
    (hN : l.Nodup) (h : ∀ v, v ≠ r → f v = g v) :
    (l.map f).sum = (l.map g).sum + (if r ∈ l then f r - g r else 0) := by
  induction l with
  | nil => simp
  | cons a l ih =>
    have hn := List.nodup_cons.mp hN
    have ih' := ih hn.2
    rw [List.map_cons, List.map_cons, List.sum_cons, List.sum_cons]
    by_cases har : a = r
    · subst har
      rw [if_neg' hn.1] at ih'
      rw [if_pos' (List.mem_cons_self ..)]
      omega
    · have e : (r ∈ a :: l) ↔ r ∈ l := by
        rw [List.mem_cons]
        constructor
        · rintro (e | e)
          · exact absurd e.symm har
          · exact e
        · exact Or.inr
      rw [h a har]
      by_cases hr : r ∈ l
      · rw [if_pos' hr] at ih'; rw [if_pos' (e.mpr hr)]; omega
      · rw [if_neg' hr] at ih'; rw [if_neg' (fun h => hr (e.mp h))]; omega

/-! ### deleting the arcs out of a source -/

/-- `r` has in-degree 0. -/
def Source (D : Pos n) (r : Fin n) : Prop := ∀ a, (a, r) ∉ D

/-- The position without the arcs out of `r`. -/
def delOut (D : Pos n) (r : Fin n) : Pos n := D.filter (fun a => a.1 != r)

theorem mem_delOut {D : Pos n} {r a b : Fin n} : (a, b) ∈ delOut D r ↔ (a, b) ∈ D ∧ a ≠ r := by
  simp [delOut, List.mem_filter]

theorem conn_filter {D : Pos n} (p : Arc n → Bool) {a b : Fin n} (h : Conn (D.filter p) a b) :
    Conn D a b := by
  refine RT.mono ?_ h
  rintro x y (h | h)
  · exact Or.inl (List.mem_filter.mp h).1
  · exact Or.inr (List.mem_filter.mp h).1

theorem forest_filter (p : Arc n → Bool) : ∀ (D : Pos n), Forest D → Forest (D.filter p)
  | [], _ => trivial
  | (u, v) :: D, hF => by
    rw [List.filter_cons]
    by_cases hp : p (u, v) = true
    · rw [if_pos' hp]
      exact ⟨fun hc => hF.1 (conn_filter p hc), forest_filter p D hF.2⟩
    · rw [if_neg' hp]
      exact forest_filter p D hF.2

theorem outs_delOut_ne {D : Pos n} {r v : Fin n} (h : v ≠ r) : outs (delOut D r) v = outs D v := by
  unfold outs delOut
  rw [List.filter_filter]
  congr 1
  apply List.filter_congr
  intro a _
  by_cases e : a.1 = v
  · simp [e, h]
  · simp [e]

theorem outs_delOut_self (D : Pos n) (r : Fin n) : outs (delOut D r) r = [] := by
  unfold outs delOut
  rw [List.filter_filter, List.map_eq_nil_iff, List.filter_eq_nil_iff]
  intro a _
  by_cases e : a.1 = r <;> simp [e]

theorem length_delOut (D : Pos n) (r : Fin n) : (delOut D r).length + outdeg D r = D.length := by
  induction D with
  | nil => rfl
  | cons a D ih =>
    have e1 : delOut (a :: D) r = if (a.1 != r) = true then a :: delOut D r else delOut D r := by
      unfold delOut; rw [List.filter_cons]
    rw [e1, outdeg_cons]
    by_cases h : a.1 = r
    · have : ¬ ((a.1 != r) = true) := by simp [h]
      rw [if_neg' this, h, ind_self]
      simp only [List.length_cons]; omega
    · have : (a.1 != r) = true := by simp [h]
      rw [if_pos' this, ind_ne h]
      simp only [List.length_cons]; omega

theorem valid_delOut {D : Pos n} (hV : Valid D) (r : Fin n) : Valid (delOut D r) := by
  intro v
  by_cases h : v = r
  · subst h; rw [outdeg_eq_length, outs_delOut_self]; exact Nat.zero_le _
  · rw [outdeg_eq_length, outs_delOut_ne h, ← outdeg_eq_length]; exact hV v

section
variable {D : Pos n} {r : Fin n}

theorem source_out_ne (hS : Source D r) {v o : Fin n} (ho : o ∈ outs D v) : o ≠ r :=
  fun e => hS v (e ▸ mem_outs.mp ho)

theorem hF_delOut (hS : Source D r) : ∀ (f : Nat) (v : Fin n), v ≠ r →
    hF (delOut D r) f v = hF D f v := by
  intro f
  induction f with
  | zero => intro v _; rfl
  | succ f ih =>
    intro v hv
    show (if outdeg (delOut D r) v = 2 then 1 + minL ((outs (delOut D r) v).map (hF (delOut D r) f))
      else 0) = (if outdeg D v = 2 then 1 + minL ((outs D v).map (hF D f)) else 0)
    rw [outdeg_eq_length, outs_delOut_ne hv, ← outdeg_eq_length,
      List.map_congr_left (fun o ho => ih o (source_out_ne hS ho))]

theorem hh_delOut (hS : Source D r) {v : Fin n} (hv : v ≠ r) : hh (delOut D r) v = hh D v :=
  hF_delOut hS n v hv

theorem eps_delOut (hS : Source D r) {v : Fin n} (hv : v ≠ r) : eps (delOut D r) v = eps D v := by
  unfold eps
  rw [outs_delOut_ne hv, List.map_congr_left (fun o ho => hh_delOut hS (source_out_ne hS ho))]

theorem wv_delOut (hS : Source D r) {v : Fin n} (hv : v ≠ r) : wv (delOut D r) v = wv D v := by
  unfold wv
  rw [outs_delOut_ne hv, List.map_congr_left (fun o ho => by
    rw [hh_delOut hS (source_out_ne hS ho), eps_delOut hS (source_out_ne hS ho)])]

theorem gamv_delOut (hS : Source D r) {v : Fin n} (hv : v ≠ r) :
    gamv (delOut D r) v = gamv D v := by
  unfold gamv; rw [hh_delOut hS hv, eps_delOut hS hv]

theorem phi_delOut (hS : Source D r) : Phi D = Phi (delOut D r) + wv D r := by
  have h := sum_change (List.finRange n) (wv D) (wv (delOut D r)) r (List.nodup_finRange n)
    (fun v hv => (wv_delOut hS hv).symm)
  rw [if_pos' (List.mem_finRange r), wv_nil (outs_delOut_self D r)] at h
  unfold Phi
  omega

theorem conn_del {a b : Fin n} (h : Conn (delOut D r) a b) : Conn D a b := conn_filter _ h

/-- A connection that survives the deletion avoids `r`. -/
theorem ca_of_del (hS : Source D r) {a b : Fin n} (h : Conn (delOut D r) a b) : CA D r a b := by
  refine RT.mono ?_ h
  rintro x y (h | h)
  · have := mem_delOut.mp h
    exact ⟨Or.inl this.1, this.2, fun e => hS x (e ▸ this.1)⟩
  · have := mem_delOut.mp h
    exact ⟨Or.inr this.1, fun e => hS y (e ▸ this.1), this.2⟩

/-- Everything connected to the source `r` is `r` or hangs below one of its out-neighbours. -/
theorem conn_source (hS : Source D r) {s : Fin n} (h : Conn D r s) :
    s = r ∨ ∃ c, (r, c) ∈ D ∧ Conn (delOut D r) c s := by
  refine RT.preserve (Q := fun s => s = r ∨ ∃ c, (r, c) ∈ D ∧ Conn (delOut D r) c s) ?_ h
    (Or.inl rfl)
  intro a b hQ hab
  by_cases hb : b = r
  · exact Or.inl hb
  rcases hQ with rfl | ⟨c, hc, hca⟩
  · rcases hab with hab | hab
    · exact Or.inr ⟨b, hab, RT.refl _⟩
    · exact absurd hab (hS b)
  · refine Or.inr ⟨c, hc, hca.tail ?_⟩
    rcases hab with hab | hab
    · exact Or.inl (mem_delOut.mpr ⟨hab, fun e => by
        subst e
        have := (ca_of_del hS hca)
        -- `a = r` is impossible: `c` is not `r`-avoiding-connected to `r`
        cases this.symm with
        | refl => exact hS _ hc
        | step h1 _ => exact h1.2.1 rfl⟩)
    · exact Or.inr (mem_delOut.mpr ⟨hab, hb⟩)

end

/-- A forest with at least one arc has a vertex of in-degree 0 with an out-neighbour. -/
theorem exists_source {D : Pos n} (hF' : Forest D) : ∀ (fuel : Nat) (v : Fin n)
    (ws : List (Fin n)), (∀ a, a ∈ pathArcs v ws → a ∈ D) → ws.length + fuel = n →
    (∃ o, (v, o) ∈ D) → ∃ r, Source D r ∧ ∃ o, (r, o) ∈ D := by
  intro fuel
  induction fuel with
  | zero =>
    intro _ ws hw hlen _
    exfalso
    have := (walk_nodup hF' hw).length_le_of_subset (l₂ := List.finRange n)
      (fun x _ => List.mem_finRange x)
    rw [List.length_finRange, List.length_cons] at this
    omega
  | succ fuel ih =>
    intro v ws hw hlen hout
    by_cases h : ∃ p, (p, v) ∈ D
    · obtain ⟨p, hp⟩ := h
      refine ih p (v :: ws) ?_ (by simp only [List.length_cons]; omega) ⟨v, hp⟩
      intro a ha
      rcases List.mem_cons.mp ha with rfl | ha
      · exact hp
      · exact hw a ha
    · exact ⟨v, fun a ha => h ⟨a, ha⟩, hout⟩

/-! ### Lemma 6 -/

/-- `Σ_{s ∈ sel} (γ(s) − 1)`. -/
def selSum (D : Pos n) (sel : List (Fin n)) : Int := (sel.map fun s => gamv D s - 1).sum

theorem selSum_cons (D : Pos n) (s : Fin n) (sel : List (Fin n)) :
    selSum D (s :: sel) = (gamv D s - 1) + selSum D sel := by
  simp [selSum]

theorem phi_nil : Phi ([] : Pos n) = 0 :=
  sum_map_zero _ _ (fun v _ => wv_nil rfl)

/-- The inductive step of Lemma 6 at a source `r` with at least one out-neighbour: the claim for
`delOut D r` (hypothesis `ih`) gives the claim for `D`, for lists `rest` that miss the tree of
`r`, extended (A) by `r` itself or (B) by any other vertex of the tree of `r`. -/
theorem lemma6_step {D : Pos n} (hF' : Forest D) (hV : Valid D) {r o' : Fin n}
    (hS : Source D r) (ho' : (r, o') ∈ D)
    (ih : ∀ sel : List (Fin n), sel.Pairwise (fun a b => ¬ Conn (delOut D r) a b) →
      Phi (delOut D r) + selSum (delOut D r) sel ≤ (delOut D r).length)
    (rest : List (Fin n)) (hP : rest.Pairwise (fun a b => ¬ Conn D a b))
    (hr : ∀ x, x ∈ rest → ¬ Conn D r x) :
    Phi D + selSum D rest + (gamv D r - 1) ≤ D.length ∧
    ∀ s0, s0 ≠ r → Conn D r s0 → Phi D + selSum D rest + (gamv D s0 - 1) ≤ D.length := by
  have hN := forest_nodup D hF'
  have hlen := length_delOut D r
  have hphi := phi_delOut hS
  have hd : outdeg D r = (outs D r).length := outdeg_eq_length D r
  have hrest_ne : ∀ x, x ∈ rest → x ≠ r := fun x hx e => hr x hx (e ▸ RT.refl _)
  have hsum : selSum (delOut D r) rest = selSum D rest :=
    sum_map_congr _ _ _ (fun x hx => by rw [gamv_delOut hS (hrest_ne x hx)])
  have hP' : rest.Pairwise (fun a b => ¬ Conn (delOut D r) a b) :=
    hP.imp (fun h hc => h (conn_del hc))
  have harc : ∀ c, c ∈ outs D r → Conn D r c := fun c hc => RT.single (Or.inl (mem_outs.mp hc))
  have hcr : ∀ c, c ∈ outs D r → c ≠ r := fun c hc => source_out_ne hS hc
  -- a vertex of the tree of `r` is not connected (after deletion) to a vertex of `rest`
  have hsep : ∀ s, Conn D r s → ∀ x, x ∈ rest → ¬ Conn (delOut D r) s x :=
    fun s hs x hx hc => hr x hx (hs.trans (conn_del hc))
  have hw1 := eps_le D
  rcases outs_shape hV hN r with ho | ⟨c, ho⟩ | ⟨a, b, hab, ho⟩
  · exact absurd (mem_outs.mpr ho') (by rw [ho]; simp)
  · -- one out-neighbour
    have hc : c ∈ outs D r := by rw [ho]; simp
    rw [ho] at hd
    have hwr := wv_one ho
    have hgr := gamv_one ho
    constructor
    · have h1 := ih (c :: rest) (List.pairwise_cons.mpr ⟨hsep c (harc c hc), hP'⟩)
      rw [selSum_cons, hsum, gamv_delOut hS (hcr c hc)] at h1
      have h2 := w1_le_gam (hh D c) (eps D c)
      have h3 : gamv D c = gam (hh D c) (eps D c) := rfl
      simp only [List.length_cons, List.length_nil] at hd
      omega
    · intro s0 hs0 hcs0
      have h1 := ih (s0 :: rest) (List.pairwise_cons.mpr ⟨hsep s0 hcs0, hP'⟩)
      rw [selSum_cons, hsum, gamv_delOut hS hs0] at h1
      have h2 := w1_le_one (hh D c) (eps D c) (hw1 c)
      simp only [List.length_cons, List.length_nil] at hd
      omega
  · -- two out-neighbours
    have ha : a ∈ outs D r := by rw [ho]; simp
    have hb : b ∈ outs D r := by rw [ho]; simp
    rw [ho] at hd
    simp only [List.length_cons, List.length_nil] at hd
    have hwr := wv_two ho
    have hgr := gamv_two hF' ho
    have hnab : ¬ Conn (delOut D r) a b := fun hc =>
      nb D hF' r a b (Or.inl (mem_outs.mp ha)) (Or.inl (mem_outs.mp hb)) hab (ca_of_del hS hc)
    have hga : gamv D a = gam (hh D a) (eps D a) := rfl
    have hgb : gamv D b = gam (hh D b) (eps D b) := rfl
    constructor
    · have h1 := ih (a :: b :: rest) (List.pairwise_cons.mpr
        ⟨fun x hx => by
          rcases List.mem_cons.mp hx with rfl | hx
          · exact hnab
          · exact hsep a (harc a ha) x hx,
         List.pairwise_cons.mpr ⟨hsep b (harc b hb), hP'⟩⟩)
      rw [selSum_cons, selSum_cons, hsum, gamv_delOut hS (hcr a ha), gamv_delOut hS (hcr b hb)]
        at h1
      have h2 := w2_le_parent (hh D a) (eps D a) (hh D b) (eps D b) (hw1 a) (hw1 b)
      omega
    · intro s0 hs0 hcs0
      rcases conn_source hS hcs0 with e | ⟨c, hc, hcs⟩
      · exact absurd e hs0
      · have hc' := mem_outs.mpr hc
        rw [ho] at hc'
        simp only [List.mem_cons, List.not_mem_nil, or_false] at hc'
        rcases hc' with rfl | rfl
        · -- `s0` hangs below `a`; select `s0` and `b`
          have h1 := ih (s0 :: b :: rest) (List.pairwise_cons.mpr
            ⟨fun x hx => by
              rcases List.mem_cons.mp hx with rfl | hx
              · exact fun h => hnab (hcs.trans h)
              · exact hsep s0 hcs0 x hx,
             List.pairwise_cons.mpr ⟨hsep b (harc b hb), hP'⟩⟩)
          rw [selSum_cons, selSum_cons, hsum, gamv_delOut hS hs0, gamv_delOut hS (hcr b hb)]
            at h1
          have h2 := w2_le_right (hh D c) (eps D c) (hh D b) (eps D b) (hw1 c) (hw1 b)
          omega
        · have h1 := ih (s0 :: a :: rest) (List.pairwise_cons.mpr
            ⟨fun x hx => by
              rcases List.mem_cons.mp hx with rfl | hx
              · exact fun h => hnab (Conn.symm (hcs.trans h))
              · exact hsep s0 hcs0 x hx,
             List.pairwise_cons.mpr ⟨hsep a (harc a ha), hP'⟩⟩)
          rw [selSum_cons, selSum_cons, hsum, gamv_delOut hS hs0, gamv_delOut hS (hcr a ha)]
            at h1
          have h2 := w2_le_left (hh D a) (eps D a) (hh D c) (eps D c) (hw1 a) (hw1 c)
          omega

/-- LEMMA 6, general form: for vertices `sel` in pairwise different trees,
`Φ + Σ_{s ∈ sel} (γ(s) − 1) ≤ number of edges`. -/
theorem lemma6_gen : ∀ (m : Nat) (D : Pos n), D.length ≤ m → Forest D → Valid D →
    ∀ sel : List (Fin n), sel.Pairwise (fun a b => ¬ Conn D a b) →
    Phi D + selSum D sel ≤ D.length := by
  intro m
  induction m with
  | zero =>
    intro D hm _ _ sel _
    have : D = [] := List.length_eq_zero_iff.mp (by omega)
    subst this
    have : selSum ([] : Pos n) sel = 0 :=
      sum_map_zero _ _ (fun s _ => by rw [gamv_nil (D := ([] : Pos n)) rfl]; rfl)
    rw [phi_nil, this]; simp
  | succ m ih =>
    intro D hm hF' hV sel hsel
    match hD : D with
    | [] =>
      have : selSum ([] : Pos n) sel = 0 :=
        sum_map_zero _ _ (fun s _ => by rw [gamv_nil (D := ([] : Pos n)) rfl]; rfl)
      rw [phi_nil, this]; simp
    | (u, v) :: D0 =>
      rw [← hD] at hm hF' hV hsel ⊢
      have hmem : (u, v) ∈ D := by rw [hD]; exact List.mem_cons_self ..
      obtain ⟨r, hS, o', ho'⟩ := exists_source hF' n u [] (by intro a h; cases h) (by simp)
        ⟨v, hmem⟩
      have hlen := length_delOut D r
      have hpos := pos_out ho'
      have ih' := fun sel hs => ih (delOut D r) (by omega) (forest_filter _ D hF')
        (valid_delOut hV r) sel hs
      by_cases hex : ∃ s, s ∈ sel ∧ Conn D r s
      · obtain ⟨s0, hs0, hc0⟩ := hex
        obtain ⟨l1, l2, rfl⟩ := List.append_of_mem hs0
        obtain ⟨hp1, hp2, hp12⟩ := List.pairwise_append.mp hsel
        obtain ⟨hp2a, hp2b⟩ := List.pairwise_cons.mp hp2
        have hP : (l1 ++ l2).Pairwise (fun a b => ¬ Conn D a b) :=
          List.pairwise_append.mpr ⟨hp1, hp2b, fun a ha b hb =>
            hp12 a ha b (List.mem_cons_of_mem _ hb)⟩
        have hr : ∀ x, x ∈ l1 ++ l2 → ¬ Conn D r x := by
          intro x hx hc
          rcases List.mem_append.mp hx with hx | hx
          · exact hp12 x hx s0 (List.mem_cons_self ..) (hc.symm.trans hc0)
          · exact hp2a x hx (hc0.symm.trans hc)
        have hstep := lemma6_step hF' hV hS ho' ih' (l1 ++ l2) hP hr
        have hsum : selSum D (l1 ++ s0 :: l2) = selSum D (l1 ++ l2) + (gamv D s0 - 1) := by
          unfold selSum
          rw [sum_map_append, sum_map_append, List.map_cons, List.sum_cons]; omega
        rw [hsum]
        by_cases e : s0 = r
        · subst e; have := hstep.1; omega
        · have := hstep.2 s0 e hc0; omega
      · have hr : ∀ x, x ∈ sel → ¬ Conn D r x := fun x hx hc => hex ⟨x, hx, hc⟩
        have hstep := (lemma6_step hF' hV hS ho' ih' sel hsel hr).1
        have := gamv_pos D r
        omega

/-- LEMMA 6 as used by the final theorem: on every valid forest position the potential (in
thirds) is at most the number of edges. -/
theorem lemma6 {D : Pos n} (hF' : Forest D) (hV : Valid D) : Phi D ≤ D.length := by
  have := lemma6_gen D.length D (Nat.le_refl _) hF' hV [] List.Pairwise.nil
  simpa [selSum] using this

/-- The draft's form for one tree-or-forest and one chosen vertex: `Φ + γ(s) − 1 ≤ #edges`. -/
theorem lemma6_gamma {D : Pos n} (hF' : Forest D) (hV : Valid D) (s : Fin n) :
    Phi D + gamv D s ≤ D.length + 1 := by
  have := lemma6_gen D.length D (Nat.le_refl _) hF' hV [s] (List.pairwise_singleton _ _)
  rw [selSum_cons] at this
  simp [selSum] at this
  omega

end FO
