// D(m) by BFS on Z_m x Z_m (ordered pairs, no pruning, 0 an ordinary value).
// usage: dm LO HI [w]   prints "D m D(m) first_worst_r count_of_worst"
// with w: also "W m r g path" for every residue (moves A:(2a,b) B:(a,2b) C:(a+b,b) D:(a,a+b))
#include <bits/stdc++.h>
using namespace std;
int main(int c,char**v){int lo=atoi(v[1]),hi=atoi(v[2]);bool wp=c>3;
 for(int m=lo;m<=hi;m++){
  size_t S=(size_t)m*m; vector<int8_t> mv(S,-1); vector<uint32_t> pred; if(wp)pred.assign(S,0);
  vector<int> g(m,-1); vector<uint32_t> gs(m,0);
  vector<uint32_t> fr,nf; uint32_t s0=(1%m)*m+(1%m); mv[s0]=4; fr.push_back(s0); g[1%m]=0; gs[1%m]=s0; int left=m-1,t=0;
  while(left>0){t++;nf.clear();
   for(uint32_t s:fr){uint32_t a=s/m,b=s%m; uint32_t na[4]={(2*a)%m,a,(a+b)%m,a},nb[4]={b,(2*b)%m,b,(a+b)%m};
    for(int k=0;k<4;k++){uint32_t ns=na[k]*m+nb[k]; if(mv[ns]<0){mv[ns]=k; if(wp)pred[ns]=s; nf.push_back(ns);
      if(g[na[k]]<0){g[na[k]]=t;gs[na[k]]=ns;left--;} if(g[nb[k]]<0){g[nb[k]]=t;gs[nb[k]]=ns;left--;}}}}
   fr.swap(nf); if(fr.empty()){printf("ERR %d\n",m);return 1;}}
  int D=*max_element(g.begin(),g.end()),cnt=0,wr=-1; for(int r=0;r<m;r++) if(g[r]==D){cnt++; if(wr<0)wr=r;}
  printf("D %d %d %d %d\n",m,D,wr,cnt);
  if(wp) for(int r=0;r<m;r++){ string p; uint32_t s=gs[r]; while(mv[s]!=4){p+="ABCD"[mv[s]]; s=pred[s];} reverse(p.begin(),p.end());
    printf("W %d %d %d %s\n",m,r,g[r],p.empty()?"-":p.c_str()); }
 }
}
