// D(m) by BFS with visited array on Z_m x Z_m (safe pruning: BFS first-visit depth is the
// minimal depth of a state; g_m(r) = min depth of a state holding r).
// usage: dm mlo mhi [pathfile maxm_for_paths]
// output lines: m D count_worst first_worst   ; pathfile: "m r moves" (moves over A,B,C,D)
// A: a=2a  B: b=2b  C: a=a+b  D: b=a+b
#include <bits/stdc++.h>
using namespace std;
int main(int argc,char**argv){
  int mlo=atoi(argv[1]),mhi=atoi(argv[2]); FILE*pf=argc>4?fopen(argv[3],"w"):0; int pm=argc>4?atoi(argv[4]):0;
  for(int m=mlo;m<=mhi;m++){
    vector<signed char> dep((size_t)m*m,-1); vector<unsigned char> mv; vector<int> par;
    bool wp=pf&&m<=pm; if(wp){mv.assign((size_t)m*m,0);par.assign((size_t)m*m,-1);}
    vector<int> g(m,-1),gs(m,-1); int found=0; vector<int> cur,nx;
    int s0=(1%m)*m+(1%m); dep[s0]=0; cur.push_back(s0); int t=0;
    while(true){
      for(int s:cur){int a=s/m,b=s%m; if(g[a]<0){g[a]=t;gs[a]=s;found++;} if(g[b]<0){g[b]=t;gs[b]=s;found++;}}
      if(found==m||cur.empty())break;
      nx.clear();
      for(int s:cur){int a=s/m,b=s%m; int c[4][2]={{2*a%m,b},{a,2*b%m},{(a+b)%m,b},{a,(a+b)%m}};
        for(int k=0;k<4;k++){int u=c[k][0]*m+c[k][1]; if(dep[u]<0){dep[u]=t+1; if(wp){mv[u]=k;par[u]=s;} nx.push_back(u);}}}
      cur.swap(nx); t++;
    }
    if(found!=m){printf("%d FAIL\n",m);continue;}
    int D=*max_element(g.begin(),g.end()),cnt=0,fw=-1; for(int r=0;r<m;r++)if(g[r]==D){cnt++;if(fw<0)fw=r;}
    printf("%d %d %d %d\n",m,D,cnt,fw);
    if(wp)for(int r=0;r<m;r++){string p; int s=gs[r]; while(par[s]>=0){p.push_back("ABCD"[mv[s]]);s=par[s];} reverse(p.begin(),p.end()); fprintf(pf,"%d %d %s\n",m,r,p.c_str());}
  }
}
