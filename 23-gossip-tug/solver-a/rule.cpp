// Rule-restricted exact solver: one player follows a fixed simple rule, the other is optimal.
// usage: rule n D|H ruleid [lgsize]   D: Delayer follows rule => value is a LOWER bound on A(n)
//                                    H: Hastener follows rule => value is an UPPER bound on A(n)
// (derived from ab.cpp) Null-window (decision) solver with transposition table of bounds, n <= 11.
// State: n rows of n bits packed into unsigned __int128 (row i = K_i).
// search(s,t) decides "V(s) >= t".  TT stores proven lb/ub per canonical state.
// Safe prunings: (1) relabelling agents (value invariant);
// (2) ub(s) = number of unordered pairs not mutually known (each call makes its pair
//     mutually known for ever and a mutually-known pair is never legal again);
// (3) lb(s) = ceil(#non-expert agents / 2) (each call can complete at most 2 agents).
// usage: ab n tlo thi [lgsize]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128; typedef uint64_t u64;
int n; u128 FULLST; unsigned RM;
static inline unsigned K(u128 s,int i){return (unsigned)(s>>(n*i))&RM;}
struct Table{
  vector<u128> key; vector<uint8_t> lb,ub; size_t mask,cnt=0;
  Table(int lg){size_t N=size_t(1)<<lg; key.assign(N,~(u128)0); lb.assign(N,0); ub.assign(N,255); mask=N-1;}
  static inline u64 h(u128 k){u64 x=(u64)k^((u64)(k>>64)*0x9E3779B97F4A7C15ULL);x^=x>>33;x*=0xff51afd7ed558ccdULL;x^=x>>33;x*=0xc4ceb9fe1a85ec53ULL;x^=x>>33;return x;}
  size_t slot(u128 k){size_t i=h(k)&mask;while(key[i]!=~(u128)0&&key[i]!=k)i=(i+1)&mask; if(key[i]!=k){ if(cnt*10>mask*9){fprintf(stderr,"TT full\n");exit(2);} key[i]=k;cnt++;} return i;}
};
Table*T; u64 nodes=0;
u128 canon(u128 s){
  unsigned k[12]; int inv[12], base[12];
  for(int i=0;i<n;i++) k[i]=K(s,i);
  for(int i=0;i<n;i++){int c=0;for(int j=0;j<n;j++) c+=(k[j]>>i)&1; base[i]=__builtin_popcount(k[i])*13+c;}
  for(int r=0;r<2;r++){
    for(int i=0;i<n;i++){
      unsigned so=0, si=0, sm=0;
      for(int j=0;j<n;j++){ unsigned bj=base[j];
        if((k[i]>>j)&1) so+= ((bj+1)*(bj+7)*2654435761u)>>8;
        if((k[j]>>i)&1) si+=(bj+3)*(bj+11)*40503u;
        if(((k[i]>>j)&1)&&((k[j]>>i)&1)) sm+=(bj+5)*(bj+13)*69069u; }
      inv[i]= (int)(((unsigned)base[i]*1000003u) ^ (so*31u) ^ (si*131u) ^ (sm*977u)) & 0x7fffffff;
    }
    for(int i=0;i<n;i++) base[i]=inv[i];
  }
  int ord[12]; for(int i=0;i<n;i++) ord[i]=i;
  sort(ord,ord+n,[&](int a,int b){return inv[a]!=inv[b]?inv[a]<inv[b]:a<b;});
  int cs[13], nc=0; cs[0]=0;
  for(int i=1;i<=n;i++) if(i==n||inv[ord[i]]!=inv[ord[i-1]]) cs[++nc]=i;
  u128 best=~(u128)0;
  int cur[12]; memcpy(cur,ord,sizeof(int)*n);
  for(int c=0;c<nc;c++) sort(cur+cs[c],cur+cs[c+1]);
  while(true){
    int newlab[12]; for(int p=0;p<n;p++) newlab[cur[p]]=p;
    u128 code=0;
    for(int p=0;p<n;p++){ unsigned m=k[cur[p]], r=0; while(m){int j=__builtin_ctz(m); m&=m-1; r|=1u<<newlab[j];} code|=(u128)r<<(n*p);}
    if(code<best) best=code;
    int c=0;
    for(;c<nc;c++){ if(next_permutation(cur+cs[c],cur+cs[c+1])) break; }
    if(c==nc) break;
  }
  return best;
}

char SIDE; int RULE;
static inline bool legal(unsigned*k,int a,int b){return a!=b && !(((k[b]>>a)&1)&&((k[a]>>b)&1));}
// key for Delayer rule: smaller is preferred
long dkey(unsigned*k,int a){
  int part=0,kb=0; for(int b=0;b<n;b++){ if(legal(k,a,b)) part++; kb+=(k[b]>>a)&1; }
  int sz=__builtin_popcount(k[a]);
  switch(RULE){
    case 0: return part*10000L - sz*100 + a;            // fewest partners, then largest knowledge
    case 1: return part*10000L + sz*100 + a;            // fewest partners, then smallest knowledge
    case 2: return part*10000L - kb*100 + a;            // fewest partners, then most widely known
    case 3: return -kb*10000L + part*100 + a;           // most widely known, then fewest partners
    case 4: return -sz*10000L + part*100 + a;           // largest knowledge, then fewest partners
    case 5: return part*1000000L - kb*10000L + sz*100 + a;
    case 6: return part*1000000L - kb*10000L - sz*100 + a;
  }
  return a;
}
// key for Hastener rule: smaller is preferred
long hkey(unsigned*k,int a,int b){
  int un=__builtin_popcount(k[a]|k[b]); int sz=__builtin_popcount(k[b]); int kb=0; for(int c=0;c<n;c++) kb+=(k[c]>>b)&1;
  int mut = (((k[b]>>a)&1)||((k[a]>>b)&1));
  switch(RULE){
    case 0: return -un*10000L + b;
    case 1: return -un*10000L - sz*100 + b;
    case 2: return -un*10000L + sz*100 + b;
    case 3: return -un*10000L + kb*100 + b;
    case 4: return -un*10000L - kb*100 + b;
    case 5: return -un*100000L + mut*10000L + kb*100 + b;
  }
  return b;
}
int val(u128 s){
  if(s==FULLST) return 0;
  size_t sl=T->slot(s);
  if(T->ub[sl]!=255) return T->ub[sl];
  unsigned k[12]; for(int i=0;i<n;i++) k[i]=K(s,i);
  int res;
  auto child=[&](int a,int b){ u128 u=k[a]|k[b]; u128 tt=s; tt&=~((u128)RM<<(n*a)); tt&=~((u128)RM<<(n*b)); tt|=u<<(n*a); tt|=u<<(n*b); return canon(tt); };
  if(SIDE=='D'){
    int ba=-1; long bk=0;
    for(int a=0;a<n;a++){ bool act=false; for(int b=0;b<n;b++) if(legal(k,a,b)) act=true; if(!act) continue; long key=dkey(k,a); if(ba<0||key<bk){ba=a;bk=key;} }
    res=1000;
    for(int b=0;b<n;b++) if(legal(k,ba,b)) res=min(res,1+val(child(ba,b)));
  } else {
    res=0;
    for(int a=0;a<n;a++){ int bb=-1; long bk=0; for(int b=0;b<n;b++) if(legal(k,a,b)){ long key=hkey(k,a,b); if(bb<0||key<bk){bb=b;bk=key;} }
      if(bb>=0) res=max(res,1+val(child(a,bb))); }
  }
  sl=T->slot(s); T->ub[sl]=res; return res;
}
int main(int argc,char**argv){
  n=atoi(argv[1]); SIDE=argv[2][0]; RULE=atoi(argv[3]); int lg=argc>4?atoi(argv[4]):24;
  RM=(1u<<n)-1; FULLST=0; for(int i=0;i<n;i++) FULLST|=(u128)RM<<(n*i);
  T=new Table(lg);
  u128 st=0; for(int i=0;i<n;i++) st|=(u128)(1u<<i)<<(n*i);
  int v=val(canon(st));
  printf("n=%d side=%c rule=%d value=%d states=%zu\n",n,SIDE,RULE,v,T->cnt);
}
