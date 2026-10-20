# T-24522 RPG_BAT source reconstruction

Two exact default C matches, 2,048 bytes total:

- `func_8014BB80`, 1,124 bytes, commit `bf7e243bd`: typed GameState pointer reads in the two choice switches split the condition's plain-field range. The actual pointer replaces the prior fake frame home. Removes the volatile read and the fake padding.
- `func_801447E0`, 924 bytes, commit `66f4bbe19`: make D_8015EBB0 the arithmetic variable, with explicit multiply/divide stages. Declare real locals k before lim. No new fake, header override or compiler change.

Both passed Docker private default linked literal-byte proof and private RPG_BAT checksum. All 27 hashes passed using that private output and 26 unchanged read-only reference outputs; this does not claim rebuilding all 27. Headers, global migration, prototype branch checks pass (0 new, 15 known). Worker and independent coordinator standards reviews passed.

## Useful unresolved source

`RPG_BAT/func_80144C84/body.c` is source only, 28 literal rows / 28 aligned in the full current translation unit. Production stays assembly. Starting T-17113 draft252/224. Direct output-global updates138/134; typed first-stat pointer92/88; frame home82/78; staged output-global arithmetic44/44; multiplication order40/40; sum operand wrappers30/30; matched sibling's comma-return recipe28/28. Every fake home/wrapper/comma-return is documented in the archived source.

The remaining third-arm field08/EBE0 reload and store scheduling differ. The build speculates EBE0's decrement before the nested enemy-kind branch and retains field08 in a register, whereas the target reloads both fields after that branch. Natural pointer threshold/clamp forms, smaller trailing-record pointer, field writebacks, distinct clamp local, switch/while/else spellings, signed casts and field alias casts did not close the gap. The reversed conditional46/38 worsens branch shape. Do not rerun these as a broad grid. No compiler-limitation claim is established.

`func_8014BFE4`: current T-17110 baseline128/128. A typed actor-base pointer207/201; address-taking update of argument x and the third comparison185/185; combined247/247. All worse, not promoted. The prior argument stack-home and loop-strength-reduction gaps remain.

`func_80154DD8` and `func_8015528C`: current recent T-24027 source-only drafts and concrete negatives were read. They remain broad state-machine/interpolation gaps (326/288 and302/246 in T-24027). No fresh hypothesis justified rerunning those measurements.

All experiments used the existing Docker arm64 image and one compiler process. Loose probe scripts, JSON candidate sets, full source candidates and diagnostics are preserved in ignored `build/T-24522/`; no generated target asm, binary, copyrighted data or production compiler change is committed.

## Bounded threshold-snapshot follow-up

After coordinator review, a read-only dependency map motivated one new structural hypothesis: capture the third threshold's EBE0/field08 inputs in a real two-word local aggregate, then mutate plain globals. Three meaningful forms were compiled at j1: third-threshold-only capture; refresh at both initial bound and third-threshold phase boundaries; reversed member/assignment order. All score34/34. Every form retains the exact original28-row code gap and adds6 limit-spill-offset rows(0x2C to0x28); the aggregate snapshot is optimized away. No new source form was promoted. Inputs and diagnostics remain ignored under `build/T-24522/forms55.json` and `55_*.c`/`55_*.diff`.

The minimal remaining dependency blocker is now explicit: target reloads field08 and EBE0 on the noncall successor of the nested enemy-kind branch; current C carries the threshold registers forward, moves extra-minus20 into the branch slot and spills field08 along the call path. The earlier initial-bound EBE0 load also moves relative to counter writebacks. Ordinary pointer forms, branch spellings and the explicit snapshot do not reproduce the target dependency. This is evidence about tested source forms, not proof that no C source exists.
