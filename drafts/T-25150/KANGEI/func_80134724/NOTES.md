# T-25150: KANGEI func_80134724 (src/ovl/KANGEI/80134120.c)

Result: matched. Default C, 688 bytes (172 instructions plus the 7-entry jump table), clean
rebuild 27/27 sha1 OK. Commit `T-25150: match KANGEI:func_80134724`.

## Start state

NON_MATCHING guard from T-25011 at 4 rows: the default arm `D_80139ADC = (u16)v * 4 + s + 3`
(FAKE cast) kept the range-check delay slot right but put the default arm's temporaries one
ring slot late (t1..t3 instead of t0..t2). Without the cast (T-24902, T-25042) as1 hoists the
default arm's `sll t0,v0,2` into the delay slot of `beqz at, default`, where the original has
the dispatch `sll t1,t1,2`.

## What decided it

as1 builds the control flow graph in `func_41dc10` (recomp source). Every block that ends in
an indirect jump other than `jr ra` gets an edge to every block whose label is address-taken
(the `+61` flag of the label symbol). Jump-table entries are address-taken, and so is a
function whose symbol a `.word` in the same file's data names.

The original object 80134120 owns the `.data` range 80139B30-80139C80
(`config/objects/KANGEI.txt`). Its first variable, the scene table `D_80139B30`, names
func_80134724 (entry 7). Our build had the table as asm data, so as1 never saw that
reference. With the table defined in C:

- the dispatch `jr t1` gets an edge back to the function's entry block (`BACK EDGE: 19 to 3`,
  loop 3..17 and 19);
- in that loop the dispatch weighs 2 (the switch head has a successor outside the loop, the
  +1 rule) and the scan from the switch head (bb17) tries the dispatch first;
- `MOVETO bb 17 inst 7 <- bb 19 inst 1`: the dispatch `sll` fills the slot, the default arm
  keeps its `sll t0`, and the plain `v * 4 + s + 3` gives t0..t2.

Before (uncast, no table): weights bb17 1, default bb18 4, dispatch bb19 4; the default is
numbered first (reverse postorder: the taken successor is visited last) and its `sll` passes
the path test.

## Measurements (full TU, run.py here; it expects the T-24900/T-25037 capture shims copied to build/as1tr and build/t25037 and the as1-probe binary at build/t25150/as1-probe)

| Variant | Words | Rows |
| --- | --- | --- |
| T-25011 guard (u16 cast) | 172 | 4 changed words (funcdiff), score 8 |
| uncast | 172 | score 26 (hoist) |
| uncast + any table naming func_80134724, before or after the function | 172 | 0 |
| guard (cast) + table | 172 | 4 changed words again: the cast only fixed the symptom |

The table placement does not matter (as1 reads the whole file before it optimises).

## Changes

- `tools/data_island.py KANGEI/80134120` (yaml island, 6 INCLUDE_RODATA lines; build stayed
  byte-identical), then `D_80139B30` defined in C with its 30 entries. The FnTbl typedefs moved
  above the island lines.
- Declarations for the entries: KANGEI functions in `include/ovl/KANGEI.h`; main-exe
  `func_8007C784`, `change_dec_bg`, `dec_init` in `include/main_api.h`. Main names used for
  entries the KANGEI asm printed as raw words or `func_` aliases: `dec_init`, `change_dec_bg`,
  `place_init` (0x8007EC68), `normal_date_girl_in` (0x8007E390), `normal_date_girl_out`
  (0x8007E5A0), `normal_date_bg_out`.
- `func_8007C784` and `func_8007C8A4` had no link name for the KANGEI link (no KANGEI asm
  reference); added to `config/symbol_addrs_types.txt`, which feeds `build/main_names.ld`.
- The default arm lost its FAKE cast. No FAKE remains in the function.

## Lead for other functions

Any unmatched function with a jump-table switch (or another indirect jump) in an object whose
original `.data` names functions of that object may have the same miss: weights and the
xbb scan change once the data is defined in C. In the scan of all switch heads
(scan.py here) main `mod_trans_vram` (unmatched) also keeps the dispatch `sll` in the
slot although its default arm starts with a movable `li t1,1`; not checked further.

## Negatives added

- Defining the table while keeping the (u16) cast: still 4 rows. The cast is not needed and
  is wrong once the table exists.
- cfe prints Warning 709 (incompatible pointer type) for the three `s32 (void)` entries
  (`dec_init`, `func_80083474`, `func_80046500`) in the `void (*)()` table; bytes are unaffected.
