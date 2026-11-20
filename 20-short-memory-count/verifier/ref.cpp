// Referee's own search. Build: g++ -O2 -mbmi2 -DW=<w> [-DN=<states>] ref.cpp -o ref<w>
// State sets: for each machine state q,
//   P[q] = set of strings shorter than W that lead to q, string x of length L coded as (1<<L)|x  (codes 1..2^W-1)
//   Wn[q] = set of W-bit windows (newest bit = LSB) of strings of length >= W leading to q.
// Both only along DEFINED transitions (partial machine), closed under the defined transitions.
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cstdint>
#include <immintrin.h>
#ifndef W
#error define W
#endif
#ifndef N
#define N (W+1)
#endif
typedef uint64_t u64;
#if W>=6
const int NW = 1<<(W-6);
#else
const int NW = 1;
#endif
struct BS { u64 a[NW]; };
static BS M[W+1]; // M[k] = windows with popcount k

static inline void spread_half(const BS&x, int upper, int b, BS&out){
    // take lower (upper=0) or upper (upper=1) half of x (2^(W-1) entries), map index c -> 2c+b
    u64 mask = b ? 0xAAAAAAAAAAAAAAAAull : 0x5555555555555555ull;
#if W>=6
    const uint32_t* h = (const uint32_t*)x.a;
    for(int j=0;j<NW;j++) out.a[j] = _pdep_u64((u64)h[j + (upper?NW:0)], mask);
#else
    int half = 1<<(W-1);
    u64 v = upper ? (x.a[0]>>half) : (x.a[0] & ((1ull<<half)-1));
    out.a[0] = _pdep_u64(v, mask);
#endif
}
static inline void fold_spread(const BS&x, int b, BS&out){
    // window v -> (2v+b) mod 2^W
    u64 mask = b ? 0xAAAAAAAAAAAAAAAAull : 0x5555555555555555ull;
#if W>=6
    const uint32_t* h = (const uint32_t*)x.a;
    for(int j=0;j<NW;j++) out.a[j] = _pdep_u64((u64)(h[j]|h[j+NW]), mask);
#else
    int half = 1<<(W-1);
    u64 v = (x.a[0] | (x.a[0]>>half)) & ((1ull<<half)-1);
    out.a[0] = _pdep_u64(v, mask);
#endif
}

struct Node {
    BS P[N], Wn[N];
    signed char d[N][2];
    signed char sp[N];   // spread, -1 if empty C
    int used;
};
static int ORDER=0, S=0, MODE=0; // MODE 0 hist, 1 find g<=S, 2 find e<=S
static u64 nodes=0, leaves=0, found=0;
static u64 histg[W+2], histe[W+2];
static int printed=0, PRINTMAX=0;

static inline void recompute_sp(Node&nd,int q){
    int lo=-1,hi=-1;
    for(int k=0;k<=W;k++){ u64 t=0; for(int j=0;j<NW;j++) t|=nd.Wn[q].a[j]&M[k].a[j]; if(t){ if(lo<0)lo=k; hi=k; } }
    nd.sp[q] = lo<0 ? -1 : hi-lo;
}
static void propagate(Node&nd,int q0){
    unsigned dirty = 1u<<q0;
    while(dirty){
        int q=__builtin_ctz(dirty); dirty&=dirty-1;
        for(int b=0;b<2;b++){
            int r=nd.d[q][b]; if(r<0) continue;
            BS p1,w1,w2; spread_half(nd.P[q],0,b,p1); spread_half(nd.P[q],1,b,w1); fold_spread(nd.Wn[q],b,w2);
            bool ch=false, chw=false;
            for(int j=0;j<NW;j++){
                u64 np=nd.P[r].a[j]|p1.a[j]; if(np!=nd.P[r].a[j]){nd.P[r].a[j]=np;ch=true;}
                u64 nw=nd.Wn[r].a[j]|w1.a[j]|w2.a[j]; if(nw!=nd.Wn[r].a[j]){nd.Wn[r].a[j]=nw;ch=true;chw=true;}
            }
            if(chw) recompute_sp(nd,r);
            if(ch) dirty|=1u<<r;
        }
    }
}
// is there a cycle among states with spread > s using defined edges?
static inline bool cyc(const Node&nd,int s){
    unsigned R=0; for(int q=0;q<nd.used;q++) if(nd.sp[q]>s) R|=1u<<q;
    for(;;){
        unsigned R2=0;
        for(unsigned t=R;t;t&=t-1){ int q=__builtin_ctz(t);
            int a=nd.d[q][0], b=nd.d[q][1];
            if((a>=0 && (R>>a&1)) || (b>=0 && (R>>b&1))) R2|=1u<<q; }
        if(R2==R) return R!=0;
        R=R2;
    }
}
static int last_target=-1;
static bool pick(const Node&nd,int lt,int&q,int&b){
    if(ORDER==0){ // BFS: lowest state, bit 0 first
        for(q=0;q<nd.used;q++) for(b=0;b<2;b++) if(nd.d[q][b]<0) return true;
        return false;
    } else if(ORDER==1){ // follow the last target if it has a free slot, else highest-numbered state with a free slot
        if(lt>=0) for(b=0;b<2;b++) if(nd.d[lt][b]<0){ q=lt; return true; }
        for(q=nd.used-1;q>=0;q--) for(b=0;b<2;b++) if(nd.d[q][b]<0) return true;
        return false;
    } else { // ORDER 2: state with nonempty C first (largest spread), else lowest
        int best=-1,bs=-2;
        for(int x=0;x<nd.used;x++) if(nd.d[x][0]<0||nd.d[x][1]<0){ if(nd.sp[x]>bs){bs=nd.sp[x];best=x;} }
        if(best<0) return false; q=best; b = nd.d[q][0]<0?0:1; return true;
    }
}
static void rec(const Node&nd,int lt){
    nodes++;
    int q,b;
    if(!pick(nd,lt,q,b)){
        leaves++;
        int e=0; for(int x=0;x<nd.used;x++) if(nd.sp[x]>e) e=nd.sp[x];
        if(MODE==0){ int g=0; while(cyc(nd,g)) g++; histg[g]++; histe[e]++; }
        else { found++;
            if(printed<PRINTMAX){ printed++; printf("M %d %d",W,nd.used); for(int x=0;x<nd.used;x++) printf(" %d %d",nd.d[x][0],nd.d[x][1]); printf("\n"); } }
        return;
    }
    int lim = nd.used<N ? nd.used : nd.used-1;
    for(int r=0;r<=lim;r++){
        Node c=nd;
        if(r==c.used) c.used++;
        c.d[q][b]=r;
        propagate(c,q);
        if(MODE==1 && cyc(c,S)) continue;
        if(MODE==2){ bool bad=false; for(int x=0;x<c.used;x++) if(c.sp[x]>S) bad=true; if(bad) continue; }
        rec(c,r);
    }
}
int main(int argc,char**argv){
    // usage: ref MODE(hist|find|finde) S ORDER [PRINTMAX]
    if(argc<2){fprintf(stderr,"usage\n");return 2;}
    if(!strcmp(argv[1],"hist")) MODE=0; else if(!strcmp(argv[1],"find")) MODE=1; else MODE=2;
    if(argc>2) S=atoi(argv[2]); if(argc>3) ORDER=atoi(argv[3]); if(argc>4) PRINTMAX=atoi(argv[4]);
    memset(M,0,sizeof M);
    for(unsigned v=0;v<(1u<<W);v++){ int k=__builtin_popcount(v); M[k].a[v>>6]|=1ull<<(v&63); }
    Node nd; memset(&nd,0,sizeof nd);
    for(int x=0;x<N;x++){ nd.d[x][0]=nd.d[x][1]=-1; nd.sp[x]=-1; }
    nd.used=1; nd.P[0].a[0]=2; // code 1 = empty string
    rec(nd,0);
    printf("W=%d N=%d mode=%s S=%d ORDER=%d nodes=%llu leaves=%llu found=%llu\n",W,N,argv[1],S,ORDER,(unsigned long long)nodes,(unsigned long long)leaves,(unsigned long long)found);
    if(MODE==0){ printf("histg:"); for(int k=0;k<=W;k++) printf(" %d:%llu",k,(unsigned long long)histg[k]); printf("\nhiste:"); for(int k=0;k<=W;k++) printf(" %d:%llu",k,(unsigned long long)histe[k]); printf("\n"); }
    return 0;
}
