// CEGAR v2 (incremental, no restarts) for the clock game.
// Finds a finite set W of open-loop adversary plays such that EVERY clock f_0..f_{T-1}
// is beaten (|x|>B at some time <=T) by a play in W.  Exhausted controller DFS => INFEASIBLE.
// The DFS is a plain exhaustive backtracking over clocks; plays are only ever ADDED, and a
// subtree closed under fewer plays stays closed under more, so no restart is needed.
// Dead memo: key (k, t, states of plays 0..k-1): dead against the first k plays => dead.
// usage: cegar2 d B T [sym=1]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int d,B,T,SYM=1;
static inline int sg(int x){return (x>0)-(x<0);}
vector<vector<signed char>> W; vector<vector<int>> X; int F[256];
unordered_set<u64> deadmemo; size_t dfsnodes=0,memohits=0;
static inline u64 mix(u64 h,u64 v){h=(h^v)*0x100000001B3ULL;return h^(h>>29);}
void prefHashes(int t,vector<u64>&out){ // out[k] = hash of (t, plays 0..k-1), k=1..|W|
  u64 h=0x9E3779B97F4A7C15ULL*(t+1); out.resize(W.size()+1);
  for(size_t i=0;i<W.size();i++){u64 g=X[i][t]+64; for(int s=max(0,t-d);s<t;s++)g=g*3+(sg(X[i][s])+1); h=mix(h,g); out[i+1]=mix(h,i+1);}
}
struct Node{int par;signed char w;};
bool oracle(vector<signed char>&play,int&vt){
  typedef tuple<int,vector<signed char>,int> St;
  vector<map<St,int>> layer(T+1); vector<vector<Node>> nodes(T+1); vector<vector<St>> sts(T+1);
  St s0{0,vector<signed char>(d,0),0}; layer[0][s0]=0;nodes[0].push_back({-1,0});sts[0].push_back(s0);
  for(int t=0;t<T;t++){
    size_t n0=sts[t].size();
    for(size_t ii=0;ii<n0;ii++){St s=sts[t][ii];int x=get<0>(s);auto&sgv=get<1>(s);int r=get<2>(s);
      int o=sgv[0];int u=(o==-1?F[t]%3:(o==0?(F[t]/3)%3:(F[t]/9)%3))-1;
      vector<signed char> ns(sgv.begin()+1,sgv.end());ns.push_back(sg(x));
      int ws[3]={0,1,-1};
      for(int wi=0;wi<3;wi++){int w=ws[wi];if(r&&w)continue;int nx=x+u+w;
        if(nx>B||nx<-B){play.assign(T,0);play[t]=w;int cur=ii;for(int tt=t;tt>0;tt--){play[tt-1]=nodes[tt][cur].w;cur=nodes[tt][cur].par;}
          vt=t+1;return true;}
        St n{nx,ns,w!=0};
        if(!layer[t+1].count(n)){layer[t+1][n]=sts[t+1].size();sts[t+1].push_back(n);nodes[t+1].push_back({(int)ii,(signed char)w});}
      }}
  }
  return false;
}
bool survivor=false;
// advance play i along current F for ticks [0,upto); return first violation time or INT_MAX
int replay(int i,int upto){X[i][0]=0;for(int t=0;t<upto;t++){int o=t<d?0:sg(X[i][t-d]);int u=(o==-1?F[t]%3:(o==0?(F[t]/3)%3:(F[t]/9)%3))-1;int nx=X[i][t]+u+W[i][t];X[i][t+1]=nx;if(nx>B||nx<-B)return t+1;}return INT_MAX;}
// returns INT_MAX if subtree dead normally; returns v<=t meaning "a newly added play kills the current path at time v" (unwind to level v-1)
int dfs(int t){
  if(t==T){
    vector<signed char> play;int vt;
    if(!oracle(play,vt)){survivor=true;return -1;}
    int best=vt;
    W.push_back(play);X.emplace_back(T+2,0);replay(W.size()-1,vt);
    if(SYM){vector<signed char> q=play;for(auto&c:q)c=-c;W.push_back(q);X.emplace_back(T+2,0);int v2=replay(W.size()-1,T);if(v2<best)best=v2;}
    if(W.size()%100<2)fprintf(stderr,"|W|=%zu dfsnodes=%zu memo=%zu hits=%zu vt=%d\n",W.size(),dfsnodes,deadmemo.size(),memohits,vt);
    return best;
  }
  vector<u64> ph; prefHashes(t,ph);
  for(size_t k=1;k<ph.size();k++) if(deadmemo.count(ph[k])){memohits++;return INT_MAX;}
  dfsnodes++;
  static const int order[3]={0,1,-1};
  for(int a=0;a<3;a++)for(int b=0;b<3;b++)for(int c=0;c<3;c++){
    int f[3]={order[a],order[b],order[c]};
    while(true){ // retry same f after plays were added below (only if still alive at this level)
      int obsmask=0; for(size_t i=0;i<W.size();i++){int o=t<d?0:sg(X[i][t-d]);obsmask|=1<<(o+1);}
      if(W.empty())obsmask=7;
      bool skip=false;for(int o=0;o<3;o++)if(!(obsmask>>o&1)&&f[o]!=0)skip=true; if(skip)break;
      bool ok=true;
      for(size_t i=0;i<W.size();i++){int o=t<d?0:sg(X[i][t-d]);int nx=X[i][t]+f[o+1]+W[i][t];X[i][t+1]=nx;if(nx>B||nx<-B){ok=false;break;}}
      if(!ok)break;
      F[t]=(f[0]+1)+3*(f[1]+1)+9*(f[2]+1);
      int r=dfs(t+1);
      if(r==-1)return -1;
      if(r==INT_MAX)break;          // subtree dead
      if(r<=t)return r;             // new play kills path earlier than this level: unwind
      if(r==t+1)break;              // this f is killed by the new play
      // r>t+1: new play kills deeper; this f at level t still alive: retry
    }
  }
  // dead against current W
  prefHashes(t,ph); if(ph.size()>1)deadmemo.insert(ph.back());
  return INT_MAX;
}
int main(int argc,char**argv){
  d=atoi(argv[1]);B=atoi(argv[2]);T=atoi(argv[3]);if(argc>4)SYM=atoi(argv[4]);
  int r=dfs(0);
  if(survivor){printf("d=%d B=%d T=%d: a clock survives T ticks against ALL adversaries (|W|=%zu):",d,B,T,W.size());
    for(int t=0;t<T;t++)printf(" %c%c%c","-0+"[F[t]%3],"-0+"[(F[t]/3)%3],"-0+"[(F[t]/9)%3]);printf("\n");return 1;}
  if(r!=INT_MAX){printf("BUG r=%d\n",r);return 3;}
  printf("d=%d B=%d T=%d INFEASIBLE: every clock beaten by one of |W|=%zu plays (DFS nodes=%zu memo=%zu)\n",d,B,T,W.size(),dfsnodes,deadmemo.size());
  for(auto&p:W){for(int t=0;t<T;t++)putchar("-0+"[p[t]+1]);putchar('\n');}
  return 0;}
