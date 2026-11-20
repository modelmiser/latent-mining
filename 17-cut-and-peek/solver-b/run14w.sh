#!/bin/bash
cd solver-b; export TMPDIR=$PWD; ulimit -v 8000000
for k in 0 1; do ( time nice -n 15 timeout 1500 ./fast3 3 14 $k 2 ) > m14w.part$k.out 2>&1; echo "exit=$?" >> m14w.part$k.out; done
