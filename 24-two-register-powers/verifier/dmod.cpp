// referee: BFS on ordered pairs in Z_m x Z_m from (1,1); g[r]=first level at which r sits in a register.
// usage: dmod lo hi  -> lines "m D count_worst first_worst"
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <cstdint>
int main(int argc,char**argv){
  int lo=atoi(argv[1]),hi=atoi(argv[2]);
  for(int m=lo;m<=hi;m++){
    std::vector<uint8_t> seen((size_t)m*m,0); std::vector<int> g(m,-1);
    std::vector<uint32_t> fr,nf; int left=m; int t=0;
    uint32_t s0=(uint32_t)((1%m)*m+(1%m)); seen[s0]=1; fr.push_back(s0);
    while(true){
      for(uint32_t s:fr){ int a=s/m,b=s%m; if(g[a]<0){g[a]=t;left--;} if(g[b]<0){g[b]=t;left--;} }
      if(left==0) break;
      if(fr.empty()){ printf("%d UNREACHABLE\n",m); break; }
      nf.clear();
      for(uint32_t s:fr){ int a=s/m,b=s%m; int a2=(2*a)%m,b2=(2*b)%m,c=(a+b)%m;
        uint32_t z[4]={(uint32_t)(a2*m+b),(uint32_t)(a*m+b2),(uint32_t)(c*m+b),(uint32_t)(a*m+c)};
        for(int i=0;i<4;i++) if(!seen[z[i]]){seen[z[i]]=1;nf.push_back(z[i]);} }
      fr.swap(nf); t++;
    }
    int D=0,cw=0,fw=-1; for(int r=0;r<m;r++) if(g[r]>D)D=g[r];
    for(int r=0;r<m;r++) if(g[r]==D){ if(fw<0)fw=r; cw++; }
    printf("%d %d %d %d\n",m,D,cw,fw);
  }
}
