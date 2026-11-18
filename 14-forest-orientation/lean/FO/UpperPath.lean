/-
U3, part 1.  Tools for one round: the height, flag and weight of a vertex from the SET of its
out-neighbours; the arcs at a path vertex after a normal reply; vertices whose out-tree misses
the flipped path keep their height and flag.
-/
import FO.UpperLemma6
namespace FO
open Upper
variable {n : Nat}

/-! ### out-neighbour lists from their members -/

theorem outs_eq_nil {E : Pos n} {v : Fin n} (h : ∀ o, (v, o) ∉ E) : outs E v = [] := by
  match hl : outs E v with
  | [] => rfl
  | o :: _ => exact absurd (mem_outs.mp (by rw [hl]; exact List.mem_cons_self ..)) (h o)

theorem outs_eq_one {E : Pos n} (hN : E.Nodup) {v a : Fin n} (h : ∀ o, (v, o) ∈ E ↔ o = a) :
    outs E v = [a] := by
  have hn := outs_nodup E v hN
  have hm : ∀ o, o ∈ outs E v ↔ o = a := fun o => mem_outs.trans (h o)
  match hl : outs E v with
  | [] => exact absurd ((hm a).mpr rfl) (by rw [hl]; simp)
  | [b] =>
    have : b = a := (hm b).mp (by rw [hl]; simp)
    rw [this]
  | b :: c :: _ =>
    have h1 : b = a := (hm b).mp (by rw [hl]; simp)
    have h2 : c = a := (hm c).mp (by rw [hl]; simp)
    rw [hl, h1, h2] at hn
    simp at hn

theorem outs_eq_two {E : Pos n} (hN : E.Nodup) {v a b : Fin n} (hab : a ≠ b)
    (h : ∀ o, (v, o) ∈ E ↔ o = a ∨ o = b) : outs E v = [a, b] ∨ outs E v = [b, a] := by
  have hn := outs_nodup E v hN
  have hm : ∀ o, o ∈ outs E v ↔ o = a ∨ o = b := fun o => mem_outs.trans (h o)
  have ha := (hm a).mpr (Or.inl rfl)
  have hb := (hm b).mpr (Or.inr rfl)
  match hl : outs E v with
  | [] => rw [hl] at ha; simp at ha
  | [c] =>
    rw [hl] at ha hb
    simp only [List.mem_cons, List.not_mem_nil, or_false] at ha hb
    exact absurd (ha.trans hb.symm) hab
  | [c, d] =>
    rw [hl] at ha hb hn
    simp only [List.mem_cons, List.not_mem_nil, or_false] at ha hb
    rcases ha with rfl | rfl <;> rcases hb with rfl | rfl
    · exact absurd rfl hab
    · exact Or.inl rfl
    · exact Or.inr rfl
    · exact absurd rfl hab
  | c :: d :: e :: _ =>
    have hc := (hm c).mp (by rw [hl]; simp)
    have hd := (hm d).mp (by rw [hl]; simp)
    have he := (hm e).mp (by rw [hl]; simp)
    rw [hl] at hn
    simp only [List.nodup_cons, List.mem_cons, not_or] at hn
    obtain ⟨⟨h1, h2, _⟩, ⟨h3, _⟩, _⟩ := hn
    rcases hc with rfl | rfl <;> rcases hd with rfl | rfl <;> rcases he with rfl | rfl <;>
      first | exact absurd rfl h1 | exact absurd rfl h2 | exact absurd rfl h3

theorem flagNe_comm (a b : Nat) : flagNe a b = flagNe b a := by
  simp only [flagNe]; ifs_omega

/-- A vertex without out-neighbours: height 0, flag 0, weight 0. -/
theorem spec0 {E : Pos n} {v : Fin n} (h : ∀ o, (v, o) ∉ E) :
    hh E v = 0 ∧ eps E v = 0 ∧ wv E v = 0 := by
  have ho := outs_eq_nil h
  have hd : outdeg E v ≠ 2 := by rw [outdeg_eq_length, ho]; simp
  exact ⟨hh_unsat hd, eps_nil ho, wv_nil ho⟩

/-- A vertex whose only out-neighbour is `a`: height 0, flag 1, weight `w1`. -/
theorem spec1 {E : Pos n} (hN : E.Nodup) {v a : Fin n} (h : ∀ o, (v, o) ∈ E ↔ o = a) :
    hh E v = 0 ∧ eps E v = 1 ∧ wv E v = w1 (hh E a) (eps E a) := by
  have ho := outs_eq_one hN h
  have hd : outdeg E v ≠ 2 := by rw [outdeg_eq_length, ho]; simp
  exact ⟨hh_unsat hd, eps_one ho, wv_one ho⟩

/-- A vertex whose out-neighbours are exactly `a ≠ b`. -/
theorem spec2 {E : Pos n} (hF' : Forest E) {v a b : Fin n} (hab : a ≠ b)
    (h : ∀ o, (v, o) ∈ E ↔ o = a ∨ o = b) :
    hh E v = 1 + min (hh E a) (hh E b) ∧ eps E v = flagNe (hh E a) (hh E b) ∧
      wv E v = w2 (hh E a) (eps E a) (hh E b) (eps E b) := by
  rcases outs_eq_two (forest_nodup E hF') hab h with ho | ho
  · exact ⟨hh_two hF' ho, eps_two ho, wv_two ho⟩
  · refine ⟨?_, ?_, ?_⟩
    · rw [hh_two hF' ho, Nat.min_comm]
    · rw [eps_two ho, flagNe_comm]
    · rw [wv_two ho, w2_comm]

/-! ### paths -/

theorem pathArcs_append : ∀ (t : Fin n) (pre : List (Fin n)) (w : Fin n) (suf : List (Fin n)),
    pathArcs t (pre ++ w :: suf) = pathArcs t pre ++ (lastV t pre, w) :: pathArcs w suf
  | _, [], _, _ => rfl
  | t, a :: pre, w, suf => by
    show (t, a) :: pathArcs a (pre ++ w :: suf) = (t, a) :: (pathArcs a pre ++ _)
    rw [pathArcs_append a pre w suf]; rfl

theorem lastV_append : ∀ (t : Fin n) (pre : List (Fin n)) (w : Fin n), lastV t (pre ++ [w]) = w
  | _, [], _ => rfl
  | _, a :: pre, w => lastV_append a pre w

theorem npath_suffix {D : Pos n} : ∀ {t : Fin n} (pre : List (Fin n)) {w : Fin n}
    {suf : List (Fin n)}, NPath D t (pre ++ w :: suf) → NPath D w suf
  | _, [], _, _, h => h.2.2
  | _, a :: pre, _, _, h => npath_suffix (t := a) pre h.2.2

/-- Every vertex before `w` on a path reaches `w`. -/
theorem reach_to {D : Pos n} : ∀ (t : Fin n) (pre : List (Fin n)) (w : Fin n),
    (∀ a, a ∈ pathArcs t (pre ++ [w]) → a ∈ D) → ∀ q, q ∈ t :: pre → Reach D q w
  | t, [], w, h => by
    intro q hq
    simp only [List.mem_cons, List.not_mem_nil, or_false] at hq
    subst hq
    exact RT.single (h _ (List.mem_cons_self ..))
  | t, a :: pre, w, h => by
    intro q hq
    have ih := reach_to a pre w (fun x hx => h x (List.mem_cons_of_mem _ hx))
    rcases List.mem_cons.mp hq with rfl | hq
    · exact RT.step (h _ (List.mem_cons_self ..)) (ih a (List.mem_cons_self ..))
    · exact ih q hq

/-- On a shortest path, `h` of every vertex is the number of vertices after it. -/
theorem spath_heights {D : Pos n} (hF' : Forest D) : ∀ (pre : List (Fin n)) {t w : Fin n}
    {suf : List (Fin n)}, NPath D t (pre ++ w :: suf) → (pre ++ w :: suf).length = hh D t →
    hh D w = suf.length := by
  have step : ∀ {t a : Fin n} {l : List (Fin n)}, NPath D t (a :: l) →
      (a :: l).length = hh D t → l.length = hh D a := by
    intro t a l hp hl
    have h1 : Hge D (hh D t) t := hge_hh hF' t
    rw [← hl] at h1
    have h2 : Hge D l.length a := h1.2 a hp.2.1
    have h3 := (le_hh_iff hF').mpr h2
    have h4 := npath_len hp.2.2 (hge_hh hF' a)
    omega
  intro pre
  induction pre with
  | nil => intro t w suf hp hl; exact (step hp hl).symm
  | cons a pre ih => intro t w suf hp hl; exact ih hp.2.2 (step hp hl)

section
variable {D : Pos n} {x y : Fin n}

theorem mem_nreply {ws : List (Fin n)} {a b : Fin n} : (a, b) ∈ nreply D x y ws ↔
    (a = x ∧ b = y) ∨ ((a, b) ∈ D ∧ (a, b) ∉ pathArcs x ws) ∨
      ((b, a) ∈ D ∧ (b, a) ∈ pathArcs x ws) := by
  unfold nreply
  rw [List.mem_cons, mem_flipSet, Prod.mk.injEq]

variable {pre suf : List (Fin n)} {w : Fin n}

theorem path_split_nodup (h : (x :: (pre ++ w :: suf)).Nodup) :
    w ∉ x :: pre ∧ w ∉ suf ∧ (∀ q, q ∈ x :: pre → q ∉ suf) := by
  have e : x :: (pre ++ w :: suf) = (x :: pre) ++ w :: suf := rfl
  rw [e, List.nodup_append] at h
  obtain ⟨_, h2, h3⟩ := h
  have h2' := List.nodup_cons.mp h2
  exact ⟨fun hm => h3 w hm w (List.mem_cons_self ..) rfl, h2'.1,
    fun q hq hs => h3 q hq q (List.mem_cons_of_mem _ hs) rfl⟩

/-- The only path arc out of `w` goes to its successor. -/
theorem path_succ (h : (x :: (pre ++ w :: suf)).Nodup) {o : Fin n}
    (ho : (w, o) ∈ pathArcs x (pre ++ w :: suf)) : ∃ suf', suf = o :: suf' := by
  obtain ⟨h1, h2, _⟩ := path_split_nodup h
  rw [pathArcs_append, List.mem_append, List.mem_cons] at ho
  rcases ho with ho | ho | ho
  · exact absurd (mem_pathArcs ho).1 h1
  · simp only [Prod.mk.injEq] at ho
    exact absurd (ho.1 ▸ lastV_mem x pre) h1
  · cases suf with
    | nil => cases ho
    | cons s suf' =>
      rcases List.mem_cons.mp ho with e | ho
      · simp only [Prod.mk.injEq] at e
        exact ⟨suf', by rw [e.2]⟩
      · exact absurd (List.mem_cons_of_mem _ (mem_pathArcs ho).1) (by
          intro hm
          rcases List.mem_cons.mp hm with hm | hm
          · exact h2 (hm ▸ List.mem_cons_self ..)
          · rcases List.mem_cons.mp hm with hm | hm
            · exact h2 (hm ▸ List.mem_cons_self ..)
            · exact h2 (List.mem_cons_of_mem _ hm))

/-- The only path arc into `w` comes from its predecessor. -/
theorem path_pred (h : (x :: (pre ++ w :: suf)).Nodup) {o : Fin n} :
    (o, w) ∈ pathArcs x (pre ++ w :: suf) ↔ o = lastV x pre := by
  obtain ⟨h1, h2, _⟩ := path_split_nodup h
  rw [pathArcs_append, List.mem_append, List.mem_cons]
  constructor
  · rintro (ho | ho | ho)
    · exact absurd (List.mem_cons_of_mem _ (mem_pathArcs ho).2) h1
    · simp only [Prod.mk.injEq] at ho; exact ho.1
    · exact absurd (mem_pathArcs ho).2 h2
  · rintro rfl; exact Or.inr (Or.inl rfl)

/-- The arcs out of a path vertex `w ≠ x` after the normal reply: the old ones except the arc
to its successor, and the arc to its predecessor. -/
theorem arc_nreply_mid (hF' : Forest D) (hp : NPath D x (pre ++ w :: suf)) (o : Fin n) :
    (w, o) ∈ nreply D x y (pre ++ w :: suf) ↔
      ((w, o) ∈ D ∧ ∀ suf', suf ≠ o :: suf') ∨ o = lastV x pre := by
  have hnd := npath_nodup hF' hp
  obtain ⟨h1, _, _⟩ := path_split_nodup hnd
  rw [mem_nreply]
  constructor
  · rintro (⟨e, _⟩ | ⟨ho, hno⟩ | ⟨_, ho⟩)
    · exact absurd (e ▸ List.mem_cons_self ..) h1
    · refine Or.inl ⟨ho, fun suf' e => hno ?_⟩
      rw [pathArcs_append, e]
      exact List.mem_append_right _ (List.mem_cons_of_mem _ (List.mem_cons_self ..))
    · exact Or.inr ((path_pred hnd).mp ho)
  · rintro (⟨ho, hne⟩ | rfl)
    · exact Or.inr (Or.inl ⟨ho, fun hm => by
        obtain ⟨suf', e⟩ := path_succ hnd hm
        exact hne suf' e⟩)
    · have hm := (path_pred hnd (o := lastV x pre)).mpr rfl
      exact Or.inr (Or.inr ⟨npath_sub hp _ hm, hm⟩)

/-- The out-tree of an out-neighbour `c` of a path vertex `w ≠ x`, other than the successor of
`w`, contains no path vertex. -/
theorem side_avoid (hF' : Forest D) (hp : NPath D x (pre ++ w :: suf)) {c : Fin n}
    (hc : (w, c) ∈ D) (hne : ∀ suf', suf ≠ c :: suf') {q : Fin n} (hq : Reach D c q) :
    q ∉ x :: (pre ++ w :: suf) := by
  intro hm
  have e : x :: (pre ++ w :: suf) = (x :: pre) ++ w :: suf := rfl
  rw [e, List.mem_append] at hm
  have hsub : ∀ a, a ∈ pathArcs x (pre ++ [w]) → a ∈ D := by
    intro a ha
    apply npath_sub hp
    rw [pathArcs_append] at ha ⊢
    rcases List.mem_append.mp ha with ha | ha
    · exact List.mem_append_left _ ha
    · rcases List.mem_cons.mp ha with ha | ha
      · exact List.mem_append_right _ (ha ▸ List.mem_cons_self ..)
      · cases ha
  rcases hm with hm | hm
  · exact dag hF' hc (hq.trans (reach_to x pre w hsub q hm))
  rcases List.mem_cons.mp hm with hm | hm
  · rw [hm] at hq; exact dag hF' hc hq
  · obtain ⟨s, suf', e, hs, hsq⟩ := npath_reach (npath_suffix pre hp) q hm
    have : c ≠ s := fun e' => hne suf' (e' ▸ e)
    exact disjoint hF' hc hs this hq hsq

end

/-- The out-tree of an out-neighbour `c` of `x`, other than the first path vertex, contains no
path vertex. -/
theorem side_avoid_x {D : Pos n} {x : Fin n} {ws : List (Fin n)} (hF' : Forest D)
    (hp : NPath D x ws) {c : Fin n} (hc : (x, c) ∈ D) (hne : ∀ ws', ws ≠ c :: ws') {q : Fin n}
    (hq : Reach D c q) : q ∉ x :: ws := by
  intro hm
  rcases List.mem_cons.mp hm with rfl | hm
  · exact dag hF' hc hq
  · obtain ⟨s, ws', e, hs, hsq⟩ := npath_reach hp q hm
    have : c ≠ s := fun e' => hne ws' (e' ▸ e)
    exact disjoint hF' hc hs this hq hsq

/-! ### vertices that keep height and flag -/

/-- On a set `S` closed under out-arcs that misses the path, `h` and the flag are unchanged by
the normal reply. -/
theorem keep {D : Pos n} {x y : Fin n} {ws : List (Fin n)} (hF' : Forest D) (hV : Valid D)
    (hc : ¬ Conn D x y) (hp : NPath D x ws) (S : Fin n → Prop)
    (hS : ∀ c o, S c → (c, o) ∈ D → S o) (hoff : ∀ c, S c → c ∉ x :: ws) (c : Fin n) (hc' : S c) :
    hh (nreply D x y ws) c = hh D c ∧ eps (nreply D x y ws) c = eps D c := by
  have hF2 : Forest (nreply D x y ws) := forest_nreply hF' hc
  have hN2 := forest_nodup _ hF2
  have hhe : ∀ c, S c → hh (nreply D x y ws) c = hh D c := fun c hc' =>
    hh_congr hF' hF2 (fun k => hge_nreply hF' hp S hS hoff k c hc')
  refine ⟨hhe c hc', ?_⟩
  have harc : ∀ o, (c, o) ∈ nreply D x y ws ↔ (c, o) ∈ D := fun o => arc_nreply_off (hoff c hc')
  rcases outs_shape hV (forest_nodup D hF') c with ho | ⟨a, ho⟩ | ⟨a, b, hab, ho⟩
  · have h0 : ∀ o, (c, o) ∉ nreply D x y ws := by
      intro o h; have := mem_outs.mpr ((harc o).mp h); rw [ho] at this; cases this
    rw [(spec0 h0).2.1, eps_nil ho]
  · have h1 : ∀ o, (c, o) ∈ nreply D x y ws ↔ o = a := by
      intro o; rw [harc, ← mem_outs, ho]; simp
    rw [(spec1 hN2 h1).2.1, eps_one ho]
  · have h2 : ∀ o, (c, o) ∈ nreply D x y ws ↔ o = a ∨ o = b := by
      intro o; rw [harc, ← mem_outs, ho]; simp
    have ha : S a := hS c a hc' (mem_outs.mp (by rw [ho]; simp))
    have hb : S b := hS c b hc' (mem_outs.mp (by rw [ho]; simp))
    rw [(spec2 hF2 hab h2).2.1, eps_two ho, hhe a ha, hhe b hb]

end FO
