// Exhaustive search for repairable loops of length L, order n, with a SAFE extra pruning rule.
// usage: dfs2c n L k maxprint MOD R     (chunked: only prefixes with index % MOD == R; union over R = full search)
//   k = assumed number of words missing from w (run k = 0..n-1 and take the union).
// Normalisation (safe, loses only rotations): w = 0^a 1 ... 1 with 0^a a longest 0-run.
// Prune 1 (as fast.cpp): linear repeats in the prefix <= MAXREP = (L-2^n) + k.
//   (cyclic repeats = L - |C(w)| = e + k exactly when w misses k words; k <= n-1 by the pass move + P2.)
// Prune 2 ("needy cells"): for an isolated cell s whose n windows lie in the prefix, let
//   saved(s) = #{windows W through s : word(W) is a junction word of w-s, or occurs >= 2 times so far}
//   bonus(s) = min(k, #{junction words of w-s not seen so far})      (they might be missing words of w)
//   Repair after deleting s needs  (words only covered through s, not junction words) + (missing words not
//   junction words) <= n-1 (P2), i.e. at least k+1 savers. Each FUTURE repeat adds at most one saver to
//   each such s and only to cells inside one old window (n consecutive cells). So cells pairwise >= n apart
//   need disjoint future repeats:  sum of deficits over such a chain <= MAXREP - repeats so far.
// Leaves: full definition-level test (all adversary moves, all mechanic moves).
#include <bits/stdc++.h>
#include <omp.h>
using namespace std;
static int N,L,K,MAXREP; static bool NOPRUNE=false; static long long maxprint=20;
static bool univ(const vector<char>&v){
    int m=v.size(); if(m<N) return false; vector<char> seen(1<<N,0); int cnt=0; int c=0; int mask=(1<<N)-1;
    for(int j=0;j<N-1;j++) c=((c<<1)|v[j])&mask;
    for(int i=0;i<m;i++){ c=((c<<1)|v[(i+N-1)%m])&mask; if(!seen[c]){seen[c]=1;cnt++;} }
    return cnt==(1<<N);
}
static bool fixable(const vector<char>&v){
    if(univ(v)) return true; int m=v.size(); vector<char> t;
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue;
        if(m>=2){ t=v; t.erase(t.begin()+k); if(univ(t)) return true; }
        t=v; t.insert(t.begin()+k,v[k]); if(univ(t)) return true; }
    return false;
}
static bool repairable(const vector<char>&v){
    int m=v.size(); vector<char> t;
    if(!fixable(v)) return false;
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue;
        t=v; t.erase(t.begin()+k); if(!fixable(t)) return false;
        t=v; t.insert(t.begin()+k,v[k]); if(!fixable(t)) return false; }
    return true;
}
struct Cell{ int pos; int jw[8]; int nj; unsigned jsaved; };
struct S{
    vector<char> v; vector<int> cnt; vector<int> wat; int rep; int a; int zr; vector<Cell> cells;
};
static long long found=0, leaves=0, nodes=0, pruned2=0;
static inline int wordAt(const vector<char>&v,int i){ int c=0; for(int j=0;j<N;j++) c=(c<<1)|v[i+j]; return c; }
static bool prune2(S&s){
    int budget=MAXREP-s.rep; int sum=0,last=-1000000;
    for(const Cell&c: s.cells){
        int saved=0; for(int j=0;j<N;j++){ int w=s.wat[c.pos-j]; if(((c.jsaved>>j)&1)||s.cnt[w]>=2) saved++; }
        int bonus=0; for(int i=0;i<c.nj;i++) if(s.cnt[c.jw[i]]==0) bonus++; if(bonus>K) bonus=K;
        int d=K+1-saved-bonus; if(d<=0) continue;
        if(d>budget) return true;
        if(c.pos-last>=N){ sum+=d; last=c.pos; if(sum>budget) return true; }
    }
    return false;
}
static void addCell(S&s,int p){ // p isolated, windows p-N+1..p all in prefix, cells p-(N-1)..p+(N-1) known
    Cell c; c.pos=p; c.nj=0; c.jsaved=0;
    // junction words of w-s: cells [q..q+N] minus p, for q = p-(N-1) .. p-1
    for(int q=p-(N-1); q<=p-1; q++){ int w=0; for(int x=q;x<=q+N;x++){ if(x==p) continue; w=(w<<1)|s.v[x]; }
        bool dup=false; for(int i=0;i<c.nj;i++) if(c.jw[i]==w) dup=true; if(!dup) c.jw[c.nj++]=w; }
    for(int j=0;j<N;j++){ int w=s.wat[p-j]; for(int i=0;i<c.nj;i++) if(c.jw[i]==w) c.jsaved|=1u<<j; }
    s.cells.push_back(c);
}
static void leaf(S&s){
    #pragma omp atomic
    leaves++;
    if(repairable(s.v)){
        #pragma omp critical
        { found++; if(found<=maxprint){ printf("REPAIRABLE "); for(char c:s.v) putchar('0'+c); printf(" %s\n",univ(s.v)?"universal":"NOT-universal"); fflush(stdout);} }
    }
}
static void dfs(S&s,int stopdepth,vector<S>*out){
    int len=s.v.size();
    if(len==L){ leaf(s); return; }
    if(out && len>=stopdepth){ out->push_back(s); return; }
    for(int b=0;b<2;b++){
        if(len==L-1 && b==0) continue;
        int zr = b?0:s.zr+1; if(zr>s.a) continue;
        s.v.push_back(b); int nl=len+1; int w=-1; int oldrep=s.rep; int oldzr=s.zr; bool ok=true; bool added=false;
        if(nl>=N){ w=wordAt(s.v,nl-N); if(s.cnt[w]) s.rep++; if(s.rep>MAXREP) ok=false; }
        if(ok){
            if(w>=0){ s.cnt[w]++; s.wat.push_back(w); }
            s.zr=zr;
            if(!NOPRUNE){
                int p=nl-N; // cell whose right neighbourhood just became complete
                if(p>=N-1 && s.v[p]!=s.v[p-1] && s.v[p]!=s.v[p+1]){ addCell(s,p); added=true; }
                if(prune2(s)) { ok=false; 
                    #pragma omp atomic
                    pruned2++; }
            }
            if(ok) dfs(s,stopdepth,out);
            if(added) s.cells.pop_back();
            if(w>=0){ s.cnt[w]--; s.wat.pop_back(); }
            s.zr=oldzr;
        }
        s.rep=oldrep; s.v.pop_back();
    }
}
int main(int argc,char**argv){
    N=atoi(argv[1]); L=atoi(argv[2]); K=atoi(argv[3]); if(argc>4) maxprint=atoll(argv[4]); int MOD=1,R=0; if(argc>6){ MOD=atoi(argv[5]); R=atoi(argv[6]); }
    MAXREP=L-(1<<N)+K; if(MAXREP<0){ printf("n=%d L=%d k=%d: impossible (L<2^n)\n",N,L,K); return 0; }
    vector<S> pre;
    for(int a=1;a<L;a++){ S s; s.v.assign(a,0); s.cnt.assign(1<<N,0); s.rep=0; s.a=a; s.zr=a;
        bool ok=true; for(int i=0;i+N<=a;i++){ if(s.cnt[0]) s.rep++; s.cnt[0]++; s.wat.push_back(0);} if(s.rep>MAXREP) ok=false;
        if(!ok) continue; dfs(s,a+12,&pre); }
    #pragma omp parallel for schedule(dynamic,1) num_threads(2)
    for(size_t i=0;i<pre.size();i++){ if((int)(i%MOD)!=R) continue; S s=pre[i]; dfs(s,0,nullptr); }
    printf("dfs2c chunk %d/%d n=%d L=%d k=%d maxrep=%d prune2=%s prefixes=%zu leaves=%lld pruned2=%lld repairable(normalised words)=%lld\n",R,MOD,N,L,K,MAXREP,NOPRUNE?"off":"on",pre.size(),leaves,pruned2,found);
}
