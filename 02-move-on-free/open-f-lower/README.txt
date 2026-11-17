open-f-lower/  --  adversary side of "is mu^F > 5/3 ?"            (2026-11-16)
Result: NOT PROVED.  See proofs.txt (statements tagged COMPLETE / COMPUTED / NOT PROVED).

code
  core.py          game-F referee (rule enforcement), layouts, waste, thinning DP (phi)
  managers.py      FF, FF+move (solver-b S1), PG = potential-greedy managers (optional reserved 3-gaps)
  adversaries.py   P1 (control), O-dp (control, frees only 1s), P6N (solver-b P6 in F), P6R (mine)
  wb.py            WB = manager designed against P6R (knows the script, plans all 2s against the
                   adversary's own end phase, reserves 3-gaps); P6RS = P6R choosing its kept set
                   from a menu by look-ahead
  battery.py, battery2.py   test (2b): adversaries x managers, M = 24..144
  brute.py         test (2a): exhaustive best response of the manager vs the fixed adversary P6R0
  thmA_check.py    Theorem A played out: proof-version adversary vs compact managers, M = 3..60
  pat.py           which periodic first layouts survive the game-O finalisation (only 3-gapped ones)
  pergame.py/.cpp  periodic density game (exact within its model; see proofs.txt section E)
                   binaries: pergame (exhaustive S), pergame2 (+FILTER/PRUNE), pergame3 (+FIXS, MAXIN)
  scan_patterns.py scan of 350 first layouts with pergame2
  trace.py         print one game

logs
  battery_r1.log, wb_r2_small.log, wb_r2_big.log    earlier rounds
  battery2.log, battery3.log                        final battery (M = 48, 96, 144; both stopped by me near the end, see last line of each)
  brute_3_11.log, brute_12_15.log, brute_16_17.log  test (2a)  (M=17 cut by the time limit)
  thmA_check.log                                    Theorem A check, 0 violations
  pergame_6.log pergame_3.log pergame_9.log         prototype runs (python)
  pergame_cpp_a.log pergame_batch.log pergame_33.log pergame_66_debug.log
  pergame_filter.log pergame_6666.log pergame_6x8.log pergame_6x8_maxin.log scan_patterns.log
  (pergame_batch.log: I stopped the batch after home=5; pergame_6x8.log: stopped after 571 of
   6561 placements, superseded by pergame_6x8_maxin.log)

limits: every solver and test run was niced (15), capped with ulimit -v 8000000 and a timeout, at most 3 cores in total; TMPDIR here.
Nothing outside this directory was written.  Folder open-f-upper was not read.
