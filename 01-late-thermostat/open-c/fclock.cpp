// Filtered CLOCK feasibility search.
// Sound for INFEASIBILITY: states failing the filter are dropped from the reachable set
// (the adversary forfeits those plays); a subset of a winnable set is winnable, so if the
// filtered game has no infinite safe clock walk, neither has the real game.
// With filter=none (K>=d) it is the exact game, so FEASIBLE is then also exact.
// State = (x, signs of x_{t-d..t-1} [oldest = most significant], rest).
// usage: fclock d B K [maxnodes] [mode]
//   K = max number of runs allowed in the sign window (K>=d : no filter)
//   mode 0: runs count distinct-adjacent digits; mode 1: 0 merged with previous nonzero (coarse)
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
typedef vector<int> Set;
int d,B,K,P3,mode; 
static inline int sg(int x){return (x>0)-(x<0);}
static inline int enc(int x,int sc,int r){return ((x+B)*P3+sc)*2+r;}
vector<char> keepw; // per window
struct HH{size_t operator()(const pair<u64,u64>&p)const{return p.first;}};
pair<u64,u64> hs(const Set&a){u64 h1=1469598103934665603ULL,h2=0x243F6A8885A308D3ULL;
  for(int v:a){h1=(h1^(u64)v)*0x100000001B3ULL;h1^=h1>>29;h2=(h2+(u64)v+1)*0xD6E8FEB86659FD93ULL;h2^=h2>>32;}return {h1,h2};}
unordered_set<pair<u64,u64>,HH> dead;
size_t nodes=0,maxn; int maxdepth=0;
// returns false if unsafe
bool step(const Set&S,const int f[3],Set&T){
  T.clear();
  for(int s:S){int r=s&1; int q=s>>1; int sc=q%P3; int x=q/P3-B; int o=sc/(P3/3);
    int u=f[o]; int nsc=(sc%(P3/3))*3+(sg(x)+1);
    for(int w=-1;w<=1;w++){ if(r&&w)continue; int nx=x+u+w; if(nx>B||nx<-B)return false;
      if(keepw[nsc]) T.push_back(enc(nx,nsc,w!=0)); }
  }
  sort(T.begin(),T.end());T.erase(unique(T.begin(),T.end()),T.end());return true;
}
struct Fr{Set S; vector<pair<Set,int>> ch; size_t i; int f;};
int main(int argc,char**argv){
  d=atoi(argv[1]);B=atoi(argv[2]);K=atoi(argv[3]);maxn=argc>4?atoll(argv[4]):50000000;mode=argc>5?atoi(argv[5]):0;
  P3=1;for(int i=0;i<d;i++)P3*=3;
  keepw.assign(P3,1);
  for(int sc=0;sc<P3;sc++){int dg[32];int v=sc;for(int i=d-1;i>=0;i--){dg[i]=v%3;v/=3;}
    int runs=1;
    if(mode==0){for(int i=1;i<d;i++)if(dg[i]!=dg[i-1])runs++;}
    else {int last=dg[0];for(int i=1;i<d;i++){if(dg[i]==1)continue; if(last==1){last=dg[i];continue;} if(dg[i]!=last){runs++;last=dg[i];}}}
    keepw[sc]=runs<=K;}
  int zc=0;for(int i=0;i<d;i++)zc=zc*3+1;
  vector<Fr> st; map<pair<u64,u64>,int> onst;
  auto expand=[&](Fr&fr){
    int obsmask=0;for(int s:fr.S){obsmask|=1<<(((s>>1)%P3)/(P3/3));}
    vector<pair<Set,int>> c;
    for(int fc=0;fc<27;fc++){int f[3]={fc%3-1,(fc/3)%3-1,(fc/9)%3-1};bool skip=false;
      for(int o=0;o<3;o++)if(!(obsmask>>o&1)&&f[o]!=0)skip=true; if(skip)continue;
      Set T; if(!step(fr.S,f,T))continue; c.push_back({move(T),fc});}
    int m=c.size();vector<char> drop(m,0);
    for(int i=0;i<m;i++)if(!drop[i])for(int j=0;j<m;j++)if(i!=j&&!drop[j]){
      if(includes(c[j].first.begin(),c[j].first.end(),c[i].first.begin(),c[i].first.end())&&(c[i].first!=c[j].first||i<j))drop[j]=1;}
    for(int i=0;i<m;i++)if(!drop[i])fr.ch.push_back(move(c[i]));
    sort(fr.ch.begin(),fr.ch.end(),[](const pair<Set,int>&a,const pair<Set,int>&b){return a.first.size()<b.first.size();});
    fr.i=0;};
  Fr r0; r0.S={enc(0,zc,0)}; r0.f=-1; expand(r0); onst[hs(r0.S)]=0; st.push_back(move(r0)); nodes=1;
  while(!st.empty()){
    Fr&top=st.back();
    if(top.i>=top.ch.size()){auto h=hs(top.S);dead.insert(h);onst.erase(h);st.pop_back();continue;}
    Set T=move(top.ch[top.i].first);int fc=top.ch[top.i].second;top.i++;
    auto h=hs(T); if(dead.count(h))continue;
    bool cyc=onst.count(h);
    if(!cyc && T.empty()) cyc=true; // empty set: trivially safe forever (filter forfeited everything)
    if(cyc){
      printf("d=%d B=%d K=%d mode=%d FEASIBLE(in filtered game) nodes=%zu depth=%zu\n seq:",d,B,K,mode,nodes,st.size());
      for(size_t i=1;i<st.size();i++){int g=st[i].f;printf(" %c%c%c","-0+"[g%3],"-0+"[(g/3)%3],"-0+"[(g/9)%3]);}
      printf(" %c%c%c",  "-0+"[fc%3],"-0+"[(fc/3)%3],"-0+"[(fc/9)%3]);
      if(T.empty())printf("  [-> empty set]\n"); else printf("  [-> repeats stack index %d]\n",onst[h]);
      return 0;}
    if(++nodes>maxn){printf("d=%d B=%d K=%d NODE LIMIT -- UNDECIDED\n",d,B,K);return 2;}
    if((nodes&((1<<20)-1))==0){fprintf(stderr,"nodes=%zu dead=%zu depth=%zu setsize=%zu\n",nodes,dead.size(),st.size(),T.size());}
    Fr fr;fr.S=move(T);fr.f=fc;expand(fr);onst[h]=st.size();st.push_back(move(fr));
    if((int)st.size()>maxdepth)maxdepth=st.size();
  }
  printf("d=%d B=%d K=%d mode=%d INFEASIBLE nodes=%zu maxdepth=%d\n",d,B,K,mode,nodes,maxdepth);
  return 0;}
