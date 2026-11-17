// Referee's fast program (own code). Modes:
//   brute n L                 : every necklace of length L, full game test
//   route n L [rho] [comp] [MOD R] : Lemma-5 route: universal loops of length L-1,L,L+1 (Eulerian circuits of
//                               de Bruijn graph + balanced multiset of extra edges) -> candidates -> full game test
//   check n word              : full game test of one loop
#include <bits/stdc++.h>
using namespace std;
typedef unsigned char u8; typedef unsigned long long u64;
static int N, NW, MASK;
static inline int missing(const u8* v, int m){
    u64 seen=0; int c=0; for(int j=0;j<N-1;j++) c=(c<<1)|v[j%m];
    for(int i=0;i<m;i++){ int x=i+N-1; if(x>=m) x%=m; c=((c<<1)|v[x])&MASK; seen|=1ULL<<c; }
    return NW-__builtin_popcountll(seen);
}
static inline void mkloss(const u8*v,int m,int k,u8*t){ memcpy(t,v,k); memcpy(t+k,v+k+1,m-k-1); }
static inline void mkdbl(const u8*v,int m,int k,u8*t){ memcpy(t,v,k+1); memcpy(t+k+1,v+k,m-k); }
static bool fixable(const u8*v,int m){
    int ms=missing(v,m); if(ms==0) return true;
    if(m>=2*N && ms>N-1) return false;   // one edit leaves all windows avoiding the edit site unchanged: <= N-1 new windows
    u8 t[200];
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue;   // one cell per run is enough (same result)
        if(m>=2){ mkloss(v,m,k,t); if(missing(t,m-1)==0) return true; }
        mkdbl(v,m,k,t); if(missing(t,m+1)==0) return true; }
    return false;
}
static bool repairable(const u8*v,int m){
    if(!fixable(v,m)) return false; u8 t[200];
    if(m>=3) for(int k=1;k+1<m;k++){ if(v[k]!=v[k-1]&&v[k]!=v[k+1]){ mkloss(v,m,k,t); if(!fixable(t,m-1)) return false; } }  // (redundant early test; all losses are tested again below)
    for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue; mkdbl(v,m,k,t); if(!fixable(t,m+1)) return false; }
    if(m>=2) for(int k=0;k<m;k++){ if(k>0&&v[k]==v[k-1]) continue; mkloss(v,m,k,t); if(!fixable(t,m-1)) return false; }
    return true;
}
static string canon(const u8*v,int m){ string s(m,'0'),b; for(int i=0;i<m;i++) s[i]='0'+v[i]; b=s;
    for(int r=1;r<m;r++){ rotate(s.begin(),s.begin()+1,s.end()); if(s<b) b=s; } return b; }
static int nruns(const u8*v,int m){ int r=0; for(int i=0;i<m;i++) if(v[i]!=v[(i+m-1)%m]) r++; return r; }
// ---------- brute
static void brute(int L){
    long long neck=0,strs=0,nonu=0; vector<long long> perk(NW+1,0); string first;
    u8 v[64];
    for(u64 x=0;x<(1ULL<<L);x++){
        bool mn=true; int orbit=L; u64 full=(1ULL<<L)-1;
        for(int r=1;r<L;r++){ u64 y=((x<<r)|(x>>(L-r)))&full; if(y<x){mn=false;break;} if(y==x){orbit=r;break;} }
        if(!mn) continue;
        for(int i=0;i<L;i++) v[i]=(x>>(L-1-i))&1;
        if(repairable(v,L)){ neck++; strs+=orbit; int k=missing(v,L); if(k) nonu++; perk[k]++; if(first.empty()) first=canon(v,L); if(getenv("DUMP")) fprintf(stderr,"%s %d\n",canon(v,L).c_str(),k); }
    }
    printf("brute n=%d L=%d necklaces=%lld strings=%lld nonuniversal_necklaces=%lld perk:",N,L,neck,strs,nonu);
    for(int k=0;k<=N;k++) printf(" %lld",perk[k]); printf(" first=%s\n",first.c_str());
}
// ---------- route
static int L, RHO=-1, MOD=1, RR=0; static bool COMP=false, SMALL;
static set<string> sols; static long long nuni[3], ncand=0, nfound=0, prefctr=0;
static int mult[64], rem_[64]; static u8 U[200]; static int M, startNode;
static void test(const u8*w,int m){
    if(RHO>=0 && nruns(w,m)!=RHO) return;
    ncand++;
    if(repairable(w,m)){ nfound++; string c=canon(w,m);
        if(SMALL) sols.insert(c); else { printf("FOUND %s missing=%d\n",c.c_str(),missing(w,m)); fflush(stdout);} }
}
static bool FAST=false, COUNTONLY=false; static long long nfull=0, nskipuni=0, nfastrej=0;
static inline int uw(int s){ int c=0; for(int j=0;j<N;j++){ int x=s+j; x%=M; if(x<0) x+=M; c=(c<<1)|U[x]; } return c; }
// word of the edited loop: cells of U with index map; edited loop given by function pointer-free small buffers
struct Delta{ u64 miss, add; };
static int cntU[64];
// local delta of an edit on U. kind 0: loss of cell c. kind 1: doubling of cell c. kind 2/3: insert bit 0/1 before cell c.
static Delta delta(int kind,int c){
    // build local strip: cells c-N .. c+N of U (2N+1 cells), strip index of c is N
    u8 a[40], b[40]; int la=2*N+1, lb=0;
    for(int x=0;x<la;x++){ int y=(c-N+x)%M; if(y<0) y+=M; a[x]=U[y]; }
    int rs0,rs1,as0,as1; // removed window starts in a (inclusive), added window starts in b
    if(kind==0){ for(int x=0;x<la;x++) if(x!=N) b[lb++]=a[x]; rs0=1; rs1=N; as0=1; as1=N-1; }
    else if(kind==1){ for(int x=0;x<la;x++){ b[lb++]=a[x]; if(x==N) b[lb++]=a[x]; } rs0=1; rs1=N; as0=1; as1=N+1; }
    else { for(int x=0;x<la;x++){ if(x==N) b[lb++]=kind-2; b[lb++]=a[x]; } rs0=1; rs1=N-1; as0=1; as1=N; }
    int rw[16], aw[16], nr=0, na=0;
    for(int s=rs0;s<=rs1;s++){ int w=0; for(int j=0;j<N;j++) w=(w<<1)|a[s+j]; rw[nr++]=w; }
    for(int s=as0;s<=as1;s++){ int w=0; for(int j=0;j<N;j++) w=(w<<1)|b[s+j]; aw[na++]=w; }
    Delta d; d.miss=0; d.add=0;
    for(int i=0;i<nr;i++) cntU[rw[i]]--; for(int i=0;i<na;i++){ cntU[aw[i]]++; d.add|=1ULL<<aw[i]; }
    for(int i=0;i<nr;i++) if(cntU[rw[i]]==0) d.miss|=1ULL<<rw[i];
    for(int i=0;i<nr;i++) cntU[rw[i]]++; for(int i=0;i<na;i++) cntU[aw[i]]--;
    return d;
}
static inline int cdist(int a,int b){ int d=abs(a-b); return min(d,M-d); }
static void emitfast(){
    u8 t[200];
    for(int w=0;w<NW;w++) cntU[w]=0; for(int s=0;s<M;s++) cntU[uw(s)]++;
    int rho=nruns(U,M);
    Delta mv[200]; int mc[200], nm=0;
    for(int c=0;c<M;c++){ int p=(c+M-1)%M, q=(c+1)%M; if(U[c]!=U[p]&&U[c]!=U[q]){ mv[nm]=delta(1,c); mc[nm++]=c; } }
    for(int i=0;i<M;i++){ int p=(i+M-1)%M, q=(i+1)%M;
        int kinds[2], nk=0; int wrho;
        if(M==L-1){ if(U[i]!=U[p]){ kinds[nk++]=2+U[i]; wrho=rho; } else { kinds[nk++]=2+(1-U[i]); wrho=rho+2; } }
        else { if(U[i]!=U[p]){ kinds[nk++]=0; wrho=(U[i]!=U[q])?rho-2:rho; } }
        for(int z=0;z<nk;z++){
            if(RHO>=0 && wrho!=RHO) continue;
            ncand++;
            Delta di=delta(kinds[z],i);
            if(di.miss==0){ nskipuni++; continue; }   // w universal: enumerated in the M==L pass
            bool rej=false;
            for(int j=0;j<nm;j++){ if(cdist(mc[j],i)<2*N+2) continue;
                if(__builtin_popcountll((di.miss&~mv[j].add)|(mv[j].miss&~di.add))>N-1){ rej=true; break; } }
            if(rej){ nfastrej++; continue; }
            nfull++;
            if(kinds[z]==0) mkloss(U,M,i,t); else { memcpy(t,U,i); t[i]=kinds[z]-2; memcpy(t+i+1,U+i,M-i); }
            int m2=(kinds[z]==0)?M-1:M+1;
            if(repairable(t,m2)){ nfound++; string c=canon(t,m2);
                if(SMALL) sols.insert(c); else { printf("FOUND %s missing=%d\n",c.c_str(),missing(t,m2)); fflush(stdout);} }
        }
    }
}
static void emit(){ // U[0..M-1] universal loop of length M
    nuni[M-L+1]++;
    if(COUNTONLY) return;
    u8 t[200];
    if(M==L){ test(U,M); return; }
    if(FAST){ emitfast(); return; }
    if(M==L-1){
        for(int i=0;i<M;i++){ int p=(i+M-1)%M;
            if(U[i]!=U[p]){ // run start at i: lengthen this run
                if(RHO<0 || true){ memcpy(t,U,i); t[i]=U[i]; memcpy(t+i+1,U+i,M-i); test(t,M+1); } }
            else { // gap inside a run: split with the other symbol (i>=1 here unless run wraps over 0)
                memcpy(t,U,i); t[i]=1-U[i]; memcpy(t+i+1,U+i,M-i); test(t,M+1); }
        }
        return;
    }
    // M==L+1: delete one cell per run
    for(int i=0;i<M;i++){ int p=(i+M-1)%M; if(U[i]!=U[p]){ mkloss(U,M,i,t); test(t,M-1); } }
}
static void dfs(int t,int node){
    if(t==M){ if(node==startNode) emit(); return; }
    if(t==14 && MOD>1){ if((prefctr++)%MOD!=RR) return; }
    for(int b=0;b<2;b++){ int w=(node<<1)|b; if(!rem_[w]) continue;
        rem_[w]--; U[t]=(w>>(N-1))&1; dfs(t+1,w&(MASK>>1)); rem_[w]++; }
}
static int ONLYD=-9;
static void route(){
    for(int d=-1;d<=1;d++){ M=L+d; int e=M-NW; if(e<0) continue; if(ONLYD!=-9 && d!=ONLYD) continue;
        vector<int> ms(e,0); long long nms=0;
        while(true){
            // balanced?
            vector<int> bal(NW/2,0); for(int w:ms){ bal[w>>1]++; bal[w&(MASK>>1)]--; }
            bool ok=true; for(int x:bal) if(x) ok=false;
            if(ok && COMP){ vector<int> c; for(int w:ms) c.push_back(MASK^w); sort(c.begin(),c.end()); if(c<ms) ok=false; }
            if(ok && RHO>=0){ int r=NW/2; for(int w:ms) if((w>>(N-2))==1) r+=2;
                if(d==0 && r!=RHO) ok=false; if(d==-1 && r!=RHO && r!=RHO-2) ok=false; if(d==1 && r!=RHO && r!=RHO+2) ok=false; }
            if(ok){ nms++;
                for(int w=0;w<NW;w++) mult[w]=1; for(int w:ms) mult[w]++;
                int s=-1; for(int w=0;w<NW;w++) if(mult[w]==1){s=w;break;}
                if(s>=0){ memcpy(rem_,mult,sizeof(mult)); rem_[s]--; U[0]=(s>>(N-1))&1; startNode=s>>1;
                    long long before=nuni[d+1], cb=ncand; dfs(1,s&(MASK>>1));
                    if(!SMALL){ printf("len=%d extra={",M); for(int w:ms){ for(int j=N-1;j>=0;j--) putchar('0'+((w>>j)&1)); putchar(' ');} 
                        printf("} universal_loops=%lld candidates=%lld found_so_far=%lld\n",nuni[d+1]-before,ncand-cb,nfound); fflush(stdout);} }
                else printf("WARNING no start edge\n");
            }
            // next multiset (nondecreasing)
            int i=e-1; while(i>=0 && ms[i]==NW-1) i--; if(i<0) break; ms[i]++; for(int j=i+1;j<e;j++) ms[j]=ms[i];
            if(e==0) break;
        }
        if(e==0 && false) break;
    }
    long long nonu=0; vector<long long> perk(NW+1,0);
    for(auto&s:sols){ u8 v[200]; for(size_t i=0;i<s.size();i++) v[i]=s[i]-'0'; int k=missing(v,s.size()); if(k) nonu++; perk[k]++; }
    printf("route n=%d L=%d rho=%d comp=%d chunk=%d/%d universal(L-1,L,L+1)=%lld,%lld,%lld candidates=%lld found(with dups)=%lld",N,L,RHO,(int)COMP,RR,MOD,nuni[0],nuni[1],nuni[2],ncand,nfound);
    if(SMALL && getenv("DUMP")) for(auto&x:sols){ u8 v[200]; for(size_t i=0;i<x.size();i++) v[i]=x[i]-'0'; fprintf(stderr,"%s %d\n",x.c_str(),missing(v,x.size())); }
    printf(" fast: skipped_universal=%lld fast_rejected=%lld full_tests=%lld",nskipuni,nfastrej,nfull);
    if(SMALL){ printf(" necklaces=%zu nonuniversal_necklaces=%lld perk:",sols.size(),nonu); for(int k=0;k<=N;k++) printf(" %lld",perk[k]); }
    printf("\n");
}
int main(int argc,char**argv){
    string mode=argv[1]; N=atoi(argv[2]); NW=1<<N; MASK=NW-1;
    if(mode=="brute"){ brute(atoi(argv[3])); }
    else if(mode=="check"){ string s=argv[3]; u8 v[200]; for(size_t i=0;i<s.size();i++) v[i]=s[i]-'0';
        printf("n=%d len=%zu missing=%d %s\n",N,s.size(),missing(v,s.size()),repairable(v,s.size())?"REPAIRABLE":"NOT"); }
    else if(mode=="route"){ L=atoi(argv[3]); if(argc>4) RHO=atoi(argv[4]); if(argc>5) COMP=atoi(argv[5]); if(argc>7){ MOD=atoi(argv[6]); RR=atoi(argv[7]); }
        if(getenv("FAST")) FAST=true; if(getenv("COUNTONLY")) COUNTONLY=true; if(getenv("ONLYD")) ONLYD=atoi(getenv("ONLYD"));
        SMALL=(N<=5); route(); }
}
