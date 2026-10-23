# main func_8006A044 (src/main/800674B0.c), 648 bytes: matched (T-25103)

## Start state

Plain `INCLUDE_ASM`. Best body: `drafts/T-25012/main/func_8006A044/body-global.c` (T-24301 draft without its
FAKE), every update written as `D_800E6280.unk_F64[n] |= k;`. Measured in the full unit: 174 diff rows on
`funcdiff --resolve --version c`, 150 against 162 instructions.

## First divergence and the cause

Beyond the colour swap the earlier notes describe (month in `$v0`, flag byte in `$v1`), the listings disagree
in the load shape of every flag update. The original does `lui t9,%hi(F64); lbu t0,%lo(F64)(t9)`, a load into
another register than its `lui`. In the first test it is `lui v0; lbu t6,(v0); ori t7,t6,0x40; andi v0,t7,0xff`.
A plain `|=` gives `lui t3; lbu t3,(t3)` and loads the promoted byte straight into its home register. The
`lui rA; lbu rB,(rA)` shape is how IDO emits a bit-field store. Matched precedents: main `func_8007959C`
(`((Flags7D4F *)&D_800E7D4F)->b0 = 1`), and the `CharFlags` comment in `include/main_api.h`. The seven
`move tX,v0; ori; andi v0` copies that T-25013 put down to the allocator (lever 1/5) are part of the same
bit-field insert on the promoted byte.

## Changes (one at a time, rows on funcdiff --resolve --version c)

1. Every `unk_F64[n] |= k` becomes single-bit stores `((FlagsF64 *)D_800E6280.unk_F64)->bN = 1;`
   (`0x60` is `b13 = 1; b14 = 1;`): 174 -> 86 rows, 152 instructions. The load shapes and the copies
   before each `ori` now match. What is left is the month/byte colour swap and the last store of each case
   arm (`ori t,v1,k; sb t` against `move; ori; andi v0; sb v0`).
2. `s32` return with no `return` (K&R implicit int, as T-24301 found): no change on its own (86).
3. Dead read `x = D_800E6280.unk_F64[1];` at the top (FAKE). With `s32` it gives MATCH (0 rows, 162
   instructions). With `void` it gives 40 rows: the colours are right, but the last store of each arm keeps
   the direct form.

Controls: bit-fields only on F64[0] (plain `|=` on F64[1]) give 170 rows. `return D_800E6280.unk_F64[1];` at
the end gives 100 rows: it sinks every store-back to one exit store, keeps the month in `$v0` and adds a
`move v0,v1`.

## Final source

- `FlagsF64` in `include/main_api.h` (u32 single-bit fields b0..b15 plus `rest : 16`, next to `CharFlags`).
- `s32 func_8006A044();` in `include/main_api.h`. The one caller (`src/main/80075320.c`) ignores the result.
- The dead read, marked FAKE T-25103.
- Default build: MATCH 648 bytes; clean rebuild 27/27 sha1 OK; check_headers, migrate_globals --check and
  sync_protos --check-branch pass.

## Lesson for siblings

A promoted flag byte that shows `lui rA; lbu rB,(rA)` plus `move t,v0; ori; andi v0` per update is written
as bit-field stores, not `|=`. The other functions of this file that write `D_800E6280.unk_F64` or similar
flag bytes may need the same view.
