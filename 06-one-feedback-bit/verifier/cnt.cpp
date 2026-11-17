// count maximum packings (as families of distinct S-sets) for n-1<=6, memo DP on lowest free point
#include <bits/stdc++.h>
using namespace std; typedef unsigned long long u64;
int n,t; vector<vector<u64>> byLow; unordered_map<u64,pair<int,u64>> memo;
static inline int del(int x,int L,int i){int r=L-1-i; return ((x>>(r+1))<<r)|(x&((1<<r)-1));}
pair<int,u64> dp(u64 fr){ if(!fr)return {0,1}; auto it=memo.find(fr); if(it!=memo.end())return it->second;
  int p=__builtin_ctzll(fr); auto best=dp(fr&(fr-1));
  for(u64 s:byLow[p]) if((s&~fr)==0){auto v=dp(fr&~s); v.first++; if(v.first>best.first)best=v; else if(v.first==best.first)best.second+=v.second;}
  return memo[fr]=best;}
int main(int c,char**v){ n=atoi(v[1]); t=atoi(v[2]); int NP=1<<(n-1), m=n-t; set<u64> all;
  for(int u=0;u<(1<<t);u++)for(int a=0;a<(1<<m);a++)for(int b=0;b<(1<<m);b++){u64 s=0;
    for(int i=0;i<m;i++)s|=1ULL<<((u<<(m-1))|del(a,m,i)); for(int i=0;i<t;i++)s|=1ULL<<((del(u,t,i)<<m)|b); all.insert(s);}
  byLow.assign(NP,{}); for(u64 s:all)byLow[__builtin_ctzll(s)].push_back(s);
  auto r=dp(NP==64?~0ULL:((1ULL<<NP)-1)); printf("n=%d t=%d F=%d number_of_maximum_packings=%llu\n",n,t,r.first,r.second);}
