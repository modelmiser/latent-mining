/-
M2, part 1.  Out-degrees, `Hge` ("h(v) ≥ k"), the size bound, aligned lists, path flips and
the bookkeeping of a normal reply (Fact 0 and Lemma A of open-limit/proofs.txt).
-/
import FO.Graph
namespace FO
variable {n : Nat}

theorem if_pos' {c : Prop} [Decidable c] {α : Sort _} {a b : α} (h : c) : ite c a b = a := by simp [h]
theorem if_neg' {c : Prop} [Decidable c] {α : Sort _} {a b : α} (h : ¬ c) : ite c a b = b := by simp [h]

/-! ### out-neighbours -/

def Valid (D : Pos n) : Prop := ∀ v, outdeg D v ≤ 2

theorem valid_iff {D : Pos n} : valid D = true ↔ Valid D := by
  simp [valid, Valid, List.all_eq_true, List.mem_finRange]

def outs (D : Pos n) (v : Fin n) : List (Fin n) := (D.filter (fun a => a.1 == v)).map (·.2)

theorem outdeg_eq_length (D : Pos n) (v : Fin n) : outdeg D v = (outs D v).length := by
  simp [outdeg, outs, List.countP_eq_length_filter]

theorem mem_outs {D : Pos n} {v o : Fin n} : o ∈ outs D v ↔ (v, o) ∈ D := by
  simp only [outs, List.mem_map, List.mem_filter, beq_iff_eq]
  constructor
  · rintro ⟨⟨a, b⟩, ⟨h1, h2⟩, h3⟩
    simp only at h2 h3
    subst h2; subst h3; exact h1
  · intro h; exact ⟨(v, o), ⟨h, rfl⟩, rfl⟩

theorem outs_cons (a b : Fin n) (D : Pos n) (v : Fin n) :
    outs ((a, b) :: D) v = if a = v then b :: outs D v else outs D v := by
  unfold outs
  by_cases h : a = v <;> simp [List.filter_cons, h]

theorem outs_nodup : ∀ (D : Pos n) (v : Fin n), D.Nodup → (outs D v).Nodup
  | [], _, _ => by simp [outs]
  | (a, b) :: D, v, h => by
    have h' := List.nodup_cons.mp h
    rw [outs_cons]
    by_cases hav : a = v
    · rw [if_pos' hav]
      refine List.nodup_cons.mpr ⟨?_, outs_nodup D v h'.2⟩
      intro hb; exact h'.1 (hav ▸ mem_outs.mp hb)
    · rw [if_neg' hav]; exact outs_nodup D v h'.2

theorem pos_out {D : Pos n} {v o : Fin n} (h : (v, o) ∈ D) : 1 ≤ outdeg D v := by
  rw [outdeg_eq_length]
  exact List.length_pos_of_mem (mem_outs.mpr h)

theorem zero_out {D : Pos n} {v : Fin n} (h : outdeg D v = 0) (o : Fin n) : (v, o) ∉ D :=
  fun h' => by have := pos_out h'; omega

theorem one_out {D : Pos n} {v : Fin n} (h : outdeg D v = 1) :
    ∃ a, (v, a) ∈ D ∧ ∀ o, (v, o) ∈ D → o = a := by
  rw [outdeg_eq_length] at h
  match hl : outs D v, h with
  | [a], _ =>
    refine ⟨a, mem_outs.mp (by rw [hl]; simp), ?_⟩
    intro o ho
    have := mem_outs.mpr ho
    rw [hl] at this; simpa using this

theorem two_out {D : Pos n} {v : Fin n} (hN : D.Nodup) (h : outdeg D v = 2) :
    ∃ a b, a ≠ b ∧ (v, a) ∈ D ∧ (v, b) ∈ D ∧ ∀ o, (v, o) ∈ D → o = a ∨ o = b := by
  rw [outdeg_eq_length] at h
  have hn := outs_nodup D v hN
  match hl : outs D v, h with
  | [a, b], _ =>
    rw [hl] at hn
    refine ⟨a, b, ?_, mem_outs.mp (by rw [hl]; simp), mem_outs.mp (by rw [hl]; simp), ?_⟩
    · intro e; subst e; simp at hn
    · intro o ho
      have := mem_outs.mpr ho
      rw [hl] at this; simpa using this

theorem two_le {D : Pos n} {v a b : Fin n} (hN : D.Nodup) (ha : (v, a) ∈ D) (hb : (v, b) ∈ D)
    (hab : a ≠ b) : 2 ≤ outdeg D v := by
  rw [outdeg_eq_length]
  have h : [a, b].Nodup := by simp [hab]
  have := h.length_le_of_subset (l₂ := outs D v) (by
    intro x hx
    simp only [List.mem_cons, List.not_mem_nil, or_false] at hx
    rcases hx with rfl | rfl <;> exact mem_outs.mpr ‹_›)
  simpa using this

theorem three_le {D : Pos n} {v a b c : Fin n} (hN : D.Nodup) (ha : (v, a) ∈ D)
    (hb : (v, b) ∈ D) (hc : (v, c) ∈ D) (hab : a ≠ b) (hac : a ≠ c) (hbc : b ≠ c) :
    3 ≤ outdeg D v := by
  rw [outdeg_eq_length]
  have h : [a, b, c].Nodup := by simp [hab, hac, hbc]
  have := h.length_le_of_subset (l₂ := outs D v) (by
    intro x hx
    simp only [List.mem_cons, List.not_mem_nil, or_false] at hx
    rcases hx with rfl | rfl | rfl <;> exact mem_outs.mpr ‹_›)
  simpa using this

/-! ### `Hge D k v`: every directed path with fewer than `k` arcs from `v` ends in a saturated
vertex, i.e. `h(v) ≥ k`. -/

def Hge (D : Pos n) : Nat → Fin n → Prop
  | 0, _ => True
  | k + 1, v => outdeg D v = 2 ∧ ∀ o, (v, o) ∈ D → Hge D k o

theorem Hge.pred {D : Pos n} : ∀ {k : Nat} {v : Fin n}, Hge D (k + 1) v → Hge D k v
  | 0, _, _ => trivial
  | _ + 1, _, h => ⟨h.1, fun o ho => Hge.pred (h.2 o ho)⟩

theorem Hge.le {D : Pos n} {k k' : Nat} {v : Fin n} (h : Hge D k v) (hk : k' ≤ k) :
    Hge D k' v := by
  induction hk with
  | refl => exact h
  | step _ ih => exact ih h.pred

/-- SIZE BOUND (solver-b Lemma 4): `h(v) ≥ k` gives `2^(k+1) - 1` different vertices reachable
from `v`. -/
theorem size_list {D : Pos n} (hF : Forest D) : ∀ (k : Nat) (v : Fin n), Hge D k v →
    ∃ l : List (Fin n), l.Nodup ∧ (∀ c, c ∈ l → Reach D v c) ∧ l.length + 1 = 2 ^ (k + 1)
  | 0, v, _ => ⟨[v], by simp, by intro c hc; simp at hc; subst hc; exact RT.refl _, by simp⟩
  | k + 1, v, h => by
    obtain ⟨a, b, hab, ha, hb, _⟩ := two_out (forest_nodup D hF) h.1
    obtain ⟨la, na, ra, sa⟩ := size_list hF k a (h.2 a ha)
    obtain ⟨lb, nb', rb, sb⟩ := size_list hF k b (h.2 b hb)
    refine ⟨v :: (la ++ lb), ?_, ?_, ?_⟩
    · refine List.nodup_cons.mpr ⟨?_, List.nodup_append.mpr ⟨na, nb', ?_⟩⟩
      · intro hv
        rcases List.mem_append.mp hv with hv | hv
        · exact dag hF ha (ra v hv)
        · exact dag hF hb (rb v hv)
      · intro x hx y hy e
        subst e
        exact disjoint hF ha hb hab (ra x hx) (rb x hy)
    · intro c hc
      rcases List.mem_cons.mp hc with rfl | hc
      · exact RT.refl _
      · rcases List.mem_append.mp hc with hc | hc
        · exact RT.step ha (ra c hc)
        · exact RT.step hb (rb c hc)
    · simp only [List.length_cons, List.length_append]
      have : 2 ^ (k + 1 + 1) = 2 * 2 ^ (k + 1) := by rw [Nat.pow_succ]; omega
      omega

theorem size_bound {D : Pos n} (hF : Forest D) {k : Nat} {v : Fin n} (h : Hge D k v) :
    2 ^ (k + 1) ≤ n + 1 := by
  obtain ⟨l, hn, _, hl⟩ := size_list hF k v h
  have := hn.length_le_of_subset (l₂ := List.finRange n) (fun x _ => List.mem_finRange x)
  rw [List.length_finRange] at this
  omega

/-! ### aligned lists -/

/-- The arc reversed. -/
def sw (a : Arc n) : Arc n := (a.2, a.1)

theorem sw_sw (a : Arc n) : sw (sw a) = a := rfl

/-- `Al D D'`: same edges in the same order, possibly different directions. -/
def Al : Pos n → Pos n → Prop
  | [], [] => True
  | a :: D, a' :: D' => (a' = a ∨ a' = sw a) ∧ Al D D'
  | _, _ => False

theorem al_refl : ∀ D : Pos n, Al D D
  | [] => trivial
  | _ :: D => ⟨Or.inl rfl, al_refl D⟩

theorem al_symm : ∀ {D D' : Pos n}, Al D D' → Al D' D
  | [], [], _ => trivial
  | a :: D, a' :: D', h => by
    refine ⟨?_, al_symm h.2⟩
    rcases h.1 with e | e
    · exact Or.inl e.symm
    · exact Or.inr (by rw [e]; rfl)

theorem al_trans : ∀ {D D' D'' : Pos n}, Al D D' → Al D' D'' → Al D D''
  | [], [], [], _, _ => trivial
  | a :: D, a' :: D', a'' :: D'', h, h' => by
    refine ⟨?_, al_trans h.2 h'.2⟩
    rcases h.1 with e | e <;> rcases h'.1 with e' | e'
    · exact Or.inl (e'.trans e)
    · exact Or.inr (by rw [e', e])
    · exact Or.inr (by rw [e', e])
    · exact Or.inl (by rw [e', e]; rfl)

theorem al_adj : ∀ {D D' : Pos n}, Al D D' → ∀ a b, adj D a b → adj D' a b
  | [], [], _ => fun _ _ h => h
  | (u, v) :: D, a' :: D', h => by
    intro a b hab
    have ih := al_adj h.2 a b
    rcases adj_cons.mp hab with h1 | h1 | h1
    · have := ih h1
      rcases this with h2 | h2
      · exact Or.inl (List.mem_cons_of_mem _ h2)
      · exact Or.inr (List.mem_cons_of_mem _ h2)
    · obtain ⟨rfl, rfl⟩ := h1
      rcases h.1 with e | e
      · exact Or.inl (by rw [e]; exact List.mem_cons_self ..)
      · exact Or.inr (by rw [e]; exact List.mem_cons_self ..)
    · obtain ⟨rfl, rfl⟩ := h1
      rcases h.1 with e | e
      · exact Or.inr (by rw [e]; exact List.mem_cons_self ..)
      · exact Or.inl (by rw [e]; exact List.mem_cons_self ..)

theorem al_conn {D D' : Pos n} (h : Al D D') {a b : Fin n} : Conn D a b ↔ Conn D' a b :=
  ⟨RT.mono (al_adj h), RT.mono (al_adj (al_symm h))⟩

theorem al_forest : ∀ {D D' : Pos n}, Al D D' → Forest D → Forest D'
  | [], [], _, _ => trivial
  | (u, v) :: D, a' :: D', h, hF => by
    have h1 : ¬ Conn D' u v := fun hc => hF.1 ((al_conn h.2).mpr hc)
    rcases h.1 with e | e
    · rw [e]; exact ⟨h1, al_forest h.2 hF.2⟩
    · rw [e]; exact ⟨fun hc => h1 hc.symm, al_forest h.2 hF.2⟩

theorem mem_reorient : ∀ {D D' : Pos n}, D' ∈ reorient D ↔ Al D D'
  | [], D' => by cases D' <;> simp [reorient, Al]
  | (a, b) :: D, D' => by
    simp only [reorient, List.mem_flatMap, List.mem_cons, List.not_mem_nil, or_false]
    constructor
    · rintro ⟨E, hE, rfl | rfl⟩
      · exact ⟨Or.inl rfl, mem_reorient.mp hE⟩
      · exact ⟨Or.inr rfl, mem_reorient.mp hE⟩
    · intro h
      match D', h with
      | a' :: E, h =>
        refine ⟨E, mem_reorient.mpr h.2, ?_⟩
        rcases h.1 with e | e
        · exact Or.inl (by rw [e])
        · exact Or.inr (by rw [e]; rfl)

/-! ### flipping a set of arcs -/

def flipSet (D : Pos n) (P : List (Arc n)) : Pos n := D.map (fun a => if a ∈ P then sw a else a)

theorem flipSet_nil (D : Pos n) : flipSet D [] = D := by simp [flipSet]

theorem al_flipSet : ∀ (D : Pos n) (P : List (Arc n)), Al D (flipSet D P)
  | [], _ => trivial
  | a :: D, P => by
    refine ⟨?_, al_flipSet D P⟩
    by_cases h : a ∈ P <;> simp [h]

theorem mem_flipSet {D : Pos n} {P : List (Arc n)} {a b : Fin n} :
    (a, b) ∈ flipSet D P ↔ ((a, b) ∈ D ∧ (a, b) ∉ P) ∨ ((b, a) ∈ D ∧ (b, a) ∈ P) := by
  simp only [flipSet, List.mem_map]
  constructor
  · rintro ⟨e, he, heq⟩
    by_cases hp : e ∈ P
    · rw [if_pos' hp] at heq
      have : e = (b, a) := by rw [← sw_sw e, heq]; rfl
      subst this; exact Or.inr ⟨he, hp⟩
    · rw [if_neg' hp] at heq
      subst heq; exact Or.inl ⟨he, hp⟩
  · rintro (⟨h1, h2⟩ | ⟨h1, h2⟩)
    · exact ⟨(a, b), h1, by rw [if_neg' h2]⟩
    · exact ⟨(b, a), h1, by rw [if_pos' h2]; rfl⟩

/-- Indicator of `a = b`. -/
def ind (a b : Fin n) : Nat := if a = b then 1 else 0

theorem ind_self (a : Fin n) : ind a a = 1 := by simp [ind]

theorem ind_ne {a b : Fin n} (h : a ≠ b) : ind a b = 0 := by simp [ind, h]

theorem ind_le (a b : Fin n) : ind a b ≤ 1 := by unfold ind; split <;> omega

theorem outdeg_cons (a : Arc n) (D : Pos n) (v : Fin n) :
    outdeg (a :: D) v = outdeg D v + ind a.1 v := by
  simp [outdeg, List.countP_cons, ind]

theorem outdeg_flipSet (D : Pos n) (P : List (Arc n)) (v : Fin n) :
    outdeg (flipSet D P) v + D.countP (fun a => decide (a ∈ P) && (a.1 == v))
      = outdeg D v + D.countP (fun a => decide (a ∈ P) && (a.2 == v)) := by
  induction D with
  | nil => rfl
  | cons a D ih =>
    have e1 : flipSet (a :: D) P = (if a ∈ P then sw a else a) :: flipSet D P := rfl
    rw [e1, outdeg_cons, outdeg_cons, List.countP_cons, List.countP_cons]
    by_cases hP : a ∈ P <;> by_cases h1 : a.1 = v <;> by_cases h2 : a.2 = v <;>
      simp [hP, h1, h2, sw, ind] <;> omega

theorem countP_mem_eq {α : Type} [DecidableEq α] (D P : List α) (q : α → Bool)
    (hD : D.Nodup) (hP : P.Nodup) (hs : ∀ a, a ∈ P → a ∈ D) :
    D.countP (fun a => decide (a ∈ P) && q a) = P.countP q := by
  rw [List.countP_eq_length_filter, List.countP_eq_length_filter]
  apply Nat.le_antisymm
  · apply List.Nodup.length_le_of_subset (hD.filter _)
    intro a ha
    simp only [List.mem_filter, Bool.and_eq_true, decide_eq_true_eq] at ha ⊢
    exact ⟨ha.2.1, ha.2.2⟩
  · apply List.Nodup.length_le_of_subset (hP.filter _)
    intro a ha
    simp only [List.mem_filter, Bool.and_eq_true, decide_eq_true_eq] at ha ⊢
    exact ⟨hs a ha.1, ha.1, ha.2⟩

/-! ### normal paths -/

/-- The arcs of the path `t → w₁ → w₂ → ...`. -/
def pathArcs : Fin n → List (Fin n) → List (Arc n)
  | _, [] => []
  | t, w :: ws => (t, w) :: pathArcs w ws

/-- Last vertex of the path. -/
def lastV : Fin n → List (Fin n) → Fin n
  | t, [] => t
  | _, w :: ws => lastV w ws

/-- `NPath D t [w₁, ..., w_d]`: `t → w₁ → ... → w_d` is a directed path of `D`, all its vertices
except the last are saturated and the last is unsaturated.  (`d = 0`: `t` is unsaturated.) -/
def NPath (D : Pos n) : Fin n → List (Fin n) → Prop
  | t, [] => outdeg D t < 2
  | t, w :: ws => outdeg D t = 2 ∧ (t, w) ∈ D ∧ NPath D w ws

theorem npath_sub {D : Pos n} : ∀ {t : Fin n} {ws : List (Fin n)}, NPath D t ws →
    ∀ a, a ∈ pathArcs t ws → a ∈ D
  | _, [], _ => by intro a h; cases h
  | t, w :: ws, h => by
    intro a ha
    rcases List.mem_cons.mp ha with rfl | ha
    · exact h.2.1
    · exact npath_sub h.2.2 a ha

theorem npath_reach {D : Pos n} : ∀ {t : Fin n} {ws : List (Fin n)}, NPath D t ws →
    ∀ c, c ∈ ws → ∃ w ws', ws = w :: ws' ∧ (t, w) ∈ D ∧ Reach D w c
  | _, [], _ => by intro c h; cases h
  | t, w :: ws, h => by
    intro c hc
    refine ⟨w, ws, rfl, h.2.1, ?_⟩
    rcases List.mem_cons.mp hc with rfl | hc
    · exact RT.refl _
    · obtain ⟨w', ws', _, h1, h2⟩ := npath_reach h.2.2 c hc
      exact RT.step h1 h2

theorem npath_nodup {D : Pos n} (hF : Forest D) : ∀ {t : Fin n} {ws : List (Fin n)},
    NPath D t ws → (t :: ws).Nodup
  | _, [], _ => by simp
  | t, w :: ws, h => by
    refine List.nodup_cons.mpr ⟨?_, npath_nodup hF h.2.2⟩
    intro ht
    obtain ⟨w', ws', e, h1, h2⟩ := npath_reach h t ht
    exact dag hF h1 h2

theorem npath_last {D : Pos n} : ∀ {t : Fin n} {ws : List (Fin n)}, NPath D t ws →
    outdeg D (lastV t ws) < 2
  | _, [], h => h
  | _, w :: ws, h => npath_last (t := w) (ws := ws) h.2.2

theorem lastV_mem : ∀ (t : Fin n) (ws : List (Fin n)), lastV t ws ∈ t :: ws
  | _, [] => by simp [lastV]
  | t, w :: ws => List.mem_cons_of_mem _ (lastV_mem w ws)

theorem mem_pathArcs : ∀ {t : Fin n} {ws : List (Fin n)} {a b : Fin n},
    (a, b) ∈ pathArcs t ws → a ∈ t :: ws ∧ b ∈ ws
  | _, [], _, _, h => by cases h
  | t, w :: ws, a, b, h => by
    rcases List.mem_cons.mp h with e | h
    · simp only [Prod.mk.injEq] at e
      obtain ⟨rfl, rfl⟩ := e
      exact ⟨List.mem_cons_self .., List.mem_cons_self ..⟩
    · have := mem_pathArcs h
      exact ⟨List.mem_cons_of_mem _ this.1, List.mem_cons_of_mem _ this.2⟩

theorem pathArcs_nodup : ∀ {t : Fin n} {ws : List (Fin n)}, (t :: ws).Nodup →
    (pathArcs t ws).Nodup
  | _, [], _ => List.nodup_nil
  | t, w :: ws, h => by
    have h' := List.nodup_cons.mp h
    refine List.nodup_cons.mpr ⟨?_, pathArcs_nodup h'.2⟩
    intro hm
    exact h'.1 (mem_pathArcs hm).1

/-- telescoping: (arcs of the path leaving `v`) - (arcs entering `v`) = [v = t] - [v = last]. -/
theorem path_count : ∀ (t : Fin n) (ws : List (Fin n)) (v : Fin n),
    (pathArcs t ws).countP (fun a => a.1 == v) + ind (lastV t ws) v
      = (pathArcs t ws).countP (fun a => a.2 == v) + ind t v
  | t, [], v => rfl
  | t, w :: ws, v => by
    have := path_count w ws v
    have e1 : (pathArcs t (w :: ws)).countP (fun a => a.1 == v)
        = (pathArcs w ws).countP (fun a => a.1 == v) + ind t v := by
      simp [pathArcs, List.countP_cons, ind]
    have e2 : (pathArcs t (w :: ws)).countP (fun a => a.2 == v)
        = (pathArcs w ws).countP (fun a => a.2 == v) + ind w v := by
      simp [pathArcs, List.countP_cons, ind]
    rw [e1, e2]
    show _ + ind (lastV w ws) v = _
    omega

end FO
