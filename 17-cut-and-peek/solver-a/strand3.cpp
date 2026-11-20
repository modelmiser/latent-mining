// Family "R inside a subgroup": m = a*b, R = b*Ra with Ra a subset of Z_a (0 in Ra). The row splits into b strands
// (strand s = positions s + b*k); M(x) = union over strands of the multiset of Ra-windows of that strand.
// Class of a strand word = its multiset of Ra-windows. Need: multiset of the b classes constant on AND-0 inputs, and
// (correctness) no Ra-window word of any strand at input 1^n occurs as an Ra-window of any strand at an AND-0 input.
// Exact search for bb = 1..b strands by hash join. usage: strand3 n a b [onlyfull]
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n,a,b,full; unsigned AM;
vector<unsigned> Rs; vector<vector<int>> CL; // CL[r][w] = class id
vector<vector<vector<unsigned>>> WIN; // WIN[r][class] = sorted distinct window codes
struct Gad{ unsigned c; unsigned D[5]; int sig[16]; };
vector<vector<Gad>> gs; vector<set<vector<int>>> seen;
unsigned D[6],c; long long enumerated=0;
void rec(int p){
    if(p==a){
        for(int i=1;i<=n;i++) if(!D[i]) return;
        enumerated++;
        unsigned X[16]; X[0]=c; for(int x=1;x<=full;x++){ int low=__builtin_ctz(x); X[x]=X[x&(x-1)]^D[low+1]; }
        for(size_t r=0;r<Rs.size();r++){
            const vector<int>&cl=CL[r]; int s[16]; int dist[8]; int nd=0; bool ok=true;
            for(int x=0;x<full&&ok;x++){ s[x]=cl[X[x]]; bool f=false; for(int k=0;k<nd;k++) if(dist[k]==s[x]){f=true;break;} if(!f){ if(nd==b) ok=false; else dist[nd++]=s[x]; } }
            if(!ok) continue;
            s[full]=cl[X[full]]; for(int k=0;k<nd;k++) if(dist[k]==s[full]) ok=false;
            if(!ok) continue;
            vector<int> v(s,s+full+1); if(!seen[r].insert(v).second) continue;
            Gad g; g.c=c; for(int i=1;i<=n;i++) g.D[i]=D[i]; for(int x=0;x<=full;x++) g.sig[x]=s[x]; gs[r].push_back(g);
        }
        return;
    }
    for(int o=0;o<=n;o++){ if(p==0&&o!=1) continue;
        for(int bit=0;bit<2;bit++){ unsigned oc=c,od=D[o]; if(bit)c|=1u<<p; if(o)D[o]|=1u<<p; rec(p+1); c=oc; D[o]=od; } }
}
mt19937_64 rng(4242); long long nfound=0; int curr; int curbb;
bool verify(vector<int>&ids){
    auto&G=gs[curr];
    for(int x=1;x<full;x++){ vector<int> A,B; for(int i:ids){A.push_back(G[i].sig[x]);B.push_back(G[i].sig[0]);} sort(A.begin(),A.end());sort(B.begin(),B.end()); if(A!=B) return false; }
    set<unsigned> w0; for(int i:ids) for(int x=0;x<full;x++) for(unsigned t:WIN[curr][G[i].sig[x]]) w0.insert(t);
    for(int i:ids) for(unsigned t:WIN[curr][G[i].sig[full]]) if(w0.count(t)) return false;
    return true;
}
map<pair<int,int>,long long> cnt;
void report(vector<int> ids){
    if(!verify(ids)) return; nfound++; long long &k=cnt[{curr,curbb}]; k++; if(k>1) return;
    printf("FOUND n=%d a=%d b=%d Ra=",n,a,curbb); for(int q=0;q<a;q++) if((Rs[curr]>>q)&1) printf("%d,",q); printf("\n");
    for(int i:ids){ auto&g=gs[curr][i]; for(int p=0;p<a;p++)printf("%d",(g.c>>p)&1); printf(" "); for(int p=0;p<a;p++){int o=0;for(int k2=1;k2<=n;k2++) if((g.D[k2]>>p)&1)o=k2; printf("%d",o);} printf("\n"); } fflush(stdout);
}
int main(int argc,char**argv){ n=atoi(argv[1]);a=atoi(argv[2]);b=atoi(argv[3]); bool onlyfull=argc>4; full=(1<<n)-1; AM=(1u<<a)-1;
    for(unsigned R=1;R<=AM;R+=2){ bool canon=true; for(int d=1;d<a;d++){ unsigned r=((R<<d)|(R>>(a-d)))&AM; if((r&1)&&r<R){canon=false;break;} } if(!canon) continue; if(onlyfull&&R!=AM) continue; if(__builtin_popcount(R)<2) continue; Rs.push_back(R); }
    CL.resize(Rs.size()); WIN.resize(Rs.size()); gs.resize(Rs.size()); seen.resize(Rs.size());
    for(size_t r=0;r<Rs.size();r++){ map<vector<unsigned>,int> ids; CL[r].resize(1u<<a);
        for(unsigned w=0;w<=AM;w++){ vector<unsigned> ws; for(int j=0;j<a;j++){ unsigned rot=j?(((w<<j)|(w>>(a-j)))&AM):w; ws.push_back(rot&Rs[r]); } sort(ws.begin(),ws.end());
            auto it=ids.find(ws); int id; if(it==ids.end()){ id=ids.size(); ids[ws]=id; ws.erase(unique(ws.begin(),ws.end()),ws.end()); WIN[r].push_back(ws);} else id=it->second; CL[r][w]=id; } }
    rec(0);
    printf("n=%d a=%d b<=%d: Ra sets (up to translation)=%zu gadgets enumerated=%lld\n",n,a,b,Rs.size(),enumerated); fflush(stdout);
    for(curr=0;curr<(int)Rs.size();curr++){
        auto&Gd=gs[curr]; int G=Gd.size(); if(!G) continue;
        vector<u64> H(WIN[curr].size()); for(auto&h:H)h=rng(); u64 rx[16]; for(auto&r:rx)r=rng()|1;
        vector<u64> key(G); for(int g=0;g<G;g++){ u64 k=0; for(int x=1;x<full;x++) k+=rx[x]*(H[Gd[g].sig[x]]-H[Gd[g].sig[0]]); key[g]=k; }
        unordered_multimap<u64,int> mp; for(int i=0;i<G;i++) mp.insert({key[i],i});
        curbb=1; for(int i=0;i<G;i++) if(key[i]==0) report({i});
        if(b>=2){ curbb=2; for(int i=0;i<G;i++){ auto r=mp.equal_range(-key[i]); for(auto it=r.first;it!=r.second;++it) if(it->second>=i) report({i,it->second}); } }
        if(b>=3){ curbb=3; if((double)G*G>4e9){ printf("SKIP b=3 for Ra index %d (G=%d too large)\n",curr,G); } else
            for(int i=0;i<G;i++) for(int j=i;j<G;j++){ auto r=mp.equal_range(-(key[i]+key[j])); for(auto it=r.first;it!=r.second;++it) if(it->second>=j) report({i,j,it->second}); } }
        if(b>=4){ curbb=4; if((double)G*G>4e8){ printf("SKIP b=4 for Ra index %d (G=%d too large)\n",curr,G); } else {
            vector<pair<u64,u64>> ps; ps.reserve((size_t)G*(G+1)/2);
            for(int i=0;i<G;i++) for(int j=i;j<G;j++) ps.push_back({key[i]+key[j],(u64)i*1000000ull+j});
            sort(ps.begin(),ps.end());
            for(size_t t=0;t<ps.size();t++){ u64 want=-ps[t].first; auto lo=lower_bound(ps.begin(),ps.end(),make_pair(want,(u64)0));
                for(auto it=lo;it!=ps.end()&&it->first==want;++it){ if(it->second<ps[t].second) continue; report({(int)(ps[t].second/1000000),(int)(ps[t].second%1000000),(int)(it->second/1000000),(int)(it->second%1000000)}); } } } }
    }
    for(auto&kv:cnt) printf("count Ra#%d b=%d : %lld\n",kv.first.first,kv.first.second,kv.second);
    printf("RESULT n=%d a=%d b<=%d total found=%lld\n",n,a,b,nfound);
}
