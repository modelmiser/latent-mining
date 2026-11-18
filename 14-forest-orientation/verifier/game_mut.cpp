// Referee's own solver, written from the problem statement.
// usage: game MODE n J [k]
//   MODE = F : exact, Algorithm may re-orient the WHOLE forest (definition, literal)
//          M : exact, Algorithm may choose any valid orientation of the merged tree;
//              also compares, per position and per Adversary move, with normal-form replies (Lemma 1 of B),
//              checks Lemma 2 bound, the 2-component formula of A (P2 corollary), and the 2^(h+1)-1 size bound
//          N : exact, normal-form replies only
//          B : boolean "can Adversary force >= k" with normal-form replies (budget search)
//   J = 0 plain game; J >= 1 : terminal bonus psi_J (solver A's W_J game)
#include <bits/stdc++.h>
using namespace std;
typedef vector<pair<int,int>> Arcs; // (tail, head)
typedef vector<vector<pair<int,char>>> Adj;

struct Tree { int n; Arcs arcs; vector<int> od; };
vector<Tree> trees; unordered_map<string,int> treeId;

static string enc(int v,int p,const Adj& adj,char c){
  vector<string> ch;
  for(auto&e: adj[v]) if(e.first!=p) ch.push_back(enc(e.first,v,adj,e.second));
  sort(ch.begin(),ch.end());
  string s="("; s+=c; for(auto&x:ch) s+=x; s+=")"; return s;
}
static Adj mkadj(int n,const Arcs& a){
  Adj adj(n);
  for(auto&e:a){ adj[e.first].push_back({e.second,'d'}); adj[e.second].push_back({e.first,'u'}); }
  return adj;
}
static string canon(int n,const Arcs& a){
  Adj adj=mkadj(n,a); string best;
  for(int r=0;r<n;r++){ string s=enc(r,-1,adj,'r'); if(r==0||s<best) best=s; }
  return best;
}
static void parse(const string& s,int& n,Arcs& a){
  vector<int> st; n=0; a.clear();
  for(size_t i=0;i<s.size();){
    if(s[i]=='('){ int v=n++; char c=s[i+1];
      if(c=='d') a.push_back({st.back(),v}); else if(c=='u') a.push_back({v,st.back()});
      st.push_back(v); i+=2; }
    else { st.pop_back(); i++; }
  }
}
static int intern(int n,const Arcs& a){
  string c=canon(n,a);
  auto it=treeId.find(c); if(it!=treeId.end()) return it->second;
  Tree t; parse(c,t.n,t.arcs); t.od.assign(t.n,0); for(auto&e:t.arcs) t.od[e.first]++;
  int id=trees.size(); trees.push_back(t); treeId[c]=id; return id;
}
static bool valid(int n,const Arcs& a){ vector<int> od(n,0); for(auto&e:a) if(++od[e.first]>2) return false; return true; }

// h(u): shortest directed distance from u to an unsaturated vertex
static int hdist(int n,const Arcs& a,int u){
  vector<vector<int>> out(n); for(auto&e:a) out[e.first].push_back(e.second);
  if(out[u].size()<2) return 0;
  vector<int> d(n,-1); d[u]=0; deque<int> q; q.push_back(u);
  while(!q.empty()){ int x=q.front(); q.pop_front();
    for(int y:out[x]) if(d[y]<0){ d[y]=d[x]+1; if(out[y].size()<2) return d[y]; q.push_back(y);} }
  fprintf(stderr,"no unsaturated reachable?!\n"); exit(3);
}
static int maxd(int n,const Arcs& a){ int b=0; for(int u=0;u<n;u++) b=max(b,hdist(n,a,u)); return b; }

int J=0;
unordered_map<int,int> psiC;
static int psi(int id){
  if(J==0) return 0;
  auto it=psiC.find(id); if(it!=psiC.end()) return it->second;
  const Tree& t=trees[id]; int m=t.arcs.size(); int best=min(J,maxd(t.n,t.arcs));
  // all flip sets of size < J (larger ones cannot beat min(J,.))
  vector<int> idx;
  function<void(int)> rec=[&](int start){
    if(!idx.empty()){
      Arcs a=t.arcs; for(int i:idx) swap(a[i].first,a[i].second);
      if(valid(t.n,a)) best=min(best,(int)idx.size()+min(J,maxd(t.n,a)));
    }
    if((int)idx.size()+1>=J) return;
    for(int i=start;i<m;i++){ idx.push_back(i); rec(i+1); idx.pop_back(); }
  };
  rec(0);
  psiC[id]=best; return best;
}

struct Reply{ int cost,id; bool operator<(const Reply&o)const{ return cost!=o.cost?cost<o.cost:id<o.id; } bool operator==(const Reply&o)const{return cost==o.cost&&id==o.id;} };
struct Move{ vector<Reply> all, norm; bool operator<(const Move&o)const{ return all!=o.all?all<o.all:norm<o.norm; } };
bool wantAll=false;
map<pair<int,int>,vector<Move>> mcache;
long long sizeBoundViol=0;

static vector<Reply> dedupe(map<int,int>& m){ vector<Reply> r; for(auto&e:m) r.push_back({e.second,e.first}); sort(r.begin(),r.end()); return r; }

static const vector<Move>& moves(int ia,int ib){
  auto key=make_pair(ia,ib); auto it=mcache.find(key); if(it!=mcache.end()) return it->second;
  Tree A=trees[ia], B=trees[ib]; int nA=A.n, n=A.n+B.n;
  Arcs old=A.arcs; for(auto&e:B.arcs) old.push_back({e.first+nA,e.second+nA});
  vector<int> od(n,0); vector<vector<int>> out(n); for(auto&e:old){ od[e.first]++; out[e.first].push_back(e.second); }
  Adj adjA=mkadj(A.n,A.arcs), adjB=mkadj(B.n,B.arcs);
  vector<int> us,vs; { set<string> seen; for(int u=0;u<A.n;u++) if(seen.insert(enc(u,-1,adjA,'r')).second) us.push_back(u); }
  { set<string> seen; for(int v=0;v<B.n;v++) if(seen.insert(enc(v,-1,adjB,'r')).second) vs.push_back(v+nA); }
  set<Move> ms;
  for(int u:us) for(int v:vs){
    Move mv; map<int,int> rn, ra;
    auto add=[&](map<int,int>& m,int cost,const Arcs& a){ int id=intern(n,a); auto f=m.find(id); if(f==m.end()||f->second>cost) m[id]=cost; };
    for(int side=0;side<2;side++){
      int x=side?v:u, y=side?u:v;
      if(getenv("MUT")&&side==1) continue; if(od[x]<2){ Arcs a=old; a.push_back({x,y}); add(rn,0,a); }
      else {
        vector<int> path{ x };
        function<void()> dfs=[&](){
          int last=path.back();
          for(int w:out[last]){
            path.push_back(w);
            if(od[w]<2){
              Arcs a=old;
              for(size_t i=0;i+1<path.size();i++){ bool f=false; for(auto&e:a) if(e.first==path[i]&&e.second==path[i+1]){ swap(e.first,e.second); f=true; break; } if(!f){fprintf(stderr,"path arc missing\n");exit(3);} }
              a.push_back({x,y});
              if(!valid(n,a)){fprintf(stderr,"normal reply invalid\n");exit(3);}
              add(rn,(int)path.size()-1,a);
            } else dfs();
            path.pop_back();
          }
        };
        dfs();
      }
    }
    mv.norm=dedupe(rn);
    if(wantAll){
      int m=old.size();
      for(unsigned mask=0;mask<(1u<<m);mask++){
        Arcs a=old; for(int i=0;i<m;i++) if(mask>>i&1) swap(a[i].first,a[i].second);
        for(int side=0;side<2;side++){ Arcs b=a; b.push_back(side?make_pair(v,u):make_pair(u,v)); if(valid(n,b)) add(ra,__builtin_popcount(mask),b); }
      }
      mv.all=dedupe(ra);
    }
    ms.insert(mv);
  }
  auto& ref=mcache[key]; ref.assign(ms.begin(),ms.end()); return ref;
}

// all valid re-orientations of one tree (for mode F)
map<int,vector<Reply>> reoC;
static const vector<Reply>& reorient(int id){
  auto it=reoC.find(id); if(it!=reoC.end()) return it->second;
  Tree t=trees[id]; int m=t.arcs.size(); map<int,int> r;
  for(unsigned mask=0;mask<(1u<<m);mask++){
    Arcs a=t.arcs; for(int i=0;i<m;i++) if(mask>>i&1) swap(a[i].first,a[i].second);
    if(valid(t.n,a)){ int nid=intern(t.n,a); int c=__builtin_popcount(mask); auto f=r.find(nid); if(f==r.end()||f->second>c) r[nid]=c; }
  }
  auto& ref=reoC[id]; ref=dedupe(r); return ref;
}

typedef vector<int> State;
static string keyOf(const State& s){ return string((const char*)s.data(), s.size()*sizeof(int)); }
char MODE;
unordered_map<string,int8_t> memo;
long long cmpMoves=0, cmpBad=0, l2bad=0, l2eq=0, l2neq=0, twoBad=0, twoCnt=0;

static int Pm1(const State& s){
  int sat=0,q=0,g=0;
  for(int id:s){ const Tree&t=trees[id]; if(t.n==1){g++;continue;} bool hs=false; for(int o:t.od) if(o==2) hs=true; if(hs) sat++; else q++; }
  return sat+(q+g/2)/2-1;
}

static int V(const State& s){
  if(s.size()==1) return psi(s[0]);
  string k=keyOf(s); auto it=memo.find(k); if(it!=memo.end()) return it->second;
  int best=-1; set<pair<int,int>> seen;
  for(size_t i=0;i<s.size();i++) for(size_t j=i+1;j<s.size();j++){
    if(!seen.insert({s[i],s[j]}).second) continue;
    State rest; for(size_t x=0;x<s.size();x++) if(x!=i&&x!=j) rest.push_back(s[x]);
    const vector<Move> mvs=moves(s[i],s[j]);   // copy: cache may grow during recursion
    for(const Move& mv: mvs){
      int mn=1000, mnN=1000;
      auto evalr=[&](const vector<Reply>& rs,int& out){
        for(const Reply& r: rs){ State ns=rest; ns.push_back(r.id); sort(ns.begin(),ns.end()); out=min(out,r.cost+V(ns)); }
      };
      if(MODE=='N'){ evalr(mv.norm,mn); }
      else if(MODE=='M'){ evalr(mv.all,mn); evalr(mv.norm,mnN); cmpMoves++; if(mn!=mnN) cmpBad++; }
      else { // F: merged tree any orientation, and every other component any re-orientation
        for(const Reply& r: mv.all){
          function<void(size_t,int,State&)> rec=[&](size_t x,int cost,State& cur){
            if(x==rest.size()){ State ns=cur; ns.push_back(r.id); sort(ns.begin(),ns.end()); mn=min(mn,cost+V(ns)); return; }
            const vector<Reply> ro=reorient(rest[x]);
            for(const Reply& q: ro){ cur.push_back(q.id); rec(x+1,cost+q.cost,cur); cur.pop_back(); }
          };
          State cur; rec(0,r.cost,cur);
        }
      }
      best=max(best,mn);
    }
  }
  memo[k]=best;
  if(J==0){ int p=Pm1(s); if(best<p) l2bad++; if(best==max(0,p)) l2eq++; else l2neq++; }
  if(J==0 && s.size()==2 && MODE!='B'){ twoCnt++; int a=maxd(trees[s[0]].n,trees[s[0]].arcs), b=maxd(trees[s[1]].n,trees[s[1]].arcs); if(best!=min(a,b)) twoBad++; }
  return best;
}

// boolean budget search, normal-form replies
unordered_map<string,pair<uint8_t,uint8_t>> bm; long long calls=0;
static bool win(const State& s,int k){
  if(k<=0) return true;
  if(s.size()==1) return psi(s[0])>=k;
  string key=keyOf(s);
  { auto it=bm.find(key); if(it!=bm.end()){ if(k<=it->second.first) return true; if(k>=it->second.second) return false; } }
  calls++;
  set<pair<int,int>> seen; bool res=false;
  for(size_t i=0;i<s.size()&&!res;i++) for(size_t j=i+1;j<s.size()&&!res;j++){
    if(!seen.insert({s[i],s[j]}).second) continue;
    State rest; for(size_t x=0;x<s.size();x++) if(x!=i&&x!=j) rest.push_back(s[x]);
    const vector<Move> mvs=moves(s[i],s[j]);
    for(const Move& mv: mvs){
      bool ok=true;
      for(const Reply& r: mv.norm){ if(r.cost>=k) continue; State ns=rest; ns.push_back(r.id); sort(ns.begin(),ns.end()); if(!win(ns,k-r.cost)){ ok=false; break; } }
      if(ok){ res=true; break; }
    }
  }
  auto it=bm.find(key); if(it==bm.end()) it=bm.emplace(key,make_pair((uint8_t)0,(uint8_t)255)).first;
  if(res) it->second.first=max<int>(it->second.first,k); else it->second.second=min<int>(it->second.second,k);
  return res;
}

int main(int argc,char**argv){
  MODE=argv[1][0]; int n=atoi(argv[2]); J=atoi(argv[3]);
  wantAll=(MODE=='M'||MODE=='F');
  Arcs e; int single=intern(1,e);
  State start(n,single);
  if(MODE=='B'){
    int k=atoi(argv[4]); bool r=win(start,k);
    printf("mode B n=%d J=%d: Adversary forces >= %d ? %s   (states=%zu trees=%zu calls=%lld)\n",n,J,k,r?"YES":"NO",bm.size(),trees.size(),calls);
    return 0;
  }
  int v=V(start);
  // size bound of Lemma 4: a saturated u with h(u)=k needs a tree of >= 2^(k+1)-1 vertices
  for(auto&t:trees) for(int u=0;u<t.n;u++){ int h=hdist(t.n,t.arcs,u); if(t.n < (1<<(h+1))-1) sizeBoundViol++; }
  printf("mode %c n=%d J=%d value=%d states=%zu trees=%zu",MODE,n,J,v,memo.size(),trees.size());
  if(MODE=='M') printf(" | Lemma1 per-move comparisons=%lld mismatches=%lld",cmpMoves,cmpBad);
  if(J==0) printf(" | Lemma2 V<P-1 violations=%lld, V==max(0,P-1): %lld, V!=: %lld | 2-comp formula checked=%lld bad=%lld",l2bad,l2eq,l2neq,twoCnt,twoBad);
  printf(" | sizebound violations=%lld\n",sizeBoundViol);
  return 0;
}
