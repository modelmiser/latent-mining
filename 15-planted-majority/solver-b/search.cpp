// Lazy DFS search for protocols. Usage: search s N mode [maxsol]
// mode 0 = reduced (one outcome per unordered pair, no swaps), mode 1 = full ordered d (definition as written)
// States: 0=qA, 1=qB. Output map w enumerated (bitmask, bit=1 means output B).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int S,NN,MODE; int d[8][8]; // d[p][q] = p'*S+q' or -1
int wmask; long long nodes=0, sols=0, maxsol=1;
static inline int cnt(u64 c,int i){return (c>>(4*i))&15;}
// returns 0 pass, 1 fail, 2 need (np,nq)
int np_,nq_;
int checkInst(int a,int b,int x){
  int maj = a>b?0:1;
  u64 init = ((u64)a) + ((u64)b<<4); init += (1ULL<<(4*x));
  unordered_map<u64,int> id; vector<u64> cf; vector<vector<int>> adj; vector<char> selfl, term, undef;
  id[init]=0; cf.push_back(init);
  bool need=false;
  for(size_t i=0;i<cf.size();i++){
    u64 c=cf[i]; adj.emplace_back(); selfl.push_back(0); term.push_back(1); undef.push_back(0);
    for(int p=0;p<S;p++){ int cp=cnt(c,p); if(!cp) continue;
      for(int q=0;q<S;q++){ int cq=cnt(c,q); if(!cq) continue; if(p==q && cp<2) continue;
        int r=d[p][q];
        if(r<0){ undef[i]=1; term[i]=0; if(!need){need=true;np_=p;nq_=q;} continue; }
        int p2=r/S,q2=r%S; if(p2==p&&q2==q) continue;
        term[i]=0;
        u64 c2=c-(1ULL<<(4*p))-(1ULL<<(4*q))+(1ULL<<(4*p2))+(1ULL<<(4*q2));
        if(c2==c){selfl[i]=1;continue;}
        auto it=id.find(c2); int j;
        if(it==id.end()){ j=cf.size(); id[c2]=j; cf.push_back(c2);} else j=it->second;
        adj[i].push_back(j);
      }}
  }
  int n=cf.size(); vector<char> good(n);
  for(int i=0;i<n;i++){ bool g=true; for(int p=0;p<S;p++) if(cnt(cf[i],p) && ((wmask>>p)&1)!=maj) g=false; good[i]=g;
    if(!g && (term[i]||selfl[i])) return 1; }
  // Tarjan SCC iterative
  vector<int> idx(n,-1),low(n,0),comp(n,-1),st; vector<char> on(n,0); int ctr=0,nc=0;
  vector<pair<int,int>> cs;
  for(int s0=0;s0<n;s0++){ if(idx[s0]>=0) continue;
    cs.push_back({s0,0}); idx[s0]=low[s0]=ctr++; st.push_back(s0); on[s0]=1;
    while(!cs.empty()){ int v=cs.back().first; int &k=cs.back().second;
      if(k<(int)adj[v].size()){ int u=adj[v][k++];
        if(idx[u]<0){ idx[u]=low[u]=ctr++; st.push_back(u); on[u]=1; cs.push_back({u,0}); }
        else if(on[u]) low[v]=min(low[v],idx[u]);
      } else {
        if(low[v]==idx[v]){ int sz=0; bool bad=false; while(true){int u=st.back(); st.pop_back(); on[u]=0; comp[u]=nc; sz++; if(!good[u]) bad=true; if(u==v)break;} nc++; if(sz>1&&bad) return 1; }
        cs.pop_back(); if(!cs.empty()){ int pv=cs.back().first; low[pv]=min(low[pv],low[v]); }
      }
    }
  }
  return need?2:0;
}
vector<array<int,3>> insts;
void report(){
  sols++;
  if(sols<=maxsol){ printf("SOL s=%d N=%d w=",S,NN); for(int p=0;p<S;p++) putchar(((wmask>>p)&1)?'B':'A'); printf(" d:");
    for(int p=0;p<S;p++)for(int q=0;q<S;q++){ int r=d[p][q]; if(r<0) printf(" %d%d>**",p,q); else printf(" %d%d>%d%d",p,q,r/S,r%S);} printf("\n"); fflush(stdout);}
}
bool dfs(size_t from){
  nodes++;
  for(size_t i=from;i<insts.size();i++){
    int r=checkInst(insts[i][0],insts[i][1],insts[i][2]);
    if(r==1) return false;
    if(r==2){ int p=np_,q=nq_; bool any=false;
      for(int p2=0;p2<S;p2++)for(int q2=0;q2<S;q2++){
        if(MODE==0 && p2>q2) continue;
        d[p][q]=p2*S+q2; if(dfs(i)){any=true; if(sols>=maxsol){d[p][q]=-1;return true;}}
      }
      d[p][q]=-1; return any; }
  }
  report(); return true;
}
int main(int argc,char**argv){
  S=atoi(argv[1]); NN=atoi(argv[2]); MODE=atoi(argv[3]); if(argc>4) maxsol=atoll(argv[4]);
  int wfix = argc>5? atoi(argv[5]) : -1;
  for(int n=2;n<=NN;n++) for(int a=0;a<=n;a++){ int b=n-a; if(abs(a-b)<2) continue; for(int x=0;x<S;x++) insts.push_back({a,b,x}); }
  bool found=false;
  for(wmask=0; wmask<(1<<S); wmask++){
    if(wfix>=0 && wmask!=wfix) continue;
    for(int p=0;p<S;p++)for(int q=0;q<S;q++) d[p][q]= (MODE==0 && p>q)? p*S+q : -1;
    long long s0=sols; dfs(0); if(sols>s0) found=true;
    fprintf(stderr,"w=%d done nodes=%lld sols=%lld\n",wmask,nodes,sols);
    if(sols>=maxsol) break;
  }
  printf("RESULT s=%d N=%d mode=%d found=%d sols(partial-assignments)=%lld nodes=%lld\n",S,NN,MODE,found?1:0,sols,nodes);
}
