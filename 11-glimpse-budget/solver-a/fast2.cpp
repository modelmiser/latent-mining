// Belief-set solver for the glimpse cop/robber game on the complete binary tree T_h.
// usage: fast G h k [dump]      G in {E,D}
//  State at the start of a round: (j glimpses left, cop vertex c, set S of possible robber vertices).
//  Blind round:  cop -> c' in N[c];  S1 = S \ {c'};  win if S1 empty;  S' = N[S1] \ {c'}.
//  Glimpse round (j>0): adversary picks answer A (E: a singleton {v}, v in S; D: S cap branch(c,u) nonempty),
//                cop picks c' depending on A; A1 = A\{c'}; win if empty; next state (j-1, c', N[A1]\{c'}).
//  Win_j(c,S) = some blind walk reaches a win or a state where glimpsing wins against every answer.
// Vertices are heap indices 1..n, bit v of the mask.
#include <bits/stdc++.h>
using namespace std;
typedef bitset<1024> B;
static inline B bit(int v){B b; b.set(v); return b;}
int h,n; char G;
vector<int> nbc[1024];      // closed neighbourhood list: self first, then neighbours
B cl[1024];                 // closed neighbourhood mask
B branchm[1024][4];         // branchm[c][i]: component of T-c containing nbc[c][i] (i>=1)
B ALL;
static inline B expandS(B S,int c){
  B r; for(size_t v=S._Find_first(); v<S.size(); v=S._Find_next(v)) r|=cl[v];
  r.reset(c); return r;
}
struct Key{B S; int c; bool operator==(const Key&o)const{return S==o.S&&c==o.c;}};
struct KH{size_t operator()(const Key&k)const{
  uint64_t x=std::hash<B>()(k.S) ^ ((uint64_t)k.c*0x165667B19E3779F9ULL);
  x^=x>>29; x*=0xBF58476D1CE4E5B9ULL; x^=x>>32; return x;}};
enum {UNK=0,WIN=1,LOSE=2,NG=3};
struct Info{uint8_t st; uint8_t mv;}; // mv: next cop vertex for a blind move, 255 = glimpse
vector<unordered_map<Key,Info,KH>> memo;
bool win(int j,int c,B S);
// returns -1 if glimpse wins against every answer, else the failing answer (E: vertex v; D: neighbour u)
int glimpseFail(int j,int c,B S){
  if(G=='E'){
    for(int v=1;v<=n;v++) if(S[v]){
      bool ok=false;
      for(int c2:nbc[c]){ if(c2==v){ok=true;break;} }
      if(!ok) for(int c2:nbc[c]){ if(win(j-1,c2,expandS(bit(v),c2))){ok=true;break;} }
      if(!ok) return v;
    }
  } else {
    for(size_t i=1;i<nbc[c].size();i++){
      B A=S&branchm[c][i]; if(A.none()) continue;
      bool ok=false;
      for(int c2:nbc[c]){ B A1=A&~bit(c2); if(A1.none()||win(j-1,c2,expandS(A1,c2))){ok=true;break;} }
      if(!ok) return nbc[c][i];
    }
  }
  return -1;
}
struct Node{Key k; int par; int mv;};
bool win(int j,int c,B S){
  if(S.none()) return true;
  auto &M=memo[j];
  Key k0{S,c};
  { auto it=M.find(k0); if(it!=M.end()){ if(it->second.st==WIN) return true; if(it->second.st==LOSE) return false; } }
  vector<Node> q; unordered_set<Key,KH> vis;
  q.push_back({k0,-1,-1}); vis.insert(k0);
  int found=-1;
  for(size_t i=0;i<q.size()&&found<0;i++){
    Key cur=q[i].k;
    int st=UNK; { auto it=M.find(cur); if(it!=M.end()) st=it->second.st; }
    if(st==WIN){found=i;break;}
    if(st==LOSE) continue;
    if(j>0 && st!=NG){
      if(glimpseFail(j,cur.c,cur.S)<0){ M[cur]={WIN,255}; found=i; break; }
      M[cur]={NG,0};
    }
    for(int c2:nbc[cur.c]){
      B S1=cur.S&~bit(c2);
      if(S1.none()){ M[cur]={WIN,(uint8_t)c2}; found=i; break; }
      Key nk{expandS(S1,c2),c2};
      if(vis.insert(nk).second) q.push_back({nk,(int)i,c2});
    }
  }
  if(found>=0){
    int x=found;
    while(q[x].par>=0){ int p=q[x].par; M[q[p].k]={WIN,(uint8_t)q[x].mv}; x=p; }
    return true;
  }
  for(auto&nd:q) M[nd.k]={LOSE,0};
  return false;
}
int main(int argc,char**argv){
  if(argc<4){fprintf(stderr,"usage: fast G h k [dump] [allstarts]\n");return 1;}
  G=argv[1][0]; h=atoi(argv[2]); int k=atoi(argv[3]); bool dump=argc>4&&atoi(argv[4]); bool allst=argc>5&&atoi(argv[5]);
  n=(1<<(h+1))-1; if(n>1023){fprintf(stderr,"too big\n");return 1;}
  ALL.reset(); for(int v=1;v<=n;v++) ALL|=bit(v);
  for(int v=1;v<=n;v++){ nbc[v].push_back(v); if(v>1) nbc[v].push_back(v/2); if(2*v<=n){nbc[v].push_back(2*v);nbc[v].push_back(2*v+1);}
    cl[v].reset(); for(int u:nbc[v]) cl[v]|=bit(u); }
  for(int c=1;c<=n;c++) for(size_t i=1;i<nbc[c].size();i++){
    B comp=bit(nbc[c][i]); for(;;){ B e=expandS(comp,c); if(e==comp)break; comp=e; } branchm[c][i]=comp; }
  memo.resize(k+1);
  vector<int> starts; if(allst){ for(int v=1;v<=n;v++) starts.push_back(v);} else { for(int d=0;d<=h;d++) starts.push_back(1<<d); }
  int winstart=-1;
  for(int c:starts){ bool w=win(k,c,ALL&~bit(c)); printf("%c h=%d k=%d start=%d : %s\n",G,h,k,c,w?"COP WINS":"cop loses"); fflush(stdout); if(w&&winstart<0){winstart=c; if(!allst) break;} }
  size_t tot=0; for(int j=0;j<=k;j++){ printf("  memo[%d]=%zu\n",j,memo[j].size()); tot+=memo[j].size(); }
  printf("RESULT %c h=%d k=%d : %s (states cached %zu)\n",G,h,k,winstart>=0?"WIN":"LOSE",tot);
  return 0;
}
