# T-25127: ENDING func_8013A2E8 (src/ovl/ENDING/801396E0.c)

Result: matched. Default C, 4648 bytes (1162 instructions plus the jump table), ENDING.bin
sha1 OK. Commit `T-25127: match ENDING:func_8013A2E8`.

## Start state

NON_MATCHING guard from T-23002 at 12 differing lines (6 rows). The original fills the
load-delay slot after `lbu a0` of the first flag test (`D_800E6280.unk_0F6.b[1] & 1`) with the
first string's `lui a1`; our build left a `nop` there and kept `lui a1` in the call arm.
T-25056 traced the cause in the reconstructed as1: the first flag block (bb10) is in the outer
region, whose candidate-block budget (11 dominated blocks) is used up by equal-weight joins
before the weight-4 arms (bb11, bb58) are reached. Its scratch assembler with the budget
raised to 100 gave a literal match on unchanged C, but no source change was known.

## What decided it

T-25150 (KANGEI func_80134724) found that as1's CFG builder gives every block ending in an
indirect jump other than `jr ra` an edge to every address-taken label of the file, and a
function named by a `.word` in the file's own data is one. func_8013A2E8 has a jump-table
switch (`jr $t4` at 8013ACC0), and the object 801396E0 owns `.data` 8013CAC0-8013CBB0
(`config/objects/ENDING.txt`), whose scene table D_8013CB74 (read by func_801399B4) names
func_8013A2E8 (entry 3). With the table as asm data as1 never saw that reference.

Defining the table in C puts the dispatch on a back edge to the entry. bb10 then lies in a
natural-loop region, which has the larger budget (the same effect as T-25056's raised
threshold). Measured: the unchanged T-23002 guard body plus the table gives MATCH at once.
No as1 trace was taken of the new build; the region claim rests on the T-25150 mechanism and
the T-25056 counterfactual, which this result agrees with.

## Changes

- `tools/data_island.py ENDING/801396E0` (yaml island, 46 INCLUDE_RODATA lines; build
  stayed byte-identical), then the D_8013CB74 line replaced by its C definition
  (`EdFnTbl14`, 14 entries; the trailing zero word at 8013CBAC is section padding, the build
  is sha1 OK without it).
- `include/ovl/ENDING.h`: prototypes for the table's ENDING entries and for func_8013B640
  (the s32 FAKE of T-23002, moved out of the former NON_MATCHING block).
- Guard and INCLUDE_ASM removed; the T-23002 body is the default C. Its FAKEs (T-23002: `pad`,
  the `&D_800E6280.unk_100` indexing, s32 func_8013B640) were kept as they were; not
  re-tested against the new table.
- cfe prints Warning 709 for the s32 entries (func_80139E08, func_8013B5E4, func_8013B510,
  func_80046500) in the `void (*)()` table, as in KANGEI; bytes are unaffected.

## Negatives added

- A one-entry probe table outside the island (`void (*probe[1])() = { func_8013A2E8 };`)
  shifts the object's `.data`, so linked bytes differ; use the island and the real table.

## Lead for other functions

The two unmatched ENDING functions left, func_80134270 (80133C10.c) and func_80135F4C
(80134D20.c), both end a block in a non-`ra` `jr` and are both named by a `.word` in
ENDING's data, and in both cases the word lies in the function's own object's `.data`
(8013C5CC in 80133C10's 8013C4C0-8013C5F0, 8013CA50 in 80134D20's 8013C5F0-8013CAA0). Island
those objects and define the tables in C first; measure before any other lever.
