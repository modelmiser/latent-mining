// referee: integer level enumeration. State = unordered pair packed hi=larger, lo=smaller.
// Level t = set of states reachable by exactly t steps (dedup inside the level only).
// usage: lev T VMAXLEVEL  ; last level T is streamed (values only).
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <vector>
#include <algorithm>
typedef uint64_t u64;
static inline u64 pk(u64 p,u64 q){ return p>=q ? (p<<32)|q : (q<<32)|p; }
int main(int argc,char**argv){
  int T=atoi(argv[1]); int VL=atoi(argv[2]);
  size_t LIM=((size_t)1<<T);
  std::vector<int8_t> G(LIM+1,-1);
  std::vector<u64> N(T+1,0);
  std::vector<u64> cur; cur.push_back(pk(1,1));
  u64 cnt=0;
  auto mark=[&](u64 v,int t){ if(v>LIM){fprintf(stderr,"value over 2^T\n");exit(2);} if(G[v]<0){G[v]=(int8_t)t;cnt++;} };
  for(int t=0;t<=T;t++){
    for(u64 s:cur){ mark(s>>32,t); mark(s&0xffffffffULL,t); }
    N[t]=cnt; printf("t %d N %llu states %zu\n",t,(unsigned long long)cnt,cur.size()); fflush(stdout);
    if(t==T) break;
    if(t==T-1){
      for(u64 s:cur){ u64 y=s>>32,x=s&0xffffffffULL; mark(2*y,T); mark(2*x,T); mark(x+y,T); }
      N[T]=cnt; printf("t %d N %llu states streamed\n",T,(unsigned long long)cnt); break;
    }
    std::vector<u64> nx; nx.reserve(cur.size()*4);
    for(u64 s:cur){ u64 y=s>>32,x=s&0xffffffffULL;
      nx.push_back(pk(2*y,x)); nx.push_back(pk(y,2*x)); nx.push_back(pk(x+y,x)); nx.push_back(pk(y,x+y)); }
    std::vector<u64>().swap(cur);
    std::sort(nx.begin(),nx.end()); nx.erase(std::unique(nx.begin(),nx.end()),nx.end());
    cur.swap(nx);
  }
  // values with G<=VL
  FILE*f=fopen("vals.txt","w");
  for(size_t n=1;n<=LIM;n++) if(G[n]>=0 && G[n]<=VL) fprintf(f,"%zu %d\n",n,(int)G[n]);
  fclose(f);
  // excess test: among n with G(n)<=T: G(n)>=ceil(log2 n); equality iff popcount<=2
  u64 eq=0,eqbad=0,below=0,pc2bad=0;
  for(size_t n=1;n<=LIM;n++){ int c=0; while(((size_t)1<<c)<n)c++; int pc=__builtin_popcountll(n);
    if(G[n]>=0){ if(G[n]<c)below++; if(G[n]==c){eq++; if(pc>2)eqbad++;} }
    if(pc<=2 && G[n]!=c) pc2bad++; }
  printf("G==ceil count %llu ; with popcount>2: %llu ; G<ceil: %llu ; popcount<=2 with G!=ceil: %llu\n",
    (unsigned long long)eq,(unsigned long long)eqbad,(unsigned long long)below,(unsigned long long)pc2bad);
}
