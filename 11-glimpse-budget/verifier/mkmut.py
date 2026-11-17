#!/usr/bin/env python3
# Referee: build deliberately broken witnesses in verifier/mut/ (copies; solver files untouched)
A='../solver-a/'; B='../solver-b/wit/'
def rd(p): return open(p).read().split('\n')
def wr(name,lines): open('mut/'+name,'w').write('\n'.join(lines))
l=rd(A+'lose_D_h3_k1.txt'); wr('m1_A_loseD31_dropline.txt', l[:100]+l[101:])
l=rd(A+'strat_E_h3_k1.txt'); i=[k for k,x in enumerate(l) if x.startswith('0 8 210 M 4')][0]; l[i]='0 8 210 M 8'; wr('m2_A_stratE31_stay.txt', l)
l=rd(A+'strat_E_h3_k1.txt'); l[0]=l[0].replace('game E','game D'); wr('m3_A_stratE31_asD.txt', l)
l=rd(B+'E_h3_k0.txt'); l[0]='LOSING E 3 1'; wr('m4_B_loseE30_as_E31.txt', l)
l=rd(B+'D_h3_k2.txt'); l[0]='STRATEGY D 3 1'; wr('m5_B_stratD32_budget1.txt', l)
l=rd(B+'E_h3_k1.txt'); i=[k for k,x in enumerate(l) if x.startswith('NODE 104 MOVE 14 WIN')][0]; l[i]='NODE 104 MOVE 13 WIN'; wr('m6_B_stratE31_wrongleaf.txt', l)
l=rd(A+'lose_D_h3_k1.txt'); l[1]='1 1 fffc 3'; wr('m7_A_loseD31_otheranswer.txt', l)   # symmetric answer: may legitimately fail closure
l=rd(A+'strat_D_h3_k2.txt'); l=[x.replace('2 1 fffc G 2:1 3:1','2 1 fffc G 2:2 3:3') for x in l]; wr('m8_A_stratD32_stepfirst.txt', l)
