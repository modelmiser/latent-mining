// check.cpp -- check explicit protocols read from stdin (one per line, text format below) up to N.
// usage: check N [verbose] < file     line format:  s=3 qA=0 qB=1 w=ABA d: 0,1->2,2 1,0->2,2 ...   (missing = identity)
// prints for each protocol the largest n<=N such that all instances with a+b<=n are won, and the first failing instance.
#include "pp.h"
#include <iostream>
#include <sstream>
int main(int argc, char** argv) {
    int N = atoi(argv[1]); string line;
    while (getline(cin, line)) {
        if (!line.empty() && line[0] == (char)35) continue;
        size_t pos = line.find("s="); if (pos == string::npos) continue;
        Proto P; istringstream is(line.substr(pos)); string t; bool init = false;
        while (is >> t) {
            if (t.rfind("s=", 0) == 0) { P.s = atoi(t.c_str() + 2); for (int p = 0; p < P.s; p++) for (int q = 0; q < P.s; q++) P.d[p][q] = p * P.s + q; init = true; }
            else if (t.rfind("qA=", 0) == 0) P.qA = atoi(t.c_str() + 3);
            else if (t.rfind("qB=", 0) == 0) P.qB = atoi(t.c_str() + 3);
            else if (t.rfind("w=", 0) == 0) { for (int q = 0; q < P.s; q++) P.w[q] = (t[2 + q] == 'B'); }
            else if (t.find("->") != string::npos) { int p, q, p2, q2; if (sscanf(t.c_str(), "%d,%d->%d,%d", &p, &q, &p2, &q2) == 4) P.d[p][q] = p2 * P.s + q2; }
        }
        if (!init) continue;
        Checker C; C.P = &P; int best = 1; long long mx = 0; bool failed = false;
        for (int n = 2; n <= N && !failed; n++) {
            for (int a = 0; a <= n && !failed; a++) { int b = n - a; if (abs(a - b) < 2) continue;
                for (int x = 0; x < P.s; x++) { int r = C.checkInstance(a, b, x); mx = max(mx, C.lastNodes);
                    if (r) { printf("wins up to N=%d ; FAILS at (a=%d,b=%d,x=%d): %s ; maxR=%lld\n", best, a, b, x, C.failWhy.c_str(), mx); failed = true; break; } } }
            if (!failed) best = n;
        }
        if (!failed) printf("wins up to N=%d (limit reached) ; maxR=%lld\n", best, mx);
        fflush(stdout);
    }
}
