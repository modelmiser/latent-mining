// ADAPTIVE relaxation: Adder chooses a_t after seeing Firer's play (stronger Adder than in the problem).
// Adder may add at any vertex within distance D of the hull [lo,hi] of the current chips (first chip at 0).
// AdWin(c,r): Adder can force height >= H within r more rounds from position c (c = position after Firer's step).
// Gad(H;D) = least m with AdWin(empty,m).   If Firer survives m rounds here, Firer survives every LIST of
// length m all of whose adds obey the same distance-D rule relative to the chips present.
#include <bits/stdc++.h>
using namespace std;
int H, D;
unordered_map<string, int> winAt, failAt; // min r known win, max r known fail
long long nodes = 0;
static string canon(const string& c) { // strip zeros, reflect
    size_t a = c.find_first_not_of('\0');
    if (a == string::npos) return string();
    size_t b = c.find_last_not_of('\0');
    string s = c.substr(a, b - a + 1), t(s.rbegin(), s.rend());
    return min(s, t);
}
bool adwin(const string& c0, int r) {
    string c = canon(c0);
    if (r == 0) return false;
    auto iw = winAt.find(c); if (iw != winAt.end() && iw->second <= r) return true;
    auto il = failAt.find(c); if (il != failAt.end() && il->second >= r) return false;
    nodes++;
    int mx = 0; for (char ch : c) mx = max(mx, (int)ch);
    bool res = false;
    if (mx + r >= H) {
        int n = c.size();
        string p = string(D + 1, '\0') + c + string(D + 1, '\0'); // room for add and one firing
        int lo = c.empty() ? D + 1 : 1, hi = c.empty() ? D + 1 : n + 2 * D;
        for (int a = lo; a <= hi && !res; a++) {
            string q = p; q[a]++;
            if (q[a] >= H) { res = true; break; }
            // Firer options: pass or fire
            bool firerOK = !adwin(q, r - 1);
            for (int x = 1; x + 1 < (int)q.size() && !firerOK; x++) if (q[x] >= 2) {
                if (q[x-1] + 1 >= H || q[x+1] + 1 >= H) continue;
                string e = q; e[x] -= 2; e[x-1]++; e[x+1]++;
                if (!adwin(e, r - 1)) firerOK = true;
            }
            if (!firerOK) res = true;
        }
    }
    if (res) { auto& w = winAt[c]; if (!w || w > r) w = r; } else { auto& f = failAt[c]; f = max(f, r); }
    return res;
}
int main(int argc, char** argv) {
    H = atoi(argv[1]); D = atoi(argv[2]); int mmax = atoi(argv[3]);
    for (int m = 1; m <= mmax; m++) {
        bool w = adwin(string(), m);
        printf("H=%d D=%d m=%d : %s nodes=%lld states=%zu\n", H, D, m, w ? "ADDER WINS" : "firer survives", nodes, winAt.size() + failAt.size());
        fflush(stdout);
        if (w) break;
    }
}
