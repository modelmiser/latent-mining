// Heuristic (simulated annealing) search for a valid protocol (n,m), any R. Not exhaustive: only a hit means anything.
// usage: anneal n m seed seconds
#include <bits/stdc++.h>
using namespace std;
typedef unsigned int u32;
int n,m,full; u32 MASK;
int owner[32],base[32]; u32 R;
mt19937 rng;
int cost(){
    u32 c=0,D[8]={0}; for(int p=0;p<m;p++){ if(base[p])c|=1u<<p; if(owner[p])D[owner[p]]|=1u<<p; }
    for(int i=1;i<=n;i++) if(!D[i]) return 1000000;
    static u32 cod[64][32];
    int total=0;
    for(int x=0;x<=full;x++){ u32 w=c; for(int i=0;i<n;i++) if((x>>i)&1) w^=D[i+1];
        for(int j=0;j<m;j++){ // window at shift j: t_q = w_{q-j}: rotate w left by j then mask R
            u32 r=((w<<j)|(w>>(m-j)))&MASK; if(j==0) r=w; cod[x][j]=r&R; }
        sort(cod[x],cod[x]+m);
        if(x>0){ int a=0,b=0,common=0; while(a<m&&b<m){ if(cod[0][a]==cod[x][b]){common++;a++;b++;} else if(cod[0][a]<cod[x][b])a++; else b++; }
            if(x<full) total+=3*(m-common);
            else { // correctness: no word in both (set overlap, weighted by multiplicity in M(1))
                int ov=0; for(int b2=0;b2<m;b2++) if(binary_search(cod[0],cod[0]+m,cod[x][b2])) ov++;
                total+=ov; }
        }
    }
    return total;
}
int main(int argc,char**argv){ n=atoi(argv[1]); m=atoi(argv[2]); rng.seed(atoi(argv[3])); double secs=atof(argv[4]);
    full=(1<<n)-1; MASK=(1u<<m)-1; auto t0=chrono::steady_clock::now(); long long it=0; int globalbest=1e9;
    while(true){
        for(int p=0;p<m;p++){ owner[p]=rng()%(n+1); base[p]=rng()&1; } R=(rng()&MASK)|1;
        int cur=cost(); double T=3.0;
        for(int step=0;step<400000;step++,it++){
            int so[32],sb[32]; u32 sR=R; memcpy(so,owner,sizeof so); memcpy(sb,base,sizeof sb);
            int mv=rng()%10;
            if(mv<5){ int p=rng()%m; owner[p]=rng()%(n+1); base[p]=rng()&1; }
            else if(mv<8){ int p=rng()%m,q=rng()%m; swap(owner[p],owner[q]); swap(base[p],base[q]); }
            else { int q=1+rng()%(m-1); R^=1u<<q; }
            int nc=cost();
            if(nc<=cur || exp((cur-nc)/T)*4294967296.0>rng()){ cur=nc; } else { memcpy(owner,so,sizeof so); memcpy(base,sb,sizeof sb); R=sR; }
            if(cur<globalbest){ globalbest=cur; }
            if(cur==0){ printf("FOUND n=%d m=%d owner=",n,m); for(int p=0;p<m;p++)printf("%d",owner[p]); printf(" u0="); for(int p=0;p<m;p++)printf("%d",base[p]); printf(" R="); for(int q=0;q<m;q++) if((R>>q)&1) printf("%d,",q); printf("\n"); return 0; }
            T=max(0.15,T*0.99999);
            if((step&4095)==0 && chrono::duration<double>(chrono::steady_clock::now()-t0).count()>secs){ printf("anneal n=%d m=%d: no hit, best cost %d, iters %lld\n",n,m,globalbest,it); return 0; }
        }
    }
}
