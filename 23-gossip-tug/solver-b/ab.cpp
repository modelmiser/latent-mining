// Threshold (null-window) minimax with transposition table on canonical states.
// win(s,t): can Delayer force >= t further calls from s?  TT stores lo (largest t proven true),
// hi (smallest proven upper bound on value) and the Delayer agent that proved lo.
// Pruning used: (1) TT bounds (exact logic, safe); (2) static bounds  ceil(#nonfull/2) <= value <= #live pairs
// (a call fills at most two agents; each call uses a distinct live pair) -- both true bounds, so safe.
// Usage: ab n tlow thigh [log2 table size]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64; typedef unsigned __int128 u128;
int n; int FULLROW;
struct St{ unsigned short K[10]; };
struct Ent{ u64 k0,k1; }; // k1: low 40 bits key, bits 40..47 lo, 48..55 hi, 56..59 besta+1
Ent* tab; u64 TMASK; u64 used=0, nodes=0;
static inline u64 mix(u64 h){h^=h>>31;h*=0x9E3779B97F4A7C15ULL;h^=h>>29;h*=0xBF58476D1CE4E5B9ULL;h^=h>>32;return h;}
static void canon(const St&s, u64&k0,u64&k1){
    const unsigned short*K=s.K; u64 col[10];
    for(int i=0;i<n;i++){int c=0;for(int j=0;j<n;j++)c+=(K[j]>>i)&1; col[i]=__builtin_popcount(K[i])*16+c;}
    for(int r=0;r<2;r++){ u64 nc[10];
        for(int i=0;i<n;i++){ u64 so=0,si=0,sb=0;
            for(int j=0;j<n;j++){ if(j==i)continue; bool o=(K[i]>>j)&1, in=(K[j]>>i)&1; u64 h=mix(col[j]+1);
                if(o&&in)sb+=h; else if(o)so+=h; else if(in)si+=h; }
            nc[i]=mix(col[i]*0x94D049BB133111EBULL+so*3+si*7+sb*13); }
        memcpy(col,nc,sizeof nc); }
    int perm[10]; for(int i=0;i<n;i++)perm[i]=i;
    sort(perm,perm+n,[&](int a,int b){return col[a]<col[b]||(col[a]==col[b]&&a<b);});
    int gs[10],ge[10],ng=0; for(int i=0;i<n;){int j=i;while(j<n&&col[perm[j]]==col[perm[i]])j++; gs[ng]=i;ge[ng]=j;ng++; i=j;}
    u128 best=~(u128)0;
    while(true){
        int pos[10]; for(int i=0;i<n;i++)pos[perm[i]]=i;
        u128 t=0;
        for(int i=0;i<n;i++){int k=K[perm[i]],m=0; while(k){int j=__builtin_ctz(k);k&=k-1;m|=1<<pos[j];} t|=(u128)m<<(10*i);}
        if(t<best)best=t;
        int gi=0; for(;gi<ng;gi++){ if(next_permutation(perm+gs[gi],perm+ge[gi]))break; }
        if(gi==ng)break;
    }
    k0=(u64)best; k1=(u64)(best>>64);
}
static inline Ent* probe(u64 k0,u64 k1){
    u64 h=mix(k0^mix(k1))&TMASK;
    while(true){ Ent&e=tab[h]; if(e.k0==0&&e.k1==0){ if(++used>TMASK*9/10){fprintf(stderr,"TABLE FULL\n");exit(3);} e.k0=k0; e.k1=k1|((u64)0<<40)|((u64)127<<48); return &e; }
        if(e.k0==k0&&(e.k1&0xFFFFFFFFFFULL)==k1) return &e; h=(h+1)&TMASK; }
}
static inline int LO(Ent*e){return (e->k1>>40)&0xff;} static inline int HI(Ent*e){return (e->k1>>48)&0xff;} static inline int BA(Ent*e){return (int)((e->k1>>56)&0xf)-1;}
static inline void setLO(Ent*e,int v){e->k1=(e->k1&~((u64)0xff<<40))|((u64)v<<40);} static inline void setHI(Ent*e,int v){e->k1=(e->k1&~((u64)0xff<<48))|((u64)v<<48);}
static inline void setBA(Ent*e,int a){e->k1=(e->k1&~((u64)0xf<<56))|((u64)(a+1)<<56);}
bool win(const St&s,int t){
    if(t<=0)return true;
    int nonfull=0,live=0;
    for(int i=0;i<n;i++){ if(s.K[i]!=FULLROW)nonfull++; for(int j=i+1;j<n;j++) if(!((s.K[i]>>j&1)&&(s.K[j]>>i&1)))live++; }
    if(live<t)return false;
    if((nonfull+1)/2>=t)return true;
    u64 k0,k1; canon(s,k0,k1); Ent*e=probe(k0,k1); nodes++;
    if(LO(e)>=t)return true; if(HI(e)<t)return false;
    // order of a: stored best first, then by fewest legal partners
    int order[10],cnt[10],m=0; int ba=BA(e);
    // NOTE: the stored agent index refers to this raw state only if labels agree; we store per canonical key but
    // index in raw labels, so it is only a heuristic hint (may be wrong agent) -- affects speed, not correctness.
    for(int a=0;a<n;a++){int c=0;for(int b=0;b<n;b++)if(b!=a&&!((s.K[a]>>b&1)&&(s.K[b]>>a&1)))c++; cnt[a]=c; if(c)order[m++]=a;}
    sort(order,order+m,[&](int x,int y){ if((x==ba)!=(y==ba))return x==ba; if(cnt[x]!=cnt[y])return cnt[x]<cnt[y]; return __builtin_popcount(s.K[x])>__builtin_popcount(s.K[y]);});
    bool res=false;
    for(int oi=0;oi<m&&!res;oi++){ int a=order[oi];
        int bs[10],key[10],bm=0;
        for(int b=0;b<n;b++) if(b!=a&&!((s.K[a]>>b&1)&&(s.K[b]>>a&1))){ bs[bm]=b; key[b]=__builtin_popcount(s.K[a]|s.K[b])*16+__builtin_popcount(s.K[b]); bm++; }
        sort(bs,bs+bm,[&](int x,int y){return key[x]>key[y];});
        bool all=true;
        for(int bi=0;bi<bm;bi++){ int b=bs[bi]; St c=s; c.K[a]=c.K[b]=s.K[a]|s.K[b]; if(!win(c,t-1)){all=false;break;} }
        if(all){res=true; e=probe(k0,k1); setBA(e,a);} 
    }
    e=probe(k0,k1);
    if(res){ if(LO(e)<t)setLO(e,t);} else { if(HI(e)>t-1)setHI(e,t-1);} 
    return res;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); int tl=atoi(argv[2]),th=atoi(argv[3]); int lg=argc>4?atoi(argv[4]):24;
    FULLROW=(1<<n)-1; TMASK=((u64)1<<lg)-1; tab=(Ent*)calloc(TMASK+1,sizeof(Ent)); if(!tab){puts("alloc fail");return 2;}
    St s; memset(&s,0,sizeof s); for(int i=0;i<n;i++)s.K[i]=1<<i;
    for(int t=tl;t<=th;t++){ clock_t c0=clock(); bool r=win(s,t); printf("n=%d  A>=%d ? %s   (tt entries %llu, nodes %llu, %.1fs)\n",n,t,r?"YES":"NO",used,nodes,(double)(clock()-c0)/CLOCKS_PER_SEC); fflush(stdout); if(!r)break; }
}
