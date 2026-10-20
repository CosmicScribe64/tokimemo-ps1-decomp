# T-24501 circular TILE falloff packet

Historical source-only 94-cost control; the final y0 cast now matches production src/main/80049FF0.c, with its canonical prototype in main_api.h. See final proof below. Function is 1,028 bytes. Private target asm/nonmatchings/main/80049FF0/func_8004A814.s and immutable MAIN binary; no raw dump committed. Typed primitive/natural arithmetic recovery transfers only target-supported lessons from func_8004A414 and matched rendering neighbors.

## Replay

Run in existing Docker image from repository root, usual work/disc mounts:

    python3 drafts/T-24501/tile_grid_replay.py

Default complete-TU resolved cost94, 257 target/candidate words, frame0xB8. Diagnostic normalization is not literal proof. Replay creates private complete TU/object/diff under build/scratch/t24501-tile-grid; --source PATH permits retained controls. Current measured full TU also preserved privately at .scratch/t24501/tile114-full.c. No post-freeze matching compile was run; replay syntax alone checked.

## Actual target semantics

- TILE16-byte primitive, masked grid steps at least2, circular squared-distance inclusion, semitransparency1.
- Both TILE width and height are arg3 (x step), despite separate y grid step arg4. Do not fix this target quirk.
- Exactly six abs calls: abs(x distance) staged before abs(y distance), separately for each channel. Do not factor the three falloff calculations or assume abs is pure.
- RGB extraction uses unsigned red/green components, signed blue; each multiplication is converted to signed32 before division by signed radius. Upper clamp255 only; no lower clamp.
- Ending dtd_on_tpage(0,0,arg7,arg6&3,0) is retained.

The source uses natural repeated grid bounds and distance expressions, typed GameState/TILE accesses and existing SDK-compatible setWH(tile,arg3,arg3). No padding, unused variables, forced registers, volatile or compiler changes.

## Bounded controls

| Source | Cost | Target/candidate words | Frame | Finding |
| --- | ---: | --- | --- | --- |
| Original T-17000 full TU |300|257/257|E8|Raw pointer/scalarized baseline |
| Typed natural bounds, explicit dx/dy |187|257/258|C0|x/RGB homes uniformly frame+8 |
| Inline y distance only |121|257/258|B8|Exact x/RGB/frame; radius square wrongly hoisted first |
| Inline x distance too |124|257/257|B8|Target invariant order/count restored; RGB shifts+4 |
| Existing abs_x declaration before RGB |118|257/257|B8|RGB homes exact, no added variable |
| Existing setWH after coordinates (pre-consult) |114|257/257|B8|Target dimension-store scheduling restored |

Each step follows actual slot/operation evidence. Inlining x was retained despite larger numeric score because it repairs invariant multiplication order and removes the extra instruction. No compensating padding or blind declaration permutation. Private controls .scratch/t24501/tile{187,121,124,118}.c; current .scratch/t24501/tile_grid.c. Diff files tile_grid_{original,typed,y_expression,xy_expressions,abs_home,setwh}.diff remain private.

## Pre-consult residual (114 control)

First45 resolved instruction rows agree. Frame, x homeAC and RGB homesA0/9C/98 are exact; all saved-register roles agree with target (poly s0; narrowed x then abs_x s1; y distance s2; half widths then green/red s3/s4; doubled radius s5; y counter s6; x distance s7; radius fp). GameState coordinate base is v0 as target, distinct from sibling4A414's unresolved ranks.

| Invariant/slot | Target | Candidate |
| --- | --- | --- |
| max_x |84|88|
| min_y/max_y |80/7C|84/80|
| x distance squared |74|78|
| radius squared |6C|70|
| ordering byte offset |58|5C|

All offsets hex from SP. This block is uniformly+4, but source evidence does not justify an unused compensating slot. The first non-home difference is outer x preload t0 versus t9, followed by nondraw branch delay NOP versus target arg4 load; target path loads that value before skipping the draw and after AddPrim on the draw path. The source retains the actual y increment at the common loop tail. Coordinate dimensions now schedule correctly via setWH; y arithmetic and color multiply/division/tail temporaries still differ, as does some scheduling. No missing abs call, color clamp, primitive access, or saved-register role identified.

A bounded consult should investigate real CFE temporary/local ownership and branch/lifetime structure explaining the uniform+4 CSE/depth homes and common-tail preload, not add slots or assume the sibling's allocator issue applies. No further source experiments after setWH114.

## Adopted real abs_y local (94)

Parent's bounded consult adds one real s32 abs_y after b, assigning it immediately after each existing abs_x call and using it in the three color expressions. Exactly six calls remain, same order. Source adopted unchanged from /private/tmp/tokimemo-tile-consult/abs_y/body.c, SHA256 ade036f1cb696c753109a76c02a34a29b51dfc2a16566220d6fcfcd27ebdc1c5. Independent ordinary complete-TU replay confirms94/257. All stack homes and saved-register roles now match, first57 resolved rows exact. Prior114 source preserved privately as .scratch/t24501/tile114.c. Consult inner-for control identical94; draw/skip split increments worse132/261 and changed ranks, rejected. Do not repeat.

## Faithful pre-as1 block evidence

Scratch-only diagnostic wrapper captures the normal frame-transformed binasm, then runs the unchanged normal frame/as1 pipeline. Captured-object resolved output exactly equals ordinary94/257. Private paths: .scratch/t24501/tile94-pre/run.py, capture.py, pre-as1.G, captured.o; ordinary control .scratch/t24501/tile94-normal/guarded.o. No production tool change; no raw binasm or full TU committed.

First apparent x-preload mismatch (resolved row57) originates at the OUTER LATCH, local binasm record238, and is moved into the inner-entry branch delay by as1. It is not a source operation at entry. The current skip label precedes one common arg4 load at records230/232, then y-add/backedge; target skip edge has its own preloaded arg4 and draw edge reloads after AddPrim, with their common label after those loads. No fabricated edge load proposed.

Earliest emitted geometry ring difference: current y field is LH gy(t7), ADDU t8=gy+s6, SUBU t9=t8-s4, SH. Target has the same gy, full-width s6 and half-height operands/association but ADDU t1 and SUBU t0. Neither target nor current visible instruction sequence narrows y early. A possible explicit s16(y) converted only for y0 would explain erased temporary slots t8/t9; this is not proven original source and would be FAKE-marked if solely needed for temporary numbering. Full-width y must remain in circle/abs/increment paths. No cast control tested at this checkpoint.

## Final match: erased y0 conversion

One isolated diagnostic `(s16)y` in the y0 coordinate expression closes94->0, 257/257 words. It does not change the full32-bit loop, circle test, abs inputs or increments. Original early-width intent is unproved; production explicitly marks this FAKE T-24501 and explains erased-conversion temporary allocation. The final stored16-bit coordinate is unchanged by this local truncation; no other cast/control changed.

Matched pre-as1 capture directly confirms SLL t8,s6,16; SRA t9,t8,16; ADDU t1,gy,t9; SUBU t0,t1,s4; SH. as1 removes the conversion, using full s6 in exact target arithmetic. Its two temporary allocations also repair all later ring/load scheduling. Captured output equals ordinary matched257-word output. Private matched capture .scratch/t24501/tile-match-pre and earlier94 control remain; no raw binasm committed.

Final promoted default-C complete-TU private link proves all1,028 bytes equal immutable target (.scratch/t24501/tile-match-proof). Affected MAIN `ninja -j2 build/SLPM_86.053.ok` passes SHA1; check_headers, sync_protos --check and migrate_globals --check pass. Exact void(s32 x8) prototype added once to main_api.h; both width/height use arg3, all six abs calls and signed/upper-only clamp behavior retained. Parent reviewed production body/header against CODING_STANDARDS with no findings. Full27 integration checks remain central. Historical draft/replay remains94 for reproducing the causal control after production promotion; final matched body is canonical source.
