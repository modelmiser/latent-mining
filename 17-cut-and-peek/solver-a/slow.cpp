// Direct implementation of the definition. No reductions except none.
// usage: slow n m   -> prints number of valid (protocol) objects, and count restricted to
// "reduced" ones (u0!=u1 everywhere on players) with 0 in R, and one witness.
#include <bits/stdc++.h>
using namespace std;
int n,m;
int owner[32],b0[32],b1[32]; // owner 0 = H (b0 = card), else player, b0=u^0 bit, b1=u^1 bit
long long validAll=0, validRed0=0; bool printed=false;
vector<vector<int>> M(int x,int R){ // multiset of words as sorted list
    vector<int> w(m);
    for(int p=0;p<m;p++){ if(owner[p]==0) w[p]=b0[p]; else w[p]= ((x>>(owner[p]-1))&1)? b1[p]:b0[p]; }
    vector<vector<int>> res;
    for(int j=0;j<m;j++){
        vector<int> after(m);
        for(int p=0;p<m;p++) after[(p+j)%m]=w[p];
        vector<int> t;
        for(int q=0;q<m;q++) if((R>>q)&1) t.push_back(after[q]);
        res.push_back(t);
    }
    sort(res.begin(),res.end());
    return res;
}
void leaf(){
    // each player non-empty
    for(int i=1;i<=n;i++){ bool ok=false; for(int p=0;p<m;p++) if(owner[p]==i) ok=true; if(!ok) return; }
    bool red=true; for(int p=0;p<m;p++) if(owner[p]&&b0[p]==b1[p]) red=false;
    int full=(1<<n)-1;
    for(int R=1;R<(1<<m);R++){
        auto M0=M(0,R); bool ok=true;
        for(int x=1;x<full&&ok;x++) if(M(x,R)!=M0) ok=false;
        if(!ok) continue;
        auto M1=M(full,R);
        vector<vector<int>> inter;
        set_intersection(M0.begin(),M0.end(),M1.begin(),M1.end(),back_inserter(inter));
        if(!inter.empty()) continue;
        validAll++;
        if(red && (R&1)) validRed0++;
        if(!printed){printed=true; printf("witness: owner=");for(int p=0;p<m;p++)printf("%d",owner[p]);
            printf(" u0=");for(int p=0;p<m;p++)printf("%d",b0[p]);printf(" u1=");for(int p=0;p<m;p++)printf("%d",owner[p]?b1[p]:b0[p]);
            printf(" R=");for(int q=0;q<m;q++)if((R>>q)&1)printf("%d,",q);printf("\n");}
    }
}
void rec(int p){
    if(p==m){leaf();return;}
    owner[p]=0; for(int b=0;b<2;b++){b0[p]=b1[p]=b; rec(p+1);}
    for(int i=1;i<=n;i++){owner[p]=i; for(int a=0;a<2;a++)for(int b=0;b<2;b++){b0[p]=a;b1[p]=b;rec(p+1);}}
}
int main(int argc,char**argv){ n=atoi(argv[1]); m=atoi(argv[2]); rec(0);
    printf("slow n=%d m=%d validAll=%lld validReduced0inR=%lld E=%d\n",n,m,validAll,validRed0,validAll>0); }
