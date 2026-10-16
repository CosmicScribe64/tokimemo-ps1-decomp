# DATE func_80152268 (T-17204)

`body.c` needs `void func_80152684();` declared in `include/ovl/DATE.h` (the definition is old-style with `s16`
parameters and sits after the call; an `s16` prototype fails `check_headers`). The loop `do { } while (++i < 12)`
reproduces the original's loop (a temporary for `(s16)(i + 1)`, copy `or s0,t1,zero` in the branch delay slot);
the plain `for` does not. Left: the original keeps the s16 result of every `% n` in `$s0` (`mfhi s0`, narrowing into a
temporary, a dead copy `or s0,tN,zero` before each call) and the divisor 1000 / 800 in `$s1`; the draft puts `n` in `$s0`
and the result in `$a0` (its stores are dead and removed). Tried: the loop counter reused as the result, a separate s32
result, assignment expressions as call arguments, `register`, a K&R parameter, a trailing use, an s16 prototype.
See wiki/matching-notes.md, section T-17204.
