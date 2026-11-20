#!/bin/bash
# usage: run15.sh <parts...> ; runs fast3 3 15 part/8 sequentially for the given parts, one core
cd solver-b; export TMPDIR=$PWD; ulimit -v 8000000
for k in "$@"; do ( time nice -n 15 timeout 1500 ./fast3 3 15 $k 8 ) > m15.part$k.out 2>&1; echo "exit=$?" >> m15.part$k.out; done
