// slow.cpp -- independent, unpruned check. No symmetry, no domination.
// Vertices 0..n-1, tree built with explicit parent array; components found by DFS.
// Forward-explores every state reachable from the legal openings, then computes the least
// fixpoint of "cop wins" by repeated sweeps (so bounded-time capture, no cycle is ever credited).
// Node (c,S,j) = cop to move at c, robber somewhere in S (S nonempty, c not in S), j glimpses left.
// Usage: slow MODE h k      -> prints whether the cop wins MODE(h,k), and which starts win.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n, par[64]; vector<int> nbr[64]; bool exact;
u64 closedN(u64 S){ u64 R=S; for(int v=0;v<n;++v) if(S>>v&1) for(int w:nbr[v]) R|=1ULL<<w; return R; }
u64 compOf(int c,int b){ // vertices reachable from b without passing c
  u64 seen=1ULL<<b; vector<int> st{b};
  while(!st.empty()){ int v=st.back(); st.pop_back(); for(int w:nbr[v]) if(w!=c && !(seen>>w&1)){ seen|=1ULL<<w; st.push_back(w);} }
  return seen; }
vector<int> lastLabels;
vector<u64> parts(int c,u64 S){ vector<u64> r; lastLabels.clear();
  if(exact){ for(int v=0;v<n;++v) if(S>>v&1){ r.push_back(1ULL<<v); lastLabels.push_back(v);} }
  else for(int b:nbr[c]){ u64 P=S&compOf(c,b); if(P){ r.push_back(P); lastLabels.push_back(b);} }
  return r; }
struct Node{ int c,j; u64 S; vector<vector<int>> opts; // each option: list of nodes that must ALL win; empty list = immediate win
  vector<int> omove; vector<char> ogl; vector<vector<int>> olab; bool win=false; int choice=-1; };
map<tuple<int,int,u64>,int> id; vector<Node> nodes; vector<int> todo;
int get(int c,u64 S,int j){ auto key=make_tuple(c,j,S); auto it=id.find(key); if(it!=id.end()) return it->second;
  int i=nodes.size(); id[key]=i; Node x; x.c=c; x.j=j; x.S=S; nodes.push_back(x); todo.push_back(i); return i; }
int main(int argc,char**argv){
  exact=argv[1][0]=='E'; int h=atoi(argv[2]), k=atoi(argv[3]); n=(1<<(h+1))-1;
  for(int v=1;v<n;++v){ par[v]=(v-1)/2; nbr[v].push_back(par[v]); nbr[par[v]].push_back(v); }
  u64 ALL = n==64? ~0ULL : (1ULL<<n)-1;
  if(n==1){ printf("%c h=0 k=%d cop_wins=1 (robber has no start)\n",argv[1][0],k); return 0; }
  // openings: (start c, optionally glimpse in round 1)
  vector<vector<vector<int>>> startopts(n), startlab(n);
  for(int c=0;c<n;++c){ u64 S=ALL&~(1ULL<<c);
    startopts[c].push_back({get(c,S,k)}); startlab[c].push_back({});
    if(k>=1){ vector<int> v; for(u64 P:parts(c,S)) v.push_back(get(c,P,k-1)); startopts[c].push_back(v); startlab[c].push_back(lastLabels); } }
  while(!todo.empty()){ int i=todo.back(); todo.pop_back(); int c=nodes[i].c,j=nodes[i].j; u64 S=nodes[i].S;
    vector<int> mv=nbr[c]; mv.push_back(c); vector<vector<int>> opts; vector<int> om; vector<char> og; vector<vector<int>> ol;
    for(int c2:mv){ u64 S1=S&~(1ULL<<c2); if(!S1){ opts.push_back({}); om.push_back(c2); og.push_back(0); ol.push_back({}); continue; }
      u64 S2=closedN(S1)&~(1ULL<<c2);
      opts.push_back({get(c2,S2,j)}); om.push_back(c2); og.push_back(0); ol.push_back({});
      if(j>=1){ vector<int> v; for(u64 P:parts(c2,S2)) v.push_back(get(c2,P,j-1)); opts.push_back(v); om.push_back(c2); og.push_back(1); ol.push_back(lastLabels); } }
    nodes[i].opts=opts; nodes[i].omove=om; nodes[i].ogl=og; nodes[i].olab=ol;
    if(nodes.size()>40000000){ printf("too many states\n"); return 2; } }
  bool ch=true; int sweeps=0;
  while(ch){ ch=false; ++sweeps;
    for(auto&x:nodes) if(!x.win) for(size_t oi=0;oi<x.opts.size();++oi){ auto&o=x.opts[oi]; bool all=true; for(int t:o) if(!nodes[t].win){all=false;break;} if(all){ x.win=true; x.choice=oi; ch=true; break; } } }
  bool any=false; string s;
  for(int c=0;c<n;++c){ bool w=false; for(auto&o:startopts[c]){ bool all=true; for(int t:o) if(!nodes[t].win) all=false; if(all) w=true; }
    if(w){ any=true; s+=" "+to_string(c); } }
  if(argc>4){ FILE*f=fopen(argv[4],"w");
    if(!any){ fprintf(f,"LOSING %c %d %d\n",argv[1][0],h,k); // every line: c j S(hex): a state from which the cop cannot force capture
      for(auto&x:nodes) if(!x.win) fprintf(f,"%d %d %llx\n",x.c,x.j,x.S); }
    else { fprintf(f,"STRATEGY %c %d %d\n",argv[1][0],h,k);
      int c0=-1,oi0=-1; for(int c=0;c<n&&c0<0;++c) for(size_t oi=0;oi<startopts[c].size();++oi){ bool all=true; for(int t:startopts[c][oi]) if(!nodes[t].win) all=false; if(all){ c0=c; oi0=oi; break; } }
      vector<int> stack; set<int> done; auto push=[&](int t){ if(!done.count(t)){ done.insert(t); stack.push_back(t);} };
      fprintf(f,"START %d",c0); if(startlab[c0][oi0].empty()){ fprintf(f," NOGL %d\n",startopts[c0][oi0][0]); push(startopts[c0][oi0][0]); }
      else { fprintf(f," GL"); for(size_t q=0;q<startopts[c0][oi0].size();++q){ fprintf(f," %d:%d",startlab[c0][oi0][q],startopts[c0][oi0][q]); push(startopts[c0][oi0][q]); } fprintf(f,"\n"); }
      while(!stack.empty()){ int i=stack.back(); stack.pop_back(); Node&x=nodes[i]; int oi=x.choice; fprintf(f,"NODE %d MOVE %d",i,x.omove[oi]);
        if(x.opts[oi].empty()) fprintf(f," WIN\n");
        else if(!x.ogl[oi]){ fprintf(f," NOGL %d\n",x.opts[oi][0]); push(x.opts[oi][0]); }
        else { fprintf(f," GL"); for(size_t q=0;q<x.opts[oi].size();++q){ fprintf(f," %d:%d",x.olab[oi][q],x.opts[oi][q]); push(x.opts[oi][q]); } fprintf(f,"\n"); } } }
    fclose(f); }
  printf("%c h=%d k=%d cop_wins=%d states=%zu sweeps=%d winning_start_vertices(0-based heap):%s\n",argv[1][0],h,k,(int)any,nodes.size(),sweeps,s.c_str());
}
