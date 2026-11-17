// Layered state search for slip-proof / deletion-proof words, reverse input only.
// Usage: fast n L mode(slip|del) [noprune]
// State after a prefix: (p, Q): p = image of reverse input under the prefix (no slip),
// Q = set of images of reverse input under the prefix with exactly one slip (or deletion) made.
// Letter c_i: p' = c_i p ; Q' = c_i(Q) U {c_{i-1} p, c_{i+1} p}   (del mode: U {p}).
// Accept at length L iff p = id and Q subset {id}.
// The test uses ONLY the reverse input, which is a NECESSARY condition for slip-proofness,
// so "no word of length L" is a rigorous lower bound without belief (a). Witnesses are
// re-checked on all inputs by check.py.
// Pruning (safe): with r letters left, drop a state if inv(p) > r or some q in Q has inv(q) > r
// (each comparator removes at most one inversion). With 'noprune' no pruning is done.
// mode up: only slips c_i -> c_{i+1} (class U(n) of proofs.txt). NOKEEP=1 env: free old layers (no witness).
// Counts = number of words of length L accepted (exact, 128-bit).
#include <bits/stdc++.h>
using namespace std;
typedef unsigned __int128 u128;
int n, NP;
vector<vector<uint16_t>> ap; vector<uint8_t> invc;
struct Info { u128 cnt; uint32_t parent; uint8_t letter; };
string u128s(u128 x){ if(!x) return "0"; string s; while(x){ s+=char('0'+(int)(x%10)); x/=10;} reverse(s.begin(),s.end()); return s; }
int main(int argc,char**argv){
  n=atoi(argv[1]); int L=atoi(argv[2]); string mode=argv[3]; bool prune=!(argc>4);
  bool del=(mode=="del"); bool up=(mode=="up"); bool nokeep=getenv("NOKEEP")!=0;
  vector<int> perm(n); iota(perm.begin(),perm.end(),0);
  map<vector<int>,int> rank; vector<vector<int>> perms;
  do { rank[perm]=perms.size(); perms.push_back(perm);} while(next_permutation(perm.begin(),perm.end()));
  NP=perms.size(); ap.assign(n-1,vector<uint16_t>(NP)); invc.assign(NP,0);
  for(int k=0;k<NP;k++){ auto v=perms[k]; int c=0; for(int a=0;a<n;a++)for(int b=a+1;b<n;b++) if(v[a]>v[b])c++; invc[k]=c;
    for(int i=0;i<n-1;i++){ auto w=v; if(w[i]>w[i+1]) swap(w[i],w[i+1]); ap[i][k]=rank[w]; } }
  int ID=0, REV=NP-1;
  // DOM=1 env: exact state reduction. q <= q' (Bruhat/0-1 dominance: for all thresholds t and all j,
  // #{wires<=j with value>=t} of q <= that of q') implies every suffix sorting q' sorts q (proofs.txt L1(2)).
  // So drop from Q every q dominated by p or by another member of Q. Acceptance is unchanged.
  bool dom=getenv("DOM")!=0; vector<vector<bool>> le;
  if(dom){ vector<vector<uint8_t>> sig(NP); for(int k=0;k<NP;k++){ auto&v=perms[k]; for(int t=1;t<n;t++){ int c=0; for(int j=0;j<n-1;j++){ c+=(v[j]>=t); sig[k].push_back(c);} } }
    le.assign(NP,vector<bool>(NP)); int S=sig[0].size();
    for(int a=0;a<NP;a++)for(int b=0;b<NP;b++){ bool ok=true; for(int z=0;z<S;z++) if(sig[a][z]>sig[b][z]){ok=false;break;} le[a][b]=ok; } }
  auto reduce=[&](uint16_t p, vector<uint16_t>&Q){ if(!dom) return; vector<uint16_t> R; for(auto q:Q){ bool d=le[q][p]; for(size_t z=0;z<Q.size()&&!d;z++) if(Q[z]!=q && le[q][Q[z]]) d=true; if(!d) R.push_back(q);} Q.swap(R); };
  // layers: vector of (key string) + info; key = p,Q as uint16 list
  vector<vector<pair<string,Info>>> layers; vector<size_t> sizes;
  auto mk=[&](uint16_t p, const vector<uint16_t>&Q){ string s; s.resize(2+2*Q.size()); memcpy(&s[0],&p,2); if(!Q.empty()) memcpy(&s[2],Q.data(),2*Q.size()); return s; };
  layers.push_back({{mk(REV,{}),Info{1,0,0}}});
  for(int t=0;t<L;t++){
    unordered_map<string,uint32_t> idx; vector<pair<string,Info>> next;
    auto &cur=layers[t]; int r=L-t-1;
    for(uint32_t s=0;s<cur.size();s++){
      const string&k=cur[s].first; uint16_t p; memcpy(&p,&k[0],2);
      int m=(k.size()-2)/2; vector<uint16_t> Q(m); if(m) memcpy(Q.data(),&k[2],2*m);
      for(int i=0;i<n-1;i++){
        uint16_t p2=ap[i][p]; if(prune && invc[p2]>r) continue;
        vector<uint16_t> Q2; Q2.reserve(m+2); bool ok=true;
        for(auto q:Q){ uint16_t x=ap[i][q]; if(prune&&invc[x]>r){ok=false;break;} Q2.push_back(x);} 
        if(!ok) continue;
        if(del){ if(prune&&invc[p]>r) continue; Q2.push_back(p);} 
        else { for(int j:{i-1,i+1}) if(j>=0&&j<n-1&&!(up&&j==i-1)){ uint16_t x=ap[j][p]; if(prune&&invc[x]>r){ok=false;break;} Q2.push_back(x);} if(!ok) continue; }
        sort(Q2.begin(),Q2.end()); Q2.erase(unique(Q2.begin(),Q2.end()),Q2.end());
        reduce(p2,Q2); string k2=mk(p2,Q2);
        auto it=idx.find(k2);
        if(it==idx.end()){ idx.emplace(k2,(uint32_t)next.size()); next.push_back({k2,Info{cur[s].second.cnt,s,(uint8_t)i}}); }
        else next[it->second].second.cnt+=cur[s].second.cnt;
      }
    }
    fprintf(stderr,"layer %d states %zu\n",t+1,next.size());
    sizes.push_back(next.size()); layers.push_back(move(next)); if(nokeep){ vector<pair<string,Info>>().swap(layers[t]); }
    if(layers.back().empty()) break;
  }
  u128 total=0; long found=-1;
  if((int)layers.size()==L+1) for(uint32_t s=0;s<layers[L].size();s++){ const string&k=layers[L][s].first; uint16_t p; memcpy(&p,&k[0],2);
    bool ok=(p==ID); int m=(k.size()-2)/2; for(int a=0;a<m&&ok;a++){ uint16_t q; memcpy(&q,&k[2+2*a],2); if(q!=ID) ok=false; } /* with DOM, Q is empty when p=ID and all q=ID */
    if(ok){ total+=layers[L][s].second.cnt; if(found<0) found=s; } }
  printf("n=%d mode=%s L=%d prune=%d accepted_words=%s layer_sizes:",n,mode.c_str(),L,(int)prune,u128s(total).c_str());
  for(size_t z:sizes) printf(" %zu",z);
  printf("\n");
  if(found>=0 && !nokeep){ vector<int> w; uint32_t s=found; for(int t=L;t>0;t--){ w.push_back(layers[t][s].second.letter+1); s=layers[t][s].second.parent; }
    reverse(w.begin(),w.end()); printf("witness:"); for(int x:w) printf(" %d",x); printf("\n"); }
}
