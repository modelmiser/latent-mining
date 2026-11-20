// fullR.cpp: independent test of Theorem T2 (R = Z_m, three players). usage: fullR m
// For every word W (bit 0 = 0, by colour swap) and every triple of shifts s1<s2<s3 such that
// P_s := W xor rot(W,s) are pairwise disjoint, non-empty, and W^P_si^P_sk is a rotation of W for all pairs
// (i.e. the seven AND-0 rows are rotations of W), report whether W^P_s1^P_s2^P_s3 is a rotation of W.
// T2 predicts: always a rotation (nonrot = 0). Also counts n=2 protocols (pairs with W^P_s^P_t NOT a rotation).
#include <bits/stdc++.h>
using namespace std; int m; unsigned FULL;
static inline unsigned rot(unsigned w,int j){j%=m; if(!j) return w; return ((w<<j)|(w>>(m-j)))&FULL;}
int main(int argc,char**argv){ m=atoi(argv[1]); FULL=(1u<<m)-1; long long triples=0,nonrot=0,two=0,pairsrot=0;
  for(unsigned W=0;W<(1u<<m);W+=2){ unsigned rs[32],P[32]; for(int j=0;j<m;j++){rs[j]=rot(W,j);P[j]=W^rs[j];}
    auto isrot=[&](unsigned v){for(int j=0;j<m;j++) if(rs[j]==v) return true; return false;};
    static bool g[32][32];
    for(int s=1;s<m;s++)for(int t=s+1;t<m;t++){ g[s][t]=false; if(!P[s]||!P[t]||(P[s]&P[t])) continue;
      if(isrot(W^P[s]^P[t])){g[s][t]=true;pairsrot++;} else two++; }
    for(int s=1;s<m;s++)for(int t=s+1;t<m;t++) if(g[s][t]) for(int u=t+1;u<m;u++) if(g[s][u]&&g[t][u]){ triples++; if(!isrot(W^P[s]^P[t]^P[u])) nonrot++; }
  }
  printf("FULLR m=%d n2_valid(W,s,t)=%lld rot_pairs=%lld n3_triples_with_7_rotations=%lld of_which_row111_not_rotation=%lld\n",m,two,pairsrot,triples,nonrot);}
