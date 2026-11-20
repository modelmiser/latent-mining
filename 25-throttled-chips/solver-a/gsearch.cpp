// Exhaustive search for G_W(h): least m such that some list in {0..W-1}^m wins.
// State = exact set of Firer-reachable configurations after a list prefix (subset construction).
// Prunings (all exact for the window-restricted problem):
//  * memo on the canonical set (reflection of the window), keyed by a 128-bit hash (collision prob negligible, not zero)
//  * remaining length k < h - min_{c in S} max height(c)  => cannot empty S (each round adds one chip)
// Usage: gsearch h W [maxm] [NOLB]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int h, W, OFF, M; bool uselb=true;
static inline int get(u64 c,int i){return (c>>(4*i))&15;}
struct H128{u64 a,b; bool operator==(const H128&o)const{return a==o.a&&b==o.b;}};
struct HH{size_t operator()(const H128&x)const{return x.a;}};
unordered_map<H128,signed char,HH> failk;
static inline u64 mix(u64 x){x^=x>>33;x*=0xff51afd7ed558ccdULL;x^=x>>33;x*=0xc4ceb9fe1a85ec53ULL;x^=x>>33;return x;}
u64 mirror(u64 c){u64 r=0;for(int i=0;i<=M;i++){r|=(u64)get(c,i)<<(4*(M-i));}return r;}
H128 hashset(const vector<u64>&S){H128 r{0x1234567,0x89abcdef};for(u64 c:S){r.a=mix(r.a^c)+0x9e3779b97f4a7c15ULL;r.b=mix(r.b+c*0xc2b2ae3d27d4eb4fULL)^(r.a<<1);}r.a^=S.size();return r;}
void step(const vector<u64>&S,int a,vector<u64>&T){
  T.clear(); int ia=a+OFF;
  for(u64 c:S){
    if(get(c,ia)+1>=h) continue;
    u64 c1=c+((u64)1<<(4*ia));
    T.push_back(c1);
    for(int v=0;v<=M;v++){ int g=get(c1,v); if(g<2) continue;
      if(v<=1||v>=M-1){fprintf(stderr,"OVERFLOW of site array\n");exit(3);}
      if(get(c1,v-1)+1>=h||get(c1,v+1)+1>=h) continue;
      u64 c2=c1-((u64)2<<(4*v))+((u64)1<<(4*(v-1)))+((u64)1<<(4*(v+1)));
      T.push_back(c2);
    }
  }
  sort(T.begin(),T.end()); T.erase(unique(T.begin(),T.end()),T.end());
}
u64 nodes=0; size_t maxset=0; vector<int> path;
bool can(const vector<u64>&S,int k){
  if(S.empty()) return true;
  if(k==0) return false;
  nodes++; if(S.size()>maxset)maxset=S.size();
  if(uselb){ int mm=99; for(u64 c:S){int mx=0;for(int i=0;i<=M;i++)mx=max(mx,get(c,i)); mm=min(mm,mx);} if(k<h-mm) return false; }
  vector<u64> R(S.size()); for(size_t i=0;i<S.size();i++)R[i]=mirror(S[i]); sort(R.begin(),R.end());
  H128 key=hashset(R<S?R:S);
  auto it=failk.find(key); if(it!=failk.end()&&it->second>=k) return false;
  vector<u64> T;
  for(int a=0;a<W;a++){ step(S,a,T); path.push_back(a); if(can(T,k-1)) return true; path.pop_back(); }
  failk[key]=k; return false;
}
int main(int argc,char**argv){
  h=atoi(argv[1]);W=atoi(argv[2]);int maxm=argc>3?atoi(argv[3]):60; if(argc>4)uselb=false;
  if(W%2){OFF=(15-W)/2;M=14;}else{OFF=(16-W)/2;M=15;}
  vector<u64> S0={0};
  for(int m=1;m<=maxm;m++){
    path.clear();
    bool r=can(S0,m);
    fprintf(stderr,"h=%d W=%d m=%d %s nodes=%llu memo=%zu maxset=%zu\n",h,W,m,r?"WIN":"no",nodes,failk.size(),maxset);
    if(r){printf("G_W(h=%d,W=%d)=%d list:",h,W,m);for(size_t i=0;i<path.size();i++)printf("%s%d",i?",":" ",path[i]);printf("\n");return 0;}
  }
  printf("h=%d W=%d: no winning list of length <= %d\n",h,W,maxm);
}
