/-
U3 (rule L as a function, legality, cost) and U4 (assembly):
    `upper_third : ∀ n, 1 ≤ n → 3 * FO.T n + 1 ≤ n`.
-/
import FO.UpperSpec
import FO.UpperRound
namespace FO
open Upper
variable {n : Nat}

/-! ### rule L as a computable reply -/

/-- A shortest directed path from `v` to an unsaturated vertex (without `v` itself): at a
saturated vertex step to the out-neighbour of smaller `h` (the first one on a tie). -/
def spath (D : Pos n) : Nat → Fin n → List (Fin n)
  | 0, _ => []
  | f + 1, v =>
    match outs D v with
    | [a, b] => if hh D a ≤ hh D b then a :: spath D f a else b :: spath D f b
    | _ => []

/-- The code `2h + ε` that orders the two named vertices.  Comparing codes is coarser than
comparing the sorted slots `(P, Q)` lexicographically (`h = P + 1`, `ε = [P < Q]`): if the slots
of `u` are lexicographically at most those of `v` then `code u ≤ code v`.  So a rule that
compares the slots lexicographically is one way of breaking the ties of this rule. -/
def code (D : Pos n) (v : Fin n) : Nat := 2 * hh D v + eps D v

/-- RULE L's reply to the pair `(u, v)`: flip a shortest path from the endpoint `x` of smaller
code and direct the new edge out of `x`. -/
def ruleL (D : Pos n) (u v : Fin n) : Pos n :=
  if code D u ≤ code D v then nreply D u v (spath D n u) else nreply D v u (spath D n v)

/-! Sanity checks, each can fail.
Position on 8 vertices: 0 → 1, 2;  2 → 3, 4;  5 → 6, 7.  `h(0) = 1` with slots (0, 1), `h(5) = 1`
with slots (0, 0).  Rule L must flip from 5 (smaller slots): path `5 → 6` (the first
out-neighbour in list order is 7 here, see the #guard), cost 1, and Φ must rise by at least 3.
Flipping from 0 instead (path 0 → 1) raises Φ by 2 only: the tie-break is necessary for the
potential argument. -/
def exD : Pos 8 := [(0, 1), (0, 2), (2, 3), (2, 4), (5, 6), (5, 7)]
#guard (List.finRange 8).map (code exD) == [3, 0, 2, 0, 0, 2, 0, 0]
#guard ruleL exD 0 5 == nreply exD 5 0 [6]
#guard ruleL exD 5 0 == nreply exD 5 0 [6]
#guard ruleL exD 0 5 == [(5, 0), (0, 1), (0, 2), (2, 3), (2, 4), (6, 5), (5, 7)]
#guard cost exD (ruleL exD 0 5) == 1
#guard Phi exD == -1
-- after the reply: w(2) = −1, w(0) = 1, w(5) = 3·0 + 1 + 0 + 1 = 2 (out-neighbours 7: (0,0) and
-- 0: (1,1)), w(6) = 1 (one out-neighbour of height 1): Φ = 3 = −1 + 4 ≥ −1 + 3·1.
-- (My first expectation here, 5, was wrong; the #guard caught it.)
#guard Phi (ruleL exD 0 5) == 3
#guard Phi (nreply exD 0 5 [1]) == Phi exD + 2      -- the wrong endpoint: gain 2 < 3 · 1
#guard (ruleL exD 0 5) ∈ replies exD 0 5
-- a free reply: joining two isolated vertices costs nothing
#guard ruleL ([] : Pos 3) 0 1 == [(0, 1)]
#guard cost ([(0, 1)] : Pos 3) (ruleL ([(0, 1)] : Pos 3) 1 2) == 0

/-! ### the shortest path -/

theorem spath_spec {D : Pos n} (hF' : Forest D) (hV : Valid D) : ∀ (f : Nat) (v : Fin n),
    hh D v ≤ f → NPath D v (spath D f v) ∧ (spath D f v).length = hh D v := by
  have hN := forest_nodup D hF'
  intro f
  induction f with
  | zero =>
    intro v hv
    refine ⟨?_, by simp [spath]; omega⟩
    show outdeg D v < 2
    rcases outs_shape hV hN v with ho | ⟨c, ho⟩ | ⟨a, b, _, ho⟩
    · rw [outdeg_eq_length, ho]; simp
    · rw [outdeg_eq_length, ho]; simp
    · have := hh_two hF' ho; omega
  | succ f ih =>
    intro v hv
    rcases outs_shape hV hN v with ho | ⟨c, ho⟩ | ⟨a, b, _, ho⟩
    · have hd : outdeg D v < 2 := by rw [outdeg_eq_length, ho]; simp
      have e : spath D (f + 1) v = [] := by simp [spath, ho]
      rw [e]
      exact ⟨hd, by rw [hh_unsat (by omega)]; rfl⟩
    · have hd : outdeg D v < 2 := by rw [outdeg_eq_length, ho]; simp
      have e : spath D (f + 1) v = [] := by simp [spath, ho]
      rw [e]
      exact ⟨hd, by rw [hh_unsat (by omega)]; rfl⟩
    · have hd : outdeg D v = 2 := by rw [outdeg_eq_length, ho]; rfl
      have h2 := hh_two hF' ho
      have ha : (v, a) ∈ D := mem_outs.mp (by rw [ho]; simp)
      have hb : (v, b) ∈ D := mem_outs.mp (by rw [ho]; simp)
      by_cases hab : hh D a ≤ hh D b
      · have e : spath D (f + 1) v = a :: spath D f a := by simp [spath, ho, hab]
        obtain ⟨i1, i2⟩ := ih a (by omega)
        rw [e]
        exact ⟨⟨hd, ha, i1⟩, by simp only [List.length_cons]; omega⟩
      · have e : spath D (f + 1) v = b :: spath D f b := by simp [spath, ho, hab]
        obtain ⟨i1, i2⟩ := ih b (by omega)
        rw [e]
        exact ⟨⟨hd, hb, i1⟩, by simp only [List.length_cons]; omega⟩

/-! ### cost of a normal reply -/

theorem flips_self : ∀ (D : Pos n), flips D D = 0
  | [] => rfl
  | a :: D => by
    show (if a = a then 0 else 1) + flips D D = 0
    rw [flips_self D]; simp

/-- The cost of a normal reply is the length of the flipped path. -/
theorem cost_nreply {D : Pos n} {x y : Fin n} {ws : List (Fin n)} (hF' : Forest D)
    (hp : NPath D x ws) : cost D (nreply D x y ws) = ws.length := by
  have h1 := flips_flipSet hF' (al_flipSet D (pathArcs x ws)) (pathArcs x ws)
    (fun a ha haD => by
      obtain ⟨a1, a2⟩ := a
      exact mem_flipSet.mpr (Or.inr ⟨haD, ha⟩))
  have h2 : D.countP (fun a => decide (a ∈ pathArcs x ws) && true)
      = (pathArcs x ws).countP (fun _ => true) :=
    countP_mem_eq D (pathArcs x ws) (fun _ => true) (forest_nodup D hF')
      (pathArcs_nodup (npath_nodup hF' hp)) (npath_sub hp)
  have h3 : (pathArcs x ws).countP (fun _ => true) = ws.length := by
    rw [← pathArcs_length x ws]; simp
  have h4 : cost D (nreply D x y ws) = flips D (flipSet D (pathArcs x ws)) := rfl
  rw [flips_self] at h1
  omega

theorem length_nreply (D : Pos n) (x y : Fin n) (ws : List (Fin n)) :
    (nreply D x y ws).length = D.length + 1 := by
  simp [nreply, flipSet]

/-! ### the round with rule L (U3) -/

/-- RULE L, one round.  For a valid forest position `D` and two vertices in different trees,
the reply of rule L is a legal reply (an element of `replies D u v`), it is a valid forest
position with one more edge, and the potential (in thirds) rises by at least three times its
cost. -/
theorem ruleL_round {D : Pos n} (hF' : Forest D) (hV : Valid D) {u v : Fin n}
    (hc : ¬ Conn D u v) :
    ruleL D u v ∈ replies D u v ∧ Forest (ruleL D u v) ∧ Valid (ruleL D u v) ∧
      (ruleL D u v).length = D.length + 1 ∧
      Phi D + 3 * (cost D (ruleL D u v) : Int) ≤ Phi (ruleL D u v) := by
  unfold ruleL
  by_cases h : code D u ≤ code D v
  · rw [if_pos' h]
    obtain ⟨hp, hl⟩ := spath_spec hF' hV n u (hF_le D n u)
    refine ⟨mem_replies.mpr ⟨⟨Or.inl rfl, al_flipSet D _⟩, valid_nreply hF' hV hp⟩,
      forest_nreply hF' hc, valid_nreply hF' hV hp, length_nreply .., ?_⟩
    rw [cost_nreply hF' hp]
    exact round_gain hF' hV hc hp hl h
  · rw [if_neg' h]
    have hc' : ¬ Conn D v u := fun h => hc h.symm
    have h' : 2 * hh D v + eps D v ≤ 2 * hh D u + eps D u := by unfold code at h; omega
    obtain ⟨hp, hl⟩ := spath_spec hF' hV n v (hF_le D n v)
    refine ⟨mem_replies.mpr ⟨⟨Or.inr rfl, al_flipSet D _⟩, valid_nreply hF' hV hp⟩,
      forest_nreply hF' hc', valid_nreply hF' hV hp, length_nreply .., ?_⟩
    rw [cost_nreply hF' hp]
    exact round_gain hF' hV hc' hp hl h'

/-! ### assembly (U4) -/

theorem minL_le {l : List Nat} {a : Nat} (h : a ∈ l) : minL l ≤ a :=
  (le_minL_iff (l := l) (k := minL l) (by intro e; rw [e] at h; cases h)).mp (Nat.le_refl _) a h

theorem maxL_mem : ∀ (l : List Nat), maxL l = 0 ∨ maxL l ∈ l
  | [] => Or.inl rfl
  | a :: l => by
    have e : maxL (a :: l) = max a (maxL l) := rfl
    rw [e]
    by_cases h : maxL l ≤ a
    · rw [Nat.max_eq_left h]; exact Or.inr (List.mem_cons_self ..)
    · rw [Nat.max_eq_right (by omega)]
      rcases maxL_mem l with h0 | hm
      · omega
      · exact Or.inr (List.mem_cons_of_mem _ hm)

theorem conn_of_mem_moves {D : Pos n} {u v : Fin n} (h : (u, v) ∈ moves D) : ¬ Conn D u v := by
  unfold moves at h
  obtain ⟨u', _, h⟩ := List.mem_flatMap.mp h
  obtain ⟨v', hv', e⟩ := List.mem_map.mp h
  simp only [Prod.mk.injEq] at e
  obtain ⟨rfl, rfl⟩ := e
  have := (List.mem_filter.mp hv').2
  intro hc
  rw [(conn_iff D u' v').mpr hc] at this
  simp at this

/-- THE POTENTIAL ARGUMENT.  From every valid forest position `D`, with `k` rounds left:
`3 · value + Φ(D) ≤ #edges(D) + k`.  (`Φ` is in thirds; after `k` more rounds there are
`#edges + k` edges, and Lemma 6 bounds `Φ` of the final position by that number.) -/
theorem upper_value : ∀ (k : Nat) (D : Pos n), Forest D → Valid D →
    3 * (value k D : Int) + Phi D ≤ D.length + k := by
  intro k
  induction k with
  | zero =>
    intro D hF' hV
    have := lemma6 hF' hV
    show 3 * ((0 : Nat) : Int) + Phi D ≤ D.length + ((0 : Nat) : Int)
    omega
  | succ k ih =>
    intro D hF' hV
    have h6 := lemma6 hF' hV
    have e : value (k + 1) D = maxL ((moves D).map fun m =>
        minL ((replies D m.1 m.2).map fun D' => cost D D' + value k D')) := rfl
    rcases maxL_mem ((moves D).map fun m =>
        minL ((replies D m.1 m.2).map fun D' => cost D D' + value k D')) with h0 | hm
    · rw [e, h0]; omega
    · obtain ⟨⟨u, v⟩, hmv, hval⟩ := List.mem_map.mp hm
      have hc := conn_of_mem_moves hmv
      obtain ⟨hrep, hF2, hV2, hlen, hgain⟩ := ruleL_round hF' hV hc
      have h1 : minL ((replies D u v).map fun D' => cost D D' + value k D')
          ≤ cost D (ruleL D u v) + value k (ruleL D u v) :=
        minL_le (List.mem_map.mpr ⟨_, hrep, rfl⟩)
      have h2 := ih (ruleL D u v) hF2 hV2
      rw [hlen] at h2
      have h3 : value (k + 1) D ≤ cost D (ruleL D u v) + value k (ruleL D u v) := by
        rw [e, ← hval]; exact h1
      omega

/-- THEOREM 5, the statement of FO/UpperSpec.lean. -/
theorem upper : UpperStatement := by
  intro n hn
  have h := upper_value (n := n) (n - 1) [] trivial (fun v => Nat.zero_le _)
  rw [phi_nil] at h
  have e : T n = value (n - 1) ([] : Pos n) := rfl
  simp only [List.length_nil] at h
  omega

end FO

/-- THE UPPER BOUND `T(n) ≤ (n − 1)/3`, in integers. -/
theorem upper_third : ∀ n : Nat, 1 ≤ n → 3 * FO.T n + 1 ≤ n := FO.upper

/-! Checks of the final statement, each can fail.  The statement with `+ 2` is false at `n = 1`
(`T 1 = 0`); Lemma 6 in the form `lemma6_gamma` is tight on the path `0 → 1 → 2`
(`Φ = 1`, `γ(0) = 2`, two edges). -/
example : ¬ (∀ n : Nat, 1 ≤ n → 3 * FO.T n + 2 ≤ n) := by
  intro h; have := h 1 (by decide); revert this; decide
example : 3 * FO.T 2 + 1 ≤ 2 := upper_third 2 (by decide)
#guard FO.Phi ([(0, 1), (1, 2)] : FO.Pos 3) + FO.gamv ([(0, 1), (1, 2)] : FO.Pos 3) 0 == 2 + 1
