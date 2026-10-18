# T-24018 large movie-state-machine sibling ports

No functions matched. All three production functions retain their original
INCLUDE_ASM fallback; source, headers, flags, compiler and generated assembly
are unchanged. These are source-only leads, not default C progress.

The pass started from matched DATE2 func_80133AF0 and DATE func_80147958,
with T-12100 and T-23004 as prior evidence. DATE2 is reference-only. Earlier
T-23004 bodies remain intact and are preferable for EVENT func_8011DDB8.
All Python and compiler work used tools/docker.sh, one compiler at a time.

## Reproduction

```
tools/docker.sh python3 drafts/T-24018/probe.py DATE func_801462B0 drafts/T-24018/date-801462B0.c
tools/docker.sh python3 drafts/T-24018/probe.py EVENT func_8011C750 drafts/T-24018/event-8011C750.c
tools/docker.sh python3 drafts/T-24018/probe.py EVENT func_8011DDB8 drafts/T-24018/event-8011DDB8.c --proto 's32 func_8011DDB8();' --proto 's32 func_8004AFB8();' --proto 's32 func_8004A9CC();' --proto 's32 func_800433D0();' --proto 's32 func_80012D40();'
```

The harness compiles actual scratch C in a full translation unit, without
NON_MATCHING. It prints diagnostic instruction/diff counts, never a literal
linked-match verdict. The third candidate needs the recorded T-23004 return
views only in its scratch unit; no headers were edited. Saved source is C89;
inherited source-shaping forms retain FAKE comments and this ticket number.

| Target | Target instructions | Candidate instructions | Printable diff rows |
| --- | ---: | ---: | ---: |
| DATE func_801462B0 | 951 | 951 | 434 |
| EVENT func_8011C750 | 925 | 924 | 167 |
| EVENT func_8011DDB8 | 998 | 1005 | 357 |

Printable rows include insert/delete pairs and branch-offset consequences.
They are not directly comparable with T-23004's aligned-row or normalized
edit-distance numbers. Literal instruction spans are 3,804, 3,700 and 3,992
bytes respectively, excluding adjacent tables.

## Findings

DATE: the direct DATE2 source port gives 599 printable rows and 896 candidate
instructions. DATE-specific callback branches and the 300-frame timer give
473 rows / 946 instructions. An early break in case 14 recovers all 951
instructions and the exact 0x68 frame, at 434 rows. The first 442 instructions
match; callback save values and selectors allocate differently afterwards.
A two-word callback-save array gives 468/480 rows; s32 instead of the declared
u16 return changes no score. Direct stage accesses (444) and constant-index
record accesses (445/439) also fail. The array and callback cases reproduce
prior T-12100 concerns without a compiler modification.

EVENT C750: the DATE2 source skeleton, existing EventMovieWork/EventMovieState
and Rec24 views, EVENT callback/resource substitutions and 300-frame timer
compile at 317 rows. Loading the exit state directly instead of through the
state pointer gives 167. The first nonbranch difference is the case-12
selector reload in v0 instead of v1; later page-byte accesses and branch layout
still differ. Recorded s32 callback views alone reduce 317 to 311. Removing
the DATE2 mixed byte-access form gives 194/188 rows and 923 instructions;
using direct byte accesses gives 187/181 and 924. Unsigned word-pointer stage
and reversed selector-compare hypotheses give 187 rows. None reaches zero.

EVENT DDB8: a literal DATE source port is semantically incomplete: EVENT has
a different case-22 sound return, a simpler case-23 timer, different resource
selector and callback cases, and its own shutdown order. Those were adapted
from the preserved T-23004 source. The resulting candidate is still 357 rows,
1005 instructions, frame 0x68 rather than 0x60. Existing T-23004 is the better
starting point. s32 self/callee views are required to compile the source's
return-call branches but do not establish that any production type should
change. The header-unused-slot shaping inherited from DATE remains marked
FAKE and is not a claim about the EVENT source.

Private exploratory scripts, objects and target instruction comparisons remain
under ignored build/T-24018. No game-derived listings or objects are committed.

## Controls

`ninja -j1 build/ovl/DATE.ok build/ovl/EVENT.ok` has no work because production
source is unchanged. Direct Docker SHA1 comparisons with config/overlays
confirm both binaries remain exact. These controls verify retained assembly
and existing C; they add zero matching C bytes. Root review remains pending.
