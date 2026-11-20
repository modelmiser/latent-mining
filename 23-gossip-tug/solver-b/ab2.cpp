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
struct St{ unsigned short K[12]; };
struct Ent{ u64 k0,k1,k2; }; // k2: low 16 bits key, bits 40..47 lo, 48..55 hi, 56..59 besta+1
int TWIN=1; int HRULE=0; int ORD=1; int LEM=1; int BORD=2; // 0: Hastener unrestricted; >0: Hastener restricted to replies maximising a rule key (gives UPPER bounds on A)
Ent* tab; u64 TMASK; u64 used=0, nodes=0;
static inline u64 mix(u64 h){h^=h>>31;h*=0x9E3779B97F4A7C15ULL;h^=h>>29;h*=0xBF58476D1CE4E5B9ULL;h^=h>>32;return h;}
static void canon(const St&s, u64&k0,u64&k1,u64&k2,int*outpos=nullptr){
    const unsigned short*K=s.K; u64 col[12];
    for(int i=0;i<n;i++){int c=0;for(int j=0;j<n;j++)c+=(K[j]>>i)&1; col[i]=__builtin_popcount(K[i])*16+c;}
    for(int r=0;r<2;r++){ u64 nc[12];
        for(int i=0;i<n;i++){ u64 so=0,si=0,sb=0;
            for(int j=0;j<n;j++){ if(j==i)continue; bool o=(K[i]>>j)&1, in=(K[j]>>i)&1; u64 h=mix(col[j]+1);
                if(o&&in)sb+=h; else if(o)so+=h; else if(in)si+=h; }
            nc[i]=mix(col[i]*0x94D049BB133111EBULL+so*3+si*7+sb*13); }
        memcpy(col,nc,sizeof nc); }
    int perm[12]; for(int i=0;i<n;i++)perm[i]=i;
    sort(perm,perm+n,[&](int a,int b){return col[a]<col[b]||(col[a]==col[b]&&a<b);});
    int gs[12],ge[12],ng=0; for(int i=0;i<n;){int j=i;while(j<n&&col[perm[j]]==col[perm[i]])j++; gs[ng]=i;ge[ng]=j;ng++; i=j;}
    // Twin shortcut (safe): if all members of a colour group are pairwise interchangeable (the transposition (x y) is an
    // automorphism of the position), every ordering of that group gives the same relabelled position, so the group is
    // not permuted.  Shrinks the enumeration only; the minimum over the remaining orderings is unchanged.
    if(TWIN){ int ng2=0;
      for(int g=0;g<ng;g++){ bool tw=true; int x=perm[gs[g]];
        for(int q=gs[g]+1;q<ge[g]&&tw;q++){ int y=perm[q]; int mx=(1<<x)|(1<<y);
            int kx=K[x], ky=K[y]; int sw=kx&~mx; if(kx>>x&1)sw|=1<<y; if(kx>>y&1)sw|=1<<x; if(sw!=ky){tw=false;break;}
            for(int k=0;k<n;k++){ if(k==x||k==y)continue; if(((K[k]>>x)&1)!=((K[k]>>y)&1)){tw=false;break;} } }
        if(ge[g]-gs[g]>1&&tw){ /* drop group from enumeration */ } else { gs[ng2]=gs[g]; ge[ng2]=ge[g]; ng2++; } }
      ng=ng2; }
    u64 B0=~0ULL,B1=~0ULL,B2=~0ULL;
    while(true){
        int pos[12]; for(int i=0;i<n;i++)pos[perm[i]]=i;
        u64 w[3]={0,0,0};
        for(int i=0;i<n;i++){int k=K[perm[i]]; u64 m=0; while(k){int j=__builtin_ctz(k);k&=k-1;m|=1<<pos[j];} int bit=12*i; w[bit/64]|=m<<(bit%64); if(bit%64>52) w[bit/64+1]|=m>>(64-bit%64);}
        if(w[2]<B2||(w[2]==B2&&(w[1]<B1||(w[1]==B1&&w[0]<B0)))){B0=w[0];B1=w[1];B2=w[2]; if(outpos)memcpy(outpos,pos,sizeof(int)*n);}
        int gi=0; for(;gi<ng;gi++){ if(next_permutation(perm+gs[gi],perm+ge[gi]))break; }
        if(gi==ng)break;
    }
    k0=B0;k1=B1;k2=B2;
}
static inline Ent* probe(u64 k0,u64 k1,u64 k2){
    u64 h=mix(k0^mix(k1^mix(k2)))&TMASK;
    while(true){ Ent&e=tab[h]; if(e.k0==0){ if(++used>TMASK*9/10){fprintf(stderr,"TABLE FULL\n");exit(3);} e.k0=k0; e.k1=k1; e.k2=k2|((u64)127<<48); return &e; }
        if(e.k0==k0&&e.k1==k1&&(e.k2&0xFFFFFFFFFFULL)==k2) return &e; h=(h+1)&TMASK; }
}
static inline int LO(Ent*e){return (e->k2>>40)&0xff;} static inline int HI(Ent*e){return (e->k2>>48)&0xff;} static inline int BA(Ent*e){return (int)((e->k2>>56)&0xf)-1;}
static inline void setLO(Ent*e,int v){e->k2=(e->k2&~((u64)0xff<<40))|((u64)v<<40);} static inline void setHI(Ent*e,int v){e->k2=(e->k2&~((u64)0xff<<48))|((u64)v<<48);}
static inline void setBA(Ent*e,int a){e->k2=(e->k2&~((u64)0xf<<56))|((u64)(a+1)<<56);}
bool win(const St&s,int t){
    if(t<=0)return true;
    int nonfull=0,live=0;
    for(int i=0;i<n;i++){ if(s.K[i]!=FULLROW)nonfull++; for(int j=i+1;j<n;j++) if(!((s.K[i]>>j&1)&&(s.K[j]>>i&1)))live++; }
    if(live<t)return false;
    if((nonfull+1)/2>=t)return true;
    if(LEM){
      // Lemma LB: a call adds at most one new knower of any fixed secret  =>  value >= n - known(s) for every s.
      int fullmask=0; for(int i=0;i<n;i++) if(s.K[i]==FULLROW) fullmask|=1<<i;
      for(int j=0;j<n;j++){int kn=0;for(int i=0;i<n;i++)kn+=(s.K[i]>>j)&1; if(n-kn>=t)return true;}
      // Lemma UB: if some agent is full and every non-full agent misses the secret of some full agent, value <= #nonfull
      // (Hastener: named non-full a -> call such a full agent; named full a -> any legal partner becomes full).
      if(fullmask){ bool nostuck=true; for(int i=0;i<n;i++) if(s.K[i]!=FULLROW&&(fullmask&~s.K[i])==0) nostuck=false; if(nostuck&&nonfull<t) return false; }
    }
    int cpos[12]; u64 k0,k1,k2; canon(s,k0,k1,k2,cpos); Ent*e=probe(k0,k1,k2); nodes++;
    if(LO(e)>=t)return true; if(HI(e)<t)return false;
    // Delayer move ordering (heuristic only, no effect on correctness): TT hint first (stored in canonical labels),
    // then smallest "largest possible union", then fewest legal partners.
    int order[12],cnt[12],mxu[12],m=0; int bac=BA(e); int ba=-1; if(bac>=0) for(int i=0;i<n;i++) if(cpos[i]==bac) ba=i;
    for(int a=0;a<n;a++){int c=0,mu=0;for(int b=0;b<n;b++)if(b!=a&&!((s.K[a]>>b&1)&&(s.K[b]>>a&1))){c++; mu=max(mu,__builtin_popcount(s.K[a]|s.K[b]));} cnt[a]=c; mxu[a]=mu; if(c)order[m++]=a;}
    if(ORD==0) sort(order,order+m,[&](int x,int y){ if((x==ba)!=(y==ba))return x==ba; if(cnt[x]!=cnt[y])return cnt[x]<cnt[y]; return __builtin_popcount(s.K[x])>__builtin_popcount(s.K[y]);});
    else sort(order,order+m,[&](int x,int y){ if((x==ba)!=(y==ba))return x==ba; if(mxu[x]!=mxu[y])return mxu[x]<mxu[y]; if(cnt[x]!=cnt[y])return cnt[x]<cnt[y]; return x<y;});
    bool res=false;
    for(int oi=0;oi<m&&!res;oi++){ int a=order[oi];
        int bs[12],key[12],bm=0; long rk[12];
        for(int b=0;b<n;b++) if(b!=a&&!((s.K[a]>>b&1)&&(s.K[b]>>a&1))){ bs[bm]=b; { int un=__builtin_popcount(s.K[a]|s.K[b]), kb=__builtin_popcount(s.K[b]); int known=0; for(int j=0;j<n;j++)known+=(s.K[j]>>b)&1; int fresh=(kb==1&&known==1), full=(un==n);
              if(BORD==0) key[b]=un*16+kb; else if(BORD==1) key[b]=full*100000+fresh*10000+(n-known)*100+un; else if(BORD==2) key[b]=full*100000+fresh*10000+un*16+kb; else key[b]=fresh*10000+(n-known)*100+un; } bm++; }
        sort(bs,bs+bm,[&](int x,int y){return key[x]>key[y];});
        if(HRULE){ long mx=-1; for(int bi=0;bi<bm;bi++){int b=bs[bi]; int un=__builtin_popcount(s.K[a]|s.K[b]), kb=__builtin_popcount(s.K[b]); int known=0; for(int j=0;j<n;j++)known+=(s.K[j]>>b)&1;
               int fresh=(kb==1&&known==1), full=(un==n); int gain=un-__builtin_popcount(s.K[a]); int gainb=un-kb; long r=0;
               switch(HRULE){ case 1: r=fresh*10000+un*100+kb; break; case 2: r=full*100000+fresh*10000+un*100+kb; break; case 3: r=un*100+kb; break;
                 case 4: r=full*100000+fresh*10000+(n-known)*100+un; break; case 5: r=full*1000000+fresh*100000+(gain+gainb)*100+un; break; case 6: r=full*100000+fresh*10000; break; case 7: r=fresh; break; case 8: r=full*10+fresh; break;
 case 9: r=fresh*100+(n-known); break; case 10: r=full*100000+fresh*10000+(n-known)*100+kb; break; case 11: r=full*100000+fresh*10000+(n-known)*100+(n-kb); break; case 12: r=fresh*10000+(n-known)*100+un; break; case 13: r=(n-known); break; case 14: r=(n-known)*100+un; break;}
               rk[b]=r; if(r>mx)mx=r; }
            int m2=0; for(int bi=0;bi<bm;bi++) if(rk[bs[bi]]==mx) bs[m2++]=bs[bi]; bm=m2; }
        bool all=true;
        for(int bi=0;bi<bm;bi++){ int b=bs[bi]; St c=s; c.K[a]=c.K[b]=s.K[a]|s.K[b]; if(!win(c,t-1)){all=false;break;} }
        if(all){res=true; e=probe(k0,k1,k2); setBA(e,cpos[a]);} 
    }
    e=probe(k0,k1,k2);
    if(res){ if(LO(e)<t)setLO(e,t);} else { if(HI(e)>t-1)setHI(e,t-1);} 
    return res;
}
// ---- certificate dump for a YES answer (Delayer strategy DAG) ----
struct CE{ St s; int t,a; string kids; };
vector<CE> cert; map<array<u64,3>,pair<int,int>> cidx; // canonical key -> (index, t)
static const char* AL="0123456789ab";
int dumpnode(const St&raw,int t,string&permout){
    int pos[12]; u64 k0,k1,k2; canon(raw,k0,k1,k2,pos);
    permout.clear(); for(int i=0;i<n;i++)permout+=AL[pos[i]];
    array<u64,3> key={k0,k1,k2};
    auto it=cidx.find(key); if(it!=cidx.end()&&it->second.second>=t) return it->second.first;
    St s; memset(&s,0,sizeof s);
    for(int i=0;i<n;i++){int k=raw.K[i],m=0; while(k){int j=__builtin_ctz(k);k&=k-1;m|=1<<pos[j];} s.K[pos[i]]=m;}
    int idx=cert.size(); cert.push_back(CE()); cert[idx].s=s; cert[idx].t=t; cidx[key]={idx,t};
    int ga=-1;
    for(int a=0;a<n&&ga<0;a++){ bool any=false,all=true;
        for(int b=0;b<n&&all;b++) if(b!=a&&!((s.K[a]>>b&1)&&(s.K[b]>>a&1))){ any=true; St c=s; c.K[a]=c.K[b]=s.K[a]|s.K[b]; if(!win(c,t-1))all=false; }
        if(any&&all)ga=a; }
    if(ga<0){fprintf(stderr,"dump: no agent works?!\n");exit(4);}
    string kids;
    for(int b=0;b<n;b++) if(b!=ga&&!((s.K[ga]>>b&1)&&(s.K[b]>>ga&1))){ St c=s; c.K[ga]=c.K[b]=s.K[ga]|s.K[b];
        int nonfull=0; for(int i=0;i<n;i++) if(c.K[i]!=FULLROW)nonfull++;
        char buf[64];
        if(t-1<=0||(nonfull+1)/2>=t-1){ snprintf(buf,64," %c:L",AL[b]); kids+=buf; }
        else { string p; int j=dumpnode(c,t-1,p); snprintf(buf,64," %c:%d:%s",AL[b],j,p.c_str()); kids+=buf; } }
    cert[idx].a=ga; cert[idx].kids=kids; return idx;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); int tl=atoi(argv[2]),th=atoi(argv[3]); int lg=argc>4?atoi(argv[4]):24;
    FULLROW=(1<<n)-1; if(getenv("HRULE"))HRULE=atoi(getenv("HRULE")); if(getenv("ORD"))ORD=atoi(getenv("ORD")); if(getenv("LEM"))LEM=atoi(getenv("LEM")); if(getenv("TWIN"))TWIN=atoi(getenv("TWIN")); if(getenv("BORD"))BORD=atoi(getenv("BORD")); TMASK=((u64)1<<lg)-1; tab=(Ent*)calloc(TMASK+1,sizeof(Ent)); if(!tab){puts("alloc fail");return 2;}
    St s; memset(&s,0,sizeof s); for(int i=0;i<n;i++)s.K[i]=1<<i;
    if(getenv("START")){ istringstream ss(getenv("START")); for(int i=0;i<n;i++){int x; ss>>hex>>x; s.K[i]=x;} }
    for(int t=tl;t<=th;t++){ clock_t c0=clock(); bool r=win(s,t); printf("HRULE=%d n=%d  val>=%d ? %s   (tt entries %llu, nodes %llu, %.1fs)\n",HRULE,n,t,r?"YES":"NO",used,nodes,(double)(clock()-c0)/CLOCKS_PER_SEC); fflush(stdout); if(!r)break; if(getenv("CERT")&&t==th){ string p; dumpnode(s,t,p); FILE*f=fopen(getenv("CERT"),"w");
  fprintf(f,"# Delayer certificate: n=%d, root entry 0 claims >=%d calls. Line k (0-based, comments excluded): t a K_0..K_{n-1} (hex masks, agents 0-based, bit j = knows j) then for each legal partner b of a (ascending): b:L (leaf: t-1<=0 or ceil(nonfull(child)/2)>=t-1) or b:j:perm (child relabelled x->perm[x] equals state of line j, which claims >= t-1). Digits base 12 (a=10,b=11).\n",n,t);
  for(auto&e:cert){fprintf(f,"%d %c",e.t,AL[e.a]);for(int i=0;i<n;i++)fprintf(f," %x",e.s.K[i]);fprintf(f,"%s\n",e.kids.c_str());} fclose(f); printf("certificate entries %zu\n",cert.size()); } }
}
