# Block-copy state cases

One-function C files, no game code. Each has a 0x44-byte struct copy, which ugen emits as a 12-byte loop.
Its end register is the source register plus 2 (state F) or plus 3 (state B) in ugen's register rotation
t6..t9, t0..t5 (T-19001). The header of each file gives the measured states. Pages:
`wiki/ido-52-evaluation.md`, sections "T-20710" and "T-25608".

Ucode notation in the headers: `I` indirect store (`Uistr`/`Uistv`), `S` direct store, `C` call, `u` `Ujp`,
`f`/`t` `Ufjp`/`Utjp` with `[compare constant]` when a compare against a constant feeds it, `<` when the
build's detector counts the branch as a signed-immediate `<`, `M` the copy. Rules: build = `tools/ugen_pass.py`
on w2-good; A = no `fjp`/`tjp` between the store and the `<` branch; A with ujp = A, and no `ujp` either;
B = the `<` branch needs a constant of 2 or more.

T-20710 (copy placement, IDO 5.3, 5.2 and 4.1):

- `copy_entry.c`: copy at procedure entry.
- `copy_after_loop.c`: copy after a store loop.
- `copy_after_if.c`: copy after `if (n) f();`.
- `copy_after_istr.c`: copy after stores through a pointer.
- `copy_after_store.c`: copy after a loop and a global store.
- `copy_after_call.c`: copy after a call.

T-25608 (the candidate rules against IDO 4.1's ugen):

- `copy_if_then_lt8_loop.c`: store, `if (n)`, then a `G < 8` loop. Separates A from B.
- `copy_store_loop_then_lt8_loop.c`: store inside a loop on `n`, then a `G < 8` loop. Separates A from B.
- `copy_if_then_ge2.c`: store, `if (n)`, then `if (n >= 2)`. Separates A from B.
- `copy_ge1.c`: store, then `if (n >= 1)`. Separates A from B.
- `copy_gt0.c`: store, then `if (n > 0)`. Separates A from B.
- `copy_lt1_loop.c`: store, then a `G < 1` loop. Separates A from B.
- `copy_var_loop_guard.c`: store, then a loop on `n` (guard `n >= 1`, back branch `i != n`).
- `copy_ge2.c`: store, then `if (n >= 2)`. Every rule gives F (control).
- `copy_lt8_loop.c`: store, then a `G < 8` loop. Every rule gives F (control).
- `copy_lt2_loop.c`: store, then a `G < 2` loop. Every rule gives F (control).
- `copy_while_lt8.c`: store, then `while (G < 8)` (uopt adds a guard; no ujp is left).
- `copy_ujp_then_lt8_loop.c`: store in an if arm, its `ujp`, then the `G < 8` loop branch. Separates A from A with ujp.
- `copy_store_loop_then_var_loop.c`: store inside a loop on `n`, then a second loop on `n`. The shape of main `func_8006B0C8`.
- `copy_lt8_loop_then_call.c`: store, a `G < 8` loop, a call and a global store.
- `copy_store_then_gstore.c`: store, then a global store.
- `copy_store_then_call.c`: store, then a call.
- `copy_store_then_if.c`: store, then `if (n) f();` (`beqz`).
- `copy_store_then_ne_loop.c`: store, then a `G != 3` loop (uopt keeps 3 in a register).
- `copy_store_then_ge8_loop.c`: store, then a `G >= 8` loop.
- `copy_store_then_ne3_if.c`: store, then `if (n != 3)`.
- `copy_store_then_eq_reg_if.c`: store, then `if (n == G)`.
- `copy_store_then_lt_reg_if.c`: store, then `if (n < G)`.
- `copy_two_after_store.c`: store, then two copies.
- `copy_after_proc_store.c`: the previous procedure ends with a store, and the copy is at entry.

T-26000 (branches the 30 cases above leave out; IDO 4.1 against the pass's rule, which fits the 30):

- `copy_ne0_left_if.c`: `odd != 0` that uopt writes with the constant on the left, as in main `strCallback`. 4.1 B,
  pass B (B from the entry).
- `copy_store_then_ne0_left_if.c`: the same after an indirect store. 4.1 F, pass F: a compare with a constant on
  either side keeps the state (the pass's first T-26000 version gave B).
- `copy_store_then_eq0_left_if.c`: an indirect store, then `if ((G1 & 1) == 0)` (`ldc 0` on the left). 4.1 F, pass F.
- `copy_value_and_if.c`: `if (G1 & 1)`, a branch on a value. 4.1 F, pass B.
- `copy_value_ilod_if.c`: `if (P[1])`, a branch on a loaded byte. 4.1 F, pass B.
- `copy_value_reg_if.c`: `if ((G1 & 1) && G2)`, the test main `strCallback` has since T-26000. 4.1 F, pass B.
- `copy_bitfield_if.c`: a one-bit field test (`sll; bgez`), the shape before main `func_8006B0C8`'s compaction copy.
  4.1 F, pass B; the original has B there, so the pass keeps the state at a branch on a value.
- `copy_dropped_arm_store.c`: the never-true FAKE arm `if ((unsigned)G < 0) { *(unsigned char *)0 = 0; }`, which
  ugen drops. 4.1 B, 5.3 B, pass F: 4.1 ignores the store, the pass counts it (the FAKE arms rely on that).
- `copy_live_arm_zero_store.c`: control, the same store in an arm ugen keeps (`if (G1 < 0)`). 4.1 F, pass F.

Ucode notation added for these: `(R)` a value uopt keeps in a register, `[0 neq R]` a compare with the constant on
the left.

Harness: `drafts/T-25608/ido41/` (`cases.sh` runs every case through IDO 4.1's ugen, 5.3's ugen and the
rules; `run41.py CASE 41 raw tools/ugen_pass.py` and `utrace.py` run one case against the build's pass). IDO 4.1
itself is never committed (`wiki/ido-52-evaluation.md`, "Provenance"); in a worktree, mount the gitignored
`tools/local-compilers` of the main checkout read-only.
