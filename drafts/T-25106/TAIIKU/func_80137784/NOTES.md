# T-25106: TAIIKU func_80137784 (1216 bytes, 304 words)

Result: no match. The guard in `src/ovl/TAIIKU/80133C80.c` (copy: `guard-4rows.c`) is at 4 rows,
which is 2 instructions: the operand order of the two pointer compares. Frame, all uopt colours,
the ugen temporaries and the schedule match everywhere else. `natural-238rows.c` is the same body
with natural compares (`&D_8014984C[arg0] == D_8014984C`); it has no alias FAKE but 238 rows.
The header declares the four slot objects (`D_8014981C..D_8014984C`, 16 bytes each) for the guard.

Measured with `tools/funcdiff.py func_80137784 --unit TAIIKU --resolve --version c` (whole unit).

## Start state

INCLUDE_ASM. Best old body was T-24521 `phase-cursors.c`: 391 rows, frame 88 against 56.

## What closed it, in order

1. Shared 0x44 stride (391 -> 317). The original multiplies the runner's record index and arg0 (twice)
   by one `li t5,68`, but the D_801491D8 record by shifts. The runner index must be signed,
   `DISP(46 + (s32)RUN(arg0).unk_02)`, and the D_801491D8 record unsigned, `(&DISP(46))[D_801491D8]`.
   A u8 index without the cast takes its own unsigned constant and is strength-reduced.
2. No runner/sel locals (317 -> 309). `RUN(arg0)` as an expression: uopt keeps the CSE'd runner
   address in t1 and spills it to the temp slot 0x30 across `k_reset`, and reloads `unk_02` after the call
   exactly as the original (`lw t1,48(sp); lbu t2,2(t1)`). A pointer local alias-kills the `unk_02` CSE
   at the first byte store.
3. Store order `unk_02 = 1;` before `unk_03 = 0xA4;` (309 -> 303): uopt emits them the other way round.
4. Frame 56 needs exactly one declared local (`i`, the clear-loop counter). Measured: each declared scalar
   adds 4 bytes (1 local: 56, 3: 64, 4: 72, 6: 80). So sx, ex, value, acc and cnt are CSE expressions in the
   original, and the stored slot value is re-read (`if (D_8014982C[arg0] > 0)`, uopt forwards the store).
   Writing blocks 1 and 3 that way: 303 -> 240; block 2 too, in store order 982C, 983C, 984C: 238, frame 56.
   With expressions the bit order is (s16)sel, sx, ex, value, which gives a1, a2, a3, t0 as in the original.
5. `-((s16)sel / 10) * 16 - 0x30` gives `negu t8; sll` (a literal `* -16` gives `negu at`).
6. Block 3: `DISP(96 + arg0).unk_26 + D_8014984C[arg0].h[1]` (operand order of the sum) and the
   `unk_28.w` copy before the `unk_02 = 0x40; unk_03 = 0xA4;` stores.

## The blocker: slot base constants (regdiff)

With natural compares, uopt keeps the address constants D_8014984C and D_8014981C in registers
(priority 0.5 each: one free unit at the compare after the pointer's unit), ahead of the 0x44 constant
(0.4, 2 saved / denominator 5). D_8014984C takes t5, 0x44 falls to ra, D_8014981C takes a2 and
D_80122E94 falls to a3; every later register shifts. The original materialises both bases with
`lui/addiu` at each use. Probes:
- `if (arg0 == 0)` instead of the compares: 0x44 -> t5, D_80122E94 -> a2 (as original).
- integer literal compares (`(s32)p == 0x8014984C`): same colouring, but `lui at/ori`.
- a compare constant from another symbol (`(Fix32 *)&D_8014985C - 4`, also `D_80149814 + 14`,
  `D_80149808 + 17`): the base candidates lose their free unit, everything matches but the compare order.
- Not working: `&D_8014984C[0]`, `(s32)` casts, defining the four objects in C (plain or `static`),
  scalar `(&D)[arg0]` declarations, one 2D array `s32 [4][4]` (one candidate, priority 1, worse),
  `D_8014983C + 4` (D_8014983C gets promoted instead).
- uopt keys these candidates by symbol (`D_8014983C + 16` reused the D_8014983C register plus 16), so a
  natural spelling with the same symbol cannot split them; ETC func_8013B98C and TACO (cheatsheet row on
  `&D_801274D0`) matched with a separate element symbol in the same situation.

## First divergence left

Insn 119 and 263: original `bne a0,t8` / `bne a1,t9` (pointer first), the guard emits `bne t8,a0` /
`bne t9,a1`. Measured negatives for the operand order: constant on the left (uopt moves the -16 to the
other side: `addiu t8,a0,16; bne t8,t9`), `!(p != X)`, `.w` member addresses, `X + 0`, `u8 *` byte
offsets, other alias symbols, `(s32)p | 0` wrapper (breaks the entry colouring, 282 rows),
`D_8014984C + arg0` (286 rows). A declared pointer local gives pointer-first order (as in T-24521) but
costs a frame slot (64) and moves the spill slot. Reusing `i` as the slot address (`i = (s32)&D_8014984C[arg0]`,
compare `i == (s32)X`) keeps frame 56 but recolours the entry (t0 for the runner): 86 rows, 318 with block 3 too.

## Next hypothesis

The original compared a pointer *variable* (variables go first) without a frame slot. Candidates:
a dead second parameter with its home in the caller's frame (the `i` reuse is measured negative), checked by frame size and the 0x30 spill slot; or a symbol
alias for the player's slot declared in `config/symbols_TAIIKU.txt` (config change, needs approval)
in place of the D_8014985C FAKE.
