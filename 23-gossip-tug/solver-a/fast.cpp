// Fast solver for the Delayer/Hastener gossip game.
// State: n bytes packed into uint64 (byte i = K_i as bitmask), n <= 8.
// Memo on a canonical form under relabelling of agents (value is invariant
// under relabelling, so ANY relabelling is safe; mode 0 uses none at all).
// usage: fast n mode [dump]   mode 0 = raw memo, mode 1 = canonical memo
//   mode 2 = canonical memo with brute-force canonical form (min over all n! relabellings)
//   dump: write "state value" table of all canonical states to stdout file
#include <bits/stdc++.h>
using namespace std;
typedef uint64_t u64;
int n; u64 FULLST; int MODE;
static inline int K(u64 s,int i){return (s>>(8*i))&0xff;}
// open addressing hash
struct Table{
  vector<u64> key; vector<uint8_t> val; size_t mask, cnt=0;
  Table(size_t lg){key.assign(size_t(1)<<lg,~0ULL);val.assign(size_t(1)<<lg,0);mask=(size_t(1)<<lg)-1;}
  static inline u64 h(u64 x){x^=x>>33;x*=0xff51afd7ed558ccdULL;x^=x>>33;x*=0xc4ceb9fe1a85ec53ULL;x^=x>>33;return x;}
  int get(u64 k){size_t i=h(k)&mask;while(key[i]!=~0ULL){if(key[i]==k)return val[i];i=(i+1)&mask;}return -1;}
  void put(u64 k,int v){ if(cnt*10>mask*7) grow(); size_t i=h(k)&mask;while(key[i]!=~0ULL){i=(i+1)&mask;}key[i]=k;val[i]=v;cnt++;}
  void grow(){vector<u64> ok; vector<uint8_t> ov; ok.swap(key); ov.swap(val); size_t nm=(mask+1)*2; key.assign(nm,~0ULL); val.assign(nm,0); mask=nm-1; cnt=0;
    for(size_t i=0;i<ok.size();i++) if(ok[i]!=~0ULL){size_t j=h(ok[i])&mask;while(key[j]!=~0ULL)j=(j+1)&mask;key[j]=ok[i];val[j]=ov[i];cnt++;}}
};
Table *T;
u64 canon(u64 s){
  if(MODE==0) return s;
  int k[8], inv[8];
  for(int i=0;i<n;i++) k[i]=K(s,i);
  int indeg[8];
  for(int i=0;i<n;i++){int c=0;for(int j=0;j<n;j++) c+=(k[j]>>i)&1; indeg[i]=c;}
  int base[8];
  for(int i=0;i<n;i++) base[i]=__builtin_popcount(k[i])*9+indeg[i];
  // one refinement round: sums of a mixing function over out- and in-neighbours
  for(int i=0;i<n;i++){
    unsigned so=0, si=0;
    for(int j=0;j<n;j++){ if((k[i]>>j)&1) so+= (base[j]+1)*(base[j]+7)*2654435761u>>8; if((k[j]>>i)&1) si+=(base[j]+3)*(base[j]+11)*40503u; }
    inv[i]= (int)(((unsigned)base[i]*1000003u) ^ (so*31u) ^ (si*131u)) & 0x7fffffff;
  }
  int ord[8]; for(int i=0;i<n;i++) ord[i]=i;
  sort(ord,ord+n,[&](int a,int b){return inv[a]!=inv[b]?inv[a]<inv[b]:a<b;});
  // class boundaries
  int cs[9], nc=0; cs[0]=0;
  if(MODE==2){ for(int i=0;i<n;i++) ord[i]=i; nc=1; cs[1]=n; } // brute force: all n! relabellings
  else for(int i=1;i<=n;i++) if(i==n||inv[ord[i]]!=inv[ord[i-1]]) cs[++nc]=i;
  u64 best=~0ULL;
  // enumerate all orderings permuting within classes
  int cur[8]; memcpy(cur,ord,sizeof(cur));
  for(int c=0;c<nc;c++) sort(cur+cs[c],cur+cs[c+1]);
  while(true){
    // cur[pos] = old label placed at new position pos
    int newlab[8]; for(int p=0;p<n;p++) newlab[cur[p]]=p;
    u64 code=0;
    for(int p=0;p<n;p++){ int m=k[cur[p]], r=0; while(m){int j=__builtin_ctz(m); m&=m-1; r|=1<<newlab[j];} code|=(u64)r<<(8*p);}
    if(code<best) best=code;
    int c=0;
    for(;c<nc;c++){ if(next_permutation(cur+cs[c],cur+cs[c+1])) break; }
    if(c==nc) break;
  }
  return best;
}
int V(u64 s){ // s canonical
  if(s==FULLST) return 0;
  int g=T->get(s); if(g>=0) return g;
  int k[8]; for(int i=0;i<n;i++) k[i]=K(s,i);
  int best=-1;
  for(int a=0;a<n;a++){
    int worst=1000;
    for(int b=0;b<n;b++){
      if(a==b) continue;
      if(((k[b]>>a)&1) && ((k[a]>>b)&1)) continue;
      u64 u=k[a]|k[b];
      u64 t=s; t&=~((u64)0xff<<(8*a)); t&=~((u64)0xff<<(8*b)); t|=u<<(8*a); t|=u<<(8*b);
      int v=1+V(canon(t));
      if(v<worst) worst=v;
    }
    if(worst<1000 && worst>best) best=worst;
  }
  if(best<0){fprintf(stderr,"no active agent in nonterminal state\n");exit(1);}
  T->put(s,best); return best;
}
int main(int argc,char**argv){
  n=atoi(argv[1]); MODE=atoi(argv[2]); bool dump=argc>3;
  FULLST=0; for(int i=0;i<n;i++) FULLST|=(u64)((1<<n)-1)<<(8*i);
  T=new Table(20);
  u64 st=0; for(int i=0;i<n;i++) st|=(u64)(1<<i)<<(8*i);
  int v=V(canon(st));
  fprintf(stderr,"n=%d mode=%d A(n)=%d states=%zu\n",n,MODE,v,T->cnt);
  if(dump){ for(size_t i=0;i<=T->mask;i++) if(T->key[i]!=~0ULL) printf("%016llx %d\n",(unsigned long long)T->key[i],T->val[i]); }
}
