// Exact F(n,k) by memoised max-search over adversary moves.
// State: knowledge bitmasks K[0..n-1] + previous destruction set restricted to currently useful links.
// Prunings (both safe, see proofs.txt section P):
//  (1) only "useful" links (K[src] not subset of K[dst]) are ever destroyed;
//  (2) prevD is intersected with the useful links of the current state;
//  (3) state canonicalised under the dihedral group of the ring (optional, flag sym=1).
// Link index: l in [0,n): (l -> l+1) ; l in [n,2n): ((l-n) -> (l-n)-1).
// usage: fast n k [sym=1] [witness=0]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n,k,SYM=1; unsigned FULL;
struct Key{ array<uint16_t,13> K; uint32_t prev; bool operator==(const Key&o)const{return K==o.K&&prev==o.prev;} };
struct H{ size_t operator()(const Key&s)const{ u64 h=1469598103934665603ULL; for(int i=0;i<n;i++){h^=s.K[i];h*=1099511628211ULL;} h^=s.prev; h*=1099511628211ULL; h^=h>>29; return h;} };
unordered_map<Key,uint8_t,H> memo;
inline int src(int l){return l<n?l:l-n;}
inline int dst(int l){return l<n?(l+1)%n:(l-n+n-1)%n;}
inline unsigned rotm(unsigned m,int s){ return ((m<<s)|(m>>(n-s)))&FULL; }
inline unsigned refm(unsigned m){ unsigned r=0; for(int i=0;i<n;i++) if(m>>i&1) r|=1u<<((n-i)%n); return r; }
Key canon(const Key&a){
  if(!SYM) return a;
  Key best=a; bool first=true;
  for(int ref=0;ref<2;ref++) for(int s=0;s<n;s++){
    Key b; b.K.fill(0); b.prev=0;
    for(int i=0;i<n;i++){
      int j = ref? ((n-i)%n+s)%n : (i+s)%n;
      unsigned m=a.K[i]; if(ref) m=refm(m); if(s) m=rotm(m,s);
      b.K[j]=m;
    }
    for(int l=0;l<2*n;l++) if(a.prev>>l&1){
      int i=src(l); bool cw=l<n; int j= ref? ((n-i)%n+s)%n : (i+s)%n; bool cw2= ref? !cw:cw;
      b.prev|=1u<<(cw2?j:j+n);
    }
    if(first|| make_pair(b.K,b.prev)<make_pair(best.K,best.prev)){best=b;first=false;}
  }
  return best;
}
int V(Key s){ // s.prev may contain non-useful links; restrict
  int useful[26],nu=0; uint32_t um=0;
  for(int l=0;l<2*n;l++){ if(s.K[src(l)] & ~s.K[dst(l)]){useful[nu++]=l;um|=1u<<l;} }
  s.prev&=um;
  Key c=canon(s);
  auto it=memo.find(c); if(it!=memo.end()) return it->second;
  // enumerate subsets of allowed useful links of size<=k
  int al[26],na=0; for(int i=0;i<nu;i++) if(!(s.prev>>useful[i]&1)) al[na++]=useful[i];
  int best=0;
  // iterate subsets by recursion
  vector<int> st; 
  function<void(int,uint32_t,int)> rec=[&](int idx,uint32_t D,int cnt){
    if(idx==na){
      Key t; t.K=s.K; t.prev=D; bool comp=false;
      for(int l=0;l<2*n;l++) if(!(D>>l&1)) t.K[dst(l)]|=s.K[src(l)];
      for(int i=0;i<n;i++) if(t.K[i]==FULL) comp=true;
      int v= comp?1:1+V(t);
      if(v>best) best=v;
      return;
    }
    if(cnt<k) rec(idx+1,D|(1u<<al[idx]),cnt+1);
    rec(idx+1,D,cnt);
  };
  rec(0,0,0);
  memo[c]=best; return best;
}
int main(int argc,char**argv){
  n=atoi(argv[1]);k=atoi(argv[2]); if(argc>3) SYM=atoi(argv[3]); int wit=argc>4?atoi(argv[4]):0;
  FULL=(1u<<n)-1;
  Key s; s.K.fill(0); for(int i=0;i<n;i++) s.K[i]=1u<<i; s.prev=0;
  int F=V(s);
  printf("%d %d %d states=%zu\n",n,k,F,memo.size());
  if(wit){ // extract one optimal sequence by greedy re-descent
    char fn[64]; sprintf(fn,"witness_n%d_k%d.txt",n,k); FILE*f=fopen(fn,"w");
    fprintf(f,"# n=%d k=%d F=%d ; each line: round t, then destroyed links as src>dst\n",n,k,F);
    int rem=F; int t=0;
    while(rem>0){
      t++;
      uint32_t um=0; for(int l=0;l<2*n;l++) if(s.K[src(l)] & ~s.K[dst(l)]) um|=1u<<l;
      uint32_t allowed=um&~s.prev; bool found=false;
      for(uint32_t D=allowed;;D=(D-1)&allowed){
        if(__builtin_popcount(D)<=k){
          Key t2; t2.K=s.K; t2.prev=D; bool comp=false;
          for(int l=0;l<2*n;l++) if(!(D>>l&1)) t2.K[dst(l)]|=s.K[src(l)];
          for(int i=0;i<n;i++) if(t2.K[i]==FULL) comp=true;
          int v=comp?1:1+V(t2);
          if(v==rem){ fprintf(f,"%d",t); for(int l=0;l<2*n;l++) if(D>>l&1) fprintf(f," %d>%d",src(l),dst(l)); fprintf(f,"\n"); s=t2; rem--; found=true; if(comp&&rem!=0){fprintf(stderr,"BUG\n");return 1;} break; }
        }
        if(D==0) break;
      }
      if(!found){fprintf(stderr,"BUG no move\n");return 1;}
    }
    fclose(f);
  }
}
