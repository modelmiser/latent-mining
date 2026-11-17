// Referee's own solver (written from the problem statement; general tree, belief game).
// Vertices 1..n heap-labelled for T_h; a "Game" may be restricted to a connected vertex subset H.
// State at the start of a round: (j glimpses left, cop c, S = possible robber vertices, c not in S).
// Round: optional glimpse (adversary picks a consistent answer) ; cop moves to c2 in N[c] ; S1 = A\{c2}; win iff S1 empty;
// robber moves: S' = N[S1]\{c2}.
#include <bits/stdc++.h>
using namespace std;
#ifndef NB
#define NB 128
#endif
typedef bitset<NB> Set;
struct Key { int j, c; Set S; bool operator==(const Key&o) const { return j==o.j && c==o.c && S==o.S; } };
struct KH { size_t operator()(const Key&k) const { return hash<Set>()(k.S) * 1000003u + k.c * 131u + k.j; } };

struct Game {
  char mode; int n; vector<vector<int>> nb; vector<Set> nbm; // nbm[v] = closed nbhd mask
  vector<vector<Set>> br;  // br[c][i] = component of T-c containing nb[c][i]
  Set V;
  unordered_map<Key,char,KH> memo;
  long long bfsStates=0;
  Game(char mode_, int h, const Set* H=nullptr): mode(mode_) {
    n=(1<<(h+1))-1; nb.assign(n+1,{}); nbm.assign(n+1,Set()); br.assign(n+1,{});
    for(int v=1;v<=n;v++) if(!H || (*H)[v]) V[v]=1;
    for(int v=2;v<=n;v++) if(V[v]&&V[v/2]) { nb[v].push_back(v/2); nb[v/2].push_back(v); }
    for(int v=1;v<=n;v++) if(V[v]) { nbm[v][v]=1; for(int u:nb[v]) nbm[v][u]=1; }
    for(int c=1;c<=n;c++) if(V[c]) for(int u:nb[c]) {
      Set comp; vector<int> st{u}; comp[u]=1;
      while(!st.empty()){ int x=st.back(); st.pop_back(); for(int y:nb[x]) if(y!=c && !comp[y]) { comp[y]=1; st.push_back(y);} }
      br[c].push_back(comp);
    }
  }
  Set expand(const Set& S1, int c2) const { // N[S1] \ {c2}
    Set R; for(size_t v=S1._Find_first(); v<NB; v=S1._Find_next(v)) R|=nbm[v]; R[c2]=0; return R;
  }
  // after an answer set A (cop at c, must move now) with j glimpses left afterwards: can the cop win?
  bool afterAnswer(int j,int c,const Set& A){
    vector<int> mv=nb[c]; mv.push_back(c);
    for(int c2:mv){ Set A1=A; A1[c2]=0; if(A1.none()) return true; }
    for(int c2:mv){ Set A1=A; A1[c2]=0; if(win(j,c2,expand(A1,c2))) return true; }
    return false;
  }
  bool glimpseWin(int j,int c,const Set& S){ // j = glimpses left BEFORE this glimpse (>=1)
    if(mode=='E'){
      for(size_t v=S._Find_first(); v<NB; v=S._Find_next(v)){ Set A; A[v]=1; if(!afterAnswer(j-1,c,A)) return false; }
      return true;
    } else {
      for(size_t i=0;i<nb[c].size();i++){ Set A=S&br[c][i]; if(A.none()) continue; if(!afterAnswer(j-1,c,A)) return false; }
      return true;
    }
  }
  bool win(int j,int c,const Set& S){
    if(S.none()) return true;
    Key k0{j,c,S}; auto it=memo.find(k0); if(it!=memo.end()) return it->second;
    // blind closure from (c,S): cop wins iff some blind-reachable state has an immediate capture or a winning glimpse
    unordered_set<Key,KH> seen; vector<Key> q; seen.insert(k0); q.push_back(k0); bool res=false;
    for(size_t i=0;i<q.size() && !res;i++){
      Key cur=q[i]; bfsStates++;
      auto mi=memo.find(cur); if(i>0 && mi!=memo.end()){ if(mi->second){res=true;break;} else continue; }
      vector<int> mv=nb[cur.c]; mv.push_back(cur.c);
      for(int c2:mv){ Set S1=cur.S; S1[c2]=0; if(S1.none()){res=true;break;} }
      if(res) break;
      if(j>0 && glimpseWin(j,cur.c,cur.S)){res=true;break;}
      for(int c2:mv){ Set S1=cur.S; S1[c2]=0; Key nx{j,c2,expand(S1,c2)}; if(seen.insert(nx).second) q.push_back(nx); }
    }
    if(!res) for(auto&k:seen) memo[k]=0;
    memo[k0]=res; return res;
  }
};
static int depthOf(int v){ int d=0; while(v>1){v/=2;d++;} return d; }
static Set subtree(int v,int n){ Set X; vector<int> st{v}; while(!st.empty()){int x=st.back();st.pop_back(); X[x]=1; if(2*x<=n){st.push_back(2*x);st.push_back(2*x+1);} } return X; }

int main(int argc,char**argv){
  string cmd=argv[1];
  if(cmd=="table"){ // table G h kmax : number of winning starts for each k
    char G=argv[2][0]; int h=atoi(argv[3]), kmax=atoi(argv[4]);
    Game g(G,h);
    for(int k=0;k<=kmax;k++){
      int wins=0; string who;
      for(int c=1;c<=g.n;c++){ Set S=g.V; S[c]=0; if(g.win(k,c,S)){wins++; } }
      printf("%c h=%d k=%d winning_starts=%d/%d memo=%zu\n",G,h,k,wins,g.n,g.memo.size()); fflush(stdout);
      if(wins==g.n) break;
    }
  } else if(cmd=="lam"){ // lam h jmax : Psi_m seeds (c, {v,v1,v2}), c outside X=subtree(v), v != root, in T_h, game E
    int h=atoi(argv[2]), jmax=atoi(argv[3]); Game g('E',h); int bad=0;
    map<tuple<int,int>,pair<int,int>> tally; // (m,j) -> (wins,total)
    for(int v=2;v<=g.n;v++){ int m=h-depthOf(v); if(m<1) continue; Set X=subtree(v,g.n);
      for(int c=1;c<=g.n;c++){ if(X[c]) continue; Set S; S[v]=1;S[2*v]=1;S[2*v+1]=1;
        for(int j=0;j<=jmax;j++){ bool w=g.win(j,c,S); auto&t=tally[{m,j}]; t.first+=w; t.second++; if(w && m>=2*j+2){bad++; printf("VIOLATION h=%d v=%d c=%d j=%d m=%d\n",h,v,c,j,m);} } } }
    for(auto&kv:tally) printf("h=%d m=%d j=%d: cop wins %d of %d Psi seeds%s\n",h,get<0>(kv.first),get<1>(kv.first),kv.second.first,kv.second.second, get<0>(kv.first)>=2*get<1>(kv.first)+2?"  [Lam says 0]":"");
    printf("lam h=%d violations=%d\n",h,bad);
  } else if(cmd=="proj"){ // proj G h trials seed : projection lemma + glimpse-outside test on random states, H = subtree(2)
    char G=argv[2][0]; int h=atoi(argv[3]); int trials=atoi(argv[4]); mt19937 rng(atoi(argv[5]));
    Game T(G,h); Set H=subtree(2,T.n); Game Q(G,h,&H);
    long long tested=0,premise=0,viol=0, gtested=0,gprem=0,gviol=0, mono=0,monov=0;
    for(int it=0;it<trials;it++){
      int c=1+rng()%T.n; int j=rng()%3; int dens=1+rng()%9; Set S;
      for(int v=1;v<=T.n;v++) if(v!=c && (int)(rng()%10)<dens) S[v]=1;
      if(S.none()) continue;
      int pc = H[c]?c:2; Set R=S&H; R[pc]=0;
      tested++;
      bool wT=T.win(j,c,S);
      if(wT){ premise++; if(!Q.win(j,pc,R)){viol++; if(viol<=3) printf("PROJ VIOLATION c=%d j=%d S=%s\n",c,j,S.to_string().c_str());} }
      // monotonicity: random subset
      { Set S2=S; for(int v=1;v<=T.n;v++) if(S2[v] && rng()%3==0) S2[v]=0; mono++; if(wT && S2.any() && !T.win(j,c,S2)) monov++; }
      // glimpse taken outside H: if the T-cop wins by glimpsing NOW (c outside H) with j+1 left, the Q-cop wins with j
      if(!H[c]){ gtested++; if(T.glimpseWin(j+1,c,S)){ gprem++; if(!Q.win(j,pc,R)){ gviol++; if(gviol<=3) printf("GLIMPSE-OUTSIDE VIOLATION c=%d j+1=%d S=%s\n",c,j+1,S.to_string().c_str()); } } }
    }
    printf("%c h=%d: projection tested=%lld premise_true=%lld violations=%lld ; monotone tested=%lld violations=%lld ; glimpse-outside tested=%lld premise_true=%lld violations=%lld\n",G,h,tested,premise,viol,mono,monov,gtested,gprem,gviol);
  } else if(cmd=="query"){ // query G h : lines "j c hexS" on stdin (bit v = vertex v) -> prints 0/1
    char G=argv[2][0]; int h=atoi(argv[3]); Game g(G,h); int j,c; string hx;
    while(cin>>j>>c>>hx){ Set S; int bit=0; for(int i=hx.size()-1;i>=0;i--){ int d=isdigit(hx[i])?hx[i]-'0':tolower(hx[i])-'a'+10; for(int b=0;b<4;b++,bit++) if(d>>b&1) S[bit]=1; } printf("%d\n",(int)g.win(j,c,S)); }
  }
  return 0;
}
