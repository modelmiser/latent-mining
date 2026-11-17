// fast.cpp -- belief-set solver for the glimpse cop/robber games D(h,k), E(h,k) on T_h.
// State at the cop-move step: (c, S, j): cop vertex c, S = set of vertices the robber may be on
// (never contains c), j = glimpses still unspent.
//   A_j(c,S): cop wins from the cop-move step.
//   B_j(c,S): cop wins from the start of a round (before the optional glimpse).
//   B_j(c,S) = A_j(c,S) or ( j>=1 and for every non-empty part P of S : A_{j-1}(c,P) )
//   A_j(c,S) = exists c' in N[c]:  S\{c'} empty  or  B_j(c', N[S\{c'}]\{c'})
// Least fixpoint (bounded capture time). Within one j the no-glimpse moves form a one-player
// graph; we BFS it. Prunings (both safe, see proofs.txt section P):
//   (1) symmetry: states are canonicalised under tree automorphisms;
//   (2) domination: a BFS state (c,S) is dropped if an already-seen BFS state (c,S0) has S0 subset S.
// Usage: fast MODE h kmax [dump]     MODE = D or E
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
static int H, N; static bool EX;
static u128 nb[130], sub[130], ALL;
static vector<int> adj[130];
static inline u128 bit(int v){ return (u128)1 << v; }
static u128 expand(u128 S){ u128 R=S; uint64_t lo=(uint64_t)S, hi=(uint64_t)(S>>64);
  while(lo){ int v=__builtin_ctzll(lo); lo&=lo-1; R|=nb[v]; }
  while(hi){ int v=64+__builtin_ctzll(hi); hi&=hi-1; R|=nb[v]; } return R; }
struct St{ int c; u128 S; bool operator==(const St&o)const{return c==o.c&&S==o.S;} };
struct Hh{ size_t operator()(const St&s)const{ uint64_t a=(uint64_t)s.S,b=(uint64_t)(s.S>>64);
  uint64_t x=a*0x9E3779B97F4A7C15ULL ^ (b+0x7F4A7C15ULL)*0xC2B2AE3D27D4EB4FULL ^ (uint64_t)s.c*0x165667B19E3779F9ULL;
  x^=x>>29; x*=0xBF58476D1CE4E5B9ULL; x^=x>>32; return x; } };
// canonical form: cop on the leftmost path, children ordered by code (larger code first).
static u128 code_[130]; static int first_[130];
static St canon(St s){
  static int newl[130];
  for(int v=N; v>=1; --v){
    u128 b = (s.S>>v)&1;
    if(2*v>N){ code_[v]=b; continue; }
    int l=2*v, r=2*v+1; int depthv = 31-__builtin_clz(v); int sz = (1<<(H-depthv))-1; // size of child subtree
    bool copl = (sub[l]>>s.c)&1, copr=(sub[r]>>s.c)&1;
    int f;
    if(copl) f=l; else if(copr) f=r; else f = (code_[l]>=code_[r])? l : r;
    int g = l+r-f; first_[v]=f;
    code_[v] = b | (code_[f]<<1) | (code_[g]<<(1+sz));
  }
  St o; o.S=0; o.c=0; newl[1]=1;
  for(int v=1; v<=N; ++v){
    if((s.S>>v)&1) o.S|=bit(newl[v]);
    if(v==s.c) o.c=newl[v];
    if(2*v<=N){ int f=first_[v], g=4*v+1-f; newl[f]=2*newl[v]; newl[g]=2*newl[v]+1; }
  }
  return o;
}
static unordered_map<St,char,Hh> memo[40];
static unsigned long long nA=0, nBfs=0, maxFront=0;
static bool A(St s, int j);
static bool glimpseWin(int c, u128 S, int j){ // j>=1, all parts must be A_{j-1}
  if(EX){
    u128 T=S; // order: any
    for(int v=1; v<=N; ++v) if((T>>v)&1){ if(!A(St{c,bit(v)}, j-1)) return false; }
    return true;
  } else {
    for(int b: adj[c]){ u128 comp = (b==c/2)? (ALL & ~sub[c]) : sub[b]; u128 P=S&comp;
      if(P && !A(St{c,P}, j-1)) return false; }
    return true;
  }
}
static bool A(St s0, int j){
  if(s0.S==0) return true;
  St s=canon(s0);
  auto it=memo[j].find(s); if(it!=memo[j].end()) return it->second;
  ++nA;
  // BFS over no-glimpse play
  vector<St> q; q.push_back(s);
  vector<vector<u128>> seen(N+1);   // seen[c] = sets at start-of-round states visited
  bool win=false;
  for(size_t i=0; i<q.size() && !win; ++i){
    St x=q[i];
    for(int t=-1; t<(int)adj[x.c].size() && !win; ++t){
      int c2 = t<0? x.c : adj[x.c][t];
      u128 S1 = x.S & ~bit(c2);
      if(!S1){ win=true; break; }
      u128 S2 = expand(S1) & ~bit(c2);
      St y=canon(St{c2,S2});
      bool dom=false;
      for(u128 T: seen[y.c]) if((T & ~y.S)==0){ dom=true; break; }
      if(dom) continue;
      seen[y.c].push_back(y.S);
      ++nBfs;
      if(j>=1 && glimpseWin(y.c,y.S,j)){ win=true; break; }
      q.push_back(y);
    }
  }
  if(q.size()>maxFront) maxFront=q.size();
  memo[j][s]=win;
  return win;
}
static bool B(int c, u128 S, int j){
  if(S==0) return true;
  if(j>=1 && glimpseWin(c,S,j)) return true;
  return A(St{c,S},j);
}
int main(int argc,char**argv){
  EX = argv[1][0]=='E'; H=atoi(argv[2]); int kmax=atoi(argv[3]); N=(1<<(H+1))-1;
  ALL=0; for(int v=1;v<=N;++v) ALL|=bit(v);
  for(int v=N; v>=1; --v){ sub[v]=bit(v); if(2*v<=N) sub[v]|=sub[2*v]|sub[2*v+1]; }
  for(int v=1; v<=N; ++v){ nb[v]=bit(v); if(v>1){ adj[v].push_back(v/2); nb[v]|=bit(v/2);}
    if(2*v<=N){ adj[v].push_back(2*v); adj[v].push_back(2*v+1); nb[v]|=bit(2*v)|bit(2*v+1);} }
  for(int k=0; k<=kmax; ++k){
    string starts; bool any=false;
    for(int d=0; d<=H; ++d){ int c=1<<d; // one start per depth (symmetry)
      bool w = B(c, ALL & ~bit(c), k);
      if(w){ any=true; starts += " depth"+to_string(d); }
    }
    printf("%c h=%d k=%d cop_wins=%d winning_starts:%s  [A-evals=%llu bfs-states=%llu maxqueue=%llu]\n",
      EX?'E':'D', H, k, (int)any, starts.c_str(), nA, nBfs, maxFront); fflush(stdout);
    if(any && argc<=4) break;
  }
  if(argc>4){ // dump memo for cross-check: j c S(hex64) win
    for(int j=0;j<=kmax;++j) for(auto&kv: memo[j])
      printf("M %d %d %llx %d\n", j, kv.first.c, (unsigned long long)kv.first.S, (int)kv.second);
  }
}
