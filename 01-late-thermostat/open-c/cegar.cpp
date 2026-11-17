// CEGAR for the clock game: find a finite set W of open-loop adversary plays (w-sequences)
// such that EVERY clock f_0..f_{T-1} is beaten (|x|>B at some time <=T) by a play in W.
// If the controller DFS over clocks against W is exhausted => band B infeasible (proof = W).
// usage: cegar d B T [seed]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int d,B,T;
static inline int sg(int x){return (x>0)-(x<0);}
vector<vector<signed char>> W;          // plays
// controller DFS state: per play, x history (we keep full x arrays per depth)
vector<vector<int>> X;                  // X[i][t]
int F[256];
unordered_set<u64> deadmemo; size_t dfsnodes=0;
u64 keyOf(int t){ // hash of (t, per play: x_t and signs of x_{t-d..t-1}, ) plays are distinct plans so order matters
  u64 h=1469598103934665603ULL^(u64)t*0x9E3779B97F4A7C15ULL;
  for(size_t i=0;i<W.size();i++){ for(int s=max(0,t-d);s<t;s++){h=(h^(u64)(sg(X[i][s])+1))*0x100000001B3ULL;} h=(h^(u64)(X[i][t]+64))*0x100000001B3ULL; h^=h>>31;}
  return h;}
bool dfs(int t){
  if(t==T)return true;
  u64 k=keyOf(t); if(deadmemo.count(k))return false; dfsnodes++;
  int obsmask=0; for(size_t i=0;i<W.size();i++){int o=t<d?0:sg(X[i][t-d]);obsmask|=1<<(o+1);}
  static const int order[3]={0,1,-1};
  for(int a=0;a<3;a++)for(int b=0;b<3;b++)for(int c=0;c<3;c++){
    int f[3]={order[a],order[b],order[c]}; // f[o+1]
    // natural bias: try f(-)>=0, f(+)<=0 first is implicit in order; canonical: unobserved -> 0
    bool skip=false;for(int o=0;o<3;o++)if(!(obsmask>>o&1)&&f[o]!=0)skip=true; if(skip)continue;
    bool ok=true;
    for(size_t i=0;i<W.size()&&ok;i++){int o=t<d?0:sg(X[i][t-d]);int nx=X[i][t]+f[o+1]+W[i][t];X[i][t+1]=nx;if(nx>B||nx<-B)ok=false;}
    if(!ok)continue;
    F[t]=(f[0]+1)+3*(f[1]+1)+9*(f[2]+1);
    if(dfs(t+1))return true;
  }
  deadmemo.insert(k);return false;
}
// oracle: given clock F[0..T-1], find a violating play. exact forward DP on (x, window, rest) with parents.
struct Node{int par;signed char w;};
bool oracle(vector<signed char>&play,int&vt,mt19937&rng){
  // state key -> index at each layer
  typedef tuple<int,vector<signed char>,int> St; // x, last d signs, rest
  vector<map<St,int>> layer(T+1); vector<vector<Node>> nodes(T+1); vector<vector<St>> sts(T+1);
  St s0{0,vector<signed char>(d,0),0}; layer[0][s0]=0;nodes[0].push_back({-1,0});sts[0].push_back(s0);
  for(int t=0;t<T;t++){
    vector<int> idx(sts[t].size());iota(idx.begin(),idx.end(),0);
    for(int ii:idx){St s=sts[t][ii];int x=get<0>(s);auto&sgv=get<1>(s);int r=get<2>(s);
      int o=sgv[0];int u=(o==-1?F[t]%3:(o==0?(F[t]/3)%3:(F[t]/9)%3))-1;
      vector<signed char> ns(sgv.begin()+1,sgv.end());ns.push_back(sg(x));
      int ws[3]={0,1,-1};
      for(int wi=0;wi<3;wi++){int w=ws[wi];if(r&&w)continue;int nx=x+u+w;
        if(nx>B||nx<-B){ // violation: reconstruct
          play.assign(T,0);play[t]=w;int cur=ii;for(int tt=t;tt>0;tt--){play[tt-1]=nodes[tt][cur].w;cur=nodes[tt][cur].par;}
          vt=t+1;return true;}
        St n{nx,ns,w!=0};
        if(!layer[t+1].count(n)){layer[t+1][n]=sts[t+1].size();sts[t+1].push_back(n);nodes[t+1].push_back({ii,(signed char)w});}
      }}
  }
  return false;
}
int main(int argc,char**argv){
  d=atoi(argv[1]);B=atoi(argv[2]);T=atoi(argv[3]);mt19937 rng(argc>4?atoi(argv[4]):1);
  int it=0;
  while(true){
    X.assign(W.size(),vector<int>(T+2,0)); deadmemo.clear(); dfsnodes=0;
    bool surv=dfs(0);
    if(!surv){printf("d=%d B=%d T=%d INFEASIBLE: every clock beaten by one of |W|=%zu plays (last DFS nodes=%zu)\n",d,B,T,W.size(),dfsnodes);
      for(auto&p:W){for(int t=0;t<T;t++)putchar("-0+"[p[t]+1]);putchar('\n');}
      return 0;}
    vector<signed char> play;int vt;
    if(!oracle(play,vt,rng)){printf("d=%d B=%d T=%d: a clock survives T ticks against ALL adversaries (|W|=%zu):",d,B,T,W.size());
      for(int t=0;t<T;t++)printf(" %c%c%c","-0+"[F[t]%3],"-0+"[(F[t]/3)%3],"-0+"[(F[t]/9)%3]);printf("\n");return 1;}
    W.push_back(play); it++;
    if(it%50==0)fprintf(stderr,"it=%d |W|=%zu dfsnodes=%zu vt=%d\n",it,W.size(),dfsnodes,vt);
  }
}
