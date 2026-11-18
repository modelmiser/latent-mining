// Referee: memory-lean budget search. "Can Adversary force >= k on n vertices?" (normal-form replies, J>=0 terminal psi_J)
// usage: game3 n J k
// Same rules as game.cpp mode B; trees are packed one byte per vertex, merge results kept in a flat pool.
#include <bits/stdc++.h>
using namespace std;
typedef vector<pair<int,int>> Arcs; // (tail, head)
typedef vector<vector<pair<int,char>>> Adj;
unordered_map<string,int> treeId; vector<const string*> codes;

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
// packed: preorder, one byte per vertex = 1 + dir + 3*children, dir 0 root, 1 'u' (vertex->parent), 2 'd' (parent->vertex)
static string pack(const string& s){
  string out; vector<int> st;
  for(size_t i=0;i<s.size();){
    if(s[i]=='('){ char c=s[i+1]; if(!st.empty()) out[st.back()]+=3; st.push_back(out.size()); out.push_back(1+(c=='u'?1:c=='d'?2:0)); i+=2; }
    else { st.pop_back(); i++; }
  }
  return out;
}
static string canon(int n,const Arcs& a){
  Adj adj=mkadj(n,a);
  int deg[64], alive=n; bool dead[64]; vector<int> layer;
  for(int v=0;v<n;v++){ deg[v]=adj[v].size(); dead[v]=false; if(deg[v]<=1) layer.push_back(v); }
  while(alive>2){ vector<int> nx; for(int v:layer){ dead[v]=true; alive--; for(auto&e:adj[v]) if(!dead[e.first]&&--deg[e.first]==1) nx.push_back(e.first); } layer=nx; }
  string best; bool first=true;
  for(int r:layer){ string s=enc(r,-1,adj,'r'); if(first||s<best) best=s; first=false; }
  return pack(best);
}
static void parse(const string& s,int& n,Arcs& a){
  n=s.size(); a.clear(); vector<pair<int,int>> st; // (vertex, children left)
  for(int v=0;v<n;v++){
    int b=(unsigned char)s[v]-1, dir=b%3, ch=b/3;
    if(dir==1) a.push_back({v,st.back().first}); else if(dir==2) a.push_back({st.back().first,v});
    if(!st.empty()) st.back().second--;
    st.push_back({v,ch});
    while(!st.empty()&&st.back().second==0) st.pop_back();
  }
}
static int intern(int n,const Arcs& a){
  string c=canon(n,a);
  auto it=treeId.find(c); if(it!=treeId.end()) return it->second;
  int id=codes.size(); auto ins=treeId.emplace(c,id); codes.push_back(&ins.first->first); return id;
}
static bool valid(int n,const Arcs& a){ vector<int> od(n,0); for(auto&e:a) if(++od[e.first]>2) return false; return true; }
static int hdist(int n,const Arcs& a,int u){
  vector<vector<int>> out(n); for(auto&e:a) out[e.first].push_back(e.second);
  if(out[u].size()<2) return 0;
  vector<int> d(n,-1); d[u]=0; deque<int> q; q.push_back(u);
  while(!q.empty()){ int x=q.front(); q.pop_front();
    for(int y:out[x]) if(d[y]<0){ d[y]=d[x]+1; if(out[y].size()<2) return d[y]; q.push_back(y);} }
  fprintf(stderr,"no unsaturated reachable?!\n"); exit(3);
}
static int maxd(int n,const Arcs& a){ int b=0; for(int u=0;u<n;u++) b=max(b,hdist(n,a,u)); return b; }
int J=0, KMAX=0;
unordered_map<int,int> psiC;
static int psi(int id){
  if(J==0) return 0;
  auto it=psiC.find(id); if(it!=psiC.end()) return it->second;
  int n; Arcs base; parse(*codes[id],n,base); int m=base.size(); int best=min(J,maxd(n,base));
  vector<int> idx;
  function<void(int)> rec=[&](int start){
    if(!idx.empty()){ Arcs a=base; for(int i:idx) swap(a[i].first,a[i].second); if(valid(n,a)) best=min(best,(int)idx.size()+min(J,maxd(n,a))); }
    if((int)idx.size()+1>=J) return;
    for(int i=start;i<m;i++){ idx.push_back(i); rec(i+1); idx.pop_back(); }
  };
  rec(0); psiC[id]=best; return best;
}
vector<uint32_t> pool; unordered_map<uint64_t,uint32_t> midx;
static uint32_t moves(int ia,int ib){
  uint64_t key=((uint64_t)ia<<32)|(uint32_t)ib; auto it=midx.find(key); if(it!=midx.end()) return it->second;
  int nA,nB; Arcs aA,aB; parse(*codes[ia],nA,aA); parse(*codes[ib],nB,aB); int n=nA+nB;
  Arcs old=aA; for(auto&e:aB) old.push_back({e.first+nA,e.second+nA});
  vector<int> od(n,0); vector<vector<int>> out(n); for(auto&e:old){ od[e.first]++; out[e.first].push_back(e.second); }
  Adj adjA=mkadj(nA,aA), adjB=mkadj(nB,aB);
  vector<int> us,vs; { set<string> seen; for(int u=0;u<nA;u++) if(seen.insert(enc(u,-1,adjA,'r')).second) us.push_back(u); }
  { set<string> seen; for(int v=0;v<nB;v++) if(seen.insert(enc(v,-1,adjB,'r')).second) vs.push_back(v+nA); }
  set<vector<uint32_t>> ms;
  for(int u:us) for(int v:vs){
    map<int,int> rn;
    auto add=[&](int cost,const Arcs& a){ int id=intern(n,a); auto f=rn.find(id); if(f==rn.end()||f->second>cost) rn[id]=cost; };
    for(int side=0;side<2;side++){
      int x=side?v:u, y=side?u:v;
      if(od[x]<2){ Arcs a=old; a.push_back({x,y}); add(0,a); }
      else {
        vector<int> path{ x };
        function<void()> dfs=[&](){
          if((int)path.size()>KMAX) return;          // a path of >= KMAX arcs costs >= the whole budget: never usable
          int last=path.back();
          for(int w:out[last]){
            path.push_back(w);
            if(od[w]<2){
              Arcs a=old;
              for(size_t i=0;i+1<path.size();i++){ bool f=false; for(auto&e:a) if(e.first==path[i]&&e.second==path[i+1]){ swap(e.first,e.second); f=true; break; } if(!f){fprintf(stderr,"path arc missing\n");exit(3);} }
              a.push_back({x,y});
              if(!valid(n,a)){fprintf(stderr,"normal reply invalid\n");exit(3);}
              add((int)path.size()-1,a);
            } else dfs();
            path.pop_back();
          }
        };
        dfs();
      }
    }
    vector<pair<int,int>> r; for(auto&e:rn) r.push_back({e.second,e.first}); sort(r.begin(),r.end());
    vector<uint32_t> mv; for(auto&e:r) mv.push_back(((uint32_t)e.first<<28)|(uint32_t)e.second);
    ms.insert(mv);
  }
  uint32_t off=pool.size(); pool.push_back(ms.size());
  for(auto&mv:ms){ pool.push_back(mv.size()); for(uint32_t x:mv) pool.push_back(x); }
  midx[key]=off; return off;
}
typedef vector<int> State;
// state key: concatenation of the packed codes of its trees in sorted-id order (self-delimiting), n bytes
static string keyOf(const State& s){ string k; for(int id:s) k+=*codes[id]; return k; }
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
    uint32_t p=moves(s[i],s[j]); uint32_t nm=pool[p++];
    for(uint32_t m=0;m<nm&&!res;m++){
      uint32_t nr=pool[p++]; bool ok=true;
      for(uint32_t r=0;r<nr;r++){
        uint32_t x=pool[p+r]; int cost=x>>28, id=x&0x0fffffff;
        if(cost>=k) continue;
        State ns=rest; ns.push_back(id); sort(ns.begin(),ns.end());
        if(!win(ns,k-cost)){ ok=false; break; }
      }
      p+=nr; if(ok) res=true;
    }
  }
  auto it=bm.find(key); if(it==bm.end()) it=bm.emplace(key,make_pair((uint8_t)0,(uint8_t)255)).first;
  if(res) it->second.first=max<int>(it->second.first,k); else it->second.second=min<int>(it->second.second,k);
  return res;
}
int main(int argc,char**argv){
  int n=atoi(argv[1]); J=atoi(argv[2]); int k=atoi(argv[3]); KMAX=k;
  Arcs e; int single=intern(1,e); State start(n,single);
  bool r=win(start,k);
  printf("game3 n=%d J=%d: Adversary forces >= %d ? %s   (states=%zu trees=%zu pairs=%zu pool=%zu calls=%lld)\n",n,J,k,r?"YES":"NO",bm.size(),codes.size(),midx.size(),pool.size(),calls);
  return 0;
}
