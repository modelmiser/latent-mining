/-
U2, part 1.  The numeric height `hh`, the flag `eps`, the weight `wv` and the potential `Phi`
of a real position, and the link between `hh` and the predicate `Hge` ("h(v) ≥ k") of the
lower-bound development.
-/
import FO.Reply
import FO.UpperCore
namespace FO
open Upper
variable {n : Nat}

/-- `h` with fuel: `hF D f v = min (h(v), f)`. -/
def hF (D : Pos n) : Nat → Fin n → Nat
  | 0, _ => 0
  | f + 1, v => if outdeg D v = 2 then 1 + minL ((outs D v).map (hF D f)) else 0

/-- `h(v)`: 0 if `v` is unsaturated, else 1 + the smaller `h` of its two out-neighbours.
(Fuel `n` is enough in a forest: `h(v) < n`, see `hge_lt` and `le_hh_iff`.) -/
def hh (D : Pos n) (v : Fin n) : Nat := hF D n v

/-- The flag `ε(v)`: 0 if the two slots of `v` are equal, 1 otherwise.  The slots are the `h`
of the out-neighbours and −1 for each missing one, so: no out-neighbour → equal; one → unequal;
two → compare their heights. -/
def eps (D : Pos n) (v : Fin n) : Nat :=
  match (outs D v).map (hh D) with
  | [] => 0
  | [_] => 1
  | [p, q] => flagNe p q
  | _ => 0

/-- The weight of `v` (in thirds): the table of UpperCore applied to the (height, flag) pairs of
its out-neighbours. -/
def wv (D : Pos n) (v : Fin n) : Int := wt ((outs D v).map fun c => (hh D c, eps D c))

/-- `γ(v) = 3 h(v) + 1 + ε(v)`. -/
def gamv (D : Pos n) (v : Fin n) : Int := gam (hh D v) (eps D v)

/-- The potential (in thirds): the sum of the weights of all vertices. -/
def Phi (D : Pos n) : Int := ((List.finRange n).map (wv D)).sum

/-! Hand computations, each can fail.
Path 0 → 1 → 2: `w(2) = 0`, `w(1) = 0` (one out-neighbour, a leaf), `w(0) = 1` (one
out-neighbour of height 0 and flag 1).  Complete binary tree on 7 vertices: the two inner
vertices are cherries (−1 each), the root has two out-neighbours (1, 0): 3·1 − 1 = 2; Φ = 0.
A star with all arcs pointing to the centre: every weight is 0. -/
#guard Phi ([(0, 1), (1, 2)] : Pos 3) == 1
#guard (List.finRange 3).map (hh ([(0, 1), (1, 2)] : Pos 3)) == [0, 0, 0]
#guard (List.finRange 3).map (eps ([(0, 1), (1, 2)] : Pos 3)) == [1, 1, 0]
#guard Phi ([(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)] : Pos 7) == 0
#guard (List.finRange 7).map (hh ([(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)] : Pos 7))
  == [2, 1, 1, 0, 0, 0, 0]
#guard (List.finRange 7).map (wv ([(0, 1), (0, 2), (1, 3), (1, 4), (2, 5), (2, 6)] : Pos 7))
  == [2, -1, -1, 0, 0, 0, 0]
#guard Phi ([(1, 0), (2, 0), (3, 0)] : Pos 4) == 0
-- cherry 0 → 1, 2 with 1 → 3: out-neighbours of 0 are (0, 1) and (0, 0): 3·0 − 1 + 1 + 0 = 0
#guard (List.finRange 4).map (wv ([(0, 1), (0, 2), (1, 3)] : Pos 4)) == [0, 0, 0, 0]
#guard Phi ([] : Pos 5) == 0

/-! ### minimum of a list -/

theorem le_minL_iff : ∀ {l : List Nat} {k : Nat}, l ≠ [] → (k ≤ minL l ↔ ∀ x, x ∈ l → k ≤ x)
  | [a], k, _ => by simp [minL]
  | a :: b :: l, k, _ => by
    have ih := le_minL_iff (l := b :: l) (k := k) (by simp)
    have e : minL (a :: b :: l) = min a (minL (b :: l)) := rfl
    rw [e, Nat.le_min, ih]
    constructor
    · rintro ⟨h1, h2⟩ x hx
      rcases List.mem_cons.mp hx with rfl | hx
      · exact h1
      · exact h2 x hx
    · intro h
      exact ⟨h a (List.mem_cons_self ..), fun x hx => h x (List.mem_cons_of_mem _ hx)⟩

/-! ### `hh` and `Hge` -/

theorem outs_ne_nil {D : Pos n} {v : Fin n} (h : outdeg D v = 2) : outs D v ≠ [] := by
  intro e; rw [outdeg_eq_length, e] at h; cases h

theorem le_hF_iff (D : Pos n) : ∀ (f k : Nat) (v : Fin n), k ≤ f → (k ≤ hF D f v ↔ Hge D k v) := by
  intro f
  induction f with
  | zero =>
    intro k v hk
    have : k = 0 := by omega
    subst this
    simp [Hge]
  | succ f ih =>
    intro k v hk
    cases k with
    | zero => simp [Hge]
    | succ k =>
      show k + 1 ≤ (if outdeg D v = 2 then 1 + minL ((outs D v).map (hF D f)) else 0) ↔
        (outdeg D v = 2 ∧ ∀ o, (v, o) ∈ D → Hge D k o)
      by_cases hd : outdeg D v = 2
      · rw [if_pos' hd]
        have hne : (outs D v).map (hF D f) ≠ [] := by
          intro e; exact outs_ne_nil hd (List.map_eq_nil_iff.mp e)
        have h1 : k + 1 ≤ 1 + minL ((outs D v).map (hF D f)) ↔
            k ≤ minL ((outs D v).map (hF D f)) := by omega
        rw [h1, le_minL_iff hne]
        constructor
        · intro h
          exact ⟨hd, fun o ho => (ih k o (by omega)).mp
            (h _ (List.mem_map_of_mem (mem_outs.mpr ho)))⟩
        · rintro ⟨_, h⟩ x hx
          obtain ⟨o, ho, rfl⟩ := List.mem_map.mp hx
          exact (ih k o (by omega)).mpr (h o (mem_outs.mp ho))
      · rw [if_neg' hd]
        constructor
        · intro h; omega
        · intro h; exact absurd h.1 hd

theorem hF_le (D : Pos n) : ∀ (f : Nat) (v : Fin n), hF D f v ≤ f := by
  intro f v
  apply Nat.le_of_not_lt
  intro h
  have h1 : f + 1 ≤ hF D f v := h
  cases f with
  | zero => simp [hF] at h1
  | succ f =>
    have h2 : f + 1 ≤ hF D (f + 1) v := by omega
    have h3 := (le_hF_iff D (f + 1) (f + 1) v (Nat.le_refl _)).mp h2
    have h4 : hF D (f + 1) v = (if outdeg D v = 2 then 1 + minL ((outs D v).map (hF D f)) else 0) :=
      rfl
    rw [if_pos' h3.1] at h4
    have hne : (outs D v).map (hF D f) ≠ [] := by
      intro e; exact outs_ne_nil h3.1 (List.map_eq_nil_iff.mp e)
    have h5 : f + 1 ≤ minL ((outs D v).map (hF D f)) := by omega
    rw [le_minL_iff hne] at h5
    match hl : outs D v with
    | [] => exact outs_ne_nil h3.1 hl
    | o :: _ =>
      have := h5 (hF D f o) (List.mem_map_of_mem (by rw [hl]; exact List.mem_cons_self ..))
      have := hF_le D f o
      omega

/-- In a forest `h(v) < n`. -/
theorem hge_lt {D : Pos n} (hF' : Forest D) {k : Nat} {v : Fin n} (h : Hge D k v) : k < n := by
  have h1 := size_bound hF' h
  have h2 := Nat.lt_two_pow_self (n := k + 1)
  omega

/-- `hh` is the `h` of the lower-bound development: `k ≤ h(v)` iff `Hge D k v`. -/
theorem le_hh_iff {D : Pos n} (hF' : Forest D) {k : Nat} {v : Fin n} :
    k ≤ hh D v ↔ Hge D k v := by
  constructor
  · intro h
    exact (le_hF_iff D n k v (Nat.le_trans h (hF_le D n v))).mp h
  · intro h
    exact (le_hF_iff D n k v (Nat.le_of_lt (hge_lt hF' h))).mpr h

theorem hge_hh {D : Pos n} (hF' : Forest D) (v : Fin n) : Hge D (hh D v) v :=
  (le_hh_iff hF').mp (Nat.le_refl _)

/-- Two forests' heights agree at `v` if the predicates `Hge` agree at `v`. -/
theorem hh_congr {D D' : Pos n} (hF1 : Forest D) (hF2 : Forest D') {v v' : Fin n}
    (h : ∀ k, Hge D' k v' ↔ Hge D k v) : hh D' v' = hh D v := by
  apply Nat.le_antisymm
  · exact (le_hh_iff hF1).mpr ((h _).mp (hge_hh hF2 v'))
  · exact (le_hh_iff hF2).mpr ((h _).mpr (hge_hh hF1 v))

theorem hh_unsat {D : Pos n} {v : Fin n} (h : outdeg D v ≠ 2) : hh D v = 0 := by
  unfold hh
  cases n with
  | zero => rfl
  | succ m => show (if outdeg D v = 2 then _ else 0) = 0; rw [if_neg' h]

theorem hh_two {D : Pos n} (hF' : Forest D) {v a b : Fin n} (ho : outs D v = [a, b]) :
    hh D v = 1 + min (hh D a) (hh D b) := by
  have hd : outdeg D v = 2 := by rw [outdeg_eq_length, ho]; rfl
  have hmem : ∀ o, (v, o) ∈ D ↔ o = a ∨ o = b := by
    intro o; rw [← mem_outs, ho]; simp
  apply Nat.le_antisymm
  · cases hk : hh D v with
    | zero => omega
    | succ k =>
      have h1 : Hge D (k + 1) v := hk ▸ hge_hh hF' v
      have ha := (le_hh_iff hF').mpr (h1.2 a ((hmem a).mpr (Or.inl rfl)))
      have hb := (le_hh_iff hF').mpr (h1.2 b ((hmem b).mpr (Or.inr rfl)))
      omega
  · have e : 1 + min (hh D a) (hh D b) = min (hh D a) (hh D b) + 1 := by omega
    rw [e]
    apply (le_hh_iff hF').mpr
    refine ⟨hd, fun o ho' => ?_⟩
    rcases (hmem o).mp ho' with rfl | rfl
    · exact (le_hh_iff hF').mp (Nat.min_le_left _ _)
    · exact (le_hh_iff hF').mp (Nat.min_le_right _ _)

/-! ### flag, weight and `γ` by the shape of the out-neighbour list -/

theorem eps_nil {D : Pos n} {v : Fin n} (ho : outs D v = []) : eps D v = 0 := by
  simp [eps, ho]
theorem eps_one {D : Pos n} {v c : Fin n} (ho : outs D v = [c]) : eps D v = 1 := by
  simp [eps, ho]
theorem eps_two {D : Pos n} {v a b : Fin n} (ho : outs D v = [a, b]) :
    eps D v = flagNe (hh D a) (hh D b) := by
  simp [eps, ho]

theorem eps_le (D : Pos n) (v : Fin n) : eps D v ≤ 1 := by
  unfold eps
  split
  · omega
  · omega
  · exact flagNe_le _ _
  · omega

theorem wv_nil {D : Pos n} {v : Fin n} (ho : outs D v = []) : wv D v = 0 := by
  simp [wv, ho, wt]
theorem wv_one {D : Pos n} {v c : Fin n} (ho : outs D v = [c]) :
    wv D v = w1 (hh D c) (eps D c) := by
  simp [wv, ho, wt]
theorem wv_two {D : Pos n} {v a b : Fin n} (ho : outs D v = [a, b]) :
    wv D v = w2 (hh D a) (eps D a) (hh D b) (eps D b) := by
  simp [wv, ho, wt]

/-- The list of out-neighbours of a vertex of a valid forest position has one of three shapes. -/
theorem outs_shape {D : Pos n} (hV : Valid D) (hN : D.Nodup) (v : Fin n) :
    outs D v = [] ∨ (∃ c, outs D v = [c]) ∨ (∃ a b, a ≠ b ∧ outs D v = [a, b]) := by
  have h := hV v
  rw [outdeg_eq_length] at h
  have hn := outs_nodup D v hN
  match hl : outs D v with
  | [] => exact Or.inl rfl
  | [c] => exact Or.inr (Or.inl ⟨c, rfl⟩)
  | [a, b] =>
    refine Or.inr (Or.inr ⟨a, b, ?_, rfl⟩)
    rw [hl] at hn
    intro e; subst e; simp at hn
  | _ :: _ :: _ :: _ => rw [hl] at h; simp at h

theorem gamv_nil {D : Pos n} {v : Fin n} (ho : outs D v = []) : gamv D v = 1 := by
  have hd : outdeg D v ≠ 2 := by rw [outdeg_eq_length, ho]; simp
  simp [gamv, gam, hh_unsat hd, eps_nil ho]
theorem gamv_one {D : Pos n} {v c : Fin n} (ho : outs D v = [c]) : gamv D v = 2 := by
  have hd : outdeg D v ≠ 2 := by rw [outdeg_eq_length, ho]; simp
  simp [gamv, gam, hh_unsat hd, eps_one ho]
theorem gamv_two {D : Pos n} (hF' : Forest D) {v a b : Fin n} (ho : outs D v = [a, b]) :
    gamv D v = gam (min (hh D a) (hh D b) + 1) (flagNe (hh D a) (hh D b)) := by
  simp only [gamv, hh_two hF' ho, eps_two ho]
  rw [Nat.add_comm]

theorem gamv_pos (D : Pos n) (v : Fin n) : 1 ≤ gamv D v := by
  simp only [gamv, gam]; omega

end FO
