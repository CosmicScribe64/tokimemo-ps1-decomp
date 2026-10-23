# T-24909 ETC func_8014790C (cancelled run)

Best body from the cancelled T-24909 run, removed from the tree because it is above the ~30-row
guard rule (about 44 resolved rows at cancellation; the pre-run baseline was 57 rows after
T-24706). Structure: two clear loops over the GameState text buffers, the px/py/pv/r walk, the
D_8011ECD0 double materialization (`lui s0` walk vs `move s0,s6`), switch compares in `$v0` vs
`$v1`. Reuse this body as the starting point for the next Class C run; do not restart from m2c.
