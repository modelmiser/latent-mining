// slow.cpp: direct implementation of the definition, no pruning, no symmetry.
// usage: slow n m
// position type: H with card c (2 types) or player i with (card if x_i=0, card if x_i=1) (4 types each)
#include <bits/stdc++.h>
using namespace std;
int n,m; int owner[32],c0[32],c1[32];
long long nvalid=0,ncfgvalid=0; int minR=99,maxR=0; bool full=false; bool printed=false;
static inline unsigned rot(unsigned w,int j){unsigned M=(1u<<m)-1;return ((w<<j)|(w>>(m-j)))&M;}
void leaf(){
  bool seen[8]={0}; for(int p=0;p<m;p++) if(owner[p]>=0) seen[owner[p]]=1;
  for(int i=0;i<n;i++) if(!seen[i]) return;
  int X=1<<n; unsigned w[64];
  for(int x=0;x<X;x++){unsigned v=0; for(int p=0;p<m;p++){int c; if(owner[p]<0)c=c0[p]; else c=((x>>owner[p])&1)?c1[p]:c0[p]; if(c)v|=1u<<p;} w[x]=v;}
  bool any=false;
  for(unsigned R=1;R<(1u<<m);R++){
    vector<unsigned> M0(m),Mx(m);
    for(int j=0;j<m;j++) M0[j]=rot(w[0],j)&R;   // t_q = w_{q-j}: bit q of rotl(w,j)
    sort(M0.begin(),M0.end());
    bool ok=true;
    for(int x=1;x<X-1&&ok;x++){ for(int j=0;j<m;j++) Mx[j]=rot(w[x],j)&R; sort(Mx.begin(),Mx.end()); if(Mx!=M0) ok=false; }
    if(!ok) continue;
    for(int j=0;j<m&&ok;j++){unsigned t=rot(w[X-1],j)&R; if(binary_search(M0.begin(),M0.end(),t)) ok=false;}
    if(!ok) continue;
    nvalid++; any=true; int k=__builtin_popcount(R); minR=min(minR,k); maxR=max(maxR,k); if(R==(1u<<m)-1) full=true;
    if(!printed){printed=true; printf("WITNESS n=%d m=%d owner=",n,m); for(int p=0;p<m;p++) putchar(owner[p]<0?'H':'0'+owner[p]);
      printf(" c0="); for(int p=0;p<m;p++) putchar('0'+c0[p]); printf(" c1="); for(int p=0;p<m;p++) putchar('0'+c1[p]);
      printf(" R="); for(int p=0;p<m;p++) putchar((R>>p&1)?'1':'0'); puts("");}
  }
  if(any) ncfgvalid++;
}
void rec(int p){
  if(p==m){leaf();return;}
  for(int c=0;c<2;c++){owner[p]=-1;c0[p]=c1[p]=c;rec(p+1);}
  for(int i=0;i<n;i++)for(int a=0;a<2;a++)for(int b=0;b<2;b++){owner[p]=i;c0[p]=a;c1[p]=b;rec(p+1);}
}
int main(int argc,char**argv){n=atoi(argv[1]);m=atoi(argv[2]);rec(0);
  printf("SLOW n=%d m=%d E=%d valid_pairs=%lld valid_cfgs=%lld minR=%d maxR=%d fullR=%d\n",n,m,nvalid>0,nvalid,ncfgvalid,nvalid?minR:0,maxR,(int)full);}
