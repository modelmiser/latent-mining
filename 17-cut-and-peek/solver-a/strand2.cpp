// Subgroup-R family, exactly b strands of length a (b=1..4), n players.
// Only gadgets whose necklace class takes <= b distinct values on AND-0 inputs can occur (the multiset of b classes is constant),
// and whose all-ones class is none of them. Enumerate those (position 0 = a card of player 1, wlog by rotating the strand), then hash-join.
// usage: strand2 n a b
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n,a,b,full;
vector<unsigned> CAN;
struct Gad{ unsigned c; unsigned D[5]; unsigned sig[16]; };
vector<Gad> gs; set<vector<unsigned>> seen;
unsigned D[6],c; long long enumerated=0;
void rec(int p){
    if(p==a){
        for(int i=1;i<=n;i++) if(!D[i]) return;
        enumerated++;
        unsigned s[16]; unsigned dist[8]; int nd=0;
        unsigned X[16]; X[0]=0; for(int x=1;x<=full;x++){ int low=__builtin_ctz(x); X[x]=X[x&(x-1)]^D[low+1]; }
        for(int x=0;x<full;x++){ s[x]=CAN[c^X[x]]; bool f=false; for(int k=0;k<nd;k++) if(dist[k]==s[x]){f=true;break;} if(!f){ if(nd==b) return; dist[nd++]=s[x]; } }
        s[full]=CAN[c^X[full]]; for(int k=0;k<nd;k++) if(dist[k]==s[full]) return;
        vector<unsigned> v(s,s+full+1); if(!seen.insert(v).second) return;
        Gad g; g.c=c; for(int i=1;i<=n;i++) g.D[i]=D[i]; for(int x=0;x<=full;x++) g.sig[x]=s[x]; gs.push_back(g); return;
    }
    for(int o=0;o<=n;o++){ if(p==0&&o!=1) continue;
        for(int bit=0;bit<2;bit++){ unsigned oc=c,od=D[o]; if(bit)c|=1u<<p; if(o)D[o]|=1u<<p; rec(p+1); c=oc; D[o]=od; } }
}
unordered_map<unsigned,u64> H; u64 rx[64]; mt19937_64 rng(777);
u64 hh(unsigned u){ auto it=H.find(u); if(it!=H.end()) return it->second; return H[u]=rng(); }
vector<u64> key;
bool verify(vector<int>&ids){
    for(int x=1;x<full;x++){ vector<unsigned> A,B; for(int i:ids){A.push_back(gs[i].sig[x]);B.push_back(gs[i].sig[0]);} sort(A.begin(),A.end());sort(B.begin(),B.end()); if(A!=B) return false; }
    set<unsigned> z0; for(int i:ids) for(int x=0;x<full;x++) z0.insert(gs[i].sig[x]);
    for(int i:ids) if(z0.count(gs[i].sig[full])) return false;
    return true;
}
long long nfound=0;
void report(vector<int> ids){
    if(!verify(ids)) return; nfound++; if(nfound>3) return;
    printf("FOUND n=%d a=%d b=%d\n",n,a,b); for(int i:ids){ for(int p=0;p<a;p++)printf("%d",(gs[i].c>>p)&1); printf(" "); for(int p=0;p<a;p++){int o=0;for(int k=1;k<=n;k++) if((gs[i].D[k]>>p)&1)o=k; printf("%d",o);} printf("\n"); } fflush(stdout);
}
int main(int argc,char**argv){ n=atoi(argv[1]);a=atoi(argv[2]);b=atoi(argv[3]); full=(1<<n)-1;
    CAN.resize(1u<<a); for(unsigned w=0;w<(1u<<a);w++){ unsigned best=w; for(int d=1;d<a;d++){ unsigned r=((w<<d)|(w>>(a-d)))&((1u<<a)-1); best=min(best,r);} CAN[w]=best; }
    rec(0);
    for(auto&r:rx)r=rng()|1;
    int G=gs.size(); key.resize(G);
    for(int g=0;g<G;g++){ u64 k=0; for(int x=1;x<full;x++) k+=rx[x]*(hh(gs[g].sig[x])-hh(gs[g].sig[0])); key[g]=k; }
    printf("n=%d a=%d b=%d gadgets enumerated=%lld kept signatures=%d\n",n,a,b,enumerated,G); fflush(stdout);
    unordered_multimap<u64,int> mp; for(int i=0;i<G;i++) mp.insert({key[i],i});
    if(b==1){ for(int i=0;i<G;i++) if(key[i]==0) report({i}); }
    else if(b==2){ for(int i=0;i<G;i++){ auto r=mp.equal_range(-key[i]); for(auto it=r.first;it!=r.second;++it) if(it->second>=i) report({i,it->second}); } }
    else if(b==3){ for(int i=0;i<G;i++) for(int j=i;j<G;j++){ auto r=mp.equal_range(-(key[i]+key[j])); for(auto it=r.first;it!=r.second;++it) if(it->second>=j) report({i,j,it->second}); } }
    else if(b==4){ vector<pair<u64,u64>> ps; ps.reserve((size_t)G*(G+1)/2);
        for(int i=0;i<G;i++) for(int j=i;j<G;j++) ps.push_back({key[i]+key[j],(u64)i*1000000ull+j});
        sort(ps.begin(),ps.end());
        for(size_t t=0;t<ps.size();t++){ u64 want=-ps[t].first; auto lo=lower_bound(ps.begin(),ps.end(),make_pair(want,(u64)0));
            for(auto it=lo;it!=ps.end()&&it->first==want;++it){ if(it->second<ps[t].second) continue; report({(int)(ps[t].second/1000000),(int)(ps[t].second%1000000),(int)(it->second/1000000),(int)(it->second%1000000)}); } } }
    else if(b==5){ vector<pair<u64,u64>> ps; ps.reserve((size_t)G*(G+1)/2);
        for(int i=0;i<G;i++) for(int j=i;j<G;j++) ps.push_back({key[i]+key[j],(u64)i*1000000ull+j});
        sort(ps.begin(),ps.end());
        size_t TB=1; while(TB<ps.size()*8) TB<<=1; vector<bool> bloom(TB); // 1-bit filter on pair keys (no false negatives)
        for(auto&pr:ps) bloom[(pr.first*0x9E3779B97F4A7C15ull)>>(64-__builtin_ctzll(TB))]=true;
        int sh=64-__builtin_ctzll(TB);
        for(int i=0;i<G;i++) for(int j=i;j<G;j++){ u64 kij=key[i]+key[j]; for(int k=j;k<G;k++){ u64 want=-(kij+key[k]);
            if(!bloom[(want*0x9E3779B97F4A7C15ull)>>sh]) continue;
            auto lo=lower_bound(ps.begin(),ps.end(),make_pair(want,(u64)0));
            for(auto it=lo;it!=ps.end()&&it->first==want;++it){ if((int)(it->second/1000000)<k) continue; report({i,j,k,(int)(it->second/1000000),(int)(it->second%1000000)}); } } } }
    printf("RESULT n=%d a=%d b=%d found=%lld\n",n,a,b,nfound);
}
