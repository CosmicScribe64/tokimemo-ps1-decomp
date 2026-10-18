# T-24030 BUNKASAI sibling provenance

Both assigned functions now use verified default C in their production files. Final source is kept there rather than duplicated in this directory. This note preserves the measured sibling transfer and rejected controls; ignored logs under `build/bunkasai24030/` are not archived game material.

## Dialogue state machine, func_80151364

Starting source: the two-row `NON_MATCHING` guard in `src/ovl/BUNKASAI/801511E0.c`, documented by T-22012. Matched sibling `func_801537BC` and recent T-24016 state machines suggest fresh GameState byte reads and direct dialogue stores. The guarded source already uses direct dialogue stores.

Remove the shared `u8 year` and replace its assignment expressions with fresh `game->unk_03E` reads, retaining the pre-existing case-12 pointer reassignment. Diagnostic cost becomes zero. A direct-call switch without this change stays at 2; removing the pointer reassignment gives 16. The retained reassignment remains explicitly FAKE; its removal is not justified.

The target guard and assembly fallback were removed before running default linked proof: 844 literal bytes match, BUNKASAI checksum passes. First coherent commit: `f41f35d3a`.

## Festival initialization, func_80142700

Start from the T-18700 typed draft and matched BUNKAKEN `func_80144F70` family (sibling ratio 0.886). Use the existing `BunkasaiGirlBytes` table and a consistent `GameState *g`. Write the girl-flag copy as an eleven-element loop using `unk_0C.f.b1`, and the clear as a 64-word loop over the actual `D_80122DD0` buffer. Preserve the target's `func_8008585C()` before the girl loop, its EBC/EA4/E98 store order, and its shorter case-8 branch structure. Use the sibling's declaration order: loop index, GameState pointer, copied table.

This gives cost 22 after the necessary scratch declaration for the existing C-defined void helper `func_80142B98`. Declaring `func_801433C0` void fixes the clear-loop v0/v1 allocation (22 to 4). That callee remains assembly: evidence is its void matched BUNKAKEN sibling, the census's void body, no explicit returned value in its assembly tail, and its sole C caller ignoring the result. This is not a claim of an existing C definition. A void declaration for the earlier `func_80142BE8` call has no effect (22).

The remaining case-0 load/store order uses the sibling's explicitly FAKE `(&D_80122E98)[10] += 1` view. Alone it gives 18; combined with the void callee view it gives zero. A plain arithmetic dependency on the just-cleared D_80122E98 stays at 22. The unknown enclosing block layout is not asserted as recovered. Both necessary forward declarations were reported to the coordinator before the overlay-header edit; main_api.h is unchanged.

Final normal-default proof verifies 1,176 bytes for func_80142700 and rechecks 844 bytes for func_80151364 together. The affected checksum, header/global checks, and prototype guard pass. Logs: `final-proof.log`, `final-build.log`, `final-headers.log`, `final-globals.log`, `final-protos.log` under `build/bunkasai24030/`. All probes and verification use Docker and builds use `ninja -j1`; no per-function toolchain exceptions or generated assembly edits.
