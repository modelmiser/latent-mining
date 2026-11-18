import FO
#print axioms FO.T_lower_bound
#print axioms FO.T_lower_bound_pow
#print axioms FO.main
#print axioms FO.transfer
#print axioms FO.shadow_path
#print axioms FO.normal_game_bound
#print axioms FO.type_lemma
#print axioms FO.lemmaB
#print axioms FO.outdeg_nreply
#print axioms FO.hge_sibling
#print axioms FO.hge_other
#print axioms FO.size_bound
#print axioms FO.Core.core_bound
#print axioms FO.conn_iff
#print axioms FO.nb
#check @FO.T_lower_bound
#check @FO.T_lower_bound_pow
#check @FO.normal_game_bound
#check @FO.transfer
-- upper bound (U1: arithmetic core)
#print axioms FO.Upper.w2_mono
#print axioms FO.Upper.w1_mono
#print axioms FO.Upper.lemma7_steps
#print axioms FO.Upper.path_start
#print axioms FO.Upper.path_interior
#print axioms FO.Upper.path_end_none
#print axioms FO.Upper.path_end_some
#print axioms FO.Upper.free_none
#print axioms FO.Upper.free_some
#print axioms FO.Upper.rest_gain
#print axioms FO.Upper.path_gain
#print axioms FO.Upper.w2_le_parent
#check @FO.Upper.path_gain
-- upper bound (U2: Lemma 6)
#print axioms FO.le_hh_iff
#print axioms FO.hh_two
#print axioms FO.lemma6_gen
#print axioms FO.lemma6
#print axioms FO.lemma6_gamma
#check @FO.lemma6
#check @FO.lemma6_gen
-- upper bound (U3: the round; U4: assembly)
#print axioms FO.suffix_profile
#print axioms FO.path_round
#print axioms FO.off_path
#print axioms FO.round_gain
#print axioms FO.spath_spec
#print axioms FO.cost_nreply
#print axioms FO.ruleL_round
#print axioms FO.upper_value
#print axioms FO.upper
#print axioms upper_third
#check @FO.round_gain
#check @FO.ruleL_round
#check @FO.upper_value
#check @upper_third
