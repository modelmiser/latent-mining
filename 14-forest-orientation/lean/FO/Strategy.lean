/-
M3.  Theorem 1 for the normal-form game: Adversary's strategy (main phase, cleanup) forces
the bound against every sequence of normal replies.  Combines M1 (`Core.core_bound`) with M2.
-/
import FO.Reply
namespace FO
variable {n : Nat}

/-- Adversary's legal moves in the normal-form game: two vertices in different components. -/
def NLegal (D : Pos n) (m : Fin n × Fin n) : Prop := ¬ Conn D m.1 m.2

/-- Algorithm's NORMAL replies to the named pair `m`: choose an endpoint `t` (the other one is
`hd`) and a path `t → w₁ → ... → w_d` (`d ≥ 0`) whose vertices are saturated except the last;
flip the path and add `t → hd`; pay `d`. -/
def NReply (D : Pos n) (m : Fin n × Fin n) (c : Nat) (D' : Pos n) : Prop :=
  ∃ t hd ws, ((t = m.1 ∧ hd = m.2) ∨ (t = m.2 ∧ hd = m.1)) ∧ NPath D t ws ∧
    D' = nreply D t hd ws ∧ c = ws.length

/-- `Rep D l`: `D` is a valid orientation of a forest and its trees can be listed with one
witness each, the witnesses having the indices listed in `l` (in some order). -/
def Rep (D : Pos n) (l : List Nat) : Prop :=
  Forest D ∧ Valid D ∧ ∃ wl : List (Fin n × Nat), (wl.map (·.2)).Perm l ∧
    wl.Pairwise (fun a b => ¬ Conn D a.1 b.1) ∧ (∀ z, ∃ a, a ∈ wl ∧ Conn D a.1 z) ∧
    ∀ a, a ∈ wl → HasIdx D a.2 a.1

theorem rep_perm (D : Pos n) (l l' : List Nat) (h : Rep D l) (hp : l.Perm l') : Rep D l' := by
  obtain ⟨hF, hV, wl, h1, h2⟩ := h
  exact ⟨hF, hV, wl, h1.trans hp, h2⟩

theorem rep_size (D : Pos n) (l : List Nat) (h : Rep D l) (i : Nat) (hi : i ∈ l) :
    2 ^ (i / 2 + 1) ≤ n + 1 := by
  obtain ⟨hF, _, wl, h1, _, _, h4⟩ := h
  have hi' := h1.mem_iff.mpr hi
  obtain ⟨a, ha, rfl⟩ := List.mem_map.mp hi'
  exact size_bound hF (h4 a ha).1

theorem conn_nil {a b : Fin n} (h : Conn ([] : Pos n) a b) : a = b := by
  have := (conn_iff [] a b).mpr h
  simpa [conn] using this

theorem rep_start (n : Nat) : Rep ([] : Pos n) (List.replicate n 0) := by
  refine ⟨trivial, fun v => Nat.zero_le _, (List.finRange n).map (fun v => (v, 0)), ?_, ?_, ?_, ?_⟩
  · have : ((List.finRange n).map (fun v => (v, 0))).map (·.2) = List.replicate n 0 := by
      rw [List.eq_replicate_iff]
      refine ⟨by simp, ?_⟩
      intro b hb
      simp only [List.map_map, List.mem_map] at hb
      obtain ⟨_, _, rfl⟩ := hb
      rfl
    rw [this]
  · rw [List.pairwise_map]
    exact (List.nodup_finRange n).imp (fun hab hc => hab (conn_nil hc))
  · intro z
    exact ⟨(z, 0), List.mem_map.mpr ⟨z, List.mem_finRange z, rfl⟩, RT.refl _⟩
  · intro a ha
    obtain ⟨v, _, rfl⟩ := List.mem_map.mp ha
    exact hasIdx_zero _ _

theorem extract1 (wl : List (Fin n × Nat)) (i : Nat) (rest : List Nat)
    (h : (wl.map (·.2)).Perm (i :: rest)) :
    ∃ x wl1, wl.Perm ((x, i) :: wl1) ∧ (wl1.map (·.2)).Perm rest := by
  have hi : i ∈ wl.map (·.2) := h.mem_iff.mpr (List.mem_cons_self ..)
  obtain ⟨⟨x, i0⟩, ha, e⟩ := List.mem_map.mp hi
  simp only at e
  subst e
  have hp := List.perm_cons_erase ha
  refine ⟨x, wl.erase (x, i0), hp, ?_⟩
  have h2 := ((hp.map (·.2)).symm.trans h)
  rw [List.map_cons] at h2
  exact h2.cons_inv

/-- One merge by rule `M(i)` with replying endpoint `t`: the new forest is represented by the
old list with the two merged trees replaced by one tree of index `j`, where `TL i j cost`. -/
theorem merge_rep {D : Pos n} {t hd : Fin n} {ws : List (Fin n)} {i : Nat} {rest : List Nat}
    (hF : Forest D) (hV : Valid D) (hc : ¬ Conn D t hd) (hp : NPath D t ws)
    (hti : HasIdx D i t) (hhi : HasIdx D i hd) (wl2 : List (Fin n × Nat))
    (hpw : wl2.Pairwise (fun a b => ¬ Conn D a.1 b.1))
    (h1 : ∀ c, c ∈ wl2 → ¬ Conn D t c.1) (h2 : ∀ c, c ∈ wl2 → ¬ Conn D hd c.1)
    (hcov : ∀ z, Conn D t z ∨ Conn D hd z ∨ ∃ c, c ∈ wl2 ∧ Conn D c.1 z)
    (hidx : ∀ c, c ∈ wl2 → HasIdx D c.2 c.1) (hperm : (wl2.map (·.2)).Perm rest) :
    ∃ j, Core.TL i j ws.length ∧ Rep (nreply D t hd ws) (j :: rest) := by
  obtain ⟨j, hTL, hj⟩ := type_lemma hF hc hp hti hhi
  refine ⟨j, hTL, forest_nreply hF hc, valid_nreply hF hV hp, (t, j) :: wl2, ?_, ?_, ?_, ?_⟩
  · rw [List.map_cons]; exact hperm.cons j
  · refine List.pairwise_cons.mpr ⟨?_, ?_⟩
    · intro c hcm hcc
      rcases conn_nreply.mp hcc with h | ⟨_, h⟩ | ⟨_, h⟩
      · exact h1 c hcm h
      · exact h2 c hcm h
      · exact h1 c hcm h
    · refine hpw.imp_of_mem ?_
      intro a b ha hb hab hcc
      rcases conn_nreply.mp hcc with h | ⟨_, h⟩ | ⟨_, h⟩
      · exact hab h
      · exact h2 b hb h
      · exact h1 b hb h
  · intro z
    rcases hcov z with h | h | ⟨c, hcm, h⟩
    · exact ⟨(t, j), List.mem_cons_self .., conn_nreply.mpr (Or.inl h)⟩
    · exact ⟨(t, j), List.mem_cons_self .., conn_nreply.mpr (Or.inr (Or.inl ⟨RT.refl _, h⟩))⟩
    · exact ⟨c, List.mem_cons_of_mem _ hcm, conn_nreply.mpr (Or.inl h)⟩
  · intro a ha
    rcases List.mem_cons.mp ha with rfl | ha
    · exact hj
    · exact type_keep hF hp (h1 a ha) (hidx a ha)

theorem rep_merge (D : Pos n) (i i' : Nat) (rest : List Nat) (h : Rep D (i' :: i :: rest))
    (hii : i ≤ i') :
    ∃ m, NLegal D m ∧ ∀ c D', NReply D m c D' → ∃ j, Core.TL i j c ∧ Rep D' (j :: rest) := by
  obtain ⟨hF, hV, wl, hperm, hpw, hcov, hidx⟩ := h
  obtain ⟨x, wl1, hp1, hperm1⟩ := extract1 wl i' (i :: rest) hperm
  obtain ⟨y, wl2, hp2, hperm2⟩ := extract1 wl1 i rest hperm1
  have hp : wl.Perm ((x, i') :: (y, i) :: wl2) := hp1.trans (hp2.cons _)
  have hsym : ∀ {a b : Fin n × Nat}, ¬ Conn D a.1 b.1 → ¬ Conn D b.1 a.1 :=
    fun h hc => h hc.symm
  have hpw' := (hp.pairwise_iff hsym).mp hpw
  have hpx := List.pairwise_cons.mp hpw'
  have hpy := List.pairwise_cons.mp hpx.2
  have hxy : ¬ Conn D x y := hpx.1 (y, i) (List.mem_cons_self ..)
  have hx2 : ∀ c, c ∈ wl2 → ¬ Conn D x c.1 := fun c hc => hpx.1 c (List.mem_cons_of_mem _ hc)
  have hy2 : ∀ c, c ∈ wl2 → ¬ Conn D y c.1 := fun c hc => hpy.1 c hc
  have hix : HasIdx D i x :=
    (hidx (x, i') (hp.mem_iff.mpr (List.mem_cons_self ..))).mono hii
  have hiy : HasIdx D i y :=
    hidx (y, i) (hp.mem_iff.mpr (List.mem_cons_of_mem _ (List.mem_cons_self ..)))
  have hi2 : ∀ c, c ∈ wl2 → HasIdx D c.2 c.1 := fun c hc =>
    hidx c (hp.mem_iff.mpr (List.mem_cons_of_mem _ (List.mem_cons_of_mem _ hc)))
  have hcov' : ∀ z, Conn D x z ∨ Conn D y z ∨ ∃ c, c ∈ wl2 ∧ Conn D c.1 z := by
    intro z
    obtain ⟨a, ha, hz⟩ := hcov z
    rcases List.mem_cons.mp (hp.mem_iff.mp ha) with rfl | ha
    · exact Or.inl hz
    · rcases List.mem_cons.mp ha with rfl | ha
      · exact Or.inr (Or.inl hz)
      · exact Or.inr (Or.inr ⟨a, ha, hz⟩)
  refine ⟨(x, y), hxy, ?_⟩
  rintro c D' ⟨t, hd, ws, hth, hpath, rfl, rfl⟩
  rcases hth with ⟨rfl, rfl⟩ | ⟨rfl, rfl⟩
  · exact merge_rep hF hV hxy hpath hix hiy wl2 hpy.2 hx2 hy2 hcov' hi2 hperm2
  · exact merge_rep hF hV (fun h => hxy h.symm) hpath hiy hix wl2 hpy.2 hy2 hx2
      (fun z => by rcases hcov' z with h | h | h
                   · exact Or.inr (Or.inl h)
                   · exact Or.inl h
                   · exact Or.inr (Or.inr h)) hi2 hperm2

/-- THEOREM 1 for the normal-form game.  From the position without edges, in `n - 1` rounds,
Adversary forces every `g` with `3 g + 7 ⌊log₂(n+1)⌋ ≤ n + 5` against normal replies
(so the total payment `p` of every such play has `3 p + 7 ⌊log₂(n+1)⌋ ≥ n + 3`). -/
theorem normal_game_bound (n : Nat) (hn : 1 ≤ n) (g : Nat)
    (hg : 3 * g + 7 * Nat.log2 (n + 1) ≤ n + 5) :
    Core.Forces (NLegal (n := n)) NReply (n - 1) ([] : Pos n) g :=
  Core.core_bound NLegal NReply Rep rep_perm rep_merge n hn rep_size [] (rep_start n) g hg

end FO
