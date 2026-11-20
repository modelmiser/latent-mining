// Null-window (decision) solver with transposition table of bounds, n <= 11.
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
bool search(u128 s,int t){ // s canonical; decide V(s)>=t
  if(t<=0) return true;
  if(s==FULLST) return false;
  size_t sl=T->slot(s);
  unsigned k[12]; for(int i=0;i<n;i++) k[i]=K(s,i);
  if(T->ub[sl]==255){ // init bounds
    int nm=0, ne=0;
    for(int i=0;i<n;i++){ if(k[i]!=RM) ne++; for(int j=i+1;j<n;j++) if(!(((k[i]>>j)&1)&&((k[j]>>i)&1))) nm++; }
    T->ub[sl]=nm; T->lb[sl]=(ne+1)/2;
  }
  if(T->lb[sl]>=t) return true;
  if(T->ub[sl]<t) return false;
  nodes++;
  // Delayer move ordering: fewest legal partners first
  int cntl[12], ord[12], na=0;
  for(int a=0;a<n;a++){ int c=0; for(int b=0;b<n;b++) if(a!=b && !(((k[b]>>a)&1)&&((k[a]>>b)&1))) c++; cntl[a]=c; if(c) ord[na++]=a; }
  sort(ord,ord+na,[&](int x,int y){ if(cntl[x]!=cntl[y]) return cntl[x]<cntl[y]; return __builtin_popcount(k[x])>__builtin_popcount(k[y]); });
  for(int ia=0;ia<na;ia++){
    int a=ord[ia];
    int bs[12], nb=0;
    for(int b=0;b<n;b++) if(a!=b && !(((k[b]>>a)&1)&&((k[a]>>b)&1))) bs[nb++]=b;
    // Hastener ordering: biggest union first
    sort(bs,bs+nb,[&](int x,int y){ return __builtin_popcount(k[a]|k[x])>__builtin_popcount(k[a]|k[y]); });
    bool all=true;
    for(int ib=0;ib<nb;ib++){
      int b=bs[ib]; u128 u=k[a]|k[b];
      u128 tt=s; tt&=~((u128)RM<<(n*a)); tt&=~((u128)RM<<(n*b)); tt|=u<<(n*a); tt|=u<<(n*b);
      if(!search(canon(tt),t-1)){all=false;break;}
    }
    if(all){ sl=T->slot(s); if(T->lb[sl]<t) T->lb[sl]=t; return true; }
  }
  sl=T->slot(s); if(T->ub[sl]>t-1) T->ub[sl]=t-1; return false;
}
int main(int argc,char**argv){
  n=atoi(argv[1]); int tlo=atoi(argv[2]), thi=atoi(argv[3]); int lg=argc>4?atoi(argv[4]):24;
  RM=(1u<<n)-1; FULLST=0; for(int i=0;i<n;i++) FULLST|=(u128)RM<<(n*i);
  T=new Table(lg);
  u128 st=0; for(int i=0;i<n;i++) st|=(u128)(1u<<i)<<(n*i);
  st=canon(st);
  for(int t=tlo;t<=thi;t++){
    bool r=search(st,t);
    fprintf(stderr,"n=%d  A(n)>=%d ? %s   (TT entries %zu, nodes %llu)\n",n,t,r?"YES":"NO",T->cnt,(unsigned long long)nodes);
    if(!r) break;
  }
}
