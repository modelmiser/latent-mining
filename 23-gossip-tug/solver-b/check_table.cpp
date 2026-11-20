// Independent checker of a value table. Reads lines "v K1..Kn" (hex).
// For every entry: recompute max over active a of min over legal b of 1+value(child),
// where value(child) is found by trying ALL n! relabelings of the child until one is in the table
// (terminal child = 0). Fails if any child has no isomorphic entry, or values disagree.
// Also checks the initial position is (isomorphic to) an entry and prints its value.
#include <bits/stdc++.h>
using namespace std;
typedef unsigned long long u64;
int n; unordered_map<u64,int> T; vector<vector<int>> perms;
u64 pack(const int*K){u64 s=0;for(int i=0;i<n;i++)s|=(u64)K[i]<<(8*i);return s;}
int lookup(const int*K){
    bool full=true; for(int i=0;i<n;i++) if(K[i]!=(1<<n)-1) full=false;
    if(full) return 0;
    for(auto&p:perms){ // agent i -> new label p[i]
        int L[8];
        for(int i=0;i<n;i++){int m=0;for(int j=0;j<n;j++)if(K[i]>>j&1)m|=1<<p[j]; L[p[i]]=m;}
        auto it=T.find(pack(L)); if(it!=T.end())return it->second;
    }
    return -1;
}
int main(int argc,char**argv){
    n=atoi(argv[1]); ifstream f(argv[2]); string line;
    vector<pair<vector<int>,int>> E;
    while(getline(f,line)){ if(line.empty()||line[0]=='#')continue; istringstream ss(line); int v; ss>>v; vector<int>K(n); for(int i=0;i<n;i++){ss>>hex>>K[i];} 
        for(int i=0;i<n;i++) if(!(K[i]>>i&1)||K[i]>=(1<<n)){puts("FAIL: bad row");return 1;}
        E.push_back({K,v}); if(T.count(pack(K.data()))){puts("FAIL: duplicate");return 1;} T[pack(K.data())]=v; }
    vector<int> p(n); iota(p.begin(),p.end(),0); do perms.push_back(p); while(next_permutation(p.begin(),p.end()));
    long checked=0;
    for(auto&e:E){
        int best=-1;
        for(int a=0;a<n;a++){ int mn=1000;
            for(int b=0;b<n;b++){ if(a==b)continue; if((e.first[a]>>b&1)&&(e.first[b]>>a&1))continue;
                int K[8]; for(int i=0;i<n;i++)K[i]=e.first[i]; K[a]=K[b]=e.first[a]|e.first[b];
                int v=lookup(K); if(v<0){printf("FAIL: child missing (entry %ld)\n",checked);return 1;}
                mn=min(mn,1+v);} 
            if(mn<1000)best=max(best,mn);}
        if(best!=e.second){printf("FAIL: entry %ld has %d, recomputed %d\n",checked,e.second,best);return 1;}
        checked++;
    }
    int K0[8]; for(int i=0;i<n;i++)K0[i]=1<<i;
    int v=lookup(K0); if(v<0){puts("FAIL: initial position missing");return 1;}
    printf("OK n=%d entries=%ld A(n)=%d\n",n,checked,v);
}
