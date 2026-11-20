// Fast search. Reduced form: every player position has u1 = complement of u0.
// usage: fast n m mode sym [K k]
//  mode 0: any R (0 in R wlog), mode 1: R = Z_m only
//  sym 1: symmetry breaking (pos 0 = player 1 base 0, players in order of first use); stop at first witness unless COUNT env
//  sym 0: no symmetry breaking; counts all valid (config, R with 0 in R)
#include <bits/stdc++.h>
using namespace std;
typedef unsigned int u32;
int n,m,mode,sym,K=1,kk=0; int full; u32 MASK;
int owner[32],base[32]; int cnt[8][2];
u32 W[64];
long long leaves=0, withA=0, privR=0, valid=0, nodec=0; bool stopfirst=true; long long witprinted=0;
static inline u32 rot(u32 w,int d){ d%=m; if(d<0)d+=m; return ((w<<d)|(w>>(m-d)))&MASK; }
u32 Aset;
int Rl[32],Rn;
void printwit(){
    if(witprinted>=3) return; witprinted++;
    printf("WITNESS n=%d m=%d owner=",n,m); for(int p=0;p<m;p++)printf("%d",owner[p]);
    printf(" u0="); for(int p=0;p<m;p++)printf("%d",base[p]);
    printf(" R="); for(int i=0;i<Rn;i++)printf("%d%s",Rl[i],i+1<Rn?",":""); printf("\n"); fflush(stdout);
}
bool found=false;
void dfs(int last, u32 code[][32]){
    // code[x][j] current pattern of row x under shift j
    for(int q=last+1;q<m;q++){
        if(!((Aset>>q)&1)) continue;
        bool pre=true; for(int i=1;i<Rn;i++) if(!((Aset>>(q-Rl[i]))&1)){pre=false;break;}
        if(!pre) continue;
        u32 nc[64][32]; u32 s0[32],sx[32];
        bool ok=true;
        for(int x=0;x<=full&&ok;x++){
            for(int j=0;j<m;j++){ int p=q-j; if(p<0)p+=m; nc[x][j]=(code[x][j]<<1)|((W[x]>>p)&1); }
            if(x==0){ memcpy(s0,nc[0],sizeof(u32)*m); sort(s0,s0+m);} 
            else if(x<full){ memcpy(sx,nc[x],sizeof(u32)*m); sort(sx,sx+m); if(memcmp(s0,sx,sizeof(u32)*m)) ok=false; }
        }
        if(!ok) continue;
        privR++;
        Rl[Rn++]=q;
        memcpy(sx,nc[full],sizeof(u32)*m); sort(sx,sx+m);
        bool disj=true; { int a=0,b=0; while(a<m&&b<m){ if(s0[a]==sx[b]){disj=false;break;} if(s0[a]<sx[b])a++; else b++; } }
        if(disj){ valid++; printwit(); if(stopfirst){found=true; Rn--; return;} }
        dfs(q,nc);
        Rn--;
        if(found) return;
    }
}
void leaf(){
    for(int i=1;i<=n;i++) if(cnt[i][0]!=cnt[i][1]||cnt[i][0]==0) return;
    leaves++;
    u32 c=0,D[8]={0};
    for(int p=0;p<m;p++){ if(base[p]) c|=1u<<p; if(owner[p]) D[owner[p]]|=1u<<p; }
    for(int x=0;x<=full;x++){ u32 w=c; for(int i=0;i<n;i++) if((x>>i)&1) w^=D[i+1]; W[x]=w; }
    if(mode==1){
        // R = Z_m : all AND-0 rows rotations of W[0]; W[full] not a rotation
        for(int x=1;x<full;x++){ bool r=false; for(int d=0;d<m;d++) if(rot(W[0],d)==W[x]){r=true;break;} if(!r) return; }
        for(int d=0;d<m;d++) if(rot(W[0],d)==W[full]) return;
        valid++; Rn=m; for(int i=0;i<m;i++)Rl[i]=i; printwit(); if(stopfirst) found=true; return;
    }
    Aset=0;
    for(int d=1;d<=m/2;d++){
        int a0=__builtin_popcount(W[0]&rot(W[0],d)); bool ok=true;
        for(int x=1;x<full;x++) if(__builtin_popcount(W[x]&rot(W[x],d))!=a0){ok=false;break;}
        if(ok){ Aset|=1u<<d; Aset|=1u<<(m-d); }
    }
    if(!Aset) return;
    withA++;
    u32 code[64][32];
    for(int x=0;x<=full;x++) for(int j=0;j<m;j++){ int p=(m-j)%m; code[x][j]=(W[x]>>p)&1; }
    Rl[0]=0; Rn=1;
    // R={0}: privacy by balance; correctness impossible (same weight, 0<weight<m) unless... check anyway skipped
    dfs(0,code);
}
void rec(int p,int used){
    if(found) return;
    if(p==3 && K>1){ if((nodec++)%K!=kk) return; }
    if(p==m){leaf();return;}
    int rem=m-p, need=0;
    for(int i=1;i<=n;i++){ int d=abs(cnt[i][0]-cnt[i][1]); need+= (cnt[i][0]+cnt[i][1]==0)?2:d; }
    if(need>rem) return;
    if(sym && p==0){ owner[0]=1; base[0]=0; cnt[1][0]++; rec(1,1); cnt[1][0]--; return; }
    owner[p]=0; for(int b=0;b<2;b++){ base[p]=b; rec(p+1,used); }
    int lim = sym? min(n,used+1) : n;
    for(int i=1;i<=lim;i++){ owner[p]=i; for(int b=0;b<2;b++){ base[p]=b; cnt[i][b]++; rec(p+1,max(used,i)); cnt[i][b]--; } }
}
int main(int argc,char**argv){
    n=atoi(argv[1]); m=atoi(argv[2]); mode=atoi(argv[3]); sym=atoi(argv[4]);
    if(argc>6){K=atoi(argv[5]);kk=atoi(argv[6]);}
    if(getenv("COUNT")||!sym) stopfirst=false;
    full=(1<<n)-1; MASK=(m==32)?~0u:((1u<<m)-1);
    rec(0,0);
    printf("fast n=%d m=%d mode=%d sym=%d part=%d/%d leaves=%lld withA=%lld privR=%lld valid=%lld E%s=%d%s\n",n,m,mode,sym,kk,K,leaves,withA,privR,valid,mode?"full":"",valid>0, (stopfirst&&found)?" (stopped at first)":"");
}
