# T-25126: ENDING func_80135F4C (src/ovl/ENDING/80134D20.c)

Result: matched. Default C, 3220 bytes (805 instructions; the 12-entry jump table
jtbl_8013C010 is shared rodata). Clean rebuild 27/27 sha1 OK.

## Start state

NON_MATCHING guard from T-24047 (cost 35). Everything matched except the switch head:
as1 moved the default arm's `lui t7,%hi(D_8013C294)` into the selector's load delay
(the original keeps a `nop`) and `lui t2,%hi(D_8013C2D8)` into the `beqz` delay slot
(the original has the dispatch `sll t3,t3,2`). The rest of the rows were the branch
offsets that this two-word shift moves. T-24047 had ruled out the case set, block
order, volatile views, backend versions and about 118 source forms.

## What decided it

The T-25150 rule (wiki/as1-scheduler-rule.md, rule 8): as1 gives every block that ends in
an indirect jump an edge to every address-taken label in the file, and a function named by
a `.word` in the file's `.data` is address-taken. Object 80134D20 owns `.data`
8013C5F0-8013CAA0 (config/objects/ENDING.txt), and its function table `D_8013CA3C`
(EdFnTbl9, read by func_80135DD0) names func_80135F4C. With that data in asm, as1 never
saw the reference.

1. Probe: one extra C word `void *t[] = { func_80135F4C };` in the unit. The unresolved
   funcdiff showed only naming rows (the branch words were equal); the resolved link moved
   only because the probe added data.
2. `tools/data_island.py ENDING/80134D20` (yaml island, 272 INCLUDE_RODATA lines; ENDING
   stayed byte-identical), then `D_8013CA3C` defined in C with its nine entries
   (func_80135ECC, func_80136EE4, func_8007C784, func_80137128, func_8007C784,
   func_80135F4C, func_80136BE0, func_80042808, func_80046500). Prototypes for the five
   ENDING functions added to include/ovl/ENDING.h. func_8007C784 already had a link name
   (T-25150).
3. funcdiff --version c: MATCH; guard removed; --version default: MATCH.

## Measurements

| Variant | Result |
| --- | --- |
| T-24047 guard, data in asm | 35 diff rows (two moved `lui`, 14 branch offsets) |
| default-arm matrix cells as `*(volatile s32 *)&D_8013C280[k][5]` | 44 rows (base address in v0) |
| whole matrix `extern volatile s32 D_8013C280[3][17]` | worse (every matrix read through a base register) |
| `extern volatile s32 D_8013C294/D_8013C2D8/D_8013C31C` scalars | the hoist moves on to `lui a0/a1` of the call arguments |
| probe word naming func_80135F4C | instruction words equal |
| D_8013CA3C defined in C (island) | MATCH |

## FAKEs kept

Both T-24047 FAKEs are still needed with the table in C: removing the
`*(u8 *)(u32)&D_800E6280.unk_10A5[i + 16]` roundtrip gives 46 rows, removing the
`*(u16 *)&...unk_08` width view gives 40 rows.

## Negatives added

- Volatile views of the default-arm cells (cast or whole-matrix) change the addressing.
- Volatile scalar declarations of the three cells only move the hoist on to the next
  movable `lui` of the arm (the call arguments).

## Lead for the other ENDING functions

Two unmatched ENDING functions are named by their own object's `.data` and contain an
indirect jump: func_80134270 (80133C10.c; object data 8013C4C0-8013C5F0, table
D_8013C5BC) and func_8013A2E8 (801396E0.c; object data 8013CAC0-8013CBB0, table
D_8013CB74 entry at 8013CB80). Neither has its data in C yet. Try
`tools/data_island.py ENDING/80133C10` and `ENDING/801396E0` with the table defined in C
before more source work on their switches.
