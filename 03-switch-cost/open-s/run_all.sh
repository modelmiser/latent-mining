#!/bin/bash
# reproduces results.txt ; every run: nice 15, ulimit -v 8000000, timeout 1500, one core
cd "$(dirname "$0")"; export TMPDIR=$PWD
R() { (ulimit -v 8000000; timeout 1500 nice -n 15 "$@" 2>&1 | cut -c1-330 | head -4); }
g++ -O2 -o play_cpp play.cpp
echo "== 1. brute-force feasibility search vs naive enumerator"; R python3 test_brute.py
echo "== 2. exhaustive play, python (play.py), proven Delta=(P-1)(3P-1); --brute --exact = no memo, brute-force promise check of the raw job list at every release, exact lateness search at every terminal"
R python3 play.py 2 5 --brute --exact; R python3 play.py 3 16 --brute --exact; R python3 play.py 4 33
echo "== 3. exhaustive play, C++ (play.cpp, independent implementation), proven Delta"
for a in "2 5" "3 16" "4 33" "5 56"; do R ./play_cpp $a; done
echo "== 4. smallest Delta that passes / first that fails (C++, exhaustive)"
for a in "2 5" "2 4" "3 12" "3 11" "4 21" "4 20" "5 32" "5 31"; do R ./play_cpp $a | head -1; done
echo "== 5. negative controls (must FAIL)"
echo "-- nothreat: fork without the tight unit"; R ./play_cpp 3 16 nothreat | head -1; R ./play_cpp 5 56 nothreat | head -1; R python3 play.py 4 33 nothreat | head -1
echo "-- notight: fork unit gets the stream deadline instead of t+1"; R python3 play.py 4 33 notight | head -1
echo "-- fewrounds: P-2 rounds instead of P-1"; R ./play_cpp 4 33 fewrounds | head -1; R ./play_cpp 5 56 fewrounds | head -1
echo "-- Delta too small: promise checker must object"; R ./play_cpp 5 20 | head -1
echo "== 6. P >= 6: NOT exhaustive. (a) every scheduler with <= MAXDEV deviations from non-preemptive EDF (C++)"
for a in "6 85 ok 8" "6 45 ok 9" "7 120 ok 7" "7 60 ok 7" "8 161 ok 6" "8 77 ok 6"; do R ./play_cpp $a; done
echo "-- controls inside the restricted class (must FAIL)"; R ./play_cpp 6 85 nothreat 7 | head -1; R ./play_cpp 6 85 fewrounds 7 | head -1; R ./play_cpp 7 59 ok 6 | head -1
echo "== 6. (b) sampled random/structured schedulers (sample.py), 20000 plays each"
for a in "5 56" "6 85" "7 120" "8 161" "6 45" "7 60" "8 77"; do R python3 sample.py $a 20000; done
echo "-- note: sampling is weak for the promise: P=5 Delta=31 passes sampling but fails exhaustively"; R python3 sample.py 5 31 20000
