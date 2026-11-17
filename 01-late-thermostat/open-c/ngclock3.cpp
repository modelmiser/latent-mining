// ngclock: exact CLOCK feasibility by DFS over reachable-state-sets with LEARNED NOGOODS.
//
// Game: x_{t+1}=x_t+u_t+w_t, u_t=f_t(sgn x_{t-d}), w in {-1,0,1}, rest rule, band |x|<=B.
// Physical state s=(x, signs of x_{t-d..t-1} [oldest most significant], rest).
// A set S of states is DEAD if no clock sequence keeps every play starting in S inside the band forever.
// Monotone: a superset of a dead set is dead.  The dynamics are time-invariant, so deadness of a
// set does not depend on t.
// Normalisation ("closure"): if (x,win,free) is in S we also put (x,win,rest) in S.  Exact: every
// continuation available from the rest-state is available from the free-state.
// NOGOOD K: a set of states proved dead.  Proof obligation for K (checked independently by
// ngcheck.cpp from the emitted certificate): for EVERY map f, cstep(K,f) either leaves the band
// or contains an EARLIER nogood.  By induction on the order every nogood is dead:
// any clock's first map f sends K out of band, or onto a superset of a dead set.
// INFEASIBLE verdict: the initial set contains a nogood.
// FEASIBLE verdict: DFS reaches a set equal to one on its own stack => periodic safe clock (printed;
// verify independently with verify_clock.py).
//
// ngclock2 = ngclock + optional FILTER: states whose sign window has more than KR runs are dropped
// (adversary forfeits those plays).  Sound for INFEASIBLE (filtered image is a subset of the true image, so
// the emitted certificate still passes the UNFILTERED checker).  FEASIBLE verdicts under a filter mean nothing.
// usage: ngclock2 d B [certfile|-] [maxnodes] [KR]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int d,B,P3,NS,KR=1000; vector<char> keepw;
static inline int sg(int x){return (x>0)-(x<0);}
static inline int enc(int x,int sc,int r){return ((x+B)*P3+sc)*2+r;}
static inline int xOf(int s){return (s>>1)/P3-B;}
static inline int oOf(int s){return ((s>>1)%P3)/(P3/3);}   // 0,1,2 = -,0,+
vector<int> stampv; int stampc=0;          // membership stamps
vector<int> predv;                         // predecessor of state in last cstep
// closed step. returns false on band violation (viol = offending state). T sorted unique.
bool cstep(const vector<int>&S,const int f[3],vector<int>&T,int&viol,bool wantpred){
  T.clear(); ++stampc;
  for(int s:S){int r=s&1;int q=s>>1;int sc=q%P3;int x=q/P3-B;int o=sc/(P3/3);int u=f[o];
    int nsc=(sc%(P3/3))*3+(sg(x)+1);
    for(int w=-1;w<=1;w++){if(r&&w)continue;int nx=x+u+w;if(nx>B||nx<-B){viol=s;return false;}
      if(!keepw[nsc])continue; int n=enc(nx,nsc,w!=0);
      if(stampv[n]!=stampc){stampv[n]=stampc;T.push_back(n);if(wantpred)predv[n]=s;}
      if(w==0){int n2=n|1; if(stampv[n2]!=stampc){stampv[n2]=stampc;T.push_back(n2);if(wantpred)predv[n2]=s;}}
    }}
  sort(T.begin(),T.end());return true;
}
// nogood DB
vector<vector<int>> NG; vector<vector<int>> bucket; // bucket[e] = nogood ids keyed at element e
vector<int> mark; int markc=0;
int queryNG(const vector<int>&T){ // returns id of a nogood contained in T, or -1
  ++markc; for(int e:T)mark[e]=markc;
  for(int e:T){for(int id:bucket[e]){const vector<int>&K=NG[id];bool ok=true;for(int k:K)if(mark[k]!=markc){ok=false;break;}if(ok)return id;}}
  return -1;
}
FILE*cert=nullptr;
int addNG(const vector<int>&K){
  int id=NG.size();NG.push_back(K);
  int best=K[0];for(int k:K)if(bucket[k].size()<bucket[best].size())best=k;
  bucket[best].push_back(id);
  if(cert){fprintf(cert,"%zu",K.size());for(int k:K)fprintf(cert," %d",k);fputc('\n',cert);}
  return id;
}
struct HH{size_t operator()(const pair<u64,u64>&p)const{return p.first;}};
pair<u64,u64> hs(const vector<int>&a){u64 h1=1469598103934665603ULL,h2=0x243F6A8885A308D3ULL;
  for(int v:a){h1=(h1^(u64)v)*0x100000001B3ULL;h1^=h1>>29;h2=(h2+(u64)v+1)*0xD6E8FEB86659FD93ULL;h2^=h2>>32;}return {h1,h2};}
unordered_map<pair<u64,u64>,int,HH> onstack; vector<int> pathf;
size_t nodes=0,maxn=(size_t)4e9,ngsizeSum=0; int maxdepth=0; bool feasible=false; int cyc_from=-1;
vector<int> inC; int inCc=0;
// returns nogood id contained in S (S is dead), or -1 if feasible found
int dfs(vector<int>&S,int depth){
  nodes++; if(depth>maxdepth)maxdepth=depth;
  if((nodes&((1<<16)-1))==0)fprintf(stderr,"nodes=%zu nogoods=%zu avgK=%.1f depth=%d |S|=%zu\n",nodes,NG.size(),NG.empty()?0.0:(double)ngsizeSum/NG.size(),depth,S.size());
  if(nodes>maxn){fprintf(stderr,"NODE LIMIT\n");printf("d=%d B=%d UNDECIDED (node limit %zu)\n",d,B,maxn);exit(2);}
  int present=0;for(int s:S)present|=1<<oOf(s);
  vector<int> C; // core under construction (subset of S)
  vector<int> T,K,TC;
  // two passes: pass 0 handles maps refuted by immediate violation or by a known nogood; pass 1 recurses.
  vector<int> todo;
  for(int pass=0;pass<2;pass++){
    vector<int> list; if(pass==0){for(int fc=0;fc<27;fc++){int f[3]={fc%3-1,(fc/3)%3-1,(fc/9)%3-1};bool skip=false;for(int o=0;o<3;o++)if(!(present>>o&1)&&f[o]!=0)skip=true;if(!skip)list.push_back(fc);}}
    else list=todo;
    for(int fc:list){
      int f[3]={fc%3-1,(fc/3)%3-1,(fc/9)%3-1};int viol;
      // already refuted by current core?
      if(!C.empty()){ if(!cstep(C,f,TC,viol,false))continue; if(queryNG(TC)>=0)continue; }
      if(!cstep(S,f,T,viol,true)){ C.push_back(viol); sort(C.begin(),C.end()); continue; }
      // (ngclock3: removed a dead O(NS) copy of predv that ngclock/ngclock2 made here; logic unchanged)
      // (copying whole predv is O(NS); instead collect preds for T now)
      vector<int> predT(T.size()); for(size_t i=0;i<T.size();i++)predT[i]=predv[T[i]];
      int id=queryNG(T);
      if(id<0){
        if(pass==0){todo.push_back(fc);continue;}
        auto h=hs(T);
        auto it=onstack.find(h);
        if(it!=onstack.end()){feasible=true;cyc_from=it->second;pathf.push_back(fc);return -1;}
        onstack[h]=depth+1; pathf.push_back(fc);
        vector<int> T2(T);
        id=dfs(T2,depth+1);
        if(id<0)return -1;
        pathf.pop_back(); onstack.erase(h);
      }
      // add predecessors of NG[id] (within S) to C, preferring states already in C
      const vector<int>&KK=NG[id];
      ++inCc; for(int c:C)inC[c]=inCc;
      // recompute step from S with C-members first so preds prefer C
      {
        ++stampc; // manual pred pass
        auto visit=[&](int s){int r=s&1;int q=s>>1;int sc=q%P3;int x=q/P3-B;int o=sc/(P3/3);int u=f[o];int nsc=(sc%(P3/3))*3+(sg(x)+1);
          for(int w=-1;w<=1;w++){if(r&&w)continue;if(!keepw[nsc])continue;int nx=x+u+w;int n=enc(nx,nsc,w!=0);
            if(stampv[n]!=stampc){stampv[n]=stampc;predv[n]=s;}
            if(w==0){int n2=n|1;if(stampv[n2]!=stampc){stampv[n2]=stampc;predv[n2]=s;}}}};
        for(int c:C)visit(c);
        for(int s:S)if(inC[s]!=inCc)visit(s);
      }
      bool changed=false;
      for(int k:KK){int p=predv[k]; if(inC[p]!=inCc){inC[p]=inCc;C.push_back(p);changed=true;}}
      if(changed)sort(C.begin(),C.end());
    }
  }
  if(C.empty()){fprintf(stderr,"BUG: empty core\n");exit(3);}
  ngsizeSum+=C.size();
  return addNG(C);
}
int main(int argc,char**argv){
  d=atoi(argv[1]);B=atoi(argv[2]); if(argc>3&&strcmp(argv[3],"-"))cert=fopen(argv[3],"w"); if(argc>4)maxn=atoll(argv[4]);
  if(argc>5)KR=atoi(argv[5]);
  P3=1;for(int i=0;i<d;i++)P3*=3; NS=(2*B+1)*P3*2;
  keepw.assign(P3,1); for(int sc=0;sc<P3;sc++){int v=sc,prev=-1,runs=0;for(int i=0;i<d;i++){int g=v%3;v/=3;if(g!=prev){runs++;prev=g;}}keepw[sc]=runs<=KR;}
  stampv.assign(NS,0);predv.assign(NS,-1);mark.assign(NS,0);inC.assign(NS,0);bucket.assign(NS,{});
  if(cert)fprintf(cert,"d %d B %d\n",d,B);
  int zc=0;for(int i=0;i<d;i++)zc=zc*3+1;
  vector<int> S0={enc(0,zc,0),enc(0,zc,1)};
  onstack[hs(S0)]=0;
  int id=dfs(S0,0);
  if(feasible){
    printf("KR=%d ",KR);printf("d=%d B=%d FEASIBLE nodes=%zu nogoods=%zu; clock (f(-)f(0)f(+)), prefix then cycle from index %d:\n",d,B,nodes,NG.size(),cyc_from);
    for(size_t i=0;i<pathf.size();i++){int g=pathf[i];if((int)i==cyc_from)printf(" |");printf(" %c%c%c","-0+"[g%3],"-0+"[(g/3)%3],"-0+"[(g/9)%3]);}
    printf("\n");return 0;}
  printf("KR=%d ",KR);printf("d=%d B=%d INFEASIBLE nodes=%zu nogoods=%zu avg|K|=%.1f max depth=%d (root nogood id %d, size %zu)\n",d,B,nodes,NG.size(),(double)ngsizeSum/NG.size(),maxdepth,id,NG[id].size());
  if(cert)fclose(cert);
  return 0;}
