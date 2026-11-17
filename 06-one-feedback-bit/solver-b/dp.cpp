// Independent exact solver (no LP): F(n,t) by memoised branching on the lowest free point.
// Uses ALL strategies (u,a,b) (no dominance pruning), sets S = uD(a) ∪ D(u)b ⊂ {0,1}^(n-1)
// (n-1 <= 6 so a set is a 64-bit mask). f(mask) = max #disjoint sets inside complement of mask.
// Branch: lowest free point p is either left uncovered, or covered by exactly one set containing p.
#include <bits/stdc++.h>
using namespace std; typedef unsigned long long U;
int n,t; vector<vector<U>> byLow; unordered_map<U,int> memo; U FULL;
int val(const string&s){int v=0;for(char c:s)v=v*2+(c-'0');return v;}
vector<string> Dl(const string&s){vector<string> r;for(size_t i=0;i<s.size();i++)r.push_back(s.substr(0,i)+s.substr(i+1));return r;}
string bits(int v,int m){string s(m,'0');for(int i=0;i<m;i++)if(v>>(m-1-i)&1)s[i]='1';return s;}
int f(U mask){ if(mask==FULL) return 0; auto it=memo.find(mask); if(it!=memo.end()) return it->second;
  int p=__builtin_ctzll(~mask); int best=f(mask|(1ULL<<p));
  for(U S:byLow[p]) if(!(S&mask)) best=max(best,1+f(mask|S));
  memo[mask]=best; return best;}
int main(int c,char**v){ n=atoi(v[1]); t=atoi(v[2]); int N=1<<(n-1); FULL=(n-1==6)?~0ULL:((1ULL<<N)-1);
  byLow.assign(N,{}); set<U> all;
  for(int ui=0;ui<(1<<t);ui++)for(int ai=0;ai<(1<<(n-t));ai++)for(int bi=0;bi<(1<<(n-t));bi++){
    string u=bits(ui,t),a=bits(ai,n-t),b=bits(bi,n-t); U S=0;
    for(auto&x:Dl(a))S|=1ULL<<val(u+x); for(auto&x:Dl(u))S|=1ULL<<val(x+b); all.insert(S);}
  for(U S:all) byLow[__builtin_ctzll(S)].push_back(S);
  printf("n=%d t=%d F=%d (distinct sets %zu, memo %zu)\n",n,t,f(0),all.size(),memo.size()); }
