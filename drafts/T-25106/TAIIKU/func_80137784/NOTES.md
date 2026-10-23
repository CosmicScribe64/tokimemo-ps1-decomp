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
costs a frame slot (64) and moves the spill slot. Taking the store pointer from the alias and comparing
with the real base on the left (`D_8014984C == &((Fix32 *)&D_8014985C - 4)[arg0]`, 265 rows; `[arg0 - 4]`, 309)
recolours the entry. Reusing `i` as the slot address (`i = (s32)&D_8014984C[arg0]`,
compare `i == (s32)X`) keeps frame 56 but recolours the entry (t0 for the runner): 86 rows, 318 with block 3 too.

## Next hypothesis

The original compared a pointer *variable* (variables go first) without a frame slot. Candidates:
a dead second parameter with its home in the caller's frame (the `i` reuse is measured negative), checked by frame size and the 0x30 spill slot; or a symbol
alias for the player's slot declared in `config/symbols_TAIIKU.txt` (config change, needs approval)
in place of the D_8014985C FAKE.

## T-25201 closer (fresh eyes, 2026-10-23)

Result: no match; the guard stays at 4 rows (the two compare operand orders). Measured the cause in
the ucode itself (uopt input and output dumped through a scratch wrapper around `/opt/ido/5.3/uopt`,
rendered with `cvt_pass.parse`; nothing in the tree changed).

How the order of `==` is decided, measured on this function:
- cfe keeps the source order for two non-variable operands (`D_8014984C == &D_8014984C[arg0]` enters as
  `lda; lda; lod arg0; ixa; equ`). With a plain variable it puts the complex operand first
  (`pos == X - 16` enters as `X - 16`, `pos`).
- uopt swaps when the first operand is not a plain variable. The guard's `ptr == X - 16` leaves uopt as
  `lda 85C-16; lod a0; equ` (constant first); natural `ptr == D_8014984C` leaves as base register first too.
- A constant addend on the second operand after the swap is moved to the first: `X + 0x44 == ptr`
  (from `D_80149808.v[17]`) leaves as `ptr - 0x44 == lda`, and the `-16` alias forms on the left
  (`&(&D_8014985C)[-2]`, `&((Fix32 *)&D_8014985C)[-4]`, `(u8 *)&D_8014985C - 16`, `D_8014985C.v[-8]`)
  all give `addiu t8,a0,16; bne t8,t9` (229 rows). With `(s32)` casts uopt keeps the add, but ugen moves
  it the same way (`(Fix32 *)((s32)&D_8014985C - 16)`, `-16 + (s32)...`: 229 rows).
- So pointer first needs the constant on the left in the source *and* a zero offset: only a second
  symbol at the same address can do that. `D_8014984C == &D_8014984C[arg0]` gives the right order
  (`lod a0; lod base`) but brings the base back as a register candidate (249 rows).

Allocator data (R9 lines of the traced uopt): the D_8014984C hoisted constant (bit 238) has units
bb9 (reload), bb11 (the compare, no reload), bb24 (reload): saving 1 over 2, priority 0.5, ahead of
0x44 (bit 230: bb0 reload, bb9, bb24, saving 2 over 5 blocks = 0.4). In the guard both base units
reload, so priority 0. Had 0x44 come first (equal priority keeps the lower bit 230), the base could
only take $ra at cost 1, and 1 > 1 fails: it would stay unallocated as in the original. No C lever
found that changes those counts without code.

Negatives (new): a block-scoped `Fix32 *pos` (plain or `register`) still costs a frame slot (64);
`pos == D_8014984C` also keeps the base allocated; `(s32)` differences or xor
(`a - b == 0`, `!(a - b)`, `(a ^ b) == 0`) recolour the entry (314 rows); `(u8 *)` difference adds two
instructions (228); the T-25106 alias store pointer with the real base on the left (265, the alias
constant becomes the candidate instead).

Positive control: `alias-8rows.c` compares against undefined zero-offset aliases
(`D_8014984C_alias == &D_8014984C[arg0]`, `D_8014981C_alias == &D_8014981C[arg0]`). Every instruction
then matches, both compares included; the 8 rows are only the four `R_MIPS_HI16/LO16` relocations of the
undefined aliases.

## Next hypothesis (T-25201)

Add two overlay-only symbols to a new `config/symbols_TAIIKU.txt` (for example
`D_8014984C_alias = 0x8014984C;` and `D_8014981C_alias = 0x8014981C;`, names to be chosen) and declare
them as FAKE views in `include/ovl/TAIIKU.h`. That is a config change and needs the user's approval. Check first
that splat and `check_symbols.py` accept two names at one address and that the disassembly keeps
`D_8014984C` for the other references. Then `alias-8rows.c` should be a match and the D_8014985C FAKEs go away.

Block counts behind those priorities (flow graph of the natural draft): 0x44 is live in bb0-bb4, the
clear loop and call path (bb5-bb8, bb31), bb9, the block-2 tests bb14-bb16 and bb24: N = 15, D = 5,
saving 2. The base covers bb9-bb11 and bb24: N = 4, D = 2, saving 1. A tie at 0.5 (0x44 wins on the
lower bit) needs 0x44 at N <= 13 or one more reload-free 0x44 unit; the base drops below 0.4 only at
N >= 6 without another reload-free unit. `do`/`while` forms of the clear loop change nothing (249 rows).
