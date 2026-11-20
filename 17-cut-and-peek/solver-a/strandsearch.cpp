// Subgroup-R family, direct search for b strands of length a (n players), b = 2..4 via hashing.
// usage: strandsearch n a b
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n,a,b,full;
vector<int> CAN;
struct Gad{ unsigned c; unsigned D[6]; int sig[64]; };
vector<Gad> gs; map<vector<int>,int> seen;
unsigned D[6],c;
void rec(int p){
    if(p==a){
        for(int i=1;i<=n;i++) if(!D[i]) return;
        Gad g; g.c=c; for(int i=0;i<=n;i++) g.D[i]=D[i];
        vector<int> s(full+1);
        for(int x=0;x<=full;x++){ unsigned w=c; for(int i=0;i<n;i++) if((x>>i)&1) w^=D[i+1]; s[x]=CAN[w]; g.sig[x]=s[x]; }
        for(int x=0;x<full;x++) if(s[x]==s[full]) return; // unusable
        if(seen.count(s)) return; seen[s]=1; gs.push_back(g); return;
    }
    for(int o=0;o<=n;o++){ if(p==0&&o!=1) continue;
        for(int bit=0;bit<2;bit++){ unsigned oc=c,od=D[o]; if(bit)c|=1u<<p; if(o)D[o]|=1u<<p; rec(p+1); c=oc; D[o]=od; } }
}
u64 H[1<<16]; u64 rx[64];
vector<u64> key;
bool verify(vector<int>&ids){
    for(int x=1;x<full;x++){ vector<int> A,B; for(int i:ids){A.push_back(gs[i].sig[x]);B.push_back(gs[i].sig[0]);} sort(A.begin(),A.end());sort(B.begin(),B.end()); if(A!=B) return false; }
    set<int> z0; for(int i:ids) for(int x=0;x<full;x++) z0.insert(gs[i].sig[x]);
    for(int i:ids) if(z0.count(gs[i].sig[full])) return false;
    return true;
}
long long nfound=0;
void report(vector<int> ids){
    if(!verify(ids)) return; nfound++; if(nfound>5) return;
    printf("FOUND b=%d:",b); for(int i:ids){ printf(" [c=");for(int p=0;p<a;p++)printf("%d",(gs[i].c>>p)&1); printf(" owner="); for(int p=0;p<a;p++){int o=0;for(int k=1;k<=n;k++) if((gs[i].D[k]>>p)&1)o=k; printf("%d",o);} printf("]"); } printf("\n"); fflush(stdout);
}
int main(int argc,char**argv){ n=atoi(argv[1]);a=atoi(argv[2]);b=atoi(argv[3]); full=(1<<n)-1;
    CAN.resize(1<<a); for(unsigned w=0;w<(1u<<a);w++){ unsigned best=w; for(int d=1;d<a;d++){ unsigned r=((w<<d)|(w>>(a-d)))&((1u<<a)-1); best=min(best,r);} CAN[w]=best; }
    rec(0);
    mt19937_64 rng(12345); for(auto&h:H)h=rng(); for(auto&r:rx)r=rng()|1;
    int G=gs.size(); key.resize(G);
    for(int g=0;g<G;g++){ u64 k=0; for(int x=1;x<full;x++) k+=rx[x]*(H[gs[g].sig[x]]-H[gs[g].sig[0]]); key[g]=k; }
    printf("n=%d a=%d usable signatures=%d\n",n,a,G); fflush(stdout);
    if(b==1){ for(int i=0;i<G;i++) if(key[i]==0) report({i}); }
    else if(b==2){ unordered_multimap<u64,int> mp; for(int i=0;i<G;i++) mp.insert({key[i],i});
        for(int i=0;i<G;i++){ auto r=mp.equal_range(-key[i]); for(auto it=r.first;it!=r.second;++it) if(it->second>=i) report({i,it->second}); } }
    else if(b==3){ unordered_multimap<u64,int> mp; for(int i=0;i<G;i++) mp.insert({key[i],i});
        for(int i=0;i<G;i++) for(int j=i;j<G;j++){ auto r=mp.equal_range(-(key[i]+key[j])); for(auto it=r.first;it!=r.second;++it) if(it->second>=j) report({i,j,it->second}); } }
    else if(b==4){ vector<pair<u64,unsigned>> ps; ps.reserve((size_t)G*(G+1)/2);
        for(int i=0;i<G;i++) for(int j=i;j<G;j++) ps.push_back({key[i]+key[j],(unsigned)i*20000u+j});
        sort(ps.begin(),ps.end());
        for(size_t t=0;t<ps.size();t++){ u64 want=-ps[t].first; auto lo=lower_bound(ps.begin(),ps.end(),make_pair(want,0u));
            for(auto it=lo;it!=ps.end()&&it->first==want;++it){ if(it->second<ps[t].second) continue; report({(int)(ps[t].second/20000),(int)(ps[t].second%20000),(int)(it->second/20000),(int)(it->second%20000)}); } } }
    printf("n=%d a=%d b=%d found=%lld\n",n,a,b,nfound);
}
