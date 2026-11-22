// Gossip tug game, written from the definition in 3-question.txt.
// Position: K[i] bitmask of secrets agent i holds. Call {a,b} legal iff a notin K[b] or b notin K[a].
// Modes:
//   full N            : exact minimax A(N) (memo over relabel-normalised positions)
//   hast N P          : Hastener plays fixed policy P, Delayer optimal (max)  -> upper bound on A(N)
//   line N            : print one optimal line (after full)
// Start position can be changed: "clique M R" variants via env START="m r".
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
int N;
struct St { uint16_t K[14]; };
static inline bool legal(const St&s,int a,int b){ return a!=b && (!((s.K[b]>>a)&1) || !((s.K[a]>>b)&1)); }
static inline bool terminal(const St&s){ uint16_t F=(1u<<N)-1; for(int i=0;i<N;i++) if(s.K[i]!=F) return false; return true; }
static inline St docall(St s,int a,int b){ uint16_t u=s.K[a]|s.K[b]; s.K[a]=u; s.K[b]=u; return s; }
// normalise by relabeling (any relabeling is an isomorphism, so correctness never depends on quality)
static St normalise(const St&s){
  uint64_t sig[14]; int indeg[14];
  for(int i=0;i<N;i++){ indeg[i]=0; for(int j=0;j<N;j++) if((s.K[j]>>i)&1) indeg[i]++; }
  for(int i=0;i<N;i++) sig[i]=__builtin_popcount(s.K[i])*16+indeg[i];
  for(int r=0;r<2;r++){
    uint64_t ns[14];
    for(int i=0;i<N;i++){
      uint64_t a=0,b=0;
      for(int j=0;j<N;j++){ uint64_t h=(sig[j]+0x9e3779b97f4a7c15ULL)*0xbf58476d1ce4e5b9ULL; h^=h>>29;
        if((s.K[i]>>j)&1) a+=h; if((s.K[j]>>i)&1) b+=h*3; }
      ns[i]=sig[i]*1000003ULL + a*31 + b;
    }
    for(int i=0;i<N;i++) sig[i]=ns[i];
  }
  int ord[14]; for(int i=0;i<N;i++) ord[i]=i;
  stable_sort(ord,ord+N,[&](int x,int y){return sig[x]<sig[y];});
  int pos[14]; for(int i=0;i<N;i++) pos[ord[i]]=i;
  St t; for(int i=0;i<N;i++){ uint16_t m=0,k=s.K[ord[i]]; for(int j=0;j<N;j++) if((k>>j)&1) m|=1u<<pos[j]; t.K[i]=m; }
  return t;
}
static inline u128 key(const St&s){ u128 k=0; for(int i=0;i<N;i++) k=(k<<N)|s.K[i]; return k; }
struct H{ size_t operator()(const u128&k)const{ uint64_t a=(uint64_t)k,b=(uint64_t)(k>>64); a^=b*0x9e3779b97f4a7c15ULL; a*=0xbf58476d1ce4e5b9ULL; a^=a>>31; return a; } };
unordered_map<u128,int8_t,H> memo;
int POL=0; bool USEPOL=false;
// Hastener policies: return b for (state, named a). s is the normalised state.
static int policy(const St&s,int a){
  uint16_t F=(1u<<N)-1; int best=-1; long bs=LONG_MIN;
  int indeg[14]; for(int i=0;i<N;i++){ indeg[i]=0; for(int j=0;j<N;j++) if((s.K[j]>>i)&1) indeg[i]++; }
  for(int b=0;b<N;b++) if(legal(s,a,b)){
    long sc=0; int un=__builtin_popcount(s.K[a]|s.K[b]); int kb=__builtin_popcount(s.K[b]);
    switch(POL){
      case 0: sc=un*100-indeg[b]; break;            // max union, then least-known partner
      case 1: sc=un*100+indeg[b]; break;            // max union, then most-known partner
      case 2: sc=-indeg[b]*100+un; break;           // least-known partner, then max union
      case 3: sc=kb*100-indeg[b]; break;            // most knowledgeable partner
      case 4: { // max union; tie: least-known among {a after call...}
        St t=docall(s,a,b); int ex=0; for(int i=0;i<N;i++) if(t.K[i]==F) ex++; sc=ex*10000+un*100-indeg[b]; break; }
      case 5: { // one-step: minimise number of live pairs after call
        St t=docall(s,a,b); int live=0; for(int x=0;x<N;x++) for(int y=x+1;y<N;y++) if(legal(t,x,y)) live++; sc=-live*100-indeg[b]; break; }
      case 6: { // prefer partner that a does not know and is least known; among them max union
        sc = (((s.K[a]>>b)&1)?0:100000) + un*100 - indeg[b]; break; }
    }
    if(sc>bs){bs=sc;best=b;}
  }
  return best;
}
int solve(const St&s){ // s normalised
  if(terminal(s)) return 0;
  u128 k=key(s); auto it=memo.find(k); if(it!=memo.end()) return it->second;
  int best=-1;
  for(int a=0;a<N;a++){
    int worst=INT_MAX; bool act=false;
    if(USEPOL){ int b=policy(s,a); if(b<0) continue; act=true; worst=1+solve(normalise(docall(s,a,b))); }
    else for(int b=0;b<N;b++) if(legal(s,a,b)){ act=true; int v=1+solve(normalise(docall(s,a,b))); if(v<worst) worst=v; }
    if(act && worst>best) best=worst;
  }
  memo[k]=(int8_t)best; return best;
}
static void show(const St&s){ for(int i=0;i<N;i++){ printf(" %d:",i); for(int j=0;j<N;j++) if((s.K[i]>>j)&1) printf("%x",j); } printf("\n"); }
int main(int argc,char**argv){
  string mode=argv[1]; N=atoi(argv[2]);
  St s; for(int i=0;i<N;i++) s.K[i]=1u<<i;
  if(getenv("START")){ int m,r; sscanf(getenv("START"),"%d %d",&m,&r); N=m+r; for(int i=0;i<N;i++) s.K[i]= i<m ? (uint16_t)((1u<<m)-1) : (uint16_t)(1u<<i); }
  if(getenv("KS")){ string t=getenv("KS"); N=0; char*q=strtok(&t[0],","); while(q){ s.K[N++]=(uint16_t)strtol(q,0,16); q=strtok(0,","); } }
  if(mode=="hast"){ USEPOL=true; POL=atoi(argv[3]); }
  s=normalise(s);
  int v=solve(s);
  printf("mode=%s N=%d pol=%d value=%d states=%zu\n",mode.c_str(),N,USEPOL?POL:-1,v,memo.size());
  if(mode=="line"||getenv("LINE")){
    // print an optimal line: Delayer picks first maximising agent, Hastener (or policy) the minimising reply
    while(!terminal(s)){
      int ba=-1,bb=-1,bv=-1;
      for(int a=0;a<N;a++){ int w=INT_MAX,wb=-1;
        if(USEPOL){ int b=policy(s,a); if(b<0) continue; w=1+solve(normalise(docall(s,a,b))); wb=b; }
        else for(int b=0;b<N;b++) if(legal(s,a,b)){ int x=1+solve(normalise(docall(s,a,b))); if(x<w){w=x;wb=b;} }
        if(wb>=0 && w>bv){bv=w;ba=a;bb=wb;} }
      show(s); printf("  D names %d, H answers %d (value %d)\n",ba,bb,bv);
      s=normalise(docall(s,ba,bb));
    }
    show(s);
  }
}
