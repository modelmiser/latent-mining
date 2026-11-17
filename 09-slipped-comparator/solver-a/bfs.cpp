// Exact search for S(n) (slip mode) or A(n) (deletion mode), n <= 7.
// State after a prefix, on the reversed input n..1, seen through its threshold 0-1 vectors:
//   P = set of the n-1 threshold vectors of the unslipped image (one per weight 1..n-1)
//   U = set of threshold vectors reachable by prefixes with exactly one slip (deletion),
//       reduced to elements not dominated by another element of U or by the P vector of same weight.
// A vector is an n-bit int, bit (i-1) = value on wire i; sorted = ones on the highest wires.
// usage: bfs n mode(0=slip,1=delete) T [nored]
//   T = budget: states with level + lowerbound > T are pruned (safe if answer <= T).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
struct St { u128 P, U; bool operator<(const St&o) const { return P!=o.P ? P<o.P : U<o.U; } bool operator==(const St&o) const { return P==o.P && U==o.U; } };
int n, N, mode, T, nored=0;
int app[8][128]; u128 domS[128], domE[128]; int inv01[128], mir[128];
static inline int ctz128(u128 x){ unsigned long long lo=(unsigned long long)x; if(lo) return __builtin_ctzll(lo); return 64+__builtin_ctzll((unsigned long long)(x>>64)); }
u128 applyset(int i, u128 S){ u128 r=0; while(S){ int x=ctz128(S); S&=S-1; r|=(u128)1<<app[i][x]; } return r; }
u128 mirset(u128 S){ u128 r=0; while(S){ int x=ctz128(S); S&=S-1; r|=(u128)1<<mir[x]; } return r; }
u128 reduce(u128 P, u128 U){
  if(nored){ // only drop sorted vectors and nothing else
    u128 r=0, S=U; while(S){ int x=ctz128(S); S&=S-1; if(inv01[x]) r|=(u128)1<<x; } return r; }
  u128 kill=0, S=P; while(S){ int x=ctz128(S); S&=S-1; kill|=domE[x]; }
  S=U; while(S){ int x=ctz128(S); S&=S-1; kill|=domS[x]; }
  return U&~kill;
}
St step(const St&s, int i){
  St t; t.P=applyset(i,s.P);
  u128 U=applyset(i,s.U);
  if(mode==0){ if(i>1) U|=applyset(i-1,s.P); if(i<n-1) U|=applyset(i+1,s.P); }
  else U|=s.P;
  t.U=reduce(t.P,U); return t;
}
St canon(const St&s){ St m; m.P=mirset(s.P); m.U=mirset(s.U); return m<s?m:s; }
int lb(const St&s){ int a=0,b=0; int v[8]={0}; u128 S=s.P; while(S){int x=ctz128(S);S&=S-1; for(int j=0;j<n;j++) v[j]+=(x>>j)&1;} for(int i=0;i<n;i++) for(int j=i+1;j<n;j++) if(v[i]>v[j]) a++; S=s.U; while(S){int x=ctz128(S);S&=S-1;b=max(b,inv01[x]);} return max(a,b); }
int main(int argc,char**argv){
  n=atoi(argv[1]); mode=atoi(argv[2]); T=atoi(argv[3]); if(argc>4) nored=atoi(argv[4]); N=1<<n;
  for(int x=0;x<N;x++){
    for(int i=1;i<n;i++){ int a=(x>>(i-1))&1,b=(x>>i)&1; app[i][x]=(a==1&&b==0)? (x^(3<<(i-1))) : x; }
    int c=0,ones=0; for(int j=0;j<n;j++){ if((x>>j)&1) ones++; else c+=ones; } inv01[x]=c;
    int m=0; for(int j=0;j<n;j++) if(!((x>>j)&1)) m|=1<<(n-1-j); mir[x]=m;
  }
  // x dominates y (x at least as unsorted): same weight and every prefix of x has >= as many ones
  for(int x=0;x<N;x++){ domS[x]=domE[x]=0; for(int y=0;y<N;y++){ if(__builtin_popcount(x)!=__builtin_popcount(y)) continue; bool ok=true; for(int j=1;j<=n;j++) if(__builtin_popcount(x&((1<<j)-1))<__builtin_popcount(y&((1<<j)-1))) ok=false; if(ok){ domE[x]|=(u128)1<<y; if(x!=y) domS[x]|=(u128)1<<y; } } }
  // self-test: monotonicity of comparators under dominance (needed for the reduction)
  for(int x=0;x<N;x++) for(int y=0;y<N;y++) if((domE[x]>>y)&1) for(int i=1;i<n;i++) if(!((domE[app[i][x]]>>app[i][y])&1)){ printf("MONOTONICITY FAILS\n"); return 1; }
  St s0; s0.P=0; s0.U=0; for(int k=1;k<n;k++) s0.P|=(u128)1<<((1<<k)-1); // ones on wires 1..k
  vector<vector<St>> lev; lev.push_back({canon(s0)});
  int found=-1;
  for(int d=0; d<=T; d++){
    bool goal=false; for(auto&s:lev[d]) if(lb(s)==0 && s.U==0){ goal=true; }
    printf("level %d states %zu%s\n", d, lev[d].size(), goal?" GOAL":""); fflush(stdout);
    if(goal){ found=d; break; }
    if(d==T) break;
    vector<St> nx; nx.reserve(lev[d].size()*2);
    for(auto&s:lev[d]) for(int i=1;i<n;i++){ St t=canon(step(s,i)); if(d+1+lb(t)<=T) nx.push_back(t); }
    sort(nx.begin(),nx.end()); nx.erase(unique(nx.begin(),nx.end()),nx.end()); nx.shrink_to_fit();
    lev.push_back(move(nx));
    if(lev.back().empty()){ printf("no states left\n"); break; }
  }
  if(found<0){ printf("RESULT n=%d mode=%d : no word of size <= %d\n", n, mode, T); return 0; }
  // count optimal words: backward DP over canonical states
  vector<vector<unsigned long long>> cnt(found+1);
  cnt[found].assign(lev[found].size(),0);
  for(size_t j=0;j<lev[found].size();j++) if(lb(lev[found][j])==0 && lev[found][j].U==0) cnt[found][j]=1;
  for(int d=found-1; d>=0; d--){ cnt[d].assign(lev[d].size(),0);
    for(size_t j=0;j<lev[d].size();j++) for(int i=1;i<n;i++){ St t=canon(step(lev[d][j],i)); auto it=lower_bound(lev[d+1].begin(),lev[d+1].end(),t); if(it!=lev[d+1].end() && *it==t) cnt[d][j]+=cnt[d+1][it-lev[d+1].begin()]; } }
  printf("RESULT n=%d mode=%d : least size %d, number of optimal words %llu\n", n, mode, found, cnt[0][0]);
  // lexicographically least optimal word
  St s=s0; printf("WITNESS");
  for(int d=0; d<found; d++){ for(int i=1;i<n;i++){ St t=step(s,i), c=canon(t); auto it=lower_bound(lev[d+1].begin(),lev[d+1].end(),c); if(it!=lev[d+1].end() && *it==c && cnt[d+1][it-lev[d+1].begin()]>0){ printf(" %d",i); s=t; break; } } }
  printf("\n");
}
