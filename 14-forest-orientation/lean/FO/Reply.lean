/-
M2, part 2.  The normal reply: Fact 0, Lemma A, Lemma B and the Type Lemma
(open-limit/proofs.txt sections 0-3).
-/
import FO.Normal
import FO.Core
namespace FO
variable {n : Nat}

/-- The position after the NORMAL reply at the named vertex `t` (the other named vertex is `hd`)
along the path `ws`: flip the path `t → w₁ → ... → w_d`, add the arc `t → hd`.
`ws = []` is the free reply.  Its cost is `ws.length`. -/
def nreply (D : Pos n) (t hd : Fin n) (ws : List (Fin n)) : Pos n :=
  (t, hd) :: flipSet D (pathArcs t ws)

section
variable {D : Pos n} {t hd : Fin n} {ws : List (Fin n)}

theorem outdeg_flip_path (hF : Forest D) (hp : NPath D t ws) (v : Fin n) :
    outdeg (flipSet D (pathArcs t ws)) v + ind t v = outdeg D v + ind (lastV t ws) v := by
  have h1 := outdeg_flipSet D (pathArcs t ws) v
  have hN := forest_nodup D hF
  have hPN := pathArcs_nodup (npath_nodup hF hp)
  have h2 : D.countP (fun a => decide (a ∈ pathArcs t ws) && (a.1 == v))
      = (pathArcs t ws).countP (fun a => a.1 == v) :=
    countP_mem_eq D (pathArcs t ws) (fun a => a.1 == v) hN hPN (npath_sub hp)
  have h3 : D.countP (fun a => decide (a ∈ pathArcs t ws) && (a.2 == v))
      = (pathArcs t ws).countP (fun a => a.2 == v) :=
    countP_mem_eq D (pathArcs t ws) (fun a => a.2 == v) hN hPN (npath_sub hp)
  have h4 := path_count t ws v
  rw [h2, h3] at h1
  omega

/-- FACT 0: a normal reply raises the out-degree of the last vertex of the path by one and
changes no other out-degree. -/
theorem outdeg_nreply (hF : Forest D) (hp : NPath D t ws) (v : Fin n) :
    outdeg (nreply D t hd ws) v = outdeg D v + ind (lastV t ws) v := by
  have := outdeg_flip_path hF hp v
  rw [nreply, outdeg_cons]
  show _ + ind t v = _
  omega

theorem valid_nreply (hF : Forest D) (hV : Valid D) (hp : NPath D t ws) :
    Valid (nreply D t hd ws) := by
  intro v
  rw [outdeg_nreply hF hp]
  by_cases h : lastV t ws = v
  · subst h; have := npath_last hp; rw [ind_self]; omega
  · rw [ind_ne h]; exact hV v

theorem forest_nreply (hF : Forest D) (hc : ¬ Conn D t hd) : Forest (nreply D t hd ws) :=
  ⟨fun h => hc ((al_conn (al_flipSet D _)).mpr h), al_forest (al_flipSet D _) hF⟩

theorem conn_nreply {a b : Fin n} : Conn (nreply D t hd ws) a b ↔
    Conn D a b ∨ (Conn D a t ∧ Conn D hd b) ∨ (Conn D a hd ∧ Conn D t b) := by
  unfold nreply
  rw [Conn_cons]
  simp only [← al_conn (al_flipSet D (pathArcs t ws))]

theorem path_conn (hp : NPath D t ws) {c : Fin n} (hc : c ∈ t :: ws) : Conn D t c := by
  rcases List.mem_cons.mp hc with rfl | hc
  · exact RT.refl _
  · obtain ⟨w, ws', _, h1, h2⟩ := npath_reach hp c hc
    exact RT.step (Or.inl h1) h2.conn

/-- Arcs out of a vertex that is not on the path are untouched. -/
theorem arc_nreply_off {v o : Fin n} (hv : v ∉ t :: ws) :
    (v, o) ∈ nreply D t hd ws ↔ (v, o) ∈ D := by
  unfold nreply
  rw [List.mem_cons, mem_flipSet]
  constructor
  · rintro (e | ⟨h, _⟩ | ⟨_, h⟩)
    · simp only [Prod.mk.injEq] at e
      exact absurd (by rw [e.1]; exact List.mem_cons_self ..) hv
    · exact h
    · exact absurd (List.mem_cons_of_mem _ (mem_pathArcs h).2) hv
  · intro h
    exact Or.inr (Or.inl ⟨h, fun hm => hv (mem_pathArcs hm).1⟩)

theorem pathArcs_head {o : Fin n} (ht : t ∉ ws) :
    (t, o) ∈ pathArcs t ws ↔ ∃ ws', ws = o :: ws' := by
  cases ws with
  | nil => simp [pathArcs]
  | cons w ws' =>
    simp only [pathArcs, List.mem_cons, Prod.mk.injEq, true_and]
    constructor
    · rintro (rfl | h)
      · exact ⟨ws', rfl⟩
      · exact absurd (mem_pathArcs h).1 ht
    · rintro ⟨ws'', e⟩
      injection e with e1 _
      exact Or.inl e1.symm

/-- LEMMA A (d): the out-neighbours of `t` after the reply are `hd` and the old ones except
the first vertex of the path. -/
theorem arc_nreply_t (hF : Forest D) (hp : NPath D t ws) (o : Fin n) :
    (t, o) ∈ nreply D t hd ws ↔ o = hd ∨ ((t, o) ∈ D ∧ ∀ ws', ws ≠ o :: ws') := by
  have hn := List.nodup_cons.mp (npath_nodup hF hp)
  unfold nreply
  rw [List.mem_cons, mem_flipSet, pathArcs_head hn.1]
  constructor
  · rintro (e | ⟨h, h'⟩ | ⟨_, h⟩)
    · simp only [Prod.mk.injEq, true_and] at e; exact Or.inl e
    · exact Or.inr ⟨h, fun ws' e => h' ⟨ws', e⟩⟩
    · exact absurd (mem_pathArcs h).2 hn.1
  · rintro (rfl | ⟨h, h'⟩)
    · exact Or.inl rfl
    · exact Or.inr (Or.inl ⟨h, fun ⟨ws', e⟩ => h' ws' e⟩)

/-- `h` is unchanged on every set `S` of vertices that is closed under out-arcs and misses the
path. -/
theorem hge_nreply (hF : Forest D) (hp : NPath D t ws) (S : Fin n → Prop)
    (hS : ∀ c o, S c → (c, o) ∈ D → S o) (hoff : ∀ c, S c → c ∉ t :: ws) :
    ∀ (k : Nat) (c : Fin n), S c → (Hge (nreply D t hd ws) k c ↔ Hge D k c) := by
  intro k
  induction k with
  | zero => intro c _; exact Iff.rfl
  | succ k ih =>
    intro c hc
    have hd' : outdeg (nreply D t hd ws) c = outdeg D c := by
      rw [outdeg_nreply hF hp, ind_ne]
      · rfl
      · intro e; exact hoff c hc (e ▸ lastV_mem t ws)
    constructor
    · rintro ⟨h1, h2⟩
      exact ⟨hd' ▸ h1, fun o ho =>
        (ih o (hS c o hc ho)).mp (h2 o ((arc_nreply_off (hoff c hc)).mpr ho))⟩
    · rintro ⟨h1, h2⟩
      refine ⟨hd'.trans h1, fun o ho => ?_⟩
      have ho' := (arc_nreply_off (hoff c hc)).mp ho
      exact (ih o (hS c o hc ho')).mpr (h2 o ho')

/-- LEMMA A (c), and the same for every tree other than the one of `t`: `h` is unchanged
outside the component of `t`. -/
theorem hge_other (hF : Forest D) (hp : NPath D t ws) {k : Nat} {z : Fin n}
    (hz : ¬ Conn D t z) : Hge (nreply D t hd ws) k z ↔ Hge D k z :=
  hge_nreply hF hp (fun c => ¬ Conn D t c)
    (fun c o hc ho h => hc (h.trans (RT.single (Or.inr ho))))
    (fun c hc hm => hc (path_conn hp hm)) k z hz

/-- LEMMA A (b): `h` of an out-neighbour of `t` that is not the first vertex of the path is
unchanged. -/
theorem hge_sibling (hF : Forest D) (hp : NPath D t ws) {k : Nat} {o : Fin n}
    (ho : (t, o) ∈ D) (hne : ∀ ws', ws ≠ o :: ws') :
    Hge (nreply D t hd ws) k o ↔ Hge D k o := by
  refine hge_nreply hF hp (fun c => Reach D o c) (fun c o' hc ho' => hc.tail ho') ?_ k o
    (RT.refl _)
  intro c hc hm
  rcases List.mem_cons.mp hm with rfl | hm
  · exact dag hF ho hc
  · obtain ⟨w, ws', e, h1, h2⟩ := npath_reach hp c hm
    have : o ≠ w := fun e' => hne ws' (e' ▸ e)
    exact disjoint hF ho h1 this hc h2

/-- LEMMA A (a): a path that starts at a vertex with `h ≥ m` has at least `m` arcs. -/
theorem npath_len : ∀ {v : Fin n} {ws : List (Fin n)} {m : Nat}, NPath D v ws → Hge D m v →
    m ≤ ws.length
  | _, _, 0, _, _ => Nat.zero_le _
  | _, [], m + 1, hp, h => by
    have h1 : outdeg D _ < 2 := hp
    have := h.1; omega
  | _, w :: ws, m + 1, hp, h => by
    have := npath_len hp.2.2 (h.2 w hp.2.1)
    simp only [List.length_cons]; omega

end

/-! ### types -/

/-- `TypeGe D v p q`: `v` has type `(p - 1, q - 1)` in the sense of the paper (slot values
shifted by one, so that a hole is worth 0 and an out-neighbour `o` is worth `h(o) + 1`):
the smaller slot value is at least `p` (this is exactly `h(v) ≥ p`) and the larger is at
least `q`. -/
def TypeGe (D : Pos n) (v : Fin n) (p q : Nat) : Prop :=
  Hge D p v ∧ (q = 0 ∨ ∃ o, (v, o) ∈ D ∧ Hge D (q - 1) o)

theorem TypeGe.mono {D : Pos n} {v : Fin n} {p q p0 q0 : Nat} (h : TypeGe D v p q)
    (hp : p0 ≤ p) (hq : q0 ≤ q) : TypeGe D v p0 q0 := by
  refine ⟨h.1.le hp, ?_⟩
  rcases h.2 with h2 | ⟨o, ho, h2⟩
  · exact Or.inl (by omega)
  · exact Or.inr ⟨o, ho, h2.le (by omega)⟩

theorem typeGe_two {E : Pos n} {t hd o2 : Fin n} {r b : Nat}
    (h2 : outdeg E t = 2) (hall : ∀ o, (t, o) ∈ E → o = hd ∨ o = o2) (h1 : (t, hd) ∈ E)
    (h3 : (t, o2) ∈ E) (hb : Hge E b hd) (hr : r = 0 ∨ Hge E (r - 1) o2) :
    TypeGe E t (min r (b + 1)) (max r (b + 1)) := by
  constructor
  · cases hm : min r (b + 1) with
    | zero => trivial
    | succ m =>
      refine ⟨h2, ?_⟩
      intro o ho
      rcases hall o ho with rfl | rfl
      · exact hb.le (by omega)
      · rcases hr with hr | hr
        · omega
        · exact hr.le (by omega)
  · right
    by_cases hrb : b + 1 ≤ r
    · rcases hr with hr | hr
      · omega
      · exact ⟨o2, h3, hr.le (by omega)⟩
    · exact ⟨hd, h1, hb.le (by omega)⟩

section
variable {D : Pos n} {t hd : Fin n} {ws : List (Fin n)}

/-- The non-replying endpoint and all vertices of the other trees keep their types. -/
theorem type_keep (hF : Forest D) (hp : NPath D t ws) {z : Fin n} {p q : Nat}
    (hz : ¬ Conn D t z) (h : TypeGe D z p q) : TypeGe (nreply D t hd ws) z p q := by
  refine ⟨(hge_other hF hp hz).mpr h.1, ?_⟩
  rcases h.2 with h2 | ⟨o, ho, h2⟩
  · exact Or.inl h2
  · have hzo : ¬ Conn D t o := fun hc => hz (hc.trans (RT.single (Or.inr ho)))
    exact Or.inr ⟨o, (arc_nreply_off (fun hm => hz (path_conn hp hm))).mpr ho,
      (hge_other hF hp hzo).mpr h2⟩

/-- LEMMA B (pair lemma).  `t` has type `(p, q)` and the other named vertex `hd` (in another
tree) has `h(hd) ≥ p'`.  Every normal reply at `t` satisfies
 (X1) cost ≥ p and afterwards `t` has type `sort (q, p' + 1)`, or
 (X2) cost ≥ q and afterwards `t` has type `sort (p, p' + 1)`.
(All slot values are shifted by one against the paper.) -/
theorem lemmaB (hF : Forest D) (hc : ¬ Conn D t hd) (hp : NPath D t ws)
    {p q p' : Nat} (ht : TypeGe D t p q) (hh : Hge D p' hd) :
    (p ≤ ws.length ∧ TypeGe (nreply D t hd ws) t (min q (p' + 1)) (max q (p' + 1))) ∨
    (q ≤ ws.length ∧ TypeGe (nreply D t hd ws) t (min p (p' + 1)) (max p (p' + 1))) := by
  have hN := forest_nodup D hF
  have hhd' : Hge (nreply D t hd ws) p' hd := (hge_other hF hp hc).mpr hh
  have hhead : (t, hd) ∈ nreply D t hd ws := List.mem_cons_self ..
  obtain ⟨htp, htq⟩ := ht
  cases ws with
  | nil =>
    have hlt : outdeg D t < 2 := hp
    have hp0 : p = 0 := by
      cases p with
      | zero => rfl
      | succ p => have := htp.1; omega
    subst hp0
    refine Or.inl ⟨Nat.le_refl _, ?_⟩
    rcases htq with hq | ⟨o, ho, hoq⟩
    · subst hq
      exact ⟨by rw [Nat.zero_min]; trivial, Or.inr ⟨hd, hhead, hhd'.le (by omega)⟩⟩
    · have h1 : outdeg D t = 1 := by have := pos_out ho; omega
      obtain ⟨a, ha, hall⟩ := one_out h1
      have hoa := hall o ho
      subst hoa
      have hne : ∀ ws', ([] : List (Fin n)) ≠ o :: ws' := by intro ws' e; cases e
      refine typeGe_two (o2 := o) ?_ ?_ hhead ?_ hhd' (Or.inr ((hge_sibling hF hp ho hne).mpr hoq))
      · rw [outdeg_nreply hF hp]
        show outdeg D t + ind t t = 2
        rw [ind_self]; omega
      · intro x hx
        rcases (arc_nreply_t hF hp x).mp hx with h | ⟨h, _⟩
        · exact Or.inl h
        · exact Or.inr (hall x h)
      · exact (arc_nreply_t hF hp o).mpr (Or.inr ⟨ho, hne⟩)
  | cons w ws' =>
    have hsat : outdeg D t = 2 := hp.1
    have htw : (t, w) ∈ D := hp.2.1
    have hrest : NPath D w ws' := hp.2.2
    obtain ⟨a, b, hab, ha, hb, hall⟩ := two_out hN hsat
    have hex : ∃ o2, o2 ≠ w ∧ (t, o2) ∈ D ∧ ∀ o, (t, o) ∈ D → o = w ∨ o = o2 := by
      rcases hall w htw with rfl | rfl
      · exact ⟨b, Ne.symm hab, hb, hall⟩
      · exact ⟨a, hab, ha, fun o ho => (hall o ho).symm⟩
    obtain ⟨o2, ho2w, ho2, hall2⟩ := hex
    have hnd := List.nodup_cons.mp (npath_nodup hF hp)
    have hD't : outdeg (nreply D t hd (w :: ws')) t = 2 := by
      rw [outdeg_nreply hF hp, ind_ne]
      · exact hsat
      · intro e
        exact hnd.1 (e ▸ lastV_mem w ws')
    have hne2 : ∀ ws'', w :: ws' ≠ o2 :: ws'' := by
      intro ws'' e; injection e with e1 _; exact ho2w e1.symm
    have harcs : ∀ o, (t, o) ∈ nreply D t hd (w :: ws') → o = hd ∨ o = o2 := by
      intro o ho
      rcases (arc_nreply_t hF hp o).mp ho with h | ⟨h, hne⟩
      · exact Or.inl h
      · rcases hall2 o h with rfl | rfl
        · exact absurd rfl (hne ws')
        · exact Or.inr rfl
    have ho2' : (t, o2) ∈ nreply D t hd (w :: ws') :=
      (arc_nreply_t hF hp o2).mpr (Or.inr ⟨ho2, hne2⟩)
    have hS2 : ∀ m, Hge D m o2 → Hge (nreply D t hd (w :: ws')) m o2 :=
      fun m h => (hge_sibling hF hp ho2 hne2).mpr h
    have hlenw : ∀ m, Hge D m w → m + 1 ≤ (w :: ws').length := by
      intro m h
      have := npath_len hrest h
      simp only [List.length_cons]; omega
    by_cases hq2 : q = 0 ∨ Hge D (q - 1) o2
    · left
      refine ⟨?_, typeGe_two hD't harcs hhead ho2' hhd' (hq2.imp id (hS2 _))⟩
      cases p with
      | zero => exact Nat.zero_le _
      | succ p => exact hlenw p (htp.2 w htw)
    · right
      have hq1 : q ≠ 0 := fun h => hq2 (Or.inl h)
      have hqw : Hge D (q - 1) w := by
        rcases htq with h | ⟨o, ho, hoq⟩
        · exact absurd h hq1
        · rcases hall2 o ho with rfl | rfl
          · exact hoq
          · exact absurd (Or.inr hoq) hq2
      refine ⟨by have := hlenw (q - 1) hqw; omega,
        typeGe_two hD't harcs hhead ho2' hhd' ?_⟩
      cases p with
      | zero => exact Or.inl rfl
      | succ p => exact Or.inr (hS2 _ (htp.2 o2 ho2))

end

/-! ### indices and the Type Lemma -/

/-- `v` is a witness of index `i`: index `2k` is type `A_k = (k-1, k-1)`, index `2k+1` is
type `R_k = (k-1, k)` (shifted: `(k, k)` and `(k, k+1)`). -/
def HasIdx (D : Pos n) (i : Nat) (v : Fin n) : Prop := TypeGe D v (i / 2) ((i + 1) / 2)

/-- Types are nested. -/
theorem HasIdx.mono {D : Pos n} {i i0 : Nat} {v : Fin n} (h : HasIdx D i v) (hi : i0 ≤ i) :
    HasIdx D i0 v := TypeGe.mono h (by omega) (by omega)

theorem hasIdx_zero (D : Pos n) (v : Fin n) : HasIdx D 0 v := ⟨trivial, Or.inl rfl⟩

/-- TYPE LEMMA.  Rule `M(i)`: `t` and `hd` are witnesses of index `i` in different trees.
Every normal reply at `t` (path `ws`, cost `ws.length`) makes `t` a witness of an index
`j ∈ {i, i+1}` with `cost + F(j) ≥ 2 F(i)`. -/
theorem type_lemma {D : Pos n} {t hd : Fin n} {ws : List (Fin n)} (hF : Forest D)
    (hc : ¬ Conn D t hd) (hp : NPath D t ws) {i : Nat} (ht : HasIdx D i t)
    (hh : HasIdx D i hd) :
    ∃ j, Core.TL i j ws.length ∧ HasIdx (nreply D t hd ws) j t := by
  rcases lemmaB hF hc hp ht hh.1 with ⟨h1, h2⟩ | ⟨h1, h2⟩
  · refine ⟨i + 1, ⟨Or.inr rfl, ?_⟩, TypeGe.mono h2 (by omega) (by omega)⟩
    unfold Core.F3; omega
  · rcases Nat.mod_two_eq_zero_or_one i with he | ho
    · refine ⟨i + 1, ⟨Or.inr rfl, ?_⟩, TypeGe.mono h2 (by omega) (by omega)⟩
      unfold Core.F3; omega
    · refine ⟨i, ⟨Or.inl rfl, ?_⟩, TypeGe.mono h2 (by omega) (by omega)⟩
      unfold Core.F3; omega

end FO
