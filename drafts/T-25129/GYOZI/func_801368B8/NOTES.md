# GYOZI func_801368B8 (2256 bytes): matched, T-25129

Start: plain INCLUDE_ASM. Best earlier body was the T-24707 draft (765 funcdiff rows), which reached the
display records through two symbols (`D_80129F40` and `D_8012B8C0`) and wrote the actor-0x60 words
through the `DisplayFixActor44` view of `D_8012B8C0`.

Result: MATCH in the default build (C body, no guard), about 65 minutes. The body is in
`src/ovl/GYOZI/80136220.c`. Rows by step (scratch row count, then funcdiff):

1. Every display-record access through `D_80129F40` (`DisplayActor44` for bytes and halves,
   `DisplayFixActor44` for the +0x24/+0x2C words of actor 0x60): 765 -> 222. Reason: the words at
   +0x19A4/+0x19AC overlap the halves at +0x19A6/+0x19AE, so uopt keeps every access to that symbol
   indirect. The actor 0x61/0x62 `+= 1/2/4` increments are then memory operations, the third and
   second speed tests reload `unk_2E` after them (folded `%lo(D_8012B8EE)`), and the base lands in a
   register only where the original has `lui v0; addiu v0,%lo(D_80129F40)`. With `D_8012B8C0` for
   any of these, uopt promotes the increments into registers and the reloads go away.
2. Case 4 as `if (stop) { ... D_800F6474++; } else { move ... } return 0;`: 222 -> 34. With
   `return 0` inside the stop arm, the base address range is used in a block that also writes the
   return value, so uopt gives the base `$v1` everywhere (one range across cases 3, 4, 5 and 8).
   Measured: replacing every `return 0` with `return` gives the base `$v0` (82 rows); a case-4 use of
   the base in a `return 0` block flips it back, a use in a `break` block does not.
3. `s32 pad = D_800F647D;` (FAKE, the frame slot at 0x7C): 34 -> 12 scratch. tools/cvt_pass.py
   removes the CVT of an unsigned global only when it is referenced before the first call or branch;
   with it, case 5 keeps `D_800F647D` in `$v1` with no `move` copy (old value tested with
   `xori v0,v1,0x1E`).
4. Case 6: `D_800F6474++;` before `D_8012E674 = 1;` inside the if: the increment moves into the
   head block as in the original (2 funcdiff rows left).
5. Case 8 head (`lui v1` above `sb t8,D_8012B8C3`): add `D_80146178` to the dead entry read
   (`pad = D_800F647D + D_80146178`) and index the name table as a 2-D array,
   `D_8014617C[D_80146178]` with `extern u8 D_8014617C[][5]` (seven 5-byte names). The index load goes
   straight into an array index, so no CVT copy is made, the global stays one range in `$v1`, and as1
   schedules its `lui` above the store. 2 -> 0. func_80137188 now uses the same row index and still
   matches.

Negatives measured here: a local pointer `p = (DisplayFixActor44 *)D_80129F40` for all accesses (621
rows); `return ret = 0`; `ret = 0; goto end;` (ret gets a stack home); a local copy of the compared
value (no change); line merges and other symbols for the case 8 stores (no change); the entry read
of `D_80146178` without the 2-D index (31 rows: `$a0` plus a copy); autotweak 300 s on the 2-row
guard (nothing).

Sibling: SHOUGATU func_801350F8 has the same case-8 head and switch shape (base `D_8011ECD0`,
selector `D_80143DD8`); the five steps above should carry over.
