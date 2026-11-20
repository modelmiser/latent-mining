// referee: level enumeration to T (last level streamed), then D(m) for m<=MMAX via definition 1:
// D(m) = least t such that {n : G(n)<=t} meets every residue class mod m. Exact whenever the level-T value set covers Z_m.
#include <cstdio>
#include <cstdlib>
#include <cstdint>
#include <cmath>
#include <vector>
#include <algorithm>
#include <map>
typedef uint64_t u64;
static inline u64 pk(u64 p,u64 q){ return p>=q ? (p<<32)|q : (q<<32)|p; }
int main(int argc,char**argv){
  int T=atoi(argv[1]); int MMAX=atoi(argv[2]);
  size_t LIM=((size_t)1<<T);
  std::vector<int8_t> G(LIM+1,-1); std::vector<u64> N(T+1,0);
  std::vector<u64> cur; cur.push_back(pk(1,1)); u64 cnt=0;
  auto mark=[&](u64 v,int t){ if(G[v]<0){G[v]=(int8_t)t;cnt++;} };
  for(int t=0;t<=T;t++){
    for(u64 s:cur){ mark(s>>32,t); mark(s&0xffffffffULL,t); }
    N[t]=cnt; if(t==T) break;
    if(t==T-1){ for(u64 s:cur){ u64 y=s>>32,x=s&0xffffffffULL; mark(2*y,T); mark(2*x,T); mark(x+y,T); } N[T]=cnt; break; }
    std::vector<u64> nx; nx.reserve(cur.size()*4);
    for(u64 s:cur){ u64 y=s>>32,x=s&0xffffffffULL;
      nx.push_back(pk(2*y,x)); nx.push_back(pk(y,2*x)); nx.push_back(pk(x+y,x)); nx.push_back(pk(y,x+y)); }
    std::vector<u64>().swap(cur);
    std::sort(nx.begin(),nx.end()); nx.erase(std::unique(nx.begin(),nx.end()),nx.end()); cur.swap(nx);
  }
  std::vector<u64>().swap(cur);
  printf("N:"); for(int t=0;t<=T;t++) printf(" %llu",(unsigned long long)N[t]); puts("");
  FILE*f=fopen("G_hi.txt","w"); for(size_t n=1;n<=32768;n++) if(G[n]>=18) fprintf(f,"%zu %d\n",n,(int)G[n]); 
  int unk=0; for(size_t n=1;n<=32768;n++) if(G[n]<0) unk++; fclose(f); printf("n<=32768 with G>%d: %d\n",T,unk);
  // values grouped by level
  std::vector<std::vector<uint32_t>> byl(T+1);
  for(size_t n=1;n<=LIM;n++) if(G[n]>=0) byl[G[n]].push_back((uint32_t)n);
  std::map<int,long> gap; std::map<int,std::pair<int,int>> fl; int unc=0;
  FILE*d=fopen("D_def1_4100.txt","w");
  std::vector<uint8_t> seen; std::map<int,std::pair<double,int>> octmin; std::vector<int> gapbig;
  for(int m=2;m<=MMAX;m++){
    seen.assign(m,0); int left=m; int D=-1;
    for(int t=0;t<=T && left;t++){ for(uint32_t n:byl[t]){ uint32_t r=n%m; if(!seen[r]){seen[r]=1; if(--left==0){D=t;break;}} } }
    if(D<0){unc++; continue;}
    int Tm=0; while(N[Tm]<(u64)m) Tm++;
    gap[D-Tm]++; if(D-Tm>=2) gapbig.push_back(m);
    if(!fl.count(D)) fl[D]={m,m}; else fl[D].second=m;
    if(m<=4100) fprintf(d,"%d %d\n",m,D);
    int o=31-__builtin_clz(m); double rr=D/std::log2((double)m);
    if(!octmin.count(o)||rr<octmin[o].first) octmin[o]={rr,m};
    if((m&(m-1))==0) printf("D(2^%d)=%d\n",o,D);
  }
  fclose(d);
  printf("uncovered m (D>%d): %d\n",T,unc);
  for(auto&p:gap) printf("gap %d count %ld\n",p.first,p.second);
  printf("m with gap>=2:"); for(size_t i=0;i<gapbig.size()&&i<60;i++) printf(" %d",gapbig[i]); printf(" (total %zu)\n",gapbig.size());
  for(auto&p:fl) printf("D=%d first %d last %d\n",p.first,p.second.first,p.second.second);
  for(auto&p:octmin) printf("octave 2^%d min ratio %.4f at m=%d\n",p.first,p.second.first,p.second.second);
}
