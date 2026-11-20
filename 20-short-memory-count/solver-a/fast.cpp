// fast.cpp : exhaustive search over canonical (BFS-labelled, initially connected) machines
// with at most w+1 states.  compile: g++ -O2 -DWW=<w> fast.cpp
// modes:  min        -> no pruning, exact g for every machine, prints min and count
//         dec S [all] -> decision "exists M with g(M)<=S", with safe pruning; prints first witness
//                        (or with 'all': counts all)
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#ifndef WW
#define WW 3
#endif
const int w = WW, N = WW + 1;
const int NBITS = 1 << WW;
const int NW = (NBITS + 63) / 64;
struct BS { uint64_t a[NW]; };
static uint32_t sp16[256];
static BS PC[WW + 1];
static inline bool any(const BS&x){ for(int i=0;i<NW;i++) if(x.a[i]) return true; return false; }
// spread the low half of x: bit i -> bit 2i+b
static inline uint64_t spread32(uint32_t v){
    return (uint64_t)sp16[v&255] | ((uint64_t)sp16[(v>>8)&255]<<16) | ((uint64_t)sp16[(v>>16)&255]<<32) | ((uint64_t)sp16[v>>24]<<48);
}
static inline BS spreadLow(const BS&x,int b){
    BS r;
    for(int j=0;j<NW;j++){
        uint64_t src = x.a[j/2];
        uint32_t half = (NW==1) ? (uint32_t)src : (uint32_t)(src >> (32*(j&1)));
        r.a[j] = spread32(half) << b;
    }
    return r;
}
static inline BS foldShift(const BS&x,int b){ // window -> (2*window+b) mod 2^w
    BS f;
    if(NW==1){ uint64_t half = (NBITS>=2)? (x.a[0] | (x.a[0] >> (NBITS/2))) : x.a[0];
        uint64_t m = (NBITS/2>=64)? ~0ULL : ((1ULL<<(NBITS/2))-1);
        f.a[0] = half & m; }
    else { for(int i=0;i<NW/2;i++) f.a[i] = x.a[i] | x.a[i+NW/2]; for(int i=NW/2;i<NW;i++) f.a[i]=0; }
    return spreadLow(f,b);
}
int d[N][2]; int spr[N]; bool nonempty[N];
long long nodes=0, leaves=0, found=0;
// compute lower bounds on spread from defined transitions (d=-1 undefined), states 0..n-1
static void evalC(int n){
    BS L[N], T[N];
    memset(L,0,sizeof L); L[0].a[0]=1;
    for(int k=0;k<w;k++){
        memset(T,0,sizeof T);
        for(int q=0;q<n;q++) if(any(L[q])) for(int b=0;b<2;b++){ int t=d[q][b]; if(t<0) continue;
            BS s=spreadLow(L[q],b); for(int i=0;i<NW;i++) T[t].a[i]|=s.a[i]; }
        memcpy(L,T,sizeof L);
    }
    bool ch=true;
    while(ch){ ch=false;
        for(int q=0;q<n;q++) if(any(L[q])) for(int b=0;b<2;b++){ int t=d[q][b]; if(t<0) continue;
            BS s=foldShift(L[q],b);
            for(int i=0;i<NW;i++){ uint64_t nv=L[t].a[i]|s.a[i]; if(nv!=L[t].a[i]){L[t].a[i]=nv; ch=true;} } }
    }
    for(int q=0;q<n;q++){
        int lo=-1,hi=-1;
        for(int c=0;c<=w;c++){ bool h=false; for(int i=0;i<NW;i++) if(L[q].a[i]&PC[c].a[i]) h=true; if(h){ if(lo<0) lo=c; hi=c; } }
        nonempty[q]=(lo>=0); spr[q]= lo<0 ? -1 : hi-lo;
    }
}
// is there a cycle (via defined transitions) inside the set of states with spread >= thr ?
static bool cycleAbove(int n,int thr){
    unsigned S=0; for(int q=0;q<n;q++) if(nonempty[q] && spr[q]>=thr) S|=1u<<q;
    bool ch=true;
    while(ch && S){ ch=false;
        for(int q=0;q<n;q++) if(S>>q&1){ bool succ=false;
            for(int b=0;b<2;b++){ int t=d[q][b]; if(t>=0 && (S>>t&1)) succ=true; }
            if(!succ){ S&=~(1u<<q); ch=true; } } }
    return S!=0;
}
int mode=0, target=0; bool allw=false, noprune=false;
int best=1000; long long bestcnt=0; int bestd[N][2], bestn=0;
long long hist[WW+2];
static void printM(int n,int s){
    printf("WITNESS w=%d s=%d n=%d :",w,s,n); for(int q=0;q<n;q++) printf(" %d,%d",d[q][0],d[q][1]);
    printf("  spreads:"); for(int q=0;q<n;q++) printf(" %d",spr[q]); printf("\n");
}
static void leaf(int n){
    leaves++;
    evalC(n);
    if(mode==0){
        int gv=0; for(int s=w;s>=0;s--) if(cycleAbove(n,s)){gv=s;break;}
        hist[gv]++;
        if(gv<best){best=gv;bestcnt=1; printM(n,gv);} else if(gv==best) bestcnt++;
    } else {
        if(!cycleAbove(n,target+1)){ found++; if(found==1 || getenv("PRINTALL")) printM(n,target); if(!allw){ printf("nodes=%lld leaves=%lld\n",nodes,leaves); exit(0);} }
    }
}
static void rec(int p,int nused){
    int i=p>>1,b=p&1;
    if(i>=nused){ leaf(nused); return; }
    int lim = nused < N ? nused : N-1;
    for(int t=0;t<=lim;t++){
        d[i][b]=t; nodes++;
        int nn = (t==nused)? nused+1 : nused;
        if(mode==1 && !noprune){
            evalC(nn);
            if(cycleAbove(nn,target+1)){ continue; }
        }
        rec(p+1,nn);
    }
    d[i][b]=-1;
}
int main(int argc,char**argv){
    for(int v=0;v<256;v++){ uint32_t r=0; for(int i=0;i<8;i++) if(v>>i&1) r|=1u<<(2*i); sp16[v]=r; }
    memset(PC,0,sizeof PC);
    for(int x=0;x<NBITS;x++){ int c=__builtin_popcount(x); PC[c].a[x/64]|=1ULL<<(x%64); }
    memset(d,-1,sizeof d);
    if(argc>1 && !strcmp(argv[1],"dec")){ mode=1; target=atoi(argv[2]); for(int i=3;i<argc;i++){ if(!strcmp(argv[i],"all")) allw=true; if(!strcmp(argv[i],"noprune")) noprune=true; } }
    if(argc>1 && !strcmp(argv[1],"eval")){ // eval n d00 d01 d10 d11 ... : print spreads (-1 = empty C) and g
        int n=atoi(argv[2]); for(int q=0;q<n;q++){ d[q][0]=atoi(argv[3+2*q]); d[q][1]=atoi(argv[4+2*q]); }
        evalC(n); for(int q=0;q<n;q++) printf("%d ",spr[q]);
        int gv=0; for(int s=w;s>=0;s--) if(cycleAbove(n,s)){gv=s;break;} printf("g=%d\n",gv); return 0; }
    rec(0,1);
    if(mode==0){ printf("w=%d f=%d canonical_minimisers=%lld leaves=%lld hist:",w,best,bestcnt,leaves); for(int s=0;s<=w;s++) printf(" g=%d:%lld",s,hist[s]); printf("\n"); }
    else printf("w=%d target=%d found=%lld nodes=%lld leaves=%lld\n",w,target,found,nodes,leaves);
}
