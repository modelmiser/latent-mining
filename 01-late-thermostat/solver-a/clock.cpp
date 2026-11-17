// Exhaustive CLOCK feasibility: does an oblivious sequence (f_t) keep |x|<=B forever?
// Node = set S_t of physical states reachable at time t (bitset).  Edge = choice of f_t.
// BFS over reachable sets (only inclusion-minimal successors are kept: sound, since
// T subset T' and T' safe => T safe), then least-fixed-point of "dead".
// State = (x, signs of x_{t-d..t-1}, rest).  usage: clock d B [maxnodes]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int d, B, P3, NS, NW;
static inline int sg(int x){return (x>0)-(x<0);}
vector<int> nxt; // [state*3 + (u+1)] *3 -> successor ids, -1 none, -2 dead
int enc(int x,int sc,int r){return ((x+B)*P3+sc)*2+r;}
struct H{ size_t operator()(const pair<u64,u64>&p)const{return p.first^(p.second*0x9E3779B97F4A7C15ULL);} };
pair<u64,u64> hs(const u64*a){u64 h1=1469598103934665603ULL,h2=0x243F6A8885A308D3ULL;
  for(int i=0;i<NW;i++){h1=(h1^a[i])*0x100000001B3ULL; h1^=h1>>29; h2=(h2+a[i])*0xD6E8FEB86659FD93ULL; h2^=h2>>32;} return {h1,h2};}
int main(int argc,char**argv){
  d=atoi(argv[1]);B=atoi(argv[2]); size_t maxn= argc>3? atoll(argv[3]):30000000;
  P3=1;for(int i=0;i<d;i++)P3*=3; NS=(2*B+1)*P3*2; NW=(NS+63)/64;
  nxt.assign((size_t)NS*9,-1);
  vector<int> oldest(NS);
  for(int x=-B;x<=B;x++)for(int sc=0;sc<P3;sc++)for(int r=0;r<2;r++){
    int s=enc(x,sc,r); int old=sc/(P3/3); oldest[s]=old; // digit 0..2 = sign+1, most significant = oldest
    int nsc=(sc%(P3/3))*3+(sg(x)+1);
    for(int u=-1;u<=1;u++){int k=0;
      for(int w=0; w<3; w++){ int ww = w==0?0:(w==1?1:-1); if(r&&ww) continue;
        int nx=x+u+ww; if(abs(nx)>B){nxt[(size_t)s*9+(u+1)*3]=-2;k=3;break;}
        nxt[(size_t)s*9+(u+1)*3+k++]=enc(nx,nsc,ww!=0);}
    }}
  int zc=0;for(int i=0;i<d;i++)zc=zc*3+1;
  vector<u64> init(NW,0); {int s=enc(0,zc,0); init[s>>6]|=1ULL<<(s&63);}
  unordered_map<pair<u64,u64>,int,H> id; id.reserve(1<<22);
  vector<vector<int>> ch; vector<unsigned char> lab; // label of edge (f code) kept for first few? keep all: f code per child
  vector<vector<unsigned char>> chf;
  deque<pair<int,vector<u64>>> q;
  id[hs(init.data())]=0; ch.emplace_back(); chf.emplace_back(); q.push_back({0,init});
  vector<vector<u64>> cand; vector<int> candf;
  while(!q.empty()){
    int me=q.front().first; vector<u64> S=move(q.front().second); q.pop_front();
    vector<int> st; int obsmask=0;
    for(int w=0;w<NW;w++){u64 v=S[w];while(v){int b=__builtin_ctzll(v);v&=v-1;int s=w*64+b;st.push_back(s);obsmask|=1<<oldest[s];}}
    cand.clear();candf.clear();
    for(int fc=0;fc<27;fc++){int f[3]={fc%3-1,(fc/3)%3-1,(fc/9)%3-1}; bool skip=false;
      for(int o=0;o<3;o++) if(!(obsmask>>o&1) && f[o]!=0) skip=true; // canonical: absent obs -> 0
      if(skip)continue;
      vector<u64> T(NW,0); bool dead=false;
      for(int s:st){const int*p=&nxt[(size_t)s*9+(f[oldest[s]]+1)*3];
        if(p[0]==-2){dead=true;break;}
        for(int k=0;k<3;k++){if(p[k]<0)break;T[p[k]>>6]|=1ULL<<(p[k]&63);}}
      if(dead)continue;
      cand.push_back(move(T));candf.push_back(fc);}
    int m=cand.size(); vector<char> drop(m,0);
    for(int i=0;i<m;i++)if(!drop[i])for(int j=0;j<m;j++)if(i!=j&&!drop[j]){ // drop j if cand[i] subset cand[j]
      bool sub=true;for(int w=0;w<NW;w++)if(cand[i][w]&~cand[j][w]){sub=false;break;}
      if(sub&&(cand[i]!=cand[j]||i<j))drop[j]=1;}
    for(int i=0;i<m;i++)if(!drop[i]){auto h=hs(cand[i].data());auto it=id.find(h);int c;
      if(it==id.end()){c=ch.size();id[h]=c;ch.emplace_back();chf.emplace_back();q.push_back({c,move(cand[i])});
        if(ch.size()>maxn){printf("d=%d B=%d NODE LIMIT %zu exceeded (frontier %zu) -- UNDECIDED\n",d,B,maxn,q.size());return 2;}}
      else c=it->second;
      ch[me].push_back(c);chf[me].push_back(candf[i]);}
  }
  size_t n=ch.size(); vector<char> deadn(n,0); vector<int> cnt(n); vector<vector<int>> pr(n);
  vector<int> wk;
  for(size_t i=0;i<n;i++){cnt[i]=ch[i].size();for(int c:ch[i])pr[c].push_back(i);if(!cnt[i]){deadn[i]=1;wk.push_back(i);}}
  while(!wk.empty()){int v=wk.back();wk.pop_back();for(int p:pr[v])if(!deadn[p]&&--cnt[p]==0){deadn[p]=1;wk.push_back(p);}}
  size_t nd=0;for(size_t i=0;i<n;i++)nd+=deadn[i];
  printf("d=%d B=%d sets=%zu dead=%zu  => %s\n",d,B,n,nd,deadn[0]?"IMPOSSIBLE":"SAFE CLOCK EXISTS");
  if(!deadn[0]){ // print a witness walk until repeat
    vector<int> seen(n,-1); int v=0; vector<int> seq; int t=0;
    while(seen[v]<0){seen[v]=t++; for(size_t k=0;k<ch[v].size();k++)if(!deadn[ch[v][k]]){seq.push_back(chf[v][k]);v=ch[v][k];break;}}
    printf("witness (f(-),f(0),f(+)) prefix_len=%d cycle_len=%d:\n",seen[v],t-seen[v]);
    for(int i=0;i<t;i++){int fc=seq[i];printf("%s(%d,%d,%d)",i==seen[v]?" | ":" ",fc%3-1,(fc/3)%3-1,(fc/9)%3-1);}
    printf("\n");}
  return 0;}
