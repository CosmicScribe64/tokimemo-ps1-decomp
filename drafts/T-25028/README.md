# T-25028 bounded read-only prioritization

Current main checked at f688ec594. Docker read-only progress: 6833/6952
functions, 2104676/2276920 bytes; 119 remaining. A separate source scan
confirmed all twelve names below still have INCLUDE_ASM in current main.
No compilation, source trials, root edits or agents. Excluded contributor
GEKO/OPTION, active main/8004F870.c and separately assigned TEL.

Result: **zero qualified new semantic/control/ABI/width repairs among twelve
reviewed remaining functions**. This does not survey or classify all119.
Scores below are not used to rank candidates; historical metrics were not
recomputed. Best-body pointers mean newest useful tracked starting evidence,
not independently proved semantic equivalence or literal byte matching.

| Remaining function | Current useful body / newest specific ledger | Qualification decision |
| --- | --- | --- |
| main strCallback | in-tree guard, src/main/800563F0.c; T-25010 (also T-25001) | RECT-copy temporary is one ring slot high. The odd-mode reload already exists; do not rediscover it as a missing read. No new semantic defect. |
| main 80056BA8 | drafts/T-17014/func_80056BA8/body.c; T-24141/24142 scope | Typed StrCtx/DISPENV/DRAWENV/CdlFILE draft already repairs raw layouts. Target 80056C8C..6CB4 copies exactly three position bytes, matching draft; missing fourth byte is not a defect. Callback/stream donor data types already applied. No new call/width defect established. |
| ETC 8014790C | drafts/T-24909/ETC/func_8014790C/body.c and NOTES.md | Newer two-buffer/coordinate-walk reconstruction replaces raw m2c. Residue is switch colouring and separate materialization of D_8011ECD0; casts/typed-base/loop variants recorded. No new semantics. |
| ENDING 801330E4 | drafts/T-24924/ENDING/func_801330E4/body.c; T-24924 | Raw chance-table size/layout issue is already repaired to 36 bytes and exact frame. Remaining f9 extraction/copy coalescing and level spill/temporary placement; not a new array-width defect. |
| main parameter_up_down | in-tree guard src/main/800737A0.c; T-25008, T-25016 synthesis caveat | Existing mode/nibble temporary and row-pointer spill tail. Raw failed bed/reconstruction is not the current body. No new parameter ABI or bounds defect evidenced. |
| main 8005938C | drafts/T-17017/main/func_8005938C-closer/body.c; T-25007 | 32-byte MATRIX-copy ring consumption differs. Matched ETC8013FA7C/TACO80146D60 copies actually support the current three-slot behavior; they are not a missing donor fix for the two-slot target. No new type defect. |
| main 80054E6C | in-tree guard src/main/80053650.c; T-24915 (T-24570 older) | Current slot-word/byte views, frame and memcpy-CSE boundary repair supersede older 84-line/frame claims. Remaining loop-head order and byte-temp rotation. Do not repeat the old word-union repair. |
| main 80067870 | drafts/T-17002/main/func_80067870/body.c; T-24704 | Archive needs removed-symbol reconciliation, which is not itself a semantic finding. Checked rsin-product unsigned shift then s16 dx: target uses multu/srl/sign extension and agrees with typed draft. No newly wrong signedness shown. |
| main 80065964 | drafts/T-25010/main/func_80065964.c; T-25010, T-24501 | Natural collision/bounds reconstruction already exists; rolled goto retained. Missing duplicate coordinate ranges/dead spill homes remain a source-lifetime question, not an identified wrong bound or call. |
| main 8004A1A0 | drafts/T-25010/main/func_8004A1A0.c; T-25010 | Shared SpriteCell8/Map10/Index80 types and pointer-derived slot store already restore instruction count. Remaining constants/map and loop range colouring; obsolete private types are repaired. |
| main 80054FC0 | drafts/T-24501/main/func_80054FC0.c and -notes.md; T-24501 bounded consultation | Correct byte checksum loop, unsigned bound, stride and exact frame already reconstructed from matched loader views. Copy-ring/epilogue tail and extra word remain; no fresh checksum-width defect. |
| main 8006509C | drafts/T-24301/main/func_8006509C.c; T-24501 | GameState pointer, byte views and 15-pair indexed copy already repaired. Remaining source/destination pointer and count-register ownership/zero placement; no new copy length or semantic edge found. |

## Stale-census exclusions

Before selecting remaining functions, current source and T-25003/25004/24929
showed main80043510, main8004443C, col2sepia and ETC8013CCE4 already matched.
The 2026-10-20 table is historical127, not the current119 inventory. These
were discarded at membership filtering, not proposed as new targets.

## Strongest unanswered question and minimal evidence task

No implementation candidate is nominated from this survey. Strongest bounded
upstream question: **where does strCallback's first RECT-copy expression
consume the extra ring slot relative to target, and does a matched RECT-copy
producer share the same call/reload context?** T-25001/25010 source variants
have already failed; the odd-mode reload must remain. A useful next evidence
task would capture this unchanged full TU, map uopt-output expression order
to ugen's first two copy-word temporaries and identify an actual extra or
missing expression, then compare one matched same-width copy with its precise
context. Reject the task if it only reproduces the known one-slot mismatch
without locating a new def/use boundary. No guessed casts, pads or copy syntax
tests follow automatically. Source inspection here does not establish a new
lever or prove a compiler defect.

The older high-byte reconstruction families may still contain semantic holes;
none was demonstrated in these twelve newest useful bodies. Other107 remaining
functions were not individually audited. Raw recovered M2C_ERROR holes and
removed globals are not evidence that the newer tracked source still has them.

Private scope snapshot: build/t25028-scope.json in this isolated tree. Only
this concise source/ledger report and wiki metadata are tracked.
